
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

#define SAFE_DELETE_ARRAY(arr) if (arr) { delete [] arr; arr = NULL; }


#endif // _BECHER_UTILS_H_

