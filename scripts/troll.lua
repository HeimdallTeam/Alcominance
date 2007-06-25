-- AI pro tupouna

-- handler pri zadani nove prace
-- troll handle na tupouna
-- param tabulka s parametry
function troll_Job(troll, param)

    info("Nova prace pro tupouna ", param.type)
    SetMem(troll, "phase", 1)
    -- nastavit parametry
    SetMem(troll, "owner", param.owner)
    SetMem(troll, "remote", param.remote)
    SetMem(troll, "locked", param.locked)
    SetMem(troll, "sur", param.sur)
    SendMsg(troll, BMSG_Go, 0, param.remote)
 end

-- handler ktery se zavola vzdy pri ukoncene praci
-- troll handle na tupouna
function troll_Finish(troll)

	phase = GetMem(troll, "phase",0)
    if phase == 1 then
		-- vzit suroviny
		-- nebo na ne cekat
		remote = GetMem(troll, "remote",0)
		n = GetMem(troll, "locked",0)
		r = SendMsg(remote, BMSG_GetSur, n, {GetMem(troll, "sur",0),n})
        if r < 0 then
            -- pockat na vytezeni
            SetMem(troll, "phase", 4)
            SendMsg(remote, BMSG_MiningRegister, 10, troll)
        else
		    SetMem(troll, "phase", 3)
		    SendMsg(troll, BMSG_Go, 0, GetMem(troll, "owner",0))
        end
    end
    if phase == 3 then
		o = GetMem(troll, "owner",0)
		SendMsg(o, BMSG_InsertSur, 0, 
			{GetMem(troll, "sur",0),GetMem(troll, "locked",0)})
		SendMsg(o, BMSG_TrollIncoming, 0, troll)
		SetMem(troll, "phase", 0)
    end
    if phase == 4 then -- vytezeno
		SendMsg(troll, BMSG_Go, 0, GetMem(troll, "owner",0))
        SetMem(troll, "phase", 3)
    end
end

