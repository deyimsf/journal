import jieba
import re
from config import *
import sacrebleu


# 中文分词
def divided_zh(sentence):
    return jieba.lcut(sentence)

# 英文分词
def divided_en(sentence):
    # 使用正则表达式匹配单词和标点符号
    pattern = r'\w+|[^\w\s]'
    return re.findall(pattern, sentence)


def get_vocab(lang='en'):
    if lang == 'en':
        file_path = EN_VOCAB_PATH
    elif lang == 'zh':
        file_path = ZH_VOCAB_PATH
    
    with open(file_path, encoding='utf-8') as file:
        lines = file.read()

    id2vocab = lines.split('\n')
    vocab2id = {v:k for k,v in enumerate(id2vocab)}
    return id2vocab, vocab2id


# 参数 size 为句子长度
def get_subsequent_mask(size):
    # 1为batch维度
    mask_shape = (1, size, size)
    return 1-torch.tril(torch.ones(mask_shape)).byte()


def get_padding_mask(x, padding_idx):
    # 扩展Q维度
    return (x == padding_idx).unsqueeze(1).byte()


def bleu_score(hyp, refs):
    bleu = sacrebleu.corpus_bleu(hyp, refs, tokenize='zh')
    return round(bleu.score, 2)


def batch_greedy_decode(model, src_x, src_mask, max_len=50):

    model_mod = model.module if MULTI_GPU else model

    zh_id2vocab, _ = get_vocab('zh')
    memory = model_mod.encoder(src_x, src_mask)
    # 初始化目标值
    prob_x = torch.tensor([[SOS_ID]] * src_x.size(0))
    prob_x = prob_x.to(DEVICE)

    for _ in range(max_len):
        prob_mask = get_padding_mask(prob_x, PAD_ID)
        output = model_mod.decoder(prob_x, prob_mask, memory, src_mask)
        output = model_mod.generator(output[:, -1, :])
        predict = torch.argmax(output, dim=-1, keepdim=True)
        prob_x = torch.concat([prob_x, predict], dim=-1)
        # 全部预测结束，结束循环
        if torch.all(predict==EOS_ID).item():
            break
    # 根据预测值id，解析翻译后的句子
    batch_prob_text = []
    for prob in prob_x:
        prob_text = []
        for prob_id in prob:
            if prob_id == SOS_ID:
                continue
            if prob_id == EOS_ID:
                break
            prob_text.append(zh_id2vocab[prob_id])
        batch_prob_text.append(''.join(prob_text))
    return batch_prob_text


def print_memory():
    # 获取当前可用的GPU数量
    num_gpus = torch.cuda.device_count()
    # 遍历每个GPU，输出GPU的占用情况
    for i in range(num_gpus):
        gpu = torch.cuda.get_device_name(i)
        utilization = round(torch.cuda.max_memory_allocated(i) / 1024**3, 2)  # 显存使用量（以GB为单位）
        print(f"GPU {i}: {gpu}, Memory Utilization: {utilization} GB")


if __name__ == '__main__':
    # print(divided_zh('我爱北京天安门'))
    # print(divided_en('naver say naver!'))

    # id2vocab, vocab2id = get_vocab('zh')
    # print(id2vocab)
    # print(vocab2id)

    target = '我喜欢读书。'
    vocabs = divided_zh(target)
    print(vocabs)
    _, zh_vocab2id = get_vocab('zh')
    tokens = [zh_vocab2id.get(v, UNK_ID) for v in vocabs]
    print(tokens)