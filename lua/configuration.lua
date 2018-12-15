local configuration = {
    -- Debug visual data
    render_physics = false,
    render_sprite_rect = false,

    -- Debug logs
    event_dispatcher_logs = false,
    texture_manager_logs = false,

    -- Initial camera settings
    camera_pos = { x=0, y=-24, z=0 },
    camera_zoom = { x=3.0, y=3.0, z=1.0},

    -- Set random_seed to "-1" to seed randomly
    random_seed = -1,
    
    --initial_scene = "scenes/event_exploration",
    initial_scene = "scenes/combat",
  
    default_font = "assets/fonts/Roboto_Condensed/Regular.ttf",
    default_font_size = 12,
}

-- Define globals for libraries
class = require "lib.middleclass"
pprint = require "lib.pprint"
pprint.setup( { show_function = true } )

util = require "safe.utility"

return configuration
