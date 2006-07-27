
#include "StdAfx.h"
#include "becher.h"
#include "utils.h"
#include "buildings.h"

void BecherGameSave::WriteRI(ResourceItem &item)
{
	Write<int>(item.GetNum());
}

void BecherGameLoad::ReadRI(ResourceItem &item)
{
	item.SetNum(Read<int>());
}


