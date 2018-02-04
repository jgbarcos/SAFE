local Action = require "safe.action"

local PlayAnimation = class("PlayAnimation", Action)
function PlayAnimation:initialize( params )
    Action.initialize(self, params)

    self.entity =    util.enforce( params.entity,    4, "entity")
    self.animation = util.default( params.anim,      "idle")
    self.play_once = util.default( params.play_once, true  )
end

function PlayAnimation:on_enter()
    local ptr = safe.get_entity(self.entity)
    self.sheet = safe.get_sheet_animation(ptr)

    if self.sheet ~= nil then
        self.sheet.is_playing = true
        self.sheet.play_once = self.play_once
        self.sheet:change_animation(self.animation)
    end
end

function PlayAnimation:on_update(delta)
    if self.sheet == nil then
        return true
    end

    return not self.sheet.play_once or not self.sheet.is_playing
end
    

gamedata.actions["play_animation"] = PlayAnimation