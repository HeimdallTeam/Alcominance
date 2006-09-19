
#include "StdAfx.h"
#include "becher.h"
#include "id.h"
#include "buildings.h"
#include "elements.h"
#include "troll.h"
#include "obj_sugar.h"
#include "obj_store.h"
#include "obj_shop.h"
#include "obj_factory.h"
#include "obj_destilate.h"
#include "obj_farm.h"
#include "obj_saw.h"
#include "obj_herbe.h"
#include "obj_waterhole.h"
#include "obj_coalmine.h"
#include "obj_stonemine.h"
#include "map.h"
#include "sysobjs.h"

BecherMap::BecherMap()
{
	m_numobj = 0;
	m_numsysobj = 0;
	m_lastid = 0;
	m_terrain = NULL;
}

BecherObject * BecherMap::CreateObject(EObjType type)
{
	BecherObject * bo = NULL;

	switch (type)
	{
	case EBO_Factory:
		bo = new Factory(m_scene);break;
	case EBO_Tree:
		bo = new Tree(m_scene);break;
	case EBO_Sugar:
		bo = new Sugar(m_scene);break;
	case EBO_HerbeWoman:
		bo = new HerbeWoman(m_scene);break;
	case EBO_Farm:
		bo = new Farm(m_scene);break;
	case EBO_Bridge:
		bo = new Bridge(m_scene);break;
	case EBO_Destilate:
		bo = new Destilate(m_scene);break;
	case EBO_WaterHole:
		bo = new WaterHole(m_scene);break;
	case EBO_Store:
		bo = new Store(m_scene);break;
	case EBO_Shop:
		bo = new Shop(m_scene);break;
	case EBO_Troll:
		bo = new Troll(m_scene);break;
	case EBO_Saw:
		bo = new Saw(m_scene);break;
	case EBO_StoneMine:
		bo = new StoneMine(m_scene);break;
	case EBO_CoalMine:
		bo = new CoalMine(m_scene);break;
	default:
		assert(!"Unknown becher object");
	};

	bo->id = GenObjectID();
	return bo;
}

BecherSystemObject * BecherMap::CreateSystemObject(EObjType type)
{
	switch (type)
	{
	case EBSys_Sound:
		return new SystemObjectSound(m_scene);
	default:
		assert(!"Unknown becher object");
	};

	return NULL;
}

EObjType BecherMap::GetObjectClass(EObjType type)
{
	switch (type)
	{
	case EBO_Factory:
	case EBO_Sugar:
	case EBO_HerbeWoman:
	case EBO_Farm:
	case EBO_Destilate:
	case EBO_Store:
	case EBO_Shop:
	case EBO_Saw:
		return EBC_Building;
	case EBO_Bridge:
	case EBO_WaterHole:
	case EBO_Troll:
	case EBO_Tree:
		return type;
	case EBSys_Sound:
		return EBC_System;
	default:
		assert(!"Unknown becher object");
	};
	return EBO_None;
}

IHoeScene * BecherMap::CreateScene()
{
	if (m_scene)
	{
		m_scene->Delete();
		m_scene = NULL;
	}
	this->Create(GetEngine()->CreateScene(HOETS_GRAPH));
	HoeGetRef(GetEngine())->SetBackgroundColor(0xffb060ff);
	return m_scene;
}

bool BecherMap::SetTerrainData()
{
	m_terrain = m_scene->GetSceneEnv()->CreateGridSurface();
	m_terrain->SetTexture(0, "trava", 4, 4);
	m_terrain->SetTexture(1, "ter_war3", 8, 4);
	m_terrain->SetTexture(2, "City_Dirt", 8, 4);
	m_terrain->SetTexture(3, "City_SquareTiles", 4, 4);
	m_terrain->SetTexture(4, "Cliff1", 4, 4);
	m_terrain->SetTexture(5, "Concrete", 4, 4);
	m_terrain->SetTexture(6, "MetalPlateCliff", 4, 4);
	m_terrain->SetTexture(7, "MetalPlateTiles", 4, 4);

	m_terrain->SetTexture(10, "utes_tex", 1, 1);

	for (int i=0;i < 12;i++)
	{	
		char s[4] = "m00";
		s[1] = ((i+1) / 10) + '0';
		s[2] = ((i+1) % 10) + '0';
		m_terrain->SetModel(i,s);
	}

	return true;
}

