require 'utilities'
require 'nn'
require 'torch'
print '==> downloading dataset'

tar = 'http://torch7.s3-website-us-east-1.amazonaws.com/data/mnist.t7.tgz'

if not paths.dirp('mnist.t7') then
   os.execute('wget ' .. tar)
   os.execute('tar xvf ' .. paths.basename(tar))
end

train_file = 'mnist.t7/train_32x32.t7'
test_file = 'mnist.t7/test_32x32.t7'

print '==> loading dataset'

trainData = torch.load(train_file,'ascii')
testData = torch.load(test_file,'ascii')

print('Training Data:')
print(trainData)
print()

print('Test Data:')
print(testData)
print()

print '==> visualizing data'

if itorch then
   print('training data:')
   itorch.image(trainData.data[{ {1,256} }])
   print('test data:')
   itorch.image(testData.data[{ {1,256} }])
else
   print('please visualize data in itorch')
end

print '==> normalizing data'

trainData.data = trainData.data:float()
testData.data = testData.data:float()
mean = trainData.data[{}]:mean()
std = trainData.data[{}]:std()

trainData.data[{}]:add(-mean)
trainData.data[{}]:div(std)
testData.data[{}]:add(-mean)
testData.data[{}]:div(std)

print '==> verify statistics'

trainMean = trainData.data[{}]:mean()
trainStd = trainData.data[{}]:std()

testMean = testData.data[{}]:mean()
testStd = testData.data[{}]:std()

print('training data ' .. trainMean)
print('training data '.. trainStd)

print('test data, '.. testMean)
print('test data, '.. testStd)


print '==> saving object'
save_obj(
	'Mnist_Data.t7',
	{
	  trainData = trainData,
	  testData = testData
	}
)



