
#ifndef _BECHER_UTILS_H_
#define _BECHER_UTILS_H_

class ResourceItem;

#define ID_CHUNK(a,b,c,d) ((a) | (b) << 8 | (c) << 16 | (d) << 24)
#define ID_BECHERFILE ID_CHUNK('b','m','a','p')
#define ID_BECHERVER 4

struct MapChunk
{
	unsigned long chunk;
	unsigned long ver;
	unsigned long reserved;
};

// save game
class BecherGameSave : public HoeFileWriter
{
public:
	BecherGameSave(XHoeFile * f) : HoeFileWriter(f) {}
	void WriteRI(ResourceItem & item);
	void WriteChunk(unsigned long chunk, unsigned long ver);
};

class BecherGameLoad : public HoeFileReader
{
	MapChunk chunk;
public:
	BecherGameLoad(XHoeFile * f) : HoeFileReader(f, 0)
	{
		chunk.chunk = 0;
	}
	const MapChunk & Chunk() { return chunk; }
	bool ReadNext() 
	{
		if (!this->Read(&chunk, sizeof(chunk)))
			chunk.chunk = 0;
		return chunk.chunk != 0;
	}
	void ReadRI(ResourceItem & item);
};

#define SAFE_DELETE_ARRAY(arr) if (arr) { delete [] arr; arr = NULL; }


#endif // _BECHER_UTILS_H_

