require 'models.model_utilities'

function mlpnet(cfg)
  
  local fc_layers =  {
    { n=120, dropout=0.5, batch_norm=true },
    { n=84, dropout=0.5 }
  }
  
  return create_model(cfg, nil, fc_layers)
end

return mlpnet
