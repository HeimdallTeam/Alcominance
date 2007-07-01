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
-- ring_x - nasobek zvyraznujiciho kruhu v pomeru kruhu u stromu (ten ma 10 bodu na sirku)
-- ring_y - stejne jako ring_x
-- ring_h - jak vysoko je kruh, v bodech
-- height - rozdil terenu, kde jde jeste budova postavit
-- dobj - minimalni vzdalenost od objektu
SetVar("sizzing.tree", "ring_x=1;ring_y=1;ring_h=3;height=20;dobj=10")
SetVar("sizzing.store", "ring_x=2.5;ring_y=2.5;ring_h=5;height=20;dobj=30")
SetVar("sizzing.farm", "ring_x=5;ring_y=3.5;ring_h=5;height=20;dobj=10")

SetVar("sizzing.sugar", "ring_x=4.5;ring_y=4.5;ring_h=3;height=20;dobj=10")
SetVar("sizzing.waterhole", "ring_x=1;ring_y=1;ring_h=5;height=20;dobj=10")
SetVar("sizzing.destilate", "ring_x=4.2;ring_y=4.2;ring_h=5;height=20;dobj=10")
SetVar("sizzing.herbe", "ring_x=2;ring_y=2.3;ring_h=5;height=20;dobj=10")
SetVar("sizzing.bridge", "ring_x=1;ring_y=1;ring_h=5;height=20;dobj=10")
SetVar("sizzing.troll", "ring_x=1;ring_y=1;ring_h=5;height=20;dobj=10")
SetVar("sizzing.factory", "ring_x=7.7;ring_y=7.7;ring_h=5;height=20;dobj=50")
SetVar("sizzing.shop", "ring_x=1;ring_y=1;ring_h=5;height=20;dobj=10")
SetVar("sizzing.saw", "ring_x=2;ring_y=2.5;ring_h=5;height=20;dobj=10")
SetVar("sizzing.coalmine", "ring_x=1.8;ring_y=1.8;ring_h=5;height=20;dobj=10")
SetVar("sizzing.stonemine", "ring_x=1.8;ring_y=1.8;ring_h=5;height=20;dobj=10")
