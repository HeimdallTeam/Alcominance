
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
#include "b_msg.h"

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


Addon * BecherMap::CreateAddOnObject(EObjType type)
{
	switch (type)
	{
	case EBAO_Smoke:
		return new AddonSmoke(m_scene);
	default:
		return new Addon(m_scene, type);
	};
}

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

bool BecherMap::LoadMapChunk(BecherGameLoad & r)
{
	// tady musi byt unload
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
	case ID_CHUNK('a','d','o','n'):
		{
			ChunkDictRead dict(r);
			Addon * bo = CreateAddOnObject((EObjType)dict.KeyInt("type",0));
			float x = dict.KeyFloat("x", 0.f);
			float y = dict.KeyFloat("y", 0.f);
			float z = dict.KeyFloat("height", m_scene->GetScenePhysics()->GetHeight(x,y));
			float s = dict.KeyFloat("scale", 1.f);
#ifdef BECHER_EDITOR
			bo->SetPosition(x,y,z);
			bo->SetAngle(dict.KeyFloat("angle", 0.f));
#else
			bo->SetPosition(x,z,y);
			bo->SetOrientation(0.f,1.f,0.f,dict.KeyFloat("angle", 0.f));
#endif
			bo->SetScale(s);
			bo->Show(true);
			// load
			AddAddonObject(bo);
		}
		break;
	case ID_CHUNK('s','y','s','o'):	
		{ // system object
			ChunkDictRead dict(r);
			BecherSystemObject * bs = CreateSystemObject((EObjType)dict.KeyInt("type",0));
			bs->Load(dict);
			AddSystemObject(bs);
		} break;
	case ID_CHUNK('o','b','j','s'):
		{
			r.Read<unsigned int>();
			int no = r.Read<unsigned int>();
			for (int i=0;i < no;i++)
			{
				// nacist objekt
				if (!LoadObject(r))
					return false;
			}
		}
		break;
	case ID_CHUNK('o','b','j',' '):
		{
			ChunkDictRead dict(r);
			int id = dict.KeyInt("id", -1);
			BecherObject * bo = GetObjFromID(id);
			if (!bo || !bo->Load(dict))
				return false;
			break;
		}
		break;
	};

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
	// ulozit hlavicky vsech objektu
	w.WriteChunk(ID_CHUNK('o','b','j','s'));
	// ulozit verzi a pocet
	w.WriteValue<unsigned int>(2);
	w.WriteValue<unsigned int>((unsigned int)GetNumObj());
	for (int i=0;i < GetNumObj();i++)
	{
		BecherObject * bo = GetObj(i);
		TObjectSaveStruct s;
		s.id = bo->id;
		s.type = bo->GetType();
		s.x = bo->GetPosX();
		s.y = bo->GetPosY();
		s.angle = bo->GetAngle();
		w.Write<TObjectSaveStruct>(s);
	}
	w.WriteChunkEnd();
	// zacatek ukladani jednotlivych objektu
	for (int i=0;i < GetNumObj();i++)
	{
		w.WriteChunk(ID_CHUNK('o','b','j',' '));
		// slovnik konecne
		ChunkDictWrite dict(w);
		dict.Begin();
		BecherObject * bo = GetObj(i);
		dict.Key("id", (int)bo->id);
		bo->Save(dict);
		dict.End();
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

void BecherMap::AddAddonObject(Addon * obj)
{
    m_addon.Add(obj);
}

void BecherMap::DeleteObject(BecherObject * obj)
{
	// projet vsechny a oznamit jim novinu
	if (dynamic_cast<BecherBuilding*>(obj))
		dynamic_cast<BecherBuilding*>(obj)->SetMode(EBM_ToDelete);

	int id = obj->GetID();
	SendGameMsgId(IDMSG_ALL, BMSG_DeleteObject, id);
	m_obj.Remove(obj);
	delete obj;
}

void BecherMap::DeleteAddonObject(Addon * obj)
{	
	m_addon.Remove(obj);
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
BecherObject * BecherMap::GetObjFromID(unsigned int id)
{
	for (int i=0;i < m_obj.Count();i++)
		if (m_obj[i]->id == id)
			return m_obj[i];
	return NULL;
}

//
