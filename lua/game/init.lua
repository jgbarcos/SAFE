-- Componets
safe.register_component( require "game.character_data_component" )
safe.register_component( require "game.grid_unit_component" )

-- Systems
safe.register_system( require "game.damage_phase_system" )