import math
import torch
from torch import nn
from torch.autograd import Variable


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


class Generator(nn.Module):
    def __init__(self, d_model, vocab_size):
        super().__init__()
        self.linear = nn.Linear(d_model, vocab_size)

    def forward(self, x):
        return self.linear(x)
        # return torch.softmax(self.linear(x), dim=-1)


class Transformer(nn.Module):
    def __init__(self, src_vocab_size, tgt_vocab_size, d_model, n_head, d_ff, N=6, dropout=0.1):
        super().__init__()
        self.encoder = Encoder(src_vocab_size, d_model, n_head, d_ff, N, dropout)
        self.decoder = Decoder(tgt_vocab_size, d_model, n_head, d_ff, N, dropout)
        self.generator = Generator(d_model, tgt_vocab_size)

    def forward(self, src_x, src_mask, tgt_x, tgt_mask):
        memory = self.encoder(src_x, src_mask)
        output = self.decoder(tgt_x, tgt_mask, memory, src_mask)
        return self.generator(output)


def make_model(src_vocab_size, tgt_vocab_size, d_model, n_head, d_ff, N=6, dropout=0.1):
    model = Transformer(src_vocab_size, tgt_vocab_size, d_model, n_head, d_ff, N, dropout)
    for p in model.parameters():
        if p.dim() > 1:
            nn.init.xavier_uniform_(p)
    return model


if __name__ == '__main__':
    src_vocab_size = 1000
    tgt_vocab_size = 2000
    d_model = 512
    n_head = 8
    d_ff = 2048
    N = 6
    dropout = 0.1

    model = make_model(src_vocab_size, tgt_vocab_size, d_model, n_head, d_ff, N, dropout)
    # print(model)

    # 输入数据
    src_inputs = torch.tensor([
        [1, 2, 3],
        [4, 5, 0],
    ])
    src_mask = get_padding_mask(src_inputs, 0)

    tgt_inputs = torch.tensor([
        [1, 2, 3, 4],
        [4, 5, 0, 0],
    ])
    # 处理mask
    tgt_pad_mask = get_padding_mask(tgt_inputs, 0)
    subsequent_mask = get_subsequent_mask(4)
    tgt_mask = tgt_pad_mask | subsequent_mask

    predict = model(src_inputs, src_mask, tgt_inputs, tgt_mask)
    print(predict.shape)
