local BatchIterator = torch.class('BatchIterator')

function BatchIterator:__init(cfg, training_data, testing_data)
  self.cfg = cfg
  self.current = 1
  self.len = training_data.data:size(1)
  self.data = training_data.data
  self.label = training_data.labels
  self.testdata = testing_data.data
  self.testlabel = testing_data.labels
  -- self.data = torch.Tensor(training_data.data:size())
  -- local len = training_data.data:size(1)

  -- local shuffle = torch.randperm(len)
  -- for i=1,len do
  --   self.data[i] = training_data.data[shuffle[i]]
  -- end
end

function BatchIterator:nextTraining(count)
  local cfg = self.cfg
  count = count or cfg.batch_size

  if self.current + count - 1 > self.len then
    self.current = 1
  end

  local start = self.current
  local final = self.current + count - 1

  self.current = final + 1
  return self.data[{{start, final}}], self.label[{{start, final}}]
end

function BatchIterator:testingData(count)
  return self.testdata, self.testlabels
end
