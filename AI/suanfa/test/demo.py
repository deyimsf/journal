import math
import torch
from torch import nn
from torch.autograd import Variable
from torch.nn.utils.rnn import pad_sequence
import sacrebleu
from matplotlib import pyplot as plt


def lr_lambda_fn(step, wramup):
    lr = 0
    if step <= wramup:
        lr = step / wramup * 10
    else:
        lr = wramup / step * 10
    return max(lr, 0.1)

rates = []
total_epoch = 1000
steps = range(total_epoch)
for step in steps:
    r = lr_lambda_fn(step, total_epoch/4)
    rates.append(r)

plt.plot(steps, rates)
plt.show()

exit()


def lr_lambda_fn(step, model_size, factor, warmup):
    if step == 0:
        step = 1

    return factor * (
        model_size ** (-0.5) * min(step ** (-0.5), step * warmup ** (-1.5))
    )

d_model = 512
factor = 0.1
warmup = 400

rates = []
steps = range(0, 1000)
for step in steps:
    r = lr_lambda_fn(step, d_model, factor, warmup)
    rates.append(r)

plt.plot(steps, rates)
plt.show()


exit()

# 参考句子
refs = [['我喜欢吃苹果。', '我喜欢吃水果。'],
        ['这本书很有意思。', '这本书很好玩。'],
        ['他是一个出色的演员。', '他是一名杰出的演员。']]
# 候选句子
hyp = ['我爱吃苹果。', '这本书非常有趣。', '他是一位优秀的演员。']
bleu = sacrebleu.corpus_bleu(hyp, refs, tokenize='zh')
print(bleu.score)

exit()


batch_src = [
    [1, 2, 3],
    [4, 5]
]

src_pad = pad_sequence([torch.LongTensor(src) for src in batch_src], True, 0)
print(src_pad)

src_pad = pad_sequence([torch.LongTensor([200]+src+[300]) for src in batch_src], True, 0)
print(src_pad)
exit()

# 把数字和字母分开
batch = [
    [[1,2,3], ['a','b','c']],
    [[4,5,6], ['d','e','f']],
]

# nums = []
# abcs = []
# for num, abc in batch:
#     nums.append(num)
#     abcs.append(abc)

nums, abcs = zip(*batch)

print(nums)
print(abcs)
exit()


class Embeddings(nn.Module):
    def __init__(self, vocab_size, d_model):
        super().__init__()
        # Embedding层
        self.lut = nn.Embedding(vocab_size, d_model)
        # Embedding维数
        self.d_model = d_model

    def forward(self, x):
        # 返回x对应的embedding矩阵，需要乘以math.sqrt(d_model)
        return self.lut(x) * math.sqrt(self.d_model)


# emb = Embeddings(10, 8)
# inputs = torch.tensor([
#     [1, 2, 3],
#     [4, 5, 0],
# ])
# output1 = emb(inputs)
# print(output1.shape)



class PositionalEncoding(nn.Module):
    def __init__(self, d_model, dropout=0.1, max_len=5000):
        super().__init__()
        self.dropout = nn.Dropout(dropout)
        pe = torch.zeros(max_len, d_model)
        # 位置和除数
        position = torch.arange(0, max_len).unsqueeze(1)
        div_term = torch.exp(torch.arange(0, d_model, 2) * -math.log(10000) / d_model)
        # 修改pe矩阵的值
        pe[:, 0::2] = torch.sin(position * div_term)
        pe[:, 1::2] = torch.cos(position * div_term)
        # 扩展 batch 维度
        pe = pe.unsqueeze(0)
        # 存储为不需要计算梯度的参数
        self.register_buffer('pe', pe)

    def forward(self, x):
        x = x + Variable(self.pe[:, :x.size(1)], requires_grad=False)
        return self.dropout(x)



# class PositionalEncoding(nn.Module):
#     def __init__(self, d_model, dropout=0.1, max_len=5000):
#         super().__init__()
#         self.dropout = nn.Dropout(dropout)
#         self.pe = torch.zeros(max_len, d_model)
#         for pos in range(max_len):
#             for j in range(d_model):
#                 angle = pos / math.pow(10000, (j//2)*2 / d_model)
#                 if j % 2 == 0:
#                     self.pe[pos][j] = math.sin(angle)
#                 else:
#                     self.pe[pos][j] = math.cos(angle)

