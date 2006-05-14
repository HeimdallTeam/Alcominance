-- program s hernim systemem, nacita se pokazde pred spustenim levelu
-- funkce ktere se volaji pri urcitych akcich
-- prikazy:
-- nastaveni build kandidata: SetBuilding(id budovy, gold, wood, stone)


-- function AddButton(resource, tooltip, script)
-- function ClearButtons()
-- function PlaySound(id)

farm = farm or false

function s_tovarna()
 ClearButtons()
 AddButton(ID_NEWTROLL,2, "c_addtroll")
 AddButton(ID_PROPUST,"You want save some salary?", "c_release")
 AddButton(ID_AGROINZENYR,"Even Stupids can earn some degree", "c_agroeng")
 AddButton(ID_KOLEJOVYDOPRAVNIK,"Railway speeds up production", "c_railway")
 AddButton(ID_DESTILATOR,"Doubles efficiency of destilation", "c_destil")
 AddButton(ID_PREDPOVED,"Want some weather cast?", "c_weathercast")
 AddButton(ID_KOTEL,"Fix the holes for better efficiency", "c_firefix")
 AddButton(ID_STOPPRACI,"Stop the hard work", "c_workstop")
 PlaySound(ID_TESTSOUND+1)
end

function c_najmout()
 AddTroll(true);
end

function s_cukr()
 ClearButtons()
 AddButton(ID_STOPPRACI,2, "c_najmout")
end

function s_lihovar()
 ClearButtons()
 AddButton(ID_STOPPRACI,2, "c_najmout")
 AddButton(ID_LAHVARNA,"People pay more for nice bottles", "c_bottles")

end

function s_studna()
 ClearButtons()
 AddButton(ID_JDI,"Send him somewhere", "c_go")
 AddButton(ID_PRACUJ,"We have a work to do?", "c_work")
 AddButton(ID_CUKROVAR_ICO,"Build Sugar factory", "c_stavcukr")
 AddButton(ID_FARMA_ICO,"Build Sugar farm", "c_stavfarma")
 AddButton(ID_CHALOUPKA_ICO,"Build Herbwomen", "c_stavkorenarka")
 AddButton(ID_LIHOVAR_ICO,"Build Alcoholic house", "c_stavlihovar")
 AddButton(ID_OBCHOD_ICO,"Build Market", "c_stavobchod")
 AddButton(ID_SKLAD_ICO,"Build Storage", "c_stavsklad")
 AddButton(ID_STUDNA_ICO,"Build Waterpump", "c_stavstudna")
 AddButton(ID_TOVARNA_ICO,"Build Facility", "c_stavtovarna")
 AddButton(ID_KAMENYDUL_ICO,"Build Stone Mine", "c_stavkamen")
 AddButton(ID_STUDNA_ICO,"Build Coal Mine", "c_stavuhli")
end

function s_kamen()
 
end

function s_uhli()
end

function s_farma()
 ClearButtons()
 AddButton(ID_STOPPRACI,2, "c_najmout")
 if (not farm) then
    AddButton(ID_ZAVLAZOVANI,"How about some automated water distribution?", "c_waterfarm")
 end
end

function s_shop()
 ClearButtons()
 AddButton(ID_STOPPRACI,"Najmout delnika", "c_najmout")
end

function s_korenarka()
 ClearButtons()
end

function s_sklad()
 --info("Vybranej sklad")
 ClearButtons()
 AddButton(ID_STOPPRACI,"Najmout delnika", "c_najmout")
end

function s_tupoun()
 ClearButtons()
  AddButton(ID_CUKROVAR_ICO,"Build Sugar factory", "c_stavcukr")
end

--builds
function c_stavcukr()
  SetBuilding(EBO_Sugar,1000,10,0)
end

function c_stavfarma()
 SetBuilding(EBO_Farm,15,0,0)
end
function c_stavkorenarka()
 SetBuilding(EBO_HerbeWoman,10,0,0)
end
function c_stavlihovar()
 SetBuilding(EBO_Destilate,10,10,0)
end
function c_stavobchod()
 SetBuilding(EBO_Shop,5,15,20)
end
function c_stavsklad()
 SetBuilding(EBO_Store,5,0,5)
end
function c_stavstudna()
 SetBuilding(EBO_WaterHole,5,5,0)
end
function c_stavtovarna()
 SetBuilding(EBO_Factory,100,100,100)
end
function c_stavkamen()
end
function c_stavuhli()
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
function c_addtroll()
 AddTroll(false)
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
 PlaySound(ID_TESTSOUND+2)
 if GetCash() < 1000 then
  info("v kase je mene nez 1000")
 else
  info("v kase je vice nez 1000")
 end
end
function c_waterfarm()
 SetVar("troll_speed", 100)
 farm = true
end

function i_sugar(b)
 -- print("Cukrovar ma ",b.cane," trtiny a ",b.sugar," cukru")
 -- pokud je nejaky cukr tak odnest
 if (b.sugar > 0) then
     job = { type = 3 }
     return job    
 end
 -- jestlize se trtina vejde tak trtina
 if b.cane_avail > 0 and b.free > 0 then
     job = { type = EBS_Cane }
     return job    
 end 
 
 -- jestli je nejaka trtina tak zpracovat
 if b.cane > 0 then 
     job = { type = 2 }
     return job    
 end 
 
 return nil
end

function i_farm(bb)
 return nil
end





