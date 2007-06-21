-- AI pro tupouna

-- handler pri zadani nove prace
-- troll handle na tupouna
-- param tabulka s parametry
function troll_Job(troll, param)

    info("Nova prace pro tupouna ", param.type)
    SetMem(troll, "phase", 0)
    -- nastavit parametry
    --
end

-- handler ktery se zavola vzdy pri ukoncene praci
-- troll handle na tupouna
function troll_Finish(troll)

    x = GetMem(troll,"x", 30)
    y = GetMem(troll,"y", -30)
    -- SendMsg(troll, BMSG_Go, x, y)
    info("Jdu na ",x,",",y) 
    SetMem(troll,"x",y)
    SetMem(troll,"y",-x)

end

