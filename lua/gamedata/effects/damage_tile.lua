local Effect = require "game.effect"

local DamageTile = class("DamageTileEffect", Effect)
function DamageTile:perform(context)
  Effect.perform(self, context)
  local owner =    util.enforce( context.owner,    4, "owner" )
  local targets =  util.enforce( context.targets,  4, "targets" )
  local damage =   util.enforce( context.damage,   4, "damage" )

  local space = safe.get_space(context.space)
  local ent = space:get_entity(owner)
  local char_data = safe.get_component(ent, "CharacterDataComponent")
  local unit = safe.get_component(ent, "GridUnitComponent")
  
  local actions = {
    gamedata.actions.play_animation { 
      space = context.space,
      entity = owner, 
      anim = "shoot" },
    gamedata.actions.play_animation { 
      space = context.space,
      entity = owner, 
      anim = "idle", 
      play_once = false }
  }
  util.concat(context.action_list, actions)

  for _,id in pairs (targets.units) do
    
    local target = space:get_entity(id)
    
    local target_char_data = safe.get_component(target, "CharacterDataComponent")
    local target_unit = safe.get_component(target, "GridUnitComponent")
  
    local actions = {
      gamedata.actions.deal_damage { 
        space = context.space,
        target = target:get_name(), 
        damage = damage },
      gamedata.actions.print_damage { 
        space = context.space,
        att_team = unit.team, 
        att = char_data.name, 
        def_team = target_unit.team, 
        def = target_char_data.name, 
        damage = damage }
    }
    
    -- Stack with other actions
    util.concat(context.action_list, actions)
  end
end

gamedata.effects["damage_tile"] = DamageTile