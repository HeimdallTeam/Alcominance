
#include "StdAfx.h"
#include "becher.h"
#include "utils.h"
#include "buildings.h"

void BecherGameSave::WriteChunk(unsigned long chunk, unsigned long ver)
{
	MapChunk ch = { chunk, ver, 0 };
	Write(&ch, sizeof(ch));
}

void BecherGameSave::WriteRI(ResourceItem &item)
{
	WriteValue<int>(item.GetNum());
}

void BecherGameLoad::ReadRI(ResourceItem &item)
{
	item.SetNum(Read<int>());
}


