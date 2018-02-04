local Action = require "safe.action"

local PushUnit = class("PushUnit", Action)
function PushUnit:initialize(params)
    Action.initialize(self, params)
    
    self.target = util.enforce( params.target, 4, "target entity")
    self.x = util.default( params.x, 0)
    self.y = util.default( params.y, 0)
end

function PushUnit:on_enter()
  -- Simple push without any interaction (just checks if the pos is available)
  local ptr = safe.get_entity(self.target)
  local unit = ptr:get_component("GridUnitComponent")
  
  if(#safe.get_entities_at(unit.x+self.x, unit.y+self.y) == 0) then
    unit.x = unit.x+self.x
    unit.y = unit.y+self.y
  end
  
  
  
end

gamedata.actions["push_unit"] = PushUnit