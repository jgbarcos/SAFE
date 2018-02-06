local System = class("System")

function System:init(entities) 
   -- Called when the instance is initialized
end

function System:update(delta, entities)
    -- Called on each logic update
end

function System:on_enable()
    -- Called when the active state gets enabled
end

function System:on_disable()
    -- Called when the active state gets disabled
end

return System
