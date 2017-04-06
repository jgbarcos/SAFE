local configuration = {
    -- Debug visual data
    render_physics = false,
    render_sprite_rect = false,

    -- Debug logs
    event_dispatcher_logs = false,
    texture_manager_logs = false,

    -- Initial camera settings
    camera_pos = { x=0, y=0, z=0 },
    camera_zoom = { x=3.0, y=3.0, z=1.0},

    -- Set random_seed to "-1" to seed randomly
    random_seed = -1,
}

return configuration
