local Effect = require "game.effect"

local PushHorizontal = class("PushHorizontal", Effect)
function PushHorizontal:initialize(params)
  self.tiles = util.default( params.tiles, 1 )
end

function PushHorizontal:perform(context)
  Effect.perform(self, context)
  local owner =    util.enforce( context.owner,    4, "owner" )
  local targets =  util.enforce( context.targets,  4, "targets" )
  
  local space = safe.get_space(context.space)
  local ent = space:get_entity(owner)
  local unit = ent:get_component("GridUnitComponent")

  for _,id in pairs (targets.units) do
    local target = space:get_entity(id)
    local target_unit = safe.get_component(target, "GridUnitComponent")
    
    local diff = unit.x - target_unit.x
    
    local move = self.tiles
    if( unit.x < target_unit.x ) then
      move = -move
    end
    
    local actions = {
      gamedata.actions.push_unit { 
        space = context.space,
        target = target:get_name(), 
        x = move}
    }
    
    -- Stack with other actions
    util.concat(context.action_list, actions)
  end
end

gamedata.effects["push_horizontal"] = PushHorizontal