hoe("specialchars ěščřžýáíéůúďťüň")
hoe("showfps")

AddFile("data/intro.hx")

function ShowT(pic, tit)
 Fade("ff000000", 0.2)
 LoadImage("picture "..pic)
 SetText("titulky",tit)
 Fade("00000000", 0.2)
end

Wait(1.6)
ShowT("vary", "Město vzkvétalo")
Wait(1.6)
ShowT("vary1","Město vzkvétalo, ale já si oproti té honosné fasádě")
Wait(1.6)
ShowT("vary-2","Já si oproti té honosné fasádě připadal jak v propadlišti")
Wait(1.8)
ShowT("drunken","Lidé bohatli, ale zahodili svoje štěstí a šlapali po něm")
Wait(3)
ShowT("TMA","Vlastně jako já")
Wait(2)
ShowT("na_pivu", "čím více měli, tím více pili, aby neviděli prázdnotu svých životů")
Wait(3)
ShowT("TMA","Vlastně jako já 2")
Wait(1)
ShowT("old-man","Můj děda byl jiný, neskrýval svůj smutek jako oni")
Wait(3)
ShowT("24hour_drinking","Lidé se mu jen vysmívali, člověku ze starých dob")
Wait(2.5)

LoadImage("picture silhouette")
SetText("titulky","Báli se ho, jelikož  s nimi nespadl prázdnoty")
Wait(3)

Fade("ff000000", 0.2)
LoadImage("picture TMA")
SetText("titulky","Vlastně jako já 3")
Fade("00000000", 0.1)
Wait(2)
	
SetText("titulky","On se celý život pachtil se svým receptem")
Wait(3)

LoadImage("picture old-man")
SetText("titulky","Věřil v jeho dobrý vliv")
Wait(2)

SetText("titulky","Vždy uvařil sotva pro sebe a své přátele")
Wait(2)
Fade("ff000000", 1)

LoadImage("picture eyes")
SetText("titulky","Už se asi nemohli dívat pravdě do tváře skrze jeho oči")
Fade("00000000", 0)
Wait(4)

LoadImage("picture biti")
SetText("titulky","Museli je zničit")
Wait(2)
Fade("ff000000", 2)

LoadImage("picture TMA")
SetText("titulky","Jako by se bál toho, co jsem považoval za vysvobození")

Fade("00000000", 0)

Wait(3)

LoadImage("picture postel")
SetText("titulky","Zděsil jsem se, vida jak se na mě upnul")
Wait(3)
Fade("ff000000", 1)

LoadImage("picture sklep")
SetText("titulky","Děda trávil mnoho času zamčený v té sklepní komoře")
Fade("00000000", 4)
Wait(3)

Fade("ffcccccc", 4)
SetText("titulky","Myslel sem, že se tam zavírá se svým žalem")
Fade("00000000", 3)

SetText("titulky","Nikdy jsem za ním nechodil.")
Wait(2)

SetText("titulky"," ")
LoadImage("picture sekretar")
Wait(2)

LoadImage("picture sekretar2")
Wait(2)

LoadImage("picture suple")
Wait(2)

Fade("ff000000", 0.3)

LoadImage("picture stavba")
Fade("00000000", 0.3)
Wait(1)

LoadImage("picture plan")
Wait(1)
Fade("ff000000", 0.7)

LoadImage("picture sklenice")
Fade("00000000", 0.8)
Wait(2)

Fade("ffffffff", 0.3)

LoadImage("picture sklenice2")
Fade("00000000", 0.4)

Fade("ff000000", 3.5)