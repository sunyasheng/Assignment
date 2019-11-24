require 'torch'
require 'pl'
require 'optim'
require 'image'
require 'nngraph'
require 'gnuplot'

require 'utilities'
require 'BatchIterator'

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
cmd:option('-restore', 'saved_lenet_models/Mnist_001000.t7', 'network snapshot file name to load')
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

------------------------------------------------------------------------

function plot_accuracy( res )
	local fn = 'test_images/lenet_testAccuracy.png'
	gnuplot.pngfigure(fn)
	gnuplot.title('Test Accuracy')

	local xs = torch.range(1, #res)
	-- xs = xs*1000

	gnuplot.plot(
	{'accuracy', xs, torch.Tensor(res), '-'}
	)
	gnuplot.axis({0, #res + 1, 50, 100})
	gnuplot.xlabel('iteration/ x1000')
	gnuplot.ylabel('accuracy')
end

function evaluate( cfg, training_data_filename, model_path, network_filename )
	local res = {}
	local Data = load_obj(training_data_filename)
	local batch_iterator = BatchIterator.new(cfg, Data.trainData, Data.testData)

	local model_factory = dofile(model_path)
	local model = model_factory(cfg)

	local weights, gradient = combine_and_flatten_parameters(model)
	local test_data, test_label = batch_iterator:nextTraining()

	local classes = {}
	for i = 1,cfg.class_count do
		classes[i] = i
	end

	for i = 1, 6 do
		local fn = string.gsub(network_filename, 1, i)
		if fn and #fn > 0 then
			local stored = load_obj(fn)
			weights:copy(stored.weights)
		end
		model:evaluate()
	
		local timer = torch.Timer()
		local time = timer:time().real


		local confusion = optim.ConfusionMatrix(classes)
		confusion:zero()

		local output = model:forward(test_data)
		for i = 1, output:size(1) do
			confusion:add(output[i], test_label[i])
		end

		confusion:updateValids()
		table.insert(res, confusion.totalValid*100)

		print('Accuracy in test data is ' .. confusion.totalValid*100 .. '%')
		print('Time consuption is ' .. (timer:time().real - time))
	end

	plot_accuracy(res)
end

evaluate(cfg, opt.train, opt.model, opt.restore)










