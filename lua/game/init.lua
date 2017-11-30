-- Componets
safe.register_component( require "game.character_data_component" )
safe.register_component( require "game.grid_unit_component" )
safe.register_component( require "game.abilities_component" )

-- Systems
safe.register_system( require "game.damage_phase_system" )

-- Game data
gamedata = { 
  abilities_class = {},
  abilities_template = {},
  effects = {},
  triggers = {},
  conditions = {},
  targetting = {}
}

local Ability = require "game.ability"

function gamedata.get_ability( name ) 
  if gamedata.abilities_template[name] ~= nil then
    return Ability:new( gamedata.abilities_template[name] )
  elseif gamedata.abilities_class[name] ~= nil then
    return gamedata.abilities_class[name]:new()
  end  
  print ("error: ability ".. name .. "does not exist in gamedata")
  
end

require "gamedata.ability_shoot"
require "gamedata.ability_talk"
require "gamedata.effect_dialog"
require "gamedata.effect_damage_tile"
require "gamedata.trigger_on_use"
require "gamedata.targetting_relative_fixed"