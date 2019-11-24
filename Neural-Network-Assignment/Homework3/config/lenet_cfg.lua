 local lenet_cfg = {
  class_count = 10,  -- excluding background class
  augmentation = { vflip = 0, hflip = 0.25, random_scaling = 0, aspect_jitter = 0 },
  batch_size = 1000,
}

return lenet_cfg