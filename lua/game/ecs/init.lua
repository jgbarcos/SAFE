-- Components
safe.register_component( require "game.ecs.c_character_data" )
safe.register_component( require "game.ecs.c_grid_unit" )
safe.register_component( require "game.ecs.c_abilities" )

-- Systems
safe.register_system( require "game.ecs.s_damage_phase" )
safe.register_system( require "game.ecs.s_stats_modifier" )