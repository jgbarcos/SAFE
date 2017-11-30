local Component = require "safe.component"

local AbilitiesComponent = class ("AbilitiesComponent", Component)
    function AbilitiesComponent:initialize( params )
        Component.initialize(self, params)
        
        self.ability_names =  util.default( params.ability_names, {})
        self.abilities = util.default( params.abilities, {} )
        
        -- Instantiate abilities
        self.abilities = {}
        for _,name in pairs(self.ability_names) do
            self.abilities[name] = gamedata.get_ability(name)
        end
                
    end

return AbilitiesComponent