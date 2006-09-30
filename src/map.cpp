
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
#include "terrain.h"

BecherMap::BecherMap()
{
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

/*BecherObject * BecherMap::CreateXObject(EObjType type){

    BecherObject * bo = NULL;

	switch (type)
	{
	case EBO_Stones1:
    case EBO_Stones2:
    case EBO_Anthill:
    case EBO_Row:
		bo = new Addon(m_scene, type);
        break;
    default:
		assert(!"Unknown becher X object");
	};

	bo->id = GenObjectID();
	return bo;
}*/

BecherSystemObject * BecherMap::CreateSystemObject(EObjType type)
{
	switch (type)
	{
	case EBSys_Sound:
		return new SystemObjectSound(m_scene);
	case EBSys_Place:
		return new SystemObjectPlace(m_scene);
	case EBSys_Water:
		return new SystemObjectWater(m_scene);
	case EBSys_Coal:
		return new SystemObjectCoal(m_scene);
	case EBSys_Stone:
		return new SystemObjectStone(m_scene);
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
	case EBO_CoalMine:
	case EBO_StoneMine:
	case EBO_WaterHole:
		return EBC_Building;
	case EBO_Bridge:
	case EBO_Troll:
	case EBO_Tree:
		return type;
	case EBSys_Sound:
	case EBSys_Place:
	case EBSys_Water:
	case EBSys_Coal:
	case EBSys_Stone:
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
	::SetTerrainData(m_terrain);

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
	// tady musi byt unload

	//this->m_numobj = 0;
	//this->m_numsysobj = 0;
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
			// nahrat //
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
	m_obj.Add(obj);
}

void BecherMap::AddSystemObject(BecherSystemObject * obj)
{
	m_sysobj.Add(obj);
}

void BecherMap::DeleteObject(BecherObject * obj)
{
	// projet vsechny a oznamit jim novinu
	if (dynamic_cast<BecherBuilding*>(obj))
		dynamic_cast<BecherBuilding*>(obj)->SetMode(EBM_ToDelete);

	for (int i=0;i<m_obj.Count();i++)
	{
		m_obj[i]->OnDeleteObject(obj->GetID());
	}
	m_obj.Remove(obj);
	delete obj;
}

BecherObject * BecherMap::GetObject(const int x, const int y)
{
	return reinterpret_cast<BecherObject *>(GetView()->SelObject((float)x,(float)y));
}

void BecherMap::ComputeLastID()
{
	m_lastid = 0;
	for (int i=0;i < m_obj.Count();i++)
		if (m_obj[i]->id > m_lastid)
			m_lastid = m_obj[i]->id;
}

// najde objekt podle id
BecherObject * BecherMap::GetObjFromID(unsigned long id)
{
	for (int i=0;i < m_obj.Count();i++)
		if (m_obj[i]->id == id)
			return m_obj[i];
	return NULL;
}

//
