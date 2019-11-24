import cfg
import torch
from torch import nn

class LSTMnet(nn.Module):
    def __init__(self):
        super(LSTMnet, self).__init__()

        self.rnn = nn.LSTM(
            input_size = 310,
            hidden_size = cfg.lstm_hidden_size,    
            num_layers = cfg.lstm_num_layers,
            batch_first = True,
        )
        self.drop = nn.Dropout(cfg.drop_out_probability)
        self.out = nn.Linear(cfg.lstm_hidden_size,3)

    def forward(self, x):
        rout, (h, c) = self.rnn(x)
        drp = self.drop(rout[:, -1, :])
        out = self.out(drp)
        return out

if __name__ == '__main__':
    lstm = LSTMnet()
    print(lstm)