#     def forward(self, x):
#         return self.dropout(x + self.pe[:x.size(1)])

# pe = PositionalEncoding(8)
# output2 = pe(output1)
# print(output2.shape)


def get_padding_mask(x, padding_idx):
    # 扩展Q维度
    return (x == padding_idx).unsqueeze(1).byte()


def attention(query, key, value, mask=None, dropout=None):
    # 将query矩阵的最后一个维度值作为d_k
    d_k = query.size(-1)
    # 将key的最后两个维度互换(转置)，与query矩阵相乘，除以d_k开根号
    scores = torch.matmul(query, key.transpose(-2, -1)) / math.sqrt(d_k)
    if mask is not None:
        scores = scores.masked_fill(mask==1, -1e9)
    p_attn = torch.softmax(scores, dim = -1)
    if dropout is not None:
        p_attn = dropout(p_attn)
    return torch.matmul(p_attn, value), p_attn


# query = key = value = output2
# dropout = nn.Dropout(0.1)
# padding_mask = get_padding_mask(inputs, 0)
# result = attention(query, key, value, padding_mask, dropout=dropout)
# print(result[1].shape)



class MultiHeadedAttention(nn.Module):
    def __init__(self, d_model, n_head, dropout=0.1):
        super().__init__()
        self.dropout = nn.Dropout(dropout)

        # 头的数量要能整除词向量维度
        assert d_model % n_head == 0
        self.d_k = d_model // n_head
        self.n_head = n_head
        
        # 三个线性变换，一个多头拼接之后的线性变换
        self.W_Q = nn.Linear(d_model, d_model, bias=False)
        self.W_K = nn.Linear(d_model, d_model, bias=False)
        self.W_V = nn.Linear(d_model, d_model, bias=False)
        self.linear = nn.Linear(d_model, d_model, bias=False)

        # norm
        self.norm = nn.LayerNorm(d_model)

    def forward(self, query, key, value, mask=None):
        residual = query
        # 分头
        batch_size = query.size(0)
        query = self.W_Q(query).view(batch_size, -1, self.n_head, self.d_k).transpose(1, 2)
        key = self.W_K(key).view(batch_size, -1, self.n_head, self.d_k).transpose(1, 2)
        value = self.W_V(value).view(batch_size, -1, self.n_head, self.d_k).transpose(1, 2)
        # 计算注意力
        if mask is not None:
            mask = mask.unsqueeze(1)
        context, attn = attention(query, key, value, mask, self.dropout)
        # 拼接
        concat = context.transpose(1, 2).reshape(batch_size, -1, self.n_head * self.d_k)
        output = self.linear(concat)
        return self.norm(output + residual)


# mha = MultiHeadedAttention(8, 2)
# output3 = mha(query, key, value, padding_mask)
# print(output3.shape)




class FeedForwardNet(nn.Module):
    def __init__(self, d_model, d_ff, dropout=0.1):
        super().__init__()
        self.dropout = nn.Dropout(dropout)
        self.w1 = nn.Linear(d_model, d_ff)
        self.w2 = nn.Linear(d_ff, d_model)
        self.relu = nn.ReLU()
        self.norm = nn.LayerNorm(d_model)

    def forward(self, x):
        residual = x
        x = self.relu(self.w1(x))
        x = self.dropout(self.w2(x))
        return self.norm(x + residual)

# 调用测试
# ffn = FeedForwardNet(8, 2048)
# output4 = ffn(output3)
# print(output4.shape)



class EncoderLayer(nn.Module):
    def __init__(self, d_model, n_head, d_ff, dropout=0.1):
        super().__init__()
        self.mha = MultiHeadedAttention(d_model, n_head, dropout)
        self.ffn = FeedForwardNet(d_model, d_ff, dropout)

    def forward(self, x, mask=None):
        output = self.mha(x, x, x, mask)
        return self.ffn(output)


