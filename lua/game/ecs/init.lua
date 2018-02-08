-- Components
safe.register_component( require "game.ecs.c_character_data" )
safe.register_component( require "game.ecs.c_grid_unit" )
safe.register_component( require "game.ecs.c_abilities" )
safe.register_component( require "game.ecs.c_exploration_event" )

-- Systems
safe.register_system( require "game.ecs.s_damage_phase" )
safe.register_system( require "game.ecs.s_stats_modifier" )
safe.register_system( require "game.ecs.s_exploration" )