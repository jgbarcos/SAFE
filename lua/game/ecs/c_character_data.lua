local Component = require "safe.component"
local Modifier = require "game.modifier"

local CharacterDataComponent = class ("CharacterDataComponent", Component)
    function CharacterDataComponent:initialize( params )
        Component.initialize(self, params)
        
        self.name =   util.enforce( params.name,          4, "name")
        
        self.base = {
          health =    util.enforce( params.health,   4, "health"),
          movement =  util.enforce( params.movement, 4, "movement"),
          attack =    util.enforce( params.attack,   4, "attack")
        }
        
        self.current = util.default( params.current, {} )
        
        -- Fill with base stats 
        for k,v in pairs(self.base) do
          self.current[k] = util.default( self.current[k], v )
        end
        
        self.attack_area =    util.default( params.attack_area, {} )
        self.modifiers =      util.default( params.modifiers, {} )
        self.update_current = util.default( params.update_current, true)
    end
    
    function CharacterDataComponent:add_modifier( modifier_table )
      local new_modifier = Modifier:new(modifier_table)
      local name = new_modifier.name
      
      if self.modifiers[name] ~= nil
        and self.modifiers[name].can_stack and new_modifier.can_stack then
          self.modifiers[name]:stack(new_modifier)
      else
        self.modifiers[name] = new_modifier
      end
      self.need_update = true
    end
    
    function CharacterDataComponent:remove_modifier( id )
      self.modifiers[id] = nil
      self.need_update = true
    end

return CharacterDataComponent