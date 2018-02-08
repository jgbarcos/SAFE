local proc = require "procedural"

local entities = {}
local templates = {}

--
-- Set Systems
--
active_systems = {
  -- Safe systems
  "CameraMovementSystem",
  "RenderSystem",
  "SpriteSheetAnimatorSystem",
  
  -- Cpp systems
  "DragMovementSystem",
  
  -- Lua systems
  "ExplorationSystem",
}

--
-- Create Entities
--
--  entities[#entities+1] = {}
local event_types = {
  "empty_location", "heat_reduction",
  "empty_location", "heat_reduction",
  "empty_location", "encounter",
}
for i=1,10 do
  local x = math.random(-150,150)
  local y = math.random(-150,150)
  local event = proc:random_element(event_types)
  
  entities[#entities+1] = {
    TransformComponent = { 
      position = {x=x, y=y, z=0}
    },
    ExplorationEventComponent = {
      x = x,
      y = y,
      event = event
    },
    SpriteComponent = { 
      filename = "assets/RedArea4.png", 
      center = {x=0.5, y=0.0},
      is_vertical = false
    },
    TextBoxComponent = {
      text = "",
      center = { x=0.5, y=1.0 }
    }
  }
end


--
-- Create Templates
--
--  templates[#templates+1] = {}
templates[#templates+1] = {
  TemplateName = "ExplorationCursor",
  TransformComponent = {},
  SpriteComponent = { 
    filename = "assets/Soldier_Sniper.png", 
    center = {x=0.5, y=0.5}
  },
  DraggableComponent = {
      turn_horizontal = true
  }
}

templates[#templates+1] = {
  TemplateName = "ExplorationBarBack",
  TransformComponent = {
    position = { x=0, y=0, z=-0.1}
  },
  SpriteComponent = {
    filename = "assets/SimpleBar_Blue.png",
    is_vertical = false,
    center = { x=0.5, y=1.0 }
  }
}

templates[#templates+1] = {
  TemplateName = "ExplorationBarFill",
  TransformComponent = {
    position = { x=0, y=0, z=0 },
  },
  SpriteComponent = {
    filename = "assets/SimpleBar_Red.png",
    is_vertical = false,
    center = { x=0.5, y=1.0 }
  }
}


-- Create entities and templates
for i, e in ipairs(entities) do
    safe.create_entity(e)
end
for i, t in ipairs(templates) do
    safe.create_template(t)
end

-- Set Systems
safe.set_active_systems( active_systems, true)
