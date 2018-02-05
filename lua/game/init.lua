-- Safe functions
safe.set_active_systems = function ( systems, value )
  for _,s in pairs(systems) do
    safe.get_system(s).active = value
  end
end

-- Game data
gamedata = { 
  abilities = {
    classes = {},
    templates = {}
  },  
  effects = {},
  triggers = {},
  conditions = {},
  targetting = {},
  actions = {}
}

local Ability = require "game.ability"

function gamedata.abilities.get( name ) 
  if gamedata.abilities.templates[name] ~= nil then
    return Ability:new( gamedata.abilities.templates[name] )
  elseif gamedata.abilities.classes[name] ~= nil then
    return gamedata.abilities.classes[name]:new()
  end  
  print ("error: ability ".. name .. "does not exist in gamedata")
end

function gamedata.reload_all()
  util.load( "gamedata.abilities.cripple_shoot" )
  util.load( "gamedata.abilities.shoot" )
  util.load( "gamedata.abilities.talk" )
  util.load( "gamedata.abilities.weakness_detection" )
  
  util.load( "gamedata.actions.deal_damage" )
  util.load( "gamedata.actions.delay" )
  util.load( "gamedata.actions.play_animation" )
  util.load( "gamedata.actions.print_console" )
  util.load( "gamedata.actions.print_damage" )
  util.load( "gamedata.actions.push_unit" )

  util.load( "gamedata.effects.dialog" )
  util.load( "gamedata.effects.add_ability" )
  util.load( "gamedata.effects.damage_tile" )
  util.load( "gamedata.effects.edit_context" )
  util.load( "gamedata.effects.push_horizontal" )

  util.load( "gamedata.targetting.relative_tiles" )
  util.load( "gamedata.targetting.self" )
end

gamedata.reload_all()