require 'torch'
require 'pl'
require 'optim'
require 'image'
require 'nngraph'
require 'gnuplot'

require 'utilities'
require 'BatchIterator'
require 'objective'

----------------------------------------------------------------------
print '==> processing options'

cmd = torch.CmdLine()

cmd:text('Options:')

cmd:option('-seed', 1, 'fixed input seed for repeatable experiments')
cmd:option('-threads', 2, 'number of threads')

cmd:option('-cfg', 'config/lenet_cfg.lua', 'configuration file')
cmd:option('-train', 'Mnist_Data.t7', 'training data file name')
cmd:option('-model', 'models/lenet.lua', 'model factory file')
cmd:option('-name', 'Mnist', 'experiment name, snapshot prefix') 
cmd:option('-restore', '', 'snapshot file name')
cmd:option('-snapshot', 1000, 'snapshot interval')
cmd:option('-plot', 100, 'plot training progress interval')
cmd:option('-lr', 1E-4, 'learn rate')
cmd:option('-rms_decay', 0.9, 'RMSprop moving average dissolving factor')
cmd:option('-type', 'float', 'type: double | float | cuda')
cmd:text()
opt = cmd:parse(arg or {})

-- nb of threads and fixed seed (for repeatable experiments)
if opt.type == 'float' then
   print('==> switching to floats')
   torch.setdefaulttensortype('torch.FloatTensor')
elseif opt.type == 'cuda' then
   print('==> switching to CUDA')
   require 'cunn'
   torch.setdefaulttensortype('torch.FloatTensor')
end
torch.setnumthreads(opt.threads)
torch.manualSeed(opt.seed)

print('Command line args:')
local opt = cmd:parse(arg or {})
print(opt)

print('Options:')
local cfg = dofile(opt.cfg)
print(cfg)

----------------------------------------------------------------------
function plot_training_progress(prefix, stats)
  local fn = prefix .. '_loss.png'
  gnuplot.pngfigure(fn)
  gnuplot.title('Traning progress over time')
  
  local xs = torch.range(1, #stats.loss)
  
  gnuplot.plot(
    { 'loss', xs, torch.Tensor(stats.loss), '-' }
    )
 
  gnuplot.axis({ 0, #stats.loss, 0, 0.004 })
  gnuplot.xlabel('iteration')
  gnuplot.ylabel('loss')
  
  gnuplot.plotflush()

  fn = prefix .. '_accuracy.png'
  gnuplot.pngfigure(fn)
  gnuplot.title('Traning progress over time')
  
  local xs = torch.range(1, #stats.accuracy)
  
  gnuplot.plot(
    { 'accuracy', xs, torch.Tensor(stats.accuracy), '-' }
    )
 
  gnuplot.axis({ 0, #stats.accuracy, 0, 1 })
  gnuplot.xlabel('iteration')
  gnuplot.ylabel('accuracy')
  
  gnuplot.plotflush()

end

function load_model(cfg, model_path, network_filename)

  -- get configuration & model
  local model_factory = dofile(model_path)
  local model = model_factory(cfg)
  
  -- combine parameters from pnet and cnet into flat tensors
  local weights, gradient = combine_and_flatten_parameters(model)
  local training_stats
  if network_filename and #network_filename > 0 then
    local stored = load_obj(network_filename)
    training_stats = stored.stats
    weights:copy(stored.weights)
  end

  return model, weights, gradient, training_stats
end

function graph_training(cfg, model_path, snapshot_prefix, training_data_filename, network_filename)
	-- create/load model
	local model, weights, gradient, training_stats = load_model(cfg, model_path, network_filename)

	if not training_stats then
	training_stats = { loss={}, accuracy = {} }
	end
  
  print('Reading training data file \'' .. training_data_filename .. '\'.')
  local Data = load_obj(training_data_filename)
  print(string.format("Training data loaded -- Mnist Dataset. Total training files: %d; Total testing files: %d; classes: %d", 
    Data.trainData.data:size(1),
    Data.testData.data:size(1),
    10))
	local batch_iterator = BatchIterator.new(cfg, Data.trainData, Data.testData)
	local eval_objective_grad = create_objective(cfg, model, weights, gradient, batch_iterator, training_stats)

	local rmsprop_state = { learningRate = opt.lr, alpha = opt.rms_decay }
  -- local sgd_state = { learningRate = opt.lr, weightDecay = 0.0005, momentum = 0.9 }
  	print('Training Begin')
	for i=1,50000 do
		if i % 5000 == 0 then
		  opt.lr = opt.lr / 2
		  rmsprop_state.lr = opt.lr
		end

		local timer = torch.Timer()
		local _, loss = optim.rmsprop(eval_objective_grad, weights, rmsprop_state)

		local time = timer:time().real

		-- print(string.format('%d: loss: %f', i, loss[1]))

		if i%opt.plot == 0 then
		  plot_training_progress(snapshot_prefix, training_stats)
		end

		if i%opt.snapshot == 0 then
		  save_model(string.format('%s_%06d.t7', snapshot_prefix, i), weights, opt, training_stats)
		end
	end
	-- feature_visulize(model)
end


graph_training(cfg, opt.model, opt.name, opt.train, opt.restore)






