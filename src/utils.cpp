
#include "StdAfx.h"
#include "becher.h"
#include "game.h"
#include "utils.h"
#include "buildings.h"

void BecherGameSave::WriteChunk(unsigned long chunk, unsigned long ver)
{
	MapChunk ch = { chunk, ver, 0 };
	Write(&ch, sizeof(ch));
	m_lastsizepos = GetFile()->Tell();;
}

void BecherGameSave::WriteChunkEnd()
{
	size_t sf = GetFile()->Tell();
	GetFile()->Seek(m_lastsizepos - sizeof(unsigned long));
	WriteValue<unsigned long>(sf-m_lastsizepos);
	GetFile()->Seek(sf);
	// save size
	WriteValue<dword>(123456789);
}

void BecherGameSave::WriteRI(ResourceItem &item)
{
	WriteValue<int>(item.GetNum());
}

void BecherGameLoad::ReadRI(ResourceItem &item)
{
	item.SetNum(Read<int>());
}

bool BecherGameLoad::ReadHeader()
{
	Reset();
	if (!ReadNext())
		return false;
	if (chunk.chunk == ID_BECHERFILE)
	{
		m_savegame = false;
	}
	else if (chunk.chunk == ID_BECHERSAVE)
	{
		m_savegame = true;
	}
	else
	{
		GetCon()->Printf("Error: Bad format file");
		return false;
	}

	return true;
}



