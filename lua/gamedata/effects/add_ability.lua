local Effect = require "game.effect"

local AddAbility = class("AddAbility", Effect)
function AddAbility:initialize(params)
  self.ability = util.enforce( params.ability,   4, "ability" )
end

function AddAbility:perform(context)
  Effect.perform(self, context)
  local targets =  util.enforce( context.targets,  4, "targets" )

  local space = safe.get_space(context.space)

  for _,id in pairs (targets.units) do
    
    local target = space:get_entity(id)
    
    local target_char_data = safe.get_component(target, "CharacterDataComponent")
    local target_unit = safe.get_component(target, "GridUnitComponent")
    local target_abilities = safe.get_component(target, "AbilitiesComponent")
    target_abilities:add_ability(self.ability)
  end
end

gamedata.effects["add_ability"] = AddAbility