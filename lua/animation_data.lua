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
}

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
