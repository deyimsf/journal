import torch
import os

BASE_PATH = os.path.dirname(__file__)

TRAIN_SAMPLE_PATH = os.path.join(BASE_PATH, './datas/inputs/news/train.json')
DEV_SAMPLE_PATH = os.path.join(BASE_PATH, './datas/inputs/news/dev.json')

ZH_VOCAB_PATH = os.path.join(BASE_PATH, './datas/vocab/zh.txt')
EN_VOCAB_PATH = os.path.join(BASE_PATH, './datas/vocab/en.txt')

PAD_ID = 0
UNK_ID = 1
SOS_ID = 2
EOS_ID = 3

D_MODEL = 512
N_HEAD = 8
D_FF = 2048
N = 6
DROPOUT = 0.1

BATCH_SIZE = 20
BATCH_SIZE_GPU0 = 5
LR = 1e-5
EPOCH = 1000

MAX_LEN = 50

LABEL_SMOOTHING = 0.1

MODEL_DIR = os.path.join(BASE_PATH, './datas/checkpoints')

DEVICE = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

MULTI_GPU = False
if torch.cuda.device_count()>1:
    MULTI_GPU = True

if __name__ == '__main__':
    print(DEVICE)
    print('MULTI_GPU:', MULTI_GPU)
