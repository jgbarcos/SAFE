local module = {}

local t_idle=0.4
local t_stance_change=0.2
local t_fast_shoot=0.1

module.animations_data = {
    custom_sheet = {
        idle = {
            { rect={x=0/7, y=0/2, w=1/7, h=1/2}, time= t_idle },
            { rect={x=1/7, y=0/2, w=1/7, h=1/2}, time= t_idle },
            { rect={x=2/7, y=0/2, w=1/7, h=1/2}, time= t_idle },
            { rect={x=3/7, y=0/2, w=1/7, h=1/2}, time= t_idle }
        },
        shoot = {
            { rect={x=0/7, y=1/2, w=1/7, h=1/2}, time= t_stance_change },
            { rect={x=1/7, y=1/2, w=1/7, h=1/2}, time= t_stance_change },
            { rect={x=2/7, y=1/2, w=1/7, h=1/2}, time= t_fast_shoot },
            { rect={x=3/7, y=1/2, w=1/7, h=1/2}, time= t_fast_shoot },
            { rect={x=4/7, y=1/2, w=1/7, h=1/2}, time= t_fast_shoot },
            { rect={x=5/7, y=1/2, w=1/7, h=1/2}, time= t_fast_shoot },
            { rect={x=6/7, y=1/2, w=1/7, h=1/2}, time= t_fast_shoot },
            { rect={x=2/7, y=1/2, w=1/7, h=1/2}, time= t_fast_shoot },
            { rect={x=3/7, y=1/2, w=1/7, h=1/2}, time= t_fast_shoot },
            { rect={x=0/7, y=1/2, w=1/7, h=1/2}, time= t_stance_change }
        }
    }
}

function module.get(table, key)
    res = table[key]
    if not res then 
        res = table["DEFAULT"]
    end
    return res
end

module.center = {
    ["DEFAULT"] = {x=0.5, y=1},
    ["assets/Rebel_Rifle.png"] = { x=0.25, y=1},
    ["assets/Rebel_Spear.png"] = { x=0.35, y=1},
    ["assets/Mercenary_Sniper.png"] = { x=0.1, y=1},
    ["assets/Mercenary_Pistol.png"] = { x=0.25, y=1},
    ["assets/Armored_Heavy.png"] = { x=0.4, y=1},
    ["assets/Rebel_Firefighter.png"] = { x=0.4, y=1},
    ["assets/Soldier_Assault.png"] = { x=0.5, y=1},
    ["assets/Soldier_Shield.png"] = { x=0.5, y=1},
    ["assets/Soldier_Sniper.png"] = { x=0.25, y=1},
}

module.sheet = {
    ["assets/Soldier_Assault.png"] = module.animations_data.custom_sheet 
}

return module

--[[ --This helps with debugging
        sol::state_view view(luaT.lua_state());
        view["sprite"] = luaT.get<sol::object>("SpriteComponent");
        view["collider"] = luaT.get<sol::object>("ColliderComponent");

        view.script(R"(
            function pp(o)
                print( '{' )
                if type(o) == 'table' then
                    for k, v in pairs( o ) do
                        print(k, v)
                    end
                end
                print( '}' )
            end

            pprint = pp
        )");
        ]]--
