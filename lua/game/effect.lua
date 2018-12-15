local Effect = class("Effect")

function Effect:initialize(params)
  
end

function Effect:perform(context)
  util.enforce(context.space, 5, "space")
end

return Effect
