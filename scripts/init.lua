-- inicializacni soubor, nacita se pri spusteni hry

-- build
-- function AddFile(path)
-- function LoadResource(id, cmd)
-- function hoe(cmd)
-- function print(...)

--os.execute("htex hud.hx hud.dds")

print("Hello world, from ",_VERSION,"!")

hoe("hidefps")
--hoe("wireframe 1")

--files 
AddFile("data/textures.hx")
AddFile("data/cukrovar.hm")
AddFile("data/stromy.hm")
AddFile("data/info.hm")
AddFile("data/chalupa.hm")
AddFile("data/farma.hm")
AddFile("data/lihovar.hm")
AddFile("data/most.hm")
AddFile("data/olaf.hm")
AddFile("data/tovarna.hm")
AddFile("data/terrain.hm")
AddFile("data/studna.hm")
AddFile("data/sklad.hm")
AddFile("data/obchod.hm")
AddFile("data/staveniste.hm")
AddFile("data/trava.hm")
AddFile("data/pic.hx")
AddFile("data/czech.hx")
AddFile("data/dopln.hm")
AddFile("data/ter.hx")
AddFile("hud.hx")
--files-end
--resources
LoadResource(ID_INFO_FONT, "font 'resource/font.ttf' 16")
LoadResource(ID_DIGITFONT, "picture digit")
LoadResource(ID_FACTORY, "model tovarna")
--LoadResource(ID_STROM+1, "model strom1")
--LoadResource(ID_STROM+2, "model strom2")
--LoadResource(ID_STROM+3, "model strom3")
--LoadResource(ID_STROM+4, "model strom4")
LoadResource(201, "model strom_war3")
LoadResource(ID_SUGAR, "model cukrovar")
LoadResource(ID_DESTILATE, "model lihovar")
LoadResource(ID_BABA, "model chaloupkabk")
LoadResource(ID_STORE, "model sklad")
LoadResource(ID_SHOP, "model obchod")
LoadResource(ID_MOST, "model most")
LoadResource(ID_FARM, "model farma")
LoadResource(ID_FARM_POLE, "model pole")
LoadResource(ID_WATERHOLE, "model studna")
LoadResource(ID_TROLL, "model olaf")
--LoadResource(ID_TROLL, "generate model box 5")

LoadResource(ID_INFO_RING, "model info")
LoadResource(ID_CONSTRUCT, "model staveniste")
LoadResource(ID_CONSTRUCT_D, "model doplnky")
LoadResource(ID_CONSOLE_BACK, "picture console_pic");

LoadResource(ID_CUKROVAR_ICO,"picture cukrovar_ico")
LoadResource(ID_FARMA_ICO,"picture farma_ico")
LoadResource(ID_CHALOUPKA_ICO,"picture chaloupka_ico")
LoadResource(ID_LIHOVAR_ICO,"picture lihovar_ico")
LoadResource(ID_OBCHOD_ICO,"picture obchod_ico")
LoadResource(ID_SKLAD_ICO,"picture sklad_ico")
LoadResource(ID_STUDNA_ICO,"picture studna_ico")
LoadResource(ID_TOVARNA_ICO,"picture tovarna_ico")
LoadResource(ID_KAMENYDUL_ICO,"picture com_background")
LoadResource(ID_UHELNYDUL_ICO,"picture com_background")

LoadResource(ID_ZADNYPRIKAZ,"picture com_background")
LoadResource(ID_JDI,"picture com_go")
LoadResource(ID_PROPUST,"picture com_release")
LoadResource(ID_PRACUJ,"picture com_work")
LoadResource(ID_STOPPRACI,"picture com_workstop")
LoadResource(ID_NEWTROLL,"picture pracant")

LoadResource(ID_AGROINZENYR,"picture up_agroinzenyr")
LoadResource(ID_KOLEJOVYDOPRAVNIK,"picture up_kolejovydopravnik")
LoadResource(ID_LAHVARNA,"picture up_lahvarna")
LoadResource(ID_DESTILATOR,"picture up_pokrocilydestilator")
LoadResource(ID_PREDPOVED,"picture up_predpoved")
LoadResource(ID_KOTEL,"picture up_utesnenykotel")
LoadResource(ID_ZAVLAZOVANI,"picture up_zavlazovani")

--LoadResource(ID_ADVANCE,"picture advance")
--LoadResource(ID_BACK,"picture back")
--LoadResource(ID_CAMPAIGN,"picture campaign")
--LoadResource(ID_EXIT,"picture exit")
--LoadResource(ID_LOADGAME,"picture loadgame")
--LoadResource(ID_SETTINGS,"picture settings")
--LoadResource(ID_SINGLEMAP,"picture singlemap")
--LoadResource(ID_SAVEGAME,"picture savegame")

--LoadResource(ID_LIGHTADVANCE,"picture light_advance")
--LoadResource(ID_LIGHTBACK,"picture light_back")
--LoadResource(ID_LIGHTCAMPAIGN,"picture light_campaign")
--LoadResource(ID_LIGHTEXIT,"picture light_exit")
--LoadResource(ID_LIGHTLOADGAME,"picture light_loadgame")
--LoadResource(ID_LIGHTSETTINGS,"picture light_settings")
--LoadResource(ID_LIGHTSINGLEMAP,"picture light_singlemap")
--LoadResource(ID_LIGHTSAVEGAME,"picture light_savegame")

--LoadResource(ID_CAMPAIGNEND,"picture campaignend")
--LoadResource(ID_CAMPAIGNSTART,"picture campaignstart")

LoadResource(ID_TESTSOUND+1,"sound 'sound/kukacka-timeout.WAV'")
LoadResource(ID_TESTSOUND,"sound 'sound/klik.wav'")
LoadResource(ID_TESTSOUND+2,"sound 'sound/penizky.wav'")
--resources-end

LoadLang("resource/english.lng")

