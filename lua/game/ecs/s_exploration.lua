local System = require "safe.system"

local ExplorationSystem = class("ExplorationSystem", System)

function ExplorationSystem:init()
  game.heat = 0
end

function ExplorationSystem:on_enable(space)
  -- Cursor
  space.context.exp_cursor = space:create_entity_from_template("ExplorationCursor", "ExplorationCursor")
  
  -- Position
  local transform = safe.get_transform(space.context.exp_cursor)
  space.context.last_x = transform.position.x
  space.context.last_y = transform.position.y
  
  -- Heat bar
  space.context.fill_bar = space:create_entity_from_template("ExplorationBarFill", "ExplorationBarFill")
  space.context.back_bar = space:create_entity_from_template("ExplorationBarBack", "ExplorationBarBack")
  
end

function ExplorationSystem:update(_, space)
  local cursor_transform = safe.get_transform(space.context.exp_cursor)
  local cursor_pos = cursor_transform.position

  for _,ent in pairs(space:get_entities()) do  
    local exp_event = ent:get_component("ExplorationEventComponent")
    local transform = safe.get_transform(ent)
    local textbox = safe.get_text_box(ent)
    
    if exp_event and transform and textbox and not exp_event.activated then
      local event_pos = transform.position
      
      local dist = 10;

      if math.abs(event_pos.x - cursor_pos.x) < dist and math.abs(event_pos.y - cursor_pos.y) < dist then
        exp_event.activated = true
        local ev = gamedata.get.exploration_event(exp_event.event)
        ev.perform({})
        textbox.text = exp_event.text
        
        -- Update distances
        local x_diff = event_pos.x - space.context.last_x
        local y_diff = event_pos.y - space.context.last_y
  
        game.heat = game.heat + math.sqrt(x_diff*x_diff + y_diff*y_diff)
        
        space.context.last_x = event_pos.x
        space.context.last_y = event_pos.y
  
      end
    end
    
  end
  
  -- Calculate bar values
  local bar_sprite = safe.get_sprite(space.context.fill_bar)
  if(bar_sprite and bar_sprite:is_loaded()) then
    local h = bar_sprite:get_height()
    
    local value = math.min(1.0, game.heat / 1000.0)
    bar_sprite.clip.height = math.floor(value*h)/h
    bar_sprite.clip.y = math.ceil((1-value)*h)/h
  end
  
end


return ExplorationSystem
