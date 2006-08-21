
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
};

#define SAFE_DELETE_ARRAY(arr) if (arr) { delete [] arr; arr = NULL; }

// ptr list


#endif // _BECHER_UTILS_H_

