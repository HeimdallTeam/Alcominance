
#ifndef _BECHER_UTILS_H_
#define _BECHER_UTILS_H_

class ResourceItem;
// save game
class BecherGameSave : public HoeFileWriter
{
public:
	void WriteRI(ResourceItem & item);
};

class BecherGameLoad : public HoeFileReader
{
public:
	void ReadRI(ResourceItem & item);
};


#endif // _BECHER_UTILS_H_

