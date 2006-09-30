
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

class MiniMap
{
protected:
	BecherMap * m_map;
	IHoePicture * m_pic;
	IHoePicture * m_kriz;
	float m_posx;
	float m_posy;
public:
	MiniMap()
	{
		m_map = NULL;
		m_pic = NULL;
		m_kriz = NULL;
		m_posx = 0.f;
		m_posy = 0.f;
	}
	virtual void Draw(IHoe2D * h2d);
	void SetCameraPosition(float x, float y)
	{
		m_posx = x;
		m_posy = y;
	}
};

class WaterMiniMap : public MiniMap
{
public:
	bool Load(BecherMap * map);
};

class CoalMiniMap : public MiniMap
{
public:
	bool Load(BecherMap * map);
};

class StoneMiniMap : public MiniMap
{
public:
	bool Load(BecherMap * map);

};

class TerrainMiniMap : public MiniMap
{
public:
	bool Load(BecherMap * map, byte * bmap);
};

#endif

