--[[--Example of action list
safe.add_action_list( {
    game.actions.PlayAnimation{ entity=6, anim="shoot"},
    game.actions.PlayAnimation{ entity=6, anim="idle", play_once=false },
    game.actions.Delay{ time=2.0 },
    game.actions.PlayAnimation{ entity=6, anim="shoot" },
    game.actions.PlayAnimation{ entity=6, anim="idle", play_once=false }
})
--]]