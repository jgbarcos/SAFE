local Component = require "safe.component"

local CharacterDataComponent = class ("CharacterDataComponent", Component)
    function CharacterDataComponent:initialize( params )
        Component.initialize(self, params)
        
        self.name =             util.enforce( params.name,          4, "name")
        self.base_health =      util.enforce( params.base_health,   4, "base_health")
        self.base_movement =    util.enforce( params.base_movement, 4, "base_movement")
        self.base_attack =      util.enforce( params.base_attack,   4, "base_attack")
        self.current_health =   util.default( params.current_health, params.base_health)
        self.attack_area =      util.default( params.attack_area, {} )
    end

return CharacterDataComponent