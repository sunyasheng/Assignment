require 'BatchIterator'
require 'optim'

function create_objective(cfg, model, weights, gradient, batch_iterator, stats)
  local softmax = nn.CrossEntropyCriterion()
  
  local classes = {}
  for i = 1,cfg.class_count do
    classes[i] = i
  end

  local function lossAndGradient(w)

      confusion = optim.ConfusionMatrix(classes)
      confusion:zero()

      if w ~= weights then
        weights:copy(w) 
      end
      gradient:zero()

      model:training()
      
      local batch_size = cfg.batch_size
      local data,label = batch_iterator:nextTraining(cfg.batch_size)

      local output = model:forward(data)
      local loss = softmax:forward(output, label)

      local dloss = softmax:backward(output, label)
      model:backward(data, dloss)

      gradient:div(batch_size)
      loss = loss/batch_size
      -- print(string.format('prop: cls: %f (%d), reg: %f (%d); det: cls: %f, reg: %f', 
      --   pcls, cls_count, preg, reg_count, dcls, dreg)
      -- )
      for i = 1, output:size(1) do
        confusion:add(output[i], label[i])
      end

      confusion:updateValids()

      table.insert(stats.loss, loss)
      table.insert(stats.accuracy, confusion.totalValid)
      return loss, gradient
    end
    
    return lossAndGradient
end
