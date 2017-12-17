local System = require "safe.system"

local StatsModifierSystem = class("StatsModifierSystem", System)

function StatsModifierSystem:update(_, entities)   
  
  for _,ent in pairs(entities) do  
    local char_data = safe.get_component(ent, "CharacterDataComponent")
          
    if char_data and char_data.update_current then
      
      -- Keep health
      local health = util.default(char_data.current.health, char_data.base.health)
      
      -- Set stats to default
      for k,v in pairs(char_data.base) do
        char_data.current[k] = v
      end
      
      -- Set additive modifiers
      for _,mod in pairs(char_data.modifiers) do
        for att,changes in pairs(mod.stats) do
          char_data.current[att] = char_data.current[att] + changes.add
        end
      end
      
      -- Set multiplicative modifiers
      for _,mod in pairs(char_data.modifiers) do
        for att,changes in pairs(mod.stats) do
          char_data.current[att] = char_data.current[att] * changes.mult
        end
      end
      
      -- Recover health stat
      char_data.current.health = health
      
    end
  end
end


return StatsModifierSystem
