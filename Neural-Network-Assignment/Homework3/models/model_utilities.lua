function create_model(cfg, conv_layers, fc_layers)
  if conv_layers then

    local conv_net = nn.Sequential()
    local prev_input_count = 1

    for i,l in ipairs(conv_layers) do
      conv_net:add(nn.SpatialConvolution(prev_input_count, l.filters, l.kW, l.kH, 1, 1, l.padW, l.padH))
      conv_net:add(nn.PReLU())
      if l.dropout and l.dropout > 0 then
        conv_net:add(nn.SpatialDropout(l.dropout))
      end
      conv_net:add(nn.SpatialMaxPooling(2, 2, 2, 2):ceil())
      prev_input_count = l.filters
    end

    local input = nn.Identity()()
    local node = conv_net(input)

    local fc = nn.Sequential()
    fc:add(nn.View(-1):setNumInputDims(3))

    prev_input_count = 400 -- need to be precomputed
    for i,l in ipairs(fc_layers) do
      fc:add(nn.Linear(prev_input_count, l.n))
      if l.batch_norm then
        fc:add(nn.BatchNormalization(l.n))
      end
      fc:add(nn.PReLU())
      if l.dropout and l.dropout > 0 then
        fc:add(nn.Dropout(l.dropout))
      end
      prev_input_count = l.n
    end
    fc:add(nn.Linear(prev_input_count, cfg.class_count))
    local output = fc(node)
    nngraph.annotateNodes()

    local model = nn.gModule({ input }, { output })

    local function init(module, name)
      local function init_module(m)
        for k,v in pairs(m:findModules(name)) do
          local n = v.kW * v.kH * v.nOutputPlane
          v.weight:normal(0, math.sqrt(2 / n))
          v.bias:zero()
        end
      end
      module:apply(init_module)
    end

    init(model, 'nn.SpatialConvolution')
    return model

  elseif fc_layers then

    local input = nn.Identity()()
    local fc = nn.Sequential()
    fc:add(nn.View(-1):setNumInputDims(3))

    local prev_input_count = 32*32
    for i,l in ipairs(fc_layers) do
      fc:add(nn.Linear(prev_input_count, l.n))
      if l.batch_norm then
        fc:add(nn.BatchNormalization(l.n))
      end
      fc:add(nn.PReLU())
      if l.dropout and l.dropout > 0 then
        fc:add(nn.Dropout(l.dropout))
      end
      prev_input_count = l.n
    end
    fc:add(nn.Linear(prev_input_count, cfg.class_count))
    local output = fc(input)
    local model = nn.gModule({input}, {output})
    graph.dot(model.fg, 'model', 'model')
    return model

  end
  -- nngraph.setDebug(true)
  -- local input = torch.Tensor(5,1,32,32)
  -- model:forward(input)

end
