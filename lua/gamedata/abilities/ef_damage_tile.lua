local Effect = require "game.effect"

local DamageTile = class("DamageTileEffect", Effect)
function DamageTile:perform(context)  
  local owner =    util.enforce( context.owner,    4, "owner" )
  local targets =  util.enforce( context.targets,  4, "targets" )
  local damage =   util.enforce( context.damage,   4, "damage" )
  
  local ent = safe.get_entity(owner)
  local char_data = safe.get_component(ent, "CharacterDataComponent")
  local unit = safe.get_component(ent, "GridUnitComponent")
  
  local actions = {
    game.actions.PlayAnimation { 
      entity = owner, 
      anim = "shoot" },
    game.actions.PlayAnimation { 
      entity = owner, 
      anim = "idle", 
      play_once = false }
  }
  util.concat(context.action_list, actions)

  for _,id in pairs (targets.units) do
    
    local target = safe.get_entity(id)
    
    local target_char_data = safe.get_component(target, "CharacterDataComponent")
    local target_unit = safe.get_component(target, "GridUnitComponent")
  
    local actions = {
      game.actions.DealDamage { 
        target = target:get_name(), 
        damage = damage },
      game.actions.PrintDamage { 
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