
#ifndef _BECHER_UTILS_H_
#define _BECHER_UTILS_H_

class ResourceExp;
class BecherGameSave;
class BecherGameLoad;

#define ID_CHUNK(a,b,c,d) ((a) | (b) << 8 | (c) << 16 | (d) << 24)
#define ID_BECHERFILE ID_CHUNK('b','m','a','p')
#define ID_BECHERSAVE ID_CHUNK('b','s','a','v')
#define ID_ALCOMINANCEFILE ID_CHUNK('A','L','C','E')
#define ID_BECHERVER 7

struct MapChunk
{
	unsigned int chunk;
	unsigned int size;
};

struct BechSaveHeader
{
	unsigned int id;
	unsigned int version;
	/** kde se v souboru nachazi tabulka klicu 
	format: 
	unsigned int key[num];
	byte -> strings[num]
	*/
	unsigned int keystab; ///< misto kde se nachazi klice
	unsigned int keysize; ///< velikost tabulky stringu
	unsigned int keynum;  ///< pocet klicu
};

class StringKeysHash
{
	int m_lastid;
	const char * m_ids[150];
public:
	StringKeysHash();
	int GetStringId(const char * key) const;
	int AddStringId(const char * key);
	bool WriteToFile(BechSaveHeader * head, BecherGameSave &w);
	bool ReadFromFile(BechSaveHeader * head, BecherGameLoad &r);
};

// save game
class BecherGameSave : public HoeCore::WriteStream
{
	size_t m_lastsizepos;
	StringKeysHash m_str;
public:
	BecherGameSave(XHoeFile * f) : HoeFileWriter(f) {}
	void WriteChunk(unsigned int chunk);
	void WriteChunkEnd();
	StringKeysHash * GetStringMap() { return &m_str; };
};

class BecherGameLoad : public HoeFileReader
{
	MapChunk chunk;
	bool m_savegame;
	StringKeysHash m_str;
public:
	BecherGameLoad(XHoeFile * f) : HoeFileReader(f, 0)
	{
		chunk.chunk = 0;
	}
	bool IsSaveGame() { return m_savegame; }
	const MapChunk & Chunk() { return chunk; }
	bool ReadNext() 
	{
		if (!this->Read(&chunk, sizeof(chunk)))
			chunk.chunk = 0;
		return chunk.chunk != 0;
	}
	bool ReadHeader();
	void ReadReservedWords(int num);
	const StringKeysHash * GetStringMap() { return &m_str; }
};

struct SaveKey
{
	int key;
	int pos;
	int size;
};

/** Ulozeni dat typu slovnik */
class ChunkDictWrite
{
	int m_nkey;
	size_t m_hdr;
	SaveKey m_keys[100];
	BecherGameSave & m_bgs;
public:
	ChunkDictWrite(BecherGameSave & bgs)
		: m_bgs(bgs)
	{
	}
	//BecherGameSave & sav;
	void Begin();
	bool BeginDataKey(const char * key);
	void EndDataKey();
	int Key(const char * key, int value);
	int Key(const char * key, float value);
	int Key(const char * key, double value);
	int Key(const char * key, const char * value);
	void End();
};

/** Ulozeni dat typu slovnik */
class ChunkDictRead
{
	int m_nkey;
	SaveKey m_keys[100];
	BecherGameLoad & m_bgl;
	size_t m_hdr;
public:
	ChunkDictRead(BecherGameLoad & bgl);
	bool IsKey(const char * key) const;
	float KeyFloat(const char * key, float def) const;
	int KeyInt(const char * key, int def) const;
	double KeyDouble(const char * key, double def) const;
	bool KeyString(char * str, int size) const;
};

class Fade
{
protected:
	bool m_fading;
	dword m_tocolor;
	dword m_color;
	float m_time;
	float m_finish;
public:
	Fade()
	{
		m_fading = false;
		m_color = 0x0;
	}
	bool IfFading() { return m_fading; }
	void Update(float dtime)
	{
		// prechod pro vsechny 4 barvy
		if (m_fading)
		{
			m_time += dtime;
			if (m_time >= m_finish)
			{
				m_color = m_tocolor;
				m_fading = false;
			}
			// u kazde barvy rozdil
			byte a,r,g,b,a1,r1,g1,b1;
			float p = m_time / m_finish;
			a = (byte)((m_color&0xff000000) >> 24);
			r = (byte)((m_color&0xff0000) >> 16);
			g = (byte)((m_color&0xff00) >> 8);
			b = (byte)((m_color&0xff));
			a1 = (byte)((m_tocolor&0xff000000) >> 24);
			r1 = (byte)((m_tocolor&0xff0000) >> 16);
			g1 = (byte)((m_tocolor&0xff00) >> 8);
			b1 = (byte)((m_tocolor&0xff));
			a += (byte)((a1-a) * p);
			r += (byte)((r1-r) * p);
			g += (byte)((g1-g) * p);
			b += (byte)((b1-b) * p);
			m_color = (a << 24) | (r << 16) | (r << 8) | b;
		}
	}
	void Draw(IHoe2D * h2d)
	{
		if (m_color & 0xff000000)
		{
			h2d->SetRect(1,1);
			h2d->PaintRect(0,1,0,1,m_color, true);
		}
	}
	void ToFade(dword color, float time)
	{
		if (time > 0)
		{
			m_tocolor = color;
			m_fading = true;
		}
		else
			m_color = color;
		m_finish = time;
		m_time = 0;
	}
};

#endif // _BECHER_UTILS_H_

