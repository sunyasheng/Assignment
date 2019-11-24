require 'torch'
require 'pl'
require 'optim'
require 'image'
require 'nngraph'
require 'gnuplot'

require 'utilities'
require 'BatchIterator'
-------------------------------------------------------------------------
print '==> processing options'

cmd = torch.CmdLine()

cmd:text('Options:')

cmd:option('-seed', 1, 'fixed input seed for repeatable experiments')
cmd:option('-threads', 2, 'number of threads')

cmd:option('-cfg', 'config/lenet_cfg.lua', 'configuration file')
cmd:option('-model', 'models/lenet.lua', 'model factory file')
cmd:option('-train', 'Mnist_Data.t7', 'training data file name')
cmd:option('-type', 'float', 'type: double | float | cuda')
cmd:option('-restore', 'Mnist_006000.t7', 'network snapshot file name to load')

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
function plot_feature( feature, prefix )
	local fn = prefix .. '.png'
	gnuplot.pngfigure(fn)
	
	local row_num = math.floor(math.sqrt(feature:size(1)))
	local colunm_num = feature:size(1)/row_num
	colunm_num = math.floor(colunm_num)
	if feature:size(1) - row_num*colunm_num > 0 then
		colunm_num = colunm_num + 1
	end
	local features = torch.Tensor(row_num*(feature:size(2)+2) -2, colunm_num*(feature:size(3)+2) - 2)
	features:zero()
	for i = 1, feature:size(1) do
		local start_x = math.floor(i/colunm_num) + 1

		if i %colunm_num == 0 then
			start_x = start_x - 1
		end
		local start_y = math.floor(i - (start_x -1)*colunm_num)
		start_x = (start_x - 1) * (feature:size(2) + 2) + 1
		start_y = (start_y - 1) * (feature:size(2) + 2) + 1
		-- print('start_x = ' .. start_x .. ' start_y =' .. start_y .. ' i = ' .. i)
		for j = 1, feature:size(2) do
			for k = 1, feature:size(3) do
				features[{start_x + j - 1, start_y + k - 1}] = feature[{i, j, k}]
			end
		end
	end

	gnuplot.imagesc(features,'gray')
	gnuplot.plotflush()
end

function feature_visualzation( cfg, training_data_filename, model_path, network_filename )
	local model_factory = dofile(model_path)
	local model = model_factory(cfg)

	local weights, gradient = combine_and_flatten_parameters(model)

	if network_filename and #network_filename > 0 then
		local stored = load_obj(network_filename)
		weights:copy(stored.weights)
	end

	model:evaluate()
	print('Reading data file \'' .. training_data_filename .. '\'.')
	local Data = load_obj(training_data_filename)
	local input = Data.trainData.data[{{3}}]
	local label = Data.trainData.labels[{3}]
	local output = model:forward(input)

	for k,v in pairs(model:findModules('nn.SpatialConvolution')) do
		plot_feature(v.output[1],string.format('feature_%2d_label%2d', k, label))
		-- for i =1, v.output[1]:size(1) do
		-- 	plot_feature(v.output[{1,i,{},{}}],string.format('layer_%d_feature_%d',k, i))
		-- end
	end
end

feature_visualzation( cfg, opt.train, opt.model, opt.restore )

















