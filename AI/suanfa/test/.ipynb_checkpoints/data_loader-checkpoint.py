from config import *
from utils import *
import torch.utils.data as data
from torch.nn.utils.rnn import pad_sequence
import json
import torch


class Dataset(data.Dataset):
    def __init__(self, type='train'):
        super().__init__()
        if type == 'train':
            file_path = TRAIN_SAMPLE_PATH
        elif type == 'dev':
            file_path = DEV_SAMPLE_PATH
        # 读取文件
        with open(file_path, encoding='utf-8') as file:
            self.lines = json.loads(file.read())
        # 词表引入
        _, self.en_vocab2id = get_vocab('en')
        _, self.zh_vocab2id = get_vocab('zh')

    def __len__(self):
        return len(self.lines)
    
    def __getitem__(self, index):
        en_text, zh_text = self.lines[index]
        source = [self.en_vocab2id.get(v.lower(), UNK_ID) for v in divided_en(en_text)]
        target = [self.zh_vocab2id.get(v.lower(), UNK_ID) for v in divided_zh(zh_text)]
        return source, target, zh_text
    
    def collate_fn(self, batch):
        batch_src, batch_tgt, tgt_text = zip(*batch)
        # source
        src_x = pad_sequence([torch.LongTensor([SOS_ID] + src + [EOS_ID]) for src in batch_src], True, PAD_ID)
        src_mask = get_padding_mask(src_x, PAD_ID)
        # target
        tgt_f = pad_sequence([torch.LongTensor([SOS_ID] + tgt + [EOS_ID]) for tgt in batch_tgt], True, PAD_ID)
        tgt_x = tgt_f[:, :-1]
        tgt_pad_mask = get_padding_mask(tgt_x, PAD_ID)
        tgt_subsqueent_mask = get_subsequent_mask(tgt_x.size(1))
        tgt_mask = tgt_pad_mask | tgt_subsqueent_mask
        tgt_y = tgt_f[:, 1:]
        # return
        return src_x, src_mask, tgt_x, tgt_mask, tgt_y, tgt_text


train_dataset = Dataset('train')
train_loader = data.DataLoader(train_dataset, batch_size=BATCH_SIZE, shuffle=True, collate_fn=train_dataset.collate_fn)

dev_dataset = Dataset('dev')
dev_loader = data.DataLoader(dev_dataset, batch_size=BATCH_SIZE, shuffle=False, collate_fn=dev_dataset.collate_fn)



if __name__ == '__main__':
    ds = Dataset('dev')
    loader = data.DataLoader(ds, batch_size=2, collate_fn=ds.collate_fn)
    print(next(iter(loader)))
    