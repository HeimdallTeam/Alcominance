-- funkce pro sklad

function c_stavsklad()
 SetBuilding(EBO_Store,5,0,5)
end

function camerka()
 Camera(300, 300)
end

function camerkazpet()
 Camera(0, 0)
end

function s_sklad()
 info("Vybranej sklad")
 ClearButtons()
 AddButton(model_NEWTROLL,"Najmout delnika", "c_najmout")
 AddButton(model_NEWTROLL,"", "camerka")
 AddButton(model_NEWTROLL,"", "camerkazpet")
end

