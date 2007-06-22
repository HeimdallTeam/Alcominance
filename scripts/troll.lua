-- AI pro tupouna

-- handler pri zadani nove prace
-- troll handle na tupouna
-- param tabulka s parametry
function troll_Job(troll, param)

    info("Nova prace pro tupouna ", param.type)
    SetMem(troll, "phase", 1)
    -- nastavit parametry
    SetMem(troll, "owner", param.owner)
    SendMsg(troll, BMSG_Go, param.remote)
end

-- handler ktery se zavola vzdy pri ukoncene praci
-- troll handle na tupouna
function troll_Finish(troll)

    if GetMem(troll, "phase",0) == 1 then
        o = GetMem(troll,"owner", 0)
        if o > 0 then
            SendMsg(troll, BMSG_Go, o)
            SetMem(troll, "phase", 2)

        end
    end
end

