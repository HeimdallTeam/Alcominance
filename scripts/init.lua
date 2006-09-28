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
