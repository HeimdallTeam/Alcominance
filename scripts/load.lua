-- inicializacni soubor, nacita se pri spusteni hry

-- build
-- function AddFile(path)
-- function LoadResource(id, cmd)
-- function hoe(cmd)
-- function print(...)

--os.execute("htex hud.hx hud.dds")

-- hotovo
AddFile("data/terrain.hx")



LoadResource(model_TESTSOUND+2,"sound 'sound/datel1.ogg'")

--files 
AddFile("data_old/textures.hx")
AddFile("data_old/cukrovar.hm")
AddFile("data_old/stromy.hm")
AddFile("data_old/info.hm")
AddFile("data_old/chalupa.hm")
AddFile("data_old/farma.hm")
AddFile("data_old/lihovar.hm")
AddFile("data_old/most.hm")
-- tupoun
-- AddFile("data_old/olaf.hm")
AddFile("data_old/tupoun.hm")

AddFile("data_old/tovarna.hm")
AddFile("data_old/terrain.hm")
AddFile("data_old/studna.hm")
AddFile("data_old/sklad.hm")
AddFile("data_old/obchod.hm")
AddFile("data_old/staveniste.hm")
AddFile("data_old/drevorubec.hm")
AddFile("data_old/pic.hx")
AddFile("data_old/czech.hx")
AddFile("data_old/dopln.hm")
AddFile("data_old/utes.hm")
AddFile("data_old/dul.hm")
AddFile("data_old/hud.hx")
--files-end
--resources
LoadResource(model_INFO_FONT, "font 'resource/font.ttf' 16")
LoadResource(model_DIGITFONT, "picture digit")
LoadResource(model_FACTORY, "model tovarna")
LoadResource(model_STROM1, "model strom_war3")
LoadResource(model_STROM2, "model strom_war3")
LoadResource(model_STROM3, "model strom_war3")
LoadResource(model_STROM4, "model strom_war3")
LoadResource(model_STROMY, "model strom_war3")
LoadResource(model_SUGAR, "model cukrovar")
LoadResource(model_DESTILATE, "model lihovar")
LoadResource(model_BABA, "model chaloupkabk")
LoadResource(model_STORE, "model sklad")
LoadResource(model_SHOP, "model obchod")
LoadResource(model_MOST, "model most")
LoadResource(model_FARM, "model farma")
LoadResource(model_FARM_POLE, "model pole")
LoadResource(model_SAW, "model drevorubec")
LoadResource(model_WATERHOLE, "model studna")
LoadResource(model_COALMINE, "model dul_uhli")
--LoadResource(model_TROLL, "model olaf")
--LoadResource(model_TROLL, "generate model box 5")
LoadResource(model_TROLL, "model tupoun")

LoadResource(model_INFO_RING, "model info")
LoadResource(model_INFO_RING2, "model info2")
LoadResource(model_CONSTRUCT, "model staveniste")
LoadResource(model_CONSTRUCT_D, "model doplnky")
LoadResource(model_CONSOLE_BACK, "picture console_pic");

LoadResource(model_CUKROVAR_ICO,"picture cukrovar_ico")
LoadResource(model_FARMA_ICO,"picture farma_ico")
LoadResource(model_CHALOUPKA_ICO,"picture chaloupka_ico")
LoadResource(model_LIHOVAR_ICO,"picture lihovar_ico")
LoadResource(model_OBCHOD_ICO,"picture obchod_ico")
LoadResource(model_SKLAD_ICO,"picture sklad_ico")
LoadResource(model_STUDNA_ICO,"picture studna_ico")
LoadResource(model_TOVARNA_ICO,"picture tovarna_ico")
LoadResource(model_KAMENYDUL_ICO,"picture com_background")
LoadResource(model_UHELNYDUL_ICO,"picture com_background")

LoadResource(model_ZADNYPRIKAZ,"picture com_background")
LoadResource(model_JDI,"picture com_go")
LoadResource(model_PROPUST,"picture com_release")
LoadResource(model_PRACUJ,"picture com_work")
LoadResource(model_STOPPRACI,"picture com_workstop")
LoadResource(model_NEWTROLL,"picture pracant")

LoadResource(model_AGROINZENYR,"picture up_agroinzenyr")
LoadResource(model_KOLEJOVYDOPRAVNIK,"picture up_kolejovydopravnik")
LoadResource(model_LAHVARNA,"picture up_lahvarna")
LoadResource(model_DESTILATOR,"picture up_pokrocilydestilator")
LoadResource(model_PREDPOVED,"picture up_predpoved")
LoadResource(model_KOTEL,"picture up_utesnenykotel")
LoadResource(model_ZAVLAZOVANI,"picture up_zavlazovani")

--LoadResource(model_ADVANCE,"picture advance")
--LoadResource(model_BACK,"picture back")
--LoadResource(model_CAMPAIGN,"picture campaign")
--LoadResource(model_EXIT,"picture exit")
--LoadResource(model_LOADGAME,"picture loadgame")
--LoadResource(model_SETTINGS,"picture settings")
--LoadResource(model_SINGLEMAP,"picture singlemap")
--LoadResource(model_SAVEGAME,"picture savegame")

--LoadResource(model_LIGHTADVANCE,"picture light_advance")
--LoadResource(model_LIGHTBACK,"picture light_back")
--LoadResource(model_LIGHTCAMPAIGN,"picture light_campaign")
--LoadResource(model_LIGHTEXIT,"picture light_exit")
--LoadResource(model_LIGHTLOADGAME,"picture light_loadgame")
--LoadResource(model_LIGHTSETTINGS,"picture light_settings")
--LoadResource(model_LIGHTSINGLEMAP,"picture light_singlemap")
--LoadResource(model_LIGHTSAVEGAME,"picture light_savegame")

--LoadResource(model_CAMPAIGNEND,"picture campaignend")
--LoadResource(model_CAMPAIGNSTART,"picture campaignstart")

LoadResource(model_TESTSOUND+1,"sound 'sound/kukacka-timeout.WAV'")
LoadResource(model_TESTSOUND,"sound 'sound/datel1.wav'")
LoadResource(model_TESTSOUND+2,"sound 'sound/penizky.wav'")
--resources-end



