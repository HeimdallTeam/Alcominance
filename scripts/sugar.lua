-- funkce pro cukrovar

function c_stavcukr()
  SetBuilding(EBO_Sugar,1000,10,0)
end

function s_cukr()
 ClearButtons()
 AddButton(ID_STOPPRACI,2, "c_najmout")
 print("cukr ",ID_TESTSOUND)
 PlaySound(ID_TESTSOUND)
end


function i_sugar(b)
 print("Cukrovar ma ",b.cane," trtiny a ",b.sugar," cukru")
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
