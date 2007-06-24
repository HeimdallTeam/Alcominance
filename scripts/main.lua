-- program s hernim systemem, nacita se pokazde pred spustenim levelu
-- funkce ktere se volaji pri urcitych akcich
-- prikazy:
-- nastaveni build kandidata: SetBuilding(id budovy, gold, wood, stone)


-- function AddButton(resource, tooltip, script)
-- function ClearButtons()
-- function PlaySound(id)

-- hotovo
function c_farm()
 SetBuilding(EBO_Farm,GetVar("farm_cost"),GetVar("farm_cost_wood"),GetVar("farm_cost_stone"))
end

function c_stavdrevorubec()
 SetBuilding(EBO_Saw,GetVar("saw_cost"),GetVar("saw_cost_wood"),GetVar("saw_cost_stone"))
end

-- stare

-- najmout noveho trolla
function c_najmout(h)
 SendMsg(h, BMSG_Chief, 0,"N")
end

function c_propustit(h)
 SendMsg(h, BMSG_Chief, 0,"D")
end

function s_studna()
 ClearButtons()
end

function s_kamen()
 
end

function s_uhli()
end

function s_farma()
 ClearButtons()
 AddButton(model_STOPPRACI,2, "c_najmout")
 if (not farm) then
    AddButton(model_ZAVLAZOVANI,"How about some automated water distribution?", "c_waterfarm")
 end
end

function s_shop()
 ClearButtons()
 AddButton(model_STOPPRACI,"Najmout delnika", "c_najmout")
end

function s_herbe()
 ClearButtons()
end

function s_tupoun()
 ClearButtons()
  AddButton(model_CUKROVAR_ICO,"Build Sugar factory", "c_stavcukr")
end

--builds



function c_stavobchod()
 SetBuilding(EBO_Shop,5,15,20)
end

function c_stavstudna()
 SetBuilding(EBO_WaterHole,5,5,0)
end


--commands
function c_go()
end
function c_release()
end
function c_work()
end
function c_workstop()
end

--upgrades
function c_agroeng()
end
function c_railway()
end
function c_bottles()
end
function c_destil()
end

function c_weathercast()
 AddCash(-53)
end

function c_firefix()
 AddCash(100)
 PlaySound(model_TESTSOUND+2)
 if GetCash() < 1000 then
  info("v kase je mn: ne9> 1000")
 else
  info("v kase je vce ne9> 1000")
 end
end
function c_waterfarm()
 SetVar("troll_speed", 100)
 farm = true
end

-- vrati mnozstvi dane suroviny pro vyrobu a pocet vyrobenych jednotek
-- napr. recept S10+A10+H3+W20=1
function getReceptCount(recept, surtype)

 --ziskani mnozstvi dane suroviny
 count=tonumber(string.match(string.match(recept, surtype.."%d"), "%d"))
 
 --ziskani poctu vyrobenych jednotek
 units=tonumber(string.match(string.match(recept, "=%d"), "%d"))
 
 return count, units

end



