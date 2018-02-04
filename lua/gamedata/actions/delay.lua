local Action = require "safe.action"

local Delay = class("DelayAction", Action)

function Delay:initialize( params )
  Action.initialize(self, params)

  self.time = util.default( params.time, 0 )
end

function Delay:on_update(delta)
  self.time = self.time - delta
  return self.time < 0
end

gamedata.actions["delay"] = Delay

