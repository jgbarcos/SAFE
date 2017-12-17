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

require "gamedata.abilities.ab_cripple_shoot"
require "gamedata.abilities.ab_shoot"
require "gamedata.abilities.ab_talk"
require "gamedata.abilities.ab_weakness_detection"

require "gamedata.abilities.ef_dialog"
require "gamedata.abilities.ef_damage_tile"
require "gamedata.abilities.ef_edit_context"

require "gamedata.abilities.ta_relative_tiles"
require "gamedata.abilities.ta_self"