local Ability = require "game.ability"

local ShootAbility = class("ShootAbility", Ability)
function ShootAbility:initialize(params)
  params =            util.default( params,             {} ) 
  params.name =       util.default( params.name,        "shoot" )
  params.category =   util.default( params.category,    "active" )
  params.targetting = util.default( params.targetting,  "relative_tiles" )
  Ability.initialize(self, params)
  
  self.damage_tile = gamedata.effects["damage_tile"]:new()
  self.effects[#self.effects+1] = self.damage_tile
end

function ShootAbility:perform(context) 
  local space = safe.get_space(context.space)
  local ent = space:get_entity(context.owner)
  local char_data = safe.get_component(ent, "CharacterDataComponent")
  context.damage = char_data.current.attack
  
  Ability.perform(self, context)
end

gamedata.abilities.classes["shoot"] = ShootAbility