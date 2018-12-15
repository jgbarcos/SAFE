local Action = require "safe.action"

local PushUnit = class("PushUnit", Action)
function PushUnit:initialize(params)
    Action.initialize(self, params)
    
    self.target = util.enforce( params.target, 4, "target entity")
    self.movex = util.default( params.x, 0)
    self.movey = util.default( params.y, 0)
end

function PushUnit:on_enter()
  -- Simple push without any interaction (just checks if the pos is available)
  local space = safe.get_space(self.space)
  local ptr = space:get_entity(self.target)
  local unit = ptr:get_component("GridUnitComponent")
  
  local x = unit.x + self.movex
  local y = unit.y + self.movey
  if(safe.check_bounds(x, y) and #safe.get_entities_at(x, y) == 0) then
    unit.x = x
    unit.y = y
  end
  
end

gamedata.actions["push_unit"] = PushUnit