local Ability = class("Ability")

function Ability:initialize(params) 
  self.name =       util.enforce( params.name,     4, "name" )
  self.category =   util.enforce( params.category, 4, "category" )
  self.icon =       util.default( params.icon,         nil )
  self.trigger =    util.default( params.trigger,      nil )
  self.effects =    util.default( params.effects,      {} )
  self.targetting = util.default( params.targetting,   nil )
  -- Examples:
  -- trigger = { name = "on_use", condition = "none" }
  -- effects_template{{ name = "damage_tile", condition = { name = "is_target_mechanic" } }}
  -- targetting = "range_selection"
  
  -- Instantiate effects templates
  local ef_templates = util.default( params.effects_template, {} )
  for _,ef in pairs(ef_templates) do
    self.effects[#self.effects+1] = gamedata.effects[ef.name]:new(ef)
  end
  
end

function Ability:is_active()
  return true
end

function Ability:target()
  
  -- Call self.targetting
end

function Ability:perform(context)
  if self.targetting ~= nil then
    gamedata.targetting[self.targetting](context)
  end
  
  context.action_list = {}
  
  for i, effect in ipairs(self.effects) do
    effect:perform(context)
  end
  
  safe.add_action_list( context.action_list )    
end

return Ability

