local Component = require "safe.component"

local ExplorationEventComponent = class ("ExplorationEventComponent", Component)
    function ExplorationEventComponent:initialize( params )
        Component.initialize(self, params)
        
        self.x =        util.enforce( params.x,     4, "x" )
        self.y =        util.enforce( params.y,     4, "y" )
        self.event =    util.enforce( params.event, 4, "event name")
        self.activated = false
    end

return ExplorationEventComponent