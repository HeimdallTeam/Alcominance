
#ifndef _BECHER_UTILS_H_
#define _BECHER_UTILS_H_

class ResourceExp;

#define ID_CHUNK(a,b,c,d) ((a) | (b) << 8 | (c) << 16 | (d) << 24)
#define ID_BECHERFILE ID_CHUNK('b','m','a','p')
#define ID_BECHERSAVE ID_CHUNK('b','s','a','v')
#define ID_BECHERVER 5

struct MapChunk
{
	unsigned long chunk;
	unsigned long ver;
	unsigned long size;
};

// save game
class BecherGameSave : public HoeFileWriter
{
	size_t m_lastsizepos;
public:
	BecherGameSave(XHoeFile * f) : HoeFileWriter(f) {}
	void WriteRI(ResourceExp & item);
	void WriteChunk(unsigned long chunk, unsigned long ver);
	void WriteChunkEnd();
	void WriteReservedWords(int num);
};

class BecherGameLoad : public HoeFileReader
{
	MapChunk chunk;
	bool m_savegame;
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
	void ReadRI(ResourceExp & item);
	bool ReadHeader();
	void ReadReservedWords(int num);
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

#define SAFE_DELETE_ARRAY(arr) if (arr) { delete [] arr; arr = NULL; }

#endif // _BECHER_UTILS_H_

