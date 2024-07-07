from config import *
from utils import *
from model import make_model
from torch.nn.utils.rnn import pad_sequence


if __name__ == '__main__':

    en_id2vocab, en_vocab2id  = get_vocab('en')
    zh_id2vocab, zh_vocab2id  = get_vocab('zh')

    SRC_VOCAB_SIZE = len(en_id2vocab)
    TGT_VOCAB_SIZE = len(zh_id2vocab)

    model = make_model(SRC_VOCAB_SIZE, TGT_VOCAB_SIZE, D_MODEL, N_HEAD, D_FF, N, DROPOUT)
    model = model.to(DEVICE)
    model.load_state_dict(torch.load(MODEL_DIR + '/best_model.pth', map_location=DEVICE))

    model.eval()

    texts = [
    "Many people in the world have this dream: they hope that some day they will be able to come to china and visit beijing.",
    "Although there are still many difficulties on our way ahead,we have the confidence to run well all our undertakings and to meet the preset goal.",
]

    batch_src_token = [[en_vocab2id.get(v.lower(), UNK_ID) for v in divided_en(text)] for text in texts]
    batch_src = [torch.LongTensor([SOS_ID]+src+[EOS_ID]) for src in batch_src_token]
    src_x = pad_sequence(batch_src, True, PAD_ID)
    src_mask = get_padding_mask(src_x, PAD_ID)

    src_x = src_x.to(DEVICE)
    src_mask = src_mask.to(DEVICE)

    prob_sent = batch_greedy_decode(model, src_x, src_mask)

    print(prob_sent)