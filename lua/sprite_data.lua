local module = {}

module.animations_data = {
    dir_4_frames_2 = {
        [0] = {
            { time=0.3, rect={x=0,y=0, w=0.25, h=0.25} },
            { time=0.3, rect={x=0,y=0.25, w=0.25, h=0.25} }
        },
        [1] = {
            { time=0.3, rect={x=0.25,y=0, w=0.25, h=0.25} },
            { time=0.3, rect={x=0.25,y=0.25, w=0.25, h=0.25} }
        },
        [2] = {
            { time=0.3, rect={x=0.5,y=0, w=0.25, h=0.25} },
            { time=0.3, rect={x=0.5,y=0.25, w=0.25, h=0.25} }
        },
        [3] = {
            { time=0.3, rect={x=0.75,y=0, w=0.25, h=0.25} },
            { time=0.3, rect={x=0.75,y=0.25, w=0.25, h=0.25} }
        }
    },

    dir_4_frames_3 = {
        [0] = {
            { time=0.3, rect={x=0,y=0, w=0.25, h=0.25} },
            { time=0.3, rect={x=0,y=0.25, w=0.25, h=0.25} },
            { time=0.3, rect={x=0,y=0, w=0.25, h=0.25} },
            { time=0.3, rect={x=0,y=0.5, w=0.25, h=0.25} }
        },
        [1] = {
            { time=0.3, rect={x=0.25,y=0, w=0.25, h=0.25} },
            { time=0.3, rect={x=0.25,y=0.25, w=0.25, h=0.25} },
            { time=0.3, rect={x=0.25,y=0, w=0.25, h=0.25} },
            { time=0.3, rect={x=0.25,y=0.5, w=0.25, h=0.25} }
        },
        [2] = {
            { time=0.3, rect={x=0.5,y=0, w=0.25, h=0.25} },
            { time=0.3, rect={x=0.5,y=0.25, w=0.25, h=0.25} },
            { time=0.3, rect={x=0.5,y=0, w=0.25, h=0.25} },
            { time=0.3, rect={x=0.5,y=0.5, w=0.25, h=0.25} }
        },
        [3] = {
            { time=0.3, rect={x=0.75,y=0, w=0.25, h=0.25} },
            { time=0.3, rect={x=0.75,y=0.25, w=0.25, h=0.25} },
            { time=0.3, rect={x=0.75,y=0, w=0.25, h=0.25} },
            { time=0.3, rect={x=0.75,y=0.5, w=0.25, h=0.25} }
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
    ["assets/Soldier_Assault.png"] = { x=0.6, y=1},
    ["assets/Soldier_Shield.png"] = { x=0.5, y=1}
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
