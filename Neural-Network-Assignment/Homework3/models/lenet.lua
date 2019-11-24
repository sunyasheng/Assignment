require 'models.model_utilities'

function lenet(cfg)
  local conv_layers = { 
    { filters= 6, kW=5, kH=5, padW=0, padH=0, dropout=0.0},
    { filters=16, kW=5, kH=5, padW=0, padH=0, dropout=0.4}
  }
  
  local fc_layers =  {
    { n=120, dropout=0.5, batch_norm=true },
    { n=84, dropout=0.5 }
  }
  
  return create_model(cfg, conv_layers, fc_layers)
end

return lenet
