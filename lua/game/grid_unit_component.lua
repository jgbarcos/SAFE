local Component = require "safe.component"

local GridUnitComponent = class ("GridUnitComponent", Component)
    function GridUnitComponent:initialize( params )
        Component.initialize(self, params)
        
        self.x =        util.enforce( params.x,     4, "x" )
        self.y =        util.enforce( params.y,     4, "y" )
        self.team =     util.enforce( params.team,  4, "team" )
        self.can_move = util.default( params.can_move, true )
    end

return GridUnitComponent