local System = require "safe.system"

local StatsModifierSystem = class("StatsModifierSystem", System)

function StatsModifierSystem:init(entities)
  self.subscribed_events = {}
  self.received_events = {}
  
  local s = self
  function self.event_callback(event)
    s.received_events[#s.received_events+1] = event.type
  end    
end

function StatsModifierSystem:update(_, entities)   
  for _,ent in pairs(entities) do  
    local char_data = safe.get_component(ent, "CharacterDataComponent")          
          
    if char_data and char_data.update_current then
      -- Subscribe to new events
      for _,mod in pairs(char_data.modifiers) do
        if mod.event ~= nil and not self.subscribed_events[mod.event] then
          safe.subscribe(mod.event, self.event_callback)
          self.subscribed_events[mod.event] = true
        end
      end
      
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
    
    if char_data then
      -- Receive events, update event count and remove modifiers
      for _,event in pairs(self.received_events) do
        for k,mod in pairs(char_data.modifiers) do
          if mod.event == event then
            mod.event_count = mod.event_count - 1
            
            if mod.event_count <= 0 then
              char_data:remove_modifier(k)
            end
          end
        end
      end
    end
    
  end
  
  -- Reset received events
  self.received_events = {}
end


return StatsModifierSystem
