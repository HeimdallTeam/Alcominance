-- funkce pro sklad

function c_stavsklad()
 SetBuilding(EBO_Store,5,0,5)
end

function s_sklad()
 info("Vybranej sklad")
 ClearButtons()
 AddButton(ID_STOPPRACI,"Najmout delnika", "c_najmout")
end