class Encoder(nn.Module):
    def __init__(self, vocab_size, d_model, n_head, d_ff, N=6, dropout=0.1):
        super().__init__()
        self.emb = Embeddings(vocab_size, d_model)
        self.pos = PositionalEncoding(d_model, dropout)
        self.layers = nn.ModuleList([
            EncoderLayer(d_model, n_head, d_ff, dropout)
            for _ in range(N)
        ])

    def forward(self, x, mask):
        x = self.emb(x)
        x = self.pos(x)
        for layer in self.layers:
            x = layer(x, mask)
        return x


# inputs = torch.tensor([
#     [1, 2, 3],
#     [4, 5, 0],
# ])

# mask = get_padding_mask(inputs, 0)

# enc = Encoder(10, 8, 2, 32)
# output = enc(inputs, mask)
# print(output.shape)



# 参数 size 为句子长度
def get_subsequent_mask(size):
    # 1为batch维度
    mask_shape = (1, size, size)
    return 1-torch.tril(torch.ones(mask_shape)).byte()


# inputs = torch.tensor([
#     [1, 2, 3],
#     [4, 5, 0],
# ])
# pad_mask = get_padding_mask(inputs, 0)
# print(pad_mask)
# sub_mask = get_subsequent_mask(3)
# print(sub_mask)

# mask = sub_mask | pad_mask
# print(mask)


class DecoderLayer(nn.Module):
    def __init__(self, d_model, n_head, d_ff, dropout=0.1):
        super().__init__()
        self.self_mha = MultiHeadedAttention(d_model, n_head, dropout)
        self.mha = MultiHeadedAttention(d_model, n_head, dropout)
        self.ffn = FeedForwardNet(d_model, d_ff, dropout)

    def forward(self, x, mask, memory, src_mask):
        x = self.self_mha(x, x, x, mask)
        x = self.mha(x, memory, memory, src_mask)
        return self.ffn(x)



class Decoder(nn.Module):
    def __init__(self, vocab_size, d_model, n_head, d_ff, N=6, dropout=0.1):
        super().__init__()
        self.emb = Embeddings(vocab_size, d_model)
        self.pos = PositionalEncoding(d_model, dropout)
        self.layers = nn.ModuleList([
            DecoderLayer(d_model, n_head, d_ff, dropout)
            for _ in range(N)
        ])

    def forward(self, x, mask, memory, src_mask):
        x = self.emb(x)
        x = self.pos(x)
        for layer in self.layers:
            x = layer(x, mask, memory, src_mask)
        return x


# 编码部分
src_inputs = torch.tensor([
    [1, 2, 3],
    [4, 5, 0],
])
src_mask = get_padding_mask(src_inputs, 0)
encoder = Encoder(10, 8, 2, 32)
memory = encoder(src_inputs, src_mask)

# 解码部分
tgt_inputs = torch.tensor([
    [1, 2, 3, 4],
    [4, 5, 0, 0],
])
decoder = Decoder(20, 8, 2, 32)
# 处理mask
tgt_pad_mask = get_padding_mask(tgt_inputs, 0)
subsequent_mask = get_subsequent_mask(4)
tgt_mask = tgt_pad_mask | subsequent_mask

output = decoder(tgt_inputs, tgt_mask, memory, src_mask)

print(output.shape)


class Generator(nn.Module):
    def __init__(self, d_model, vocab_size):
        super().__init__()
        self.linear = nn.Linear(d_model, vocab_size)

    def forward(self, x):
        return torch.softmax(self.linear(x), dim=-1)


generator = Generator(8, 20)
predict = generator(output)

print(predict.shape)
print(torch.argmax(predict, dim=-1))



exit()

# 实例化类
emb = nn.Embedding(10, 8, padding_idx=0)

# lookup table
print(emb.weight)

# 单个词向量
print(emb(torch.tensor([1]))) # 抛

# 两个句子
print(emb(torch.tensor([
    [1, 2, 3], # 抛弃放
    [1, 5, 0], # 抛言P
])))

# 指定填充id
# emb = nn.Embedding(10, 8, padding_idx=0)




# scores = torch.randn(2, 3, 3)
# print(scores)

# inputs = torch.tensor([
#     [1, 2, 3],
#     [4, 5, 0]
# ])

# mask = (inputs == 0).unsqueeze(1).byte()
# print(mask)

# scores = scores.masked_fill(mask, 1e-9)
# print(scores)
# exit()
