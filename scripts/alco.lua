-- funkce pro lihovar

function c_stavlihovar()
 SetBuilding(EBO_Destilate,10,10,0)
end

function s_lihovar()
 ClearButtons()
 AddButton(ID_STOPPRACI,2, "c_najmout")
 AddButton(ID_LAHVARNA,"People pay more for nice bottles", "c_bottles")

end

