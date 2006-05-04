
#ifndef _MAP_LOADER_H_
#define _MAP_LOADER_H_

class MapLoader : public MapLoaderCallback
{
	iHoeFont * m_font;
	int progress;

	void UpdateFrame();
public:
	void SetProgress(int proc, const char * msg);
	MapLoader();

	int LoadingProgress(iHoe2D * hoe2d);
	iHoeMap * LoadMap(const char * fname);
	void Process(float proc);
};

#endif // _MAP_LOADER_H_

