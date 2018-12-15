local System = class("System")

function System:init(space) 
   -- Called when the instance is initialized
end

function System:update(delta, space)
    -- Called on each logic update
end

function System:on_enable(space)
    -- Called when the active state gets enabled
end

function System:on_disable(space)
    -- Called when the active state gets disabled
end

return System
