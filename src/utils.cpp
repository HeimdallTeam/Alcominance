
#include "StdAfx.h"
#include "becher.h"
#include "utils.h"
#include "crr.h"
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
	WriteValue<unsigned long>((unsigned long)sf-m_lastsizepos);
	GetFile()->Seek(sf);
	// save size
	WriteValue<dword>(123456789);
}

void BecherGameSave::WriteReservedWords(int num)
{
	for (int i=0;i < num;i++)
	{
		dword dw = 0;
		Write<dword>(dw);
	}
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
	if (chunk.ver < ID_BECHERVER)
	{
		GetCon()->Printf("Error: Otevirani prilis stare mapy.");
		return false;
	}
	if (chunk.ver > ID_BECHERVER)
	{
		GetCon()->Printf("Version is big (%d, req: %d). Update your executables.", chunk.ver, ID_BECHERVER);
		return false;
	}

	return true;
}

void BecherGameLoad::ReadReservedWords(int num)
{
	for (int i=0;i < num;i++)
	{
		Read<dword>();
	}
}


