lstm_num_layers = 1
time_step = 16
trainset_size = 9
batch_size = 64
echo_per_epoch = 5
log_interval = 10
num_epoch = 30
use_tensorboard = 1
is_cuda = True
is_individual = True
num_parameters = 200
TRAIN_DIR = '../train'

weight_decay = 0.05
drop_out_probability = 0.5
lstm_hidden_size = 16
learning_rate = 0.005

max_weight_decay = 0
max_drop_out_probability = 0.99
max_lstm_hidden_size = 256
max_learning_rate = -4

min_weight_decay = -9
min_drop_out_probability = 0.3
min_lstm_hidden_size = 16
min_learning_rate = -1.5