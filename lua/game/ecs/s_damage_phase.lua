local System = require "safe.system"

local DamagePhaseSystem = class("DamagePhaseSystem", System)
function DamagePhaseSystem:init()
  local function callback(event)
    local space = safe.get_space(event.payload.space)
    space.context.current_team = event.payload.team
    space.context.activate_damage_phase = true
    return true
  end    
  safe.subscribe("new_turn", callback)
end

function DamagePhaseSystem:on_enable(space)
  space.context.activate_damage_phase = false
end

function DamagePhaseSystem:update(_, space) 
  if space.context.activate_damage_phase then
    local current_turn = space.context.current_team
      
    local action_list = {}

    for _,ent in pairs(space:get_entities()) do  
      local transform = safe.get_transform(ent)
      local char_data = safe.get_component(ent, "CharacterDataComponent")
      local unit = safe.get_component(ent, "GridUnitComponent")
      
      -- Reset orig position
      if unit then
        unit.origx = unit.x
        unit.origy = unit.y
      end
      
      --[[
      if char_data and unit and unit.team == current_turn then
        
        for _,vec in pairs(char_data.attack_area) do
          local x = unit.x + vec.x
          local y = unit.y + vec.y
          
          if transform.scale.x < 0 then x = unit.x - vec.x end
          
          
          -- Check for a target
          for i,id in pairs (safe.get_entities_at(x,y)    ) do
            
            local target = space:get_entity(id)
            
            local target_char_data = safe.get_component(target, "CharacterDataComponent")
            local target_unit = safe.get_component(target, "GridUnitComponent")
          
            local actions = {
              game.actions.PlayAnimation { 
                entity = ent:get_name(), 
                anim = "shoot" },
              game.actions.PlayAnimation { 
                entity = ent:get_name(), 
                anim = "idle", 
                play_once = false },
              game.actions.DealDamage { 
                target = target:get_name(), 
                damage = char_data.base_attack },
              game.actions.PrintDamage { 
                att_team = unit.team, 
                att = char_data.name, 
                def_team = target_unit.team, 
                def = target_char_data.name, 
                damage = char_data.base_attack }
            }
            
            -- Stack with other actions
            for _, a in ipairs(actions) do
              action_list[#action_list + 1] = a
            end
          end
        end
      end --]]
    end
    -- Submit actions
    safe.add_action_list( action_list )
  end
end

return DamagePhaseSystem
