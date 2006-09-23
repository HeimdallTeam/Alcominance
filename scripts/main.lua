-- program s hernim systemem, nacita se pokazde pred spustenim levelu
-- funkce ktere se volaji pri urcitych akcich
-- prikazy:
-- nastaveni build kandidata: SetBuilding(id budovy, gold, wood, stone)


-- function AddButton(resource, tooltip, script)
-- function ClearButtons()
-- function PlaySound(id)

function c_najmout()
 AddTroll(true);
end

function s_studna()
 ClearButtons()
 AddButton(model_JDI,"Send him somewhere", "c_go")
 AddButton(model_PRACUJ,"We have a work to do?", "c_work")
 AddButton(model_CUKROVAR_ICO,"Build Sugar factory", "c_stavcukr")
 AddButton(model_FARMA_ICO,"Build Sugar farm", "c_stavfarma")
 AddButton(model_CHALOUPKA_ICO,"Build Herbwomen", "c_stavkorenarka")
 AddButton(model_LIHOVAR_ICO,"Build Alcoholic house", "c_stavlihovar")
 AddButton(model_OBCHOD_ICO,"Build Market", "c_stavobchod")
 AddButton(model_SKLAD_ICO,"Build Storage", "c_stavsklad")
 AddButton(model_STUDNA_ICO,"Build Waterpump", "c_stavstudna")
 AddButton(model_TOVARNA_ICO,"Build Facility", "c_stavtovarna")
 AddButton(model_KAMENYDUL_ICO,"Build Stone Mine", "c_stavkamen")
 AddButton(model_STUDNA_ICO,"Build Coal Mine", "c_stavuhli")
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

function s_korenarka()
 ClearButtons()
end

function s_tupoun()
 ClearButtons()
  AddButton(model_CUKROVAR_ICO,"Build Sugar factory", "c_stavcukr")
end

--builds

function c_stavfarma()
 SetBuilding(EBO_Farm,15,0,0)
end
function c_stavkorenarka()
 SetBuilding(EBO_HerbeWoman,10,0,0)
end

function c_stavobchod()
 SetBuilding(EBO_Shop,5,15,20)
end

function c_stavstudna()
 SetBuilding(EBO_WaterHole,5,5,0)
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
 PlaySound(model_TESTSOUND+2)
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






