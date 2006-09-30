
#include "StdAfx.h"
#include "becher.h"
#include "map.h"
#include "minimap.h"
#include "sysobjs.h"

static const int sizemap = 256;

void MiniMap::Draw(IHoe2D * h2d)
{
	const float left = 17.920000f;
	const float right = 107.519997f;
	const float top = 492.430267f;
	const float bottom = 580.876465f;
	const float sk = 6.f;

	h2d->SetRect(800,600);
	if (m_pic) 
	{ 
		float sx = m_map->m_sizeX;
		float sy = m_map->m_sizeY;
		h2d->BltFast(left, right, top, bottom, m_pic);
		if (m_kriz)
		{
			float xx = ((m_posx+sx * 0.5f) / sx) * (right-left);
			float yy = ((m_posy+sy * 0.5f) / sy) * (bottom-top);
			h2d->BltFast(left-sk+xx, left+sk+xx, top-sk+yy, top+sk+yy, m_kriz);

		}
	}
}

bool WaterMiniMap::Load(BecherMap * map)
{
	m_kriz = (IHoePicture*)GetEngine()->Create("picture krizek");
	m_map = map;
	float sx = map->m_sizeX;
	float sy = map->m_sizeY;

	// nahrat minimapu
	dword * p = new dword[sizemap*sizemap];
	for (int y=0;y < sizemap;y++)
		for (int x=0;x < sizemap;x++)
			p[y*sizemap+x] = 0xff000000;
	for (int i=0; i < map->GetNumSysObj();i++)
	{
		if (map->GetSysObj(i)->GetType()!=EBSys_Water)
			continue;
		SystemObjectSource * src = dynamic_cast<SystemObjectSource *>(map->GetSysObj(i));
		for (int y=0;y < sizemap;y++)
		{
 			float yy = (y * sy / sizemap) - sy * 0.5f;
			for (int x=0;x < sizemap;x++)
			{
				float xx = (x * sx / sizemap) - sx * 0.5f;
				float c = src->GetDistance(xx,yy);
				byte cc = (byte)(c * 0xff);
				dword color = 0xff000000 | cc;
				if (p[y*sizemap+x] < color)
					p[y*sizemap+x] = color;
			}
		}
	}
	m_pic = GetEngine()->CreatePicture(sizemap, sizemap, p);
	delete p;
	return true;
}

bool CoalMiniMap::Load(BecherMap * map)
{
	m_kriz = (IHoePicture*)GetEngine()->Create("picture krizek");
	m_map = map;
	float sx = map->m_sizeX;
	float sy = map->m_sizeY;

	// nahrat minimapu
	dword * p = new dword[sizemap*sizemap];
	for (int y=0;y < sizemap;y++)
		for (int x=0;x < sizemap;x++)
			p[y*sizemap+x] = 0xff000000;
	for (int i=0; i < map->GetNumSysObj();i++)
	{
		if (map->GetSysObj(i)->GetType()!=EBSys_Coal)
			continue;
		SystemObjectSource * src = dynamic_cast<SystemObjectSource *>(map->GetSysObj(i));
		for (int y=0;y < sizemap;y++)
		{
 			float yy = (y * sy / sizemap) - sy * 0.5f;
			for (int x=0;x < sizemap;x++)
			{
				float xx = (x * sx / sizemap) - sx * 0.5f;
				float c = src->GetDistance(xx,yy);
				byte cc = (byte)(c * 0xff);
				dword color = 0xff000000 | (cc<<16) | (cc<<8);
				if (p[y*sizemap+x] < color)
					p[y*sizemap+x] = color;
			}
		}
	}
	m_pic = GetEngine()->CreatePicture(sizemap, sizemap, p);
	delete p;
	return true;
}

bool StoneMiniMap::Load(BecherMap * map)
{
	m_kriz = (IHoePicture*)GetEngine()->Create("picture krizek");
	m_map = map;
	float sx = map->m_sizeX;
	float sy = map->m_sizeY;

	// nahrat minimapu
	dword * p = new dword[sizemap*sizemap];
	for (int y=0;y < sizemap;y++)
		for (int x=0;x < sizemap;x++)
			p[y*sizemap+x] = 0xff000000;
	for (int i=0; i < map->GetNumSysObj();i++)
	{
		if (map->GetSysObj(i)->GetType()!=EBSys_Stone)
			continue;
		SystemObjectSource * src = dynamic_cast<SystemObjectSource *>(map->GetSysObj(i));
		for (int y=0;y < sizemap;y++)
		{
 			float yy = (y * sy / sizemap) - sy * 0.5f;
			for (int x=0;x < sizemap;x++)
			{
				float xx = (x * sx / sizemap) - sx * 0.5f;
				float c = src->GetDistance(xx,yy);
				byte cc = (byte)(c * 0xff);
				dword color = 0xff000000 | (cc<<16) | (cc<<8) | cc;
				if (p[y*sizemap+x] < color)
					p[y*sizemap+x] = color;
			}
		}
	}
	m_pic = GetEngine()->CreatePicture(sizemap, sizemap, p);
	delete p;
	return true;
}


bool TerrainMiniMap::Load(BecherMap * map, byte * bmap)
{
	m_kriz = (IHoePicture*)GetEngine()->Create("picture krizek");
	m_map = map;
	uint sx = map->m_numX;
	uint sy = map->m_numY;
	
	// nahrat minimapu
	dword * p = new dword[sx*sx];
	for (int y=0;y < sx;y++)
	{
		int yy = y;//y * sy / sizemap;
		for (int x=0;x < sx;x++)
		{
			int xx = x;//x * sx / sizemap;
			byte col = bmap[yy*sy+xx];
			switch (col)
			{
			case 0x10:
				p[y*sx+x] = 0xffffff00;break;
			case 0x20:
				p[y*sx+x] = 0xff00ffff; break;
			case 2:
				p[y*sx+x] = 0xff0000ff; break;
			case 1:
				p[y*sx+x] = 0xffff0000; break;
			//case 0:
			//	p[y*sx+x] = 0xff800000; break;
			default:
				p[y*sx+x] = 0xffffffff; break;
			};
		}
	}
	m_pic = GetEngine()->CreatePicture(sx, sy, p);
	delete p;
	return true;
}