local Action = class("Action")
Action._num_calls=0
function Action:initialize(params) 
   -- Called when the instance is created
end

function Action:on_enter()
    -- Called while this action is active
end

function Action:on_update(delta)
    -- Called the on entering this action
    return true
end

function Action:on_exit()
    -- Called on exiting this action
end

return Action