bool BecherMap::Load(BecherGameLoad & r, bool savegame)
{
	this->m_numobj = 0;
	this->m_numsysobj = 0;

	if (!r.ReadHeader())
		return false;
	if (r.IsSaveGame())
	{
		return false;
	}
	if (r.Chunk().ver < 5)
	{
		GetCon()->Printf("Error: Otevirani prilis stare mapy.");
		return false;
	}
	if (r.Chunk().ver > ID_BECHERVER)
	{
		GetCon()->Printf("Version is big (%d, req: %d). Update your executables.", r.Chunk(), ID_BECHERVER);
		return false;
	}
	r.ReadNext();

	while (r.Chunk().chunk != ID_CHUNK('e','n','d',' '))
	{
		size_t st = r.GetFile()->Tell();
		switch (r.Chunk().chunk)
		{
		case ID_CHUNK('t','e','r','r'):
			// nejdriv textury terena..
			m_terrain->LoadDump(&r);
			m_terrain->GetDesc(&m_sizeX,&m_sizeY, &m_numX, &m_numY);
			m_distX = m_sizeX / m_numX;
			m_distY = m_sizeY / m_numY;
#ifndef BECHER_EDITOR
			//m_terrain->ReleaseData();
#endif
			break;
		case ID_CHUNK('s','y','s','o'):	
			{ // system object
			//
			dword type = r.Read<dword>();
			BecherSystemObject * bs = CreateSystemObject((EObjType)type);
			bs->Load(r);
			AddSystemObject(bs);
			} break;
		case ID_CHUNK('o','b','j',' '):
			if (!savegame)
			{
				// nacist objekt
				if (!LoadObject(r))
					return false;
				break;
			}
			else
				r.Skip(r.Chunk().size);
			break;
		case ID_CHUNK('o','b','j','u'):
			if (!savegame)
			{
				// nacist objekt
				dword id = r.Read<dword>();
				BecherObject * bo = GetObjFromID(id);
				if (!bo || !bo->Load(r))
					return false;
				break;
			}
			else
				r.Skip(r.Chunk().size);
			break;
		default:
			r.Skip(r.Chunk().size);
			break;
		};
		// musi byt zakonceny chunkem
		if (r.GetFile()->Tell() != st + r.Chunk().size)
		{
			GetCon()->Printf("Error:Chunk %c%c%c%c size no match. chunk: %d, read size: %d",
				r.Chunk().chunk & 0xff,(r.Chunk().chunk & 0xff00) >> 8,
				(r.Chunk().chunk & 0xff0000) >> 16,(r.Chunk().chunk & 0xff000000) >> 24,
				st + r.Chunk().size, r.GetFile()->Tell());
			return false;
		}
		if (r.Read<dword>() != 123456789)
		{
			GetCon()->Printf("Error:Corupt file...");
			return false;
		}

		if (!r.ReadNext())
			break;
	}

	return true;
}

bool BecherMap::LoadObject(BecherGameLoad & r)
{
	TObjectSaveStruct s;
	r.Read(&s, sizeof(s));
	assert(s.type > EBO_None && s.type < EBO_Max);
	BecherObject * bo = this->CreateObject((EObjType)s.type);
	if (!bo)
	{
		GetCon()->Printf("Bad object type (%d)", s.type);
		return false;
	}
	bo->id = s.id;
	bo->SetAngle(s.angle);
	bo->SetPosition(s.x, s.y,m_scene->GetScenePhysics()->GetHeight(s.x,s.y));
	bo->Show(true);
	// presunout
	//bo->Load(r);
	AddObject(bo);
	ComputeLastID();

	return true;
}

bool BecherMap::SaveAllObjects(BecherGameSave &w)
{
	for (int i=0;i < GetNumObj();i++)
	{
		w.WriteChunk(ID_CHUNK('o','b','j',' '), 1);
		BecherObject * bo = GetObj(i);
		TObjectSaveStruct s;
		s.id = bo->id;
		s.type = bo->GetType();
		s.x = bo->GetPosX();
		s.y = bo->GetPosY();
		s.angle = bo->GetAngle();
		w.Write<TObjectSaveStruct>(s);
		//bo->Save(w);
		// write size
		w.WriteChunkEnd();
	}
	for (int i=0;i < GetNumObj();i++)
	{
		w.WriteChunk(ID_CHUNK('o','b','j','u'), 1);
		BecherObject * bo = GetObj(i);
		w.Write<dword>(bo->id);
		bo->Save(w);
		// write size
		w.WriteChunkEnd();
	}
	return true;
}

void BecherMap::AddObject(BecherObject * obj)
{
	assert(m_numobj < 998);
	m_obj[m_numobj++] = obj;
}

void BecherMap::AddSystemObject(BecherSystemObject * obj)
{
	assert(m_numsysobj < 99);
	m_sysobj[m_numsysobj++] = obj;
}

void BecherMap::DeleteObject(BecherObject * obj)
{
	int i = FindObjIndex(obj);
	if (i == -1)
		return;
	DeleteObject(i);
}

void BecherMap::DeleteObject(int index)
{
	// ten posledni se musi dat na smazanej
	assert(index >= 0 && index < m_numobj);
	delete m_obj[index];
	if (index != m_numobj-1)
	{
		m_obj[index] = m_obj[m_numobj-1];
	}
	m_numobj--;
}

int BecherMap::FindObjIndex(BecherObject * bo)
{
	for (int i=0;i < m_numobj;i++)
		if (m_obj[i] == bo)
			return i;
	return -1;
}

BecherObject * BecherMap::GetObject(const int x, const int y)
{
	return reinterpret_cast<BecherObject *>(GetView()->SelObject((float)x,(float)y));
}

void BecherMap::ComputeLastID()
{
	m_lastid = 0;
	for (int i=0;i < m_numobj;i++)
		if (m_obj[i]->id > m_lastid)
			m_lastid = m_obj[i]->id;
}

// najde objekt podle id
BecherObject * BecherMap::GetObjFromID(unsigned long id)
{
	for (int i=0;i < m_numobj;i++)
		if (m_obj[i]->id == id)
			return m_obj[i];
	return NULL;
}

//
