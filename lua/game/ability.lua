local Ability = class("Ability")

function Ability:initialize(params) 
  self.name =             util.enforce( params.name,     4, "name" )
  self.category =         util.enforce( params.category, 4, "category" )
  self.icon =             util.default( params.icon,            nil )
  self.trigger =          util.default( params.trigger,         nil )
  self.effects =          util.default( params.effects,         {} )
  self.modifiers =        util.default( params.modifiers,       {} )
  self.targetting =       util.default( params.targetting,      nil )
  self.endturn =          util.default( params.endturn,         true )
  self.apply_modifiers =  util.default( params.apply_modifiers, true )
  -- Examples:
  -- trigger = { name = "on_use", condition = "none" }
  -- effects_template{{ name = "damage_tile", condition = { name = "is_target_mechanic" } }}
  -- targetting = "range_selection"
  -- modifiers = {{ name = "this_ability", stack = false, stats = { attack={add=2} } }}
  
  -- An effect that is added at the end if there is any modifier in context.modifiers
  self.apply_modifiers = util.default(params.apply_modifiers, true )
  
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
  -- Set context default values
  context.action_list = util.default( context.action_list, {} )
  context.modifiers = util.default( context.modifiers, {} )
  util.concat( context.modifiers, self.modifiers )
  
  if self.targetting ~= nil then
    gamedata.targetting[self.targetting](context)
  end
  
  
  -- Apply effects
  for i, effect in ipairs(self.effects) do
    effect:perform(context)
  end
  
  -- Apply modifiers
  self:add_modifiers(context) 

  safe.add_action_list( context.action_list )    
end

function Ability:add_modifiers(context)
  if self.apply_modifiers and context.targets ~= nil then
    for _,id in pairs (context.targets.units) do
      local target = safe.get_entity(id)
      local target_char_data = safe.get_component(target, "CharacterDataComponent")

      for _, mod in pairs(context.modifiers) do
        target_char_data:add_modifier( mod )
      end
    end
  end
end

return Ability

