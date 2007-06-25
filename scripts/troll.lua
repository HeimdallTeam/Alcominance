-- AI pro tupouna

-- handler pri zadani nove prace
-- troll handle na tupouna
-- param tabulka s parametry
function troll_Job(troll, param)

    info("Nova prace pro tupouna ", param.type)
    troll.phase = 1
    -- nastavit parametry
    troll.owner = param.owner
    troll.remote = param.remote
    troll.locked = param.locked
    troll.sur = param.sur
    SendMsg(troll.handle, BMSG_Go, 0, param.remote)
 end

-- handler ktery se zavola vzdy pri ukoncene praci
-- troll handle na tupouna

function troll_Finish(troll)

	phase = troll.phase
    if phase == 1 then
		-- vzit suroviny
		-- nebo na ne cekat
		r = SendMsg(troll.remote, BMSG_GetSur, troll.locked, {troll.sur,troll.locked})
        if r < 0 then
            -- pockat na vytezeni
            troll.phase = 4
            SendMsg(troll.remote, BMSG_MiningRegister, 10, troll.handle)
        else
		    troll.phase = 3
		    SendMsg(troll.handle, BMSG_Go, 0, troll.owner)
        end
    end
    if phase == 3 then
		
		SendMsg(troll.owner, BMSG_InsertSur, 0, 
			{troll.sur , troll.locked})
		SendMsg(troll.owner, BMSG_TrollIncoming, 0, troll.handle)
		troll.phase = 0
    end
    if phase == 4 then -- vytezeno
		SendMsg(troll.handle, BMSG_Go, 0, troll.owner)
        troll.phase = 3
    end
end

