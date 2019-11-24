Table of Contents
==============
— Reference
— Quick Start
- 'main' Usage

Reference
=========
This project borrows tons of code from https://github.com/andreaskoepf/faster-rcnn.torch.

Quick Start
===========
Run 'create-minist-traindata.lua' to do the data preparation. After that run 'main.lua' to train the model from scratch. Run 'feature-visualization.lua' to do feature visualization. To test the performance of this model, run 'test.lua'.

'main' Usage
============
Usage: th main.lua [options]
optinos:
-cfg filename: set the locatino of configuration file (default config/lenet_cfg.lua)
		config/lenet_cfg.lua -- for lenet model
		config/mlpnet_cfg.lua -- for feedforward network
-model filename: set the model filename (default models/lenet.lua)
		models/lenet.lua -- for lenet model
		models/mlpnet.lua -- for feedforward network
-restore filename: set the loaded model (default ' ')
		' ' -- train from scratch
		'saved_lenet_models/Mnist_006000.t7' -- load the pretrained model
