import numpy as np
import cfg
import torch
import torch.utils.data as Data
from torch.autograd import Variable
from LSTMnet import LSTMnet
from torch import nn
import os

torch.manual_seed(1)

try:
    from tensorboardX import SummaryWriter
except ImportError:
    SummaryWriter = None

use_tensorboard = cfg.use_tensorboard and SummaryWriter is not None
if use_tensorboard:
    summary_writer = SummaryWriter(os.path.join(cfg.TRAIN_DIR, 'runs'))
else:
    summary_writer = None


data_filename = '../data_used/0{}.npz'
label_filename = '../data_used/label.npy'

array_data = np.load(label_filename)

trainset_x, trainset_y, testset_x, testset_y = [], [],[],[]
trainset_tot, trainset_mean, trainset_aver = [], [], []

if cfg.is_individual:
    individual_num = 1
else:
    individual_num = 3

for num in range(1, individual_num + 1):
    zip_data = np.load(data_filename.format(num))
    files_in_zip = zip_data.keys()
    for i in range(len(files_in_zip)):
        fn = files_in_zip[i]
        tmp = np.asarray(zip_data[fn])
        tmp = tmp.transpose(1,0,2)
        tmp = tmp.reshape(tmp.shape[0],-1)

        if i < cfg.trainset_size: 
            trainset_tot.append(tmp)

trainset_tot = np.vstack(trainset_tot)
trainset_mean = np.mean(trainset_tot, 0)
trainset_std = np.std(trainset_tot, 0)

for num in range(1, individual_num):
    zip_data = np.load(data_filename.format(num))
    files_in_zip = zip_data.keys()
    for i in range(len(files_in_zip)):
        fn = files_in_zip[i]
        tmp = np.asarray(zip_data[fn])
        tmp = tmp.transpose(1,0,2)
        tmp = tmp.reshape(tmp.shape[0],-1)
        tmp = np.divide(tmp - trainset_mean, trainset_std)

        for j in range(0, len(tmp),cfg.time_step):
            if j + cfg.time_step - 1 <= len(tmp) - 1:
                if i < cfg.trainset_size:
                    trainset_x.append(tmp[j:j+cfg.time_step, :])
                    trainset_y.append(array_data[i])
                else:
                    testset_x.append(tmp[j:j+cfg.time_step, :])
                    testset_y.append(array_data[i])

X = torch.from_numpy(np.asarray(trainset_x)).type(torch.FloatTensor)
Y = torch.from_numpy(np.asarray(trainset_y)).type(torch.LongTensor)

torch_dataset = Data.TensorDataset(X, Y)

test_x = Variable(torch.from_numpy(np.asarray(testset_x)),volatile = True).type(torch.FloatTensor)
if cfg.is_cuda:
    test_x = test_x.cuda()
test_y = np.asarray(testset_y)

loader = Data.DataLoader(
    dataset = torch_dataset,   
    batch_size = cfg.batch_size,    
    shuffle = True,            
    num_workers = 4,            
)

accuracy = []

for i_parameter in range(cfg.num_parameters):
    cfg.lstm_hidden_size = np.random.randint(cfg.min_lstm_hidden_size, cfg.max_lstm_hidden_size)
    cfg.drop_out_probability = np.random.uniform(cfg.min_drop_out_probability, cfg.max_drop_out_probability)
    cfg.weight_decay = np.random.uniform(cfg.min_weight_decay, cfg.max_weight_decay)
    cfg.weight_decay = np.power(10, cfg.weight_decay)
    cfg.learning_rate = np.random.uniform(cfg.min_learning_rate, cfg.max_learning_rate)
    cfg.learning_rate = np.power(10, cfg.learning_rate)

    net = LSTMnet()
    if cfg.is_cuda:
        net.cuda()
    optimizer = torch.optim.Adam(net.parameters(), lr = cfg.learning_rate, weight_decay = cfg.weight_decay)
    loss_func = nn.CrossEntropyLoss()

    for epoch in range(cfg.num_epoch):
        for batch_x, batch_y in loader:
            bx = Variable(batch_x)
            by = Variable(batch_y)
            if cfg.is_cuda:
                bx = bx.cuda()
                by = by.cuda()
            net.train()
            output = net(bx)
            loss = loss_func(output, by)
        
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

    net.eval()
    output_test = net(test_x)
    pred_y = torch.max(output_test, 1)[1].data.cpu().numpy().squeeze()
    accuracy.append(sum(pred_y == test_y)/(float(len(test_y))))
    print('Parameter set: %d | Learning rate: %.5f, Weight decay: %.5f, Dropout: %.5f, Hidden size: %d | Accuracy %.05f '%(
        i_parameter, cfg.learning_rate, cfg.weight_decay, cfg.drop_out_probability, cfg.lstm_hidden_size, accuracy[-1]))
print(np.mean(accuracy))
