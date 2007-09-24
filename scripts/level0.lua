--	Scenar:
--
--	1. sklad s omezenym mnozstvim:
--	kameni,
--	dreva,        na postaveni par budov
--
--	uhli,         na kratsi provoz
--
--	koreni,
--	vody,
--	cukru,
--	lihu
--
--	2. hrac postavi tovarnu
--
--	3. najme delniky, vyrobi par becherovek
--
--	4. dojde koreni, 5. hrac bude nucen postavit Babku korenarku
--
--	6. dojde voda, 7. hrac bude nucen postavit Studnu
--
--	8. dojde lih, 9. -||- Lihovar
--
--	10. dojde cukr, 11. -||- Cukrovar
--
--	12. dojde trtina, 13. Cukrovar nema trtinu -> postavit Farmu
--
--	TODO drevo, uhli, kamen ? - kdy dojdou?
--	TODO zamezit alternativnim scenarum
--	TODO upravit hlasky - nektery jsou suchy (uvodni :))
--	behem by se mel hrac osamostatnovat
--	nechavat hru postupne vic a vic na nem, min napoved

function start()
  part1()
end

--uvodni info
--vytvoreni skladu
--hint pro dalsi krok - postaveni tovarny
function part1()
                                 
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Na za��tku je v�dycky tma, nebo aspo� nic")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Tak je tomu i te�")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Tv�m �kolem po celou dobu hry je v�roba Becherovky")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Pro za��tek bude t�eba n�jak�ch surovin")
  
  --postavit sklad na miste urcenem systemovym objektem
  --TODO
  hSklad = 
  
  SendGameMsg(hSklad, BMSG_AddSur, EBS_Stone, X, null);
  SendGameMsg(hSklad, BMSG_AddSur, EBS_Wood, X, null);
  SendGameMsg(hSklad, BMSG_AddSur, EBS_Coal, X, null);
  
  SendGameMsg(hSklad, BMSG_AddSur, EBS_Herbe, X, null);
  SendGameMsg(hSklad, BMSG_AddSur, EBS_Sugar, X, null);
  SendGameMsg(hSklad, BMSG_AddSur, EBS_Water, X, null);
  SendGameMsg(hSklad, BMSG_AddSur, EBS_Alco, X, null);  
  
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "A te� je�t� tov�rnu pro v�robu Becherovky")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Postav tov�rnu")

end

function factory_buildFinish()
  part2()
end

--hint pro dalsi krok - najmout tupouny
function part2()

  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "V�born�")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Te� u� nic nechyb� k v�rob� prvn�ch Becherovek")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Suroviny jsou v�echny k dispozici:")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Ko�en�")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Voda")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "L�h")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Cukr")
  
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Je�t� je ale t�eba n�kdo, kdo to ud�l�")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Najmi do tov�rny tupouny")
  
end

trollCount ::= 0
function factory_newTroll()
  trollCount ::= trollCount + 1
  
  --spusteni jen pri prvnim tupounovi
  if trollCount == 1 then
    part3()
  end
end

function part3()
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Te� u� produkci nic nezb�v�")
end

--dojde koreni
function factory_emptyHerbe()
  part4()
end

function part4()

  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Z�soby ko�en� jsou pr�zdn�")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Bude t�eba je zajistit")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Postav chaloupku babky Ko�en��ky")
  
end

function herbeWoman_buildFinish()
  part5()
end

function part5()
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Ko�en� je na cest�")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Z�skal jsi samostatn� zdroj ko�en�")
end

function factory_emptyHerbe()
  part6()
end

function part6()
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Ale ne. Do�la voda")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Vodu z�sk� ve studni")
end

function waterHole_buildFinish()
  part7()
end

function part7()
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "OK")
end

function factory_emptyAlco()
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Sklad u� nem� l�h")
  SendGameMsg(ID_MSGALL, BMSG_Info, 0, "Copak asi bude t�eba postavit?")
end

function destilate_buildFinish()
  part8()
end

function part8()
  
end