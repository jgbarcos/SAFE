local Action = require "safe.action"

local module = {}

module.PlayAnimation = class("PlayAnimation", Action)
    function module.PlayAnimation:initialize( params )
        Action.initialize(self, params)

        self.entity =    util.enforce( params.entity,    4, "entity")
        self.animation = util.default( params.anim,      "idle")
        self.play_once = util.default( params.play_once, true  )
    end

    function module.PlayAnimation:on_enter()
        local ptr = safe.get_entity(self.entity)
        self.sheet = get_sheet_animation(ptr)

        if self.sheet ~= nil then
            self.sheet.is_playing = true
            self.sheet.play_once = self.play_once
            self.sheet:change_animation(self.animation)
        end
    end

    function module.PlayAnimation:on_update(delta)
        if self.sheet == nil then
            return true
        end

        return not self.sheet.play_once or not self.sheet.is_playing
    end


module.Delay = class("Delay", Action)
    function module.Delay:initialize( params )
        Action.initialize(self, params)

        self.time = util.default( params.time, 0 )
    end

    function module.Delay:on_update(delta)
        self.time = self.time - delta
        return self.time < 0
    end

module.PrintConsole = class("PrintConsole", Action)
    function module.PrintConsole:initialize(params)
        Action.initialize(self, params)

        self.text = util.default( params.text, "" )
    end

    function module.PrintConsole:on_enter()
        print(self.text)
    end

module.PrintDamage = class("PrintDamage", Action)
    function module.PrintDamage:initialize(params)
        Action.initialize(self, params)

        self.att_team = util.default( params.att_team, "" )
        self.def_team = util.default( params.def_team, "" )
        self.att =      util.default( params.att, "" )
        self.def =      util.default( params.def, "" )
        self.damage =   util.default( params.damage, 0 )
    end

    function module.PrintDamage:on_enter()
        print(string.format(
            "> [%s]%s attacks [%s]%s for %s damage.", 
            self.att_team, self.att, self.def_team, self.def, self.damage
        ))
    end

module.DealDamage = class("DealDamage", Action)
    function module.DealDamage:initialize(params)
        Action.initialize(self, params)
        
        self.target = util.enforce( params.target, 4, "target entity")
        self.damage = util.enforce( params.damage, 4, "damage")
    end

    function module.DealDamage:on_enter()
        local ptr = safe.get_entity(self.target)
        local char_data = get_character_data(ptr)

        char_data.health = char_data.health - self.damage
    end
    
return module