local templates = {}

-- 
-- Create Templates
--
templates[#templates+1] = {
    TemplateName = "Tile",
    TransformComponent = { 
        position = {x=0, y=0, z=-10},
        scale = {x=1, y=1, z=1} 
    },
    GridTileComponent = {},
    SpriteComponent = { 
        filename = 'assets/floor_tile.png',
        center = {x=0.5, y=0.5},
        is_vertical = false
    }
}

templates[#templates+1] = {
    TemplateName = "EndTurnButton",
    TransformComponent = {
        position = {x=0, y=0; z=10},
    },
    SpriteComponent = {
        filename = 'assets/EndTurnButton.png',
        center = {x=0.5, y=0.5},
    }
}

templates[#templates+1] = {
    TemplateName = "Cursor",
    SpriteComponent = { 
        filename = "assets/Cursor.png", 
        center = {x=0.5, y=0.5}
    },
    TransformComponent = { 
        position = {x=0, y=0, z=-3},
        scale = {x=1, y=1, z=1} 
    }
}
 animations = {} 

templates[#templates+1] = {
    TemplateName = "MovementArea",
    SpriteComponent = { 
        filename = "assets/BlueArea2.png", 
        center = {x=0.5, y=0.5},
        is_vertical = false
    },
    TransformComponent = { 
        position = {x=0, y=0, z=-5.1},
        scale = {x=1, y=1, z=1} 
    }
}

templates[#templates+1] = {
    TemplateName = "ReadyArea",
    SpriteComponent = { 
        filename = "assets/WhiteArea.png", 
        center = {x=0.5, y=0.5},
        is_vertical = false
    },
    TransformComponent = { 
        position = {x=0, y=0, z=-5.1},
        scale = {x=1, y=1, z=1} 
    }
}

templates[#templates+1] = {
    TemplateName = "AttackArea",
    SpriteComponent = { 
        filename = "assets/RedArea2.png", 
        center = {x=0.5, y=0.5},
        is_vertical = false
    },
    TransformComponent = { 
        position = {x=0, y=0, z=-5.0},
        scale = {x=1, y=1, z=1} 
    }
}

templates[#templates+1] = {
    TemplateName = "DamageArea",
    SpriteComponent = { 
        filename = "assets/DamageArea3.png", 
        center = {x=0.5, y=0.5},
        is_vertical = false
    },
    TransformComponent = { 
        position = {x=0, y=0, z=-5.0},
        scale = {x=1, y=1, z=1} 
    }
}

templates[#templates+1] = {
    TemplateName = "CharDataDisplay",
    TransformComponent = { },
    TextBoxComponent = {
        center = {x=0, y=1},
    }
}

templates[#templates+1] = {
    TemplateName = "AbilityIcon",
    TransformComponent = { },
    TextBoxComponent = {
        center = {x=0.5, y=1}
    }
}


for i, t in ipairs(templates) do
    safe.create_template(t)
end