function error_handle( error_msg )
    return "error on lua: " .. error_msg
end

local configuration = {
    -- Debug data
    render_physics = false,
    render_sprite_rect = false,

    
    lua_error_handling = error_handle
}

return configuration
