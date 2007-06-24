-- funkce pro lihovar

function c_stavlihovar()

 cost = GetVar("dest_cost")
 wood = GetVar("dest_cost_wood")
 stone = GetVar("dest_cost_stone")
 
 SetBuilding(EBO_Destilate,cost,wood,stone)
end

function s_lihovar()
 ClearButtons()
 AddButton(ico_TROLLNEW,"Najmout", "c_najmout")
 AddButton(ico_TROLLFREE,"Propustit", "c_propustit")
 --PlaySound(model_TESTSOUND)
end

function i_alco(h, i)
 print("Idiot alco")
 s = GetInfo(h, BINFO_CanSugar)

 if i.sugar > 0 then
    SendMsg(h, BMSG_Chief, 0, "F>W")
 elseif s > 0 and SendMsg(h, BMSG_Chief, 0, "F>IS") == 0 then
       SendMsg(h, BMSG_Chief, 0, "W>IS")
 end
end
