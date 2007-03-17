
#include "StdAfx.h"
#include "becher.h"
#include "utils.h"
#include "crr.h"
#include "buildings.h"

void BecherGameSave::WriteChunk(unsigned long chunk)
{
	MapChunk ch = { chunk, 0 };
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
	//WriteValue<dword>(123456789);
}

bool BecherGameLoad::ReadHeader()
{
	Reset();
	BechSaveHeader head;
	if (!Read(&head, sizeof(head)))
		return false;
	if (head.id != ID_ALCOMINANCEFILE)
	{
		GetCon()->Printf("Error: Bad format file");
		return false;
	}
	if (head.version < ID_BECHERVER)
	{
		GetCon()->Printf("Version is low (%d, req: %d). Update your executables.", head.version, ID_BECHERVER);
		return false;
	}
	if (head.version > ID_BECHERVER)
	{
		GetCon()->Printf("Version is big (%d, req: %d). Update your executables.", head.version, ID_BECHERVER);
		return false;
	}
	m_str.ReadFromFile(&head,*this);	

	Seek(sizeof(head));
	return true;
}

void BecherGameLoad::ReadReservedWords(int num)
{
	for (int i=0;i < num;i++)
	{
		Read<dword>();
	}
}

void ChunkDictWrite::Begin()
{
	m_nkey = 0;
	m_bgs.WriteValue<unsigned long>(0); // velikost dat
	m_bgs.WriteValue<unsigned long>(0); // pocet klicu
	m_hdr = m_bgs.GetFile()->Tell();
}

void ChunkDictWrite::End()
{
	size_t act = m_bgs.GetFile()->Tell();
	m_bgs.GetFile()->Seek(m_hdr-2*sizeof(unsigned long));
	m_bgs.WriteValue<unsigned long>(act-m_hdr); // velikost dat
	m_bgs.WriteValue<unsigned long>(m_nkey); // pocet klicu
	m_bgs.GetFile()->Seek(act);
	m_bgs.Write(m_keys, m_nkey * sizeof(SaveKey));
}

int ChunkDictWrite::Key(const char *key, int value)
{
	// write value
	m_keys[m_nkey].key = m_bgs.GetStringMap()->AddStringId(key);
	m_keys[m_nkey].pos = m_bgs.GetFile()->Tell() - m_hdr;
	m_keys[m_nkey].size = sizeof(value);
	m_nkey++;
	m_bgs.Write<int>(value);
	return 0;
}

int ChunkDictWrite::Key(const char * key, float value)
{
	// write value
	m_keys[m_nkey].key = m_bgs.GetStringMap()->AddStringId(key);
	m_keys[m_nkey].pos = m_bgs.GetFile()->Tell() - m_hdr;
	m_keys[m_nkey].size = sizeof(value);
	m_nkey++;
	m_bgs.Write<float>(value);
	return 0;
}

////////////////////// read
ChunkDictRead::ChunkDictRead(BecherGameLoad & bgl) 
	: m_bgl(bgl)
{
	// read chunks
	dword datasize = m_bgl.Read<unsigned long>(); // velikost dat
	m_nkey = (int)m_bgl.Read<unsigned long>(); // pocet klicu
	m_hdr = m_bgl.GetFile()->Tell();
	// skip
	m_bgl.Skip(datasize);
	m_bgl.Read(m_keys, m_nkey * sizeof(SaveKey));
}

bool ChunkDictRead::IsKey(const char * key) const
{
	int kid = m_bgl.GetStringMap()->GetStringId(key);
	if (!kid)
		return false;
	for (int i=0;i < m_nkey;i++)
		if (m_keys[i].key == kid)
			return true;
	return false;
}

float ChunkDictRead::KeyFloat(const char * key, float def) const
{
	int kid = m_bgl.GetStringMap()->GetStringId(key);
	if (!kid)
		return def;
	for (int i=0;i < m_nkey;i++)
		if (m_keys[i].key == kid)
		{
			m_bgl.Seek(m_hdr+m_keys[i].pos);
			return m_bgl.Read<float>();
		}
	return def;
}

int ChunkDictRead::KeyInt(const char * key, int def) const
{
	int kid = m_bgl.GetStringMap()->GetStringId(key);
	if (!kid)
		return false;
	for (int i=0;i < m_nkey;i++)
		if (m_keys[i].key == kid)
		{
			m_bgl.Seek(m_hdr+m_keys[i].pos);
			return m_bgl.Read<int>();
		}
	return def;
}

double ChunkDictRead::KeyDouble(const char * key, double def) const
{
	assert(!"not implemented");
	return def;
}

bool ChunkDictRead::KeyString(char * str, int size) const
{
	assert(!"not implemented");
	return false;
}

StringKeysHash::StringKeysHash()
{
	m_lastid = 0;
	memset(m_ids, 0, sizeof(m_ids));
}
int StringKeysHash::GetStringId(const char * key) const
{
	for (int i=1;i <= m_lastid;i++)
		if (m_ids[i] && strcmp(key, m_ids[i])==0)
			return i;
	return 0;
}
int StringKeysHash::AddStringId(const char * key)
{
	int id = GetStringId(key);
	if (id) return id;
	m_lastid++;
	hoe_assert(m_lastid<150);
	m_ids[m_lastid] = key;
	return m_lastid;
}
bool StringKeysHash::WriteToFile(BechSaveHeader * head, BecherGameSave &w)
{
	head->keystab = (unsigned long)w.GetFile()->Tell();
	head->keynum = m_lastid;
	for (int i=1;i <= m_lastid;i++)
		w.Write<int>(i);	
	head->keysize = (unsigned long)w.GetFile()->Tell();
	// save string
	for (int i=1;i <= m_lastid;i++)
	{
		size_t l=strlen(m_ids[i]);
		w.Write(m_ids[i], l+1);
	}
	head->keysize = (unsigned long)w.GetFile()->Tell()-head->keysize;
	return true;
}

bool StringKeysHash::ReadFromFile(BechSaveHeader * head, BecherGameLoad &r)
{
	// load string map
	r.Seek(head->keystab+head->keynum*sizeof(int));
	char * string = new char[head->keysize];
	r.Read(string, head->keysize);
	r.Seek(head->keystab);
	for (int i=0;i < head->keynum;i++)
	{
		m_lastid = r.Read<int>();
		m_ids[m_lastid] = string;
		while (*string) string++;;
		string++;
	}
	return true;
}











