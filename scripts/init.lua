-- inicializacni soubor, nacita se pri spusteni hry

-- build
-- function AddFile(path)
-- function LoadResource(id, cmd)
-- function hoe(cmd)
-- function print(...)

--os.execute("htex hud.hx hud.dds")

print("Hello world, from ",_VERSION,"!")

hoe("specialchars ěščřžýáíéůúďťüň")


hoe("showfps")
--hoe("wireframe 1")
LoadLang("resource/"..GetVar("lang_file")..".lng")

-- object parameters
SetVar("sizzing.tree", "ring_x=1;ring_y=1;ring_h=3")
SetVar("sizzing.sugar", "ring_x=4.5;ring_y=4.5;ring_h=3")
SetVar("sizzing.waterhole", "ring_x=1;ring_y=1;ring_h=5")
