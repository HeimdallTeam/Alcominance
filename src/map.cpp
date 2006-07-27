
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
#include "map.h"

BecherMap::BecherMap()
{
	m_numobj = 0;
	m_lastid = 0;
	m_terrain = NULL;
}

BecherObject * BecherMap::CreateObject(unsigned long type)
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
	default:
		assert(!"Unknown becher object");
	};

	bo->id = GenObjectID();
	return bo;
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
		return EBC_Building;
	case EBO_Bridge:
	case EBO_WaterHole:
	case EBO_Troll:
	case EBO_Tree:
		return type;
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

bool BecherMap::Load(BecherGameLoad & r, bool loadobj)
{
	this->m_numobj = 0;

	if (r.Chunk().chunk == 0)
		r.ReadNext();

	if (r.Chunk().chunk != ID_BECHERFILE)
	{
		GetCon()->Printf("Bad format file");
		return false;
	}
	if (r.Chunk().ver > ID_BECHERVER)
	{
		GetCon()->Printf("Version is big (%d, req: %d). Update your executables.", r.Chunk(), ID_BECHERVER);
		return false;
	}
	r.ReadNext();

	while (r.Chunk().chunk != ID_CHUNK('O','b','j','s'))
	{
		switch (r.Chunk().chunk)
		{
		case ID_CHUNK('t','e','r','r'):
			// nejdriv textury terena..
			m_terrain->LoadDump(&r);
			m_terrain->GetDesc(&m_sizeX,&m_sizeY, &m_numX, &m_numY);
			m_distX = m_sizeX / m_numX;
			m_distY = m_sizeY / m_numY;
			break;
		};
		if (!r.ReadNext())
			break;;
	}
	// nejdrive jsou globals

	// pak mapa
	/*IHoeMaterial * mat = this->GetScene()->GetSceneEnv()->GetMaterial("trava");
	IHoeEnv::Poly * p = this->GetScene()->GetSceneEnv()->CreatePolygon(4);
	const float sizev = 500;
	float v[] = { -sizev, 0, -sizev, 0, 0,
					-sizev, 0, sizev, 0, 1,
					sizev, 0, sizev, 1, 1, 
					sizev, 0 , -sizev, 1, 0,
					 };
	p->SetPos(v, sizeof(float)*5);
	p->SetTex(v + 3, sizeof(float)*5);
	p->SetMaterial(mat);
	p->Release();*/

	// nakonec objekty
	// vsechno pres chunky


	// musi byt az na konci !!!
	if (r.Chunk().chunk != ID_CHUNK('O','b','j','s'))
	{
		GetCon()->Printf("Corupt file. chunk objs req");
		return false;
	}
	if (loadobj)
	{
		LoadObjects(r);
	}

	// svetla
	/*IHoeLight * l = m_scene->CreateLight(false);
	l->SetPosition(0,0, 50);
	l->SetColor(1,1,0.5f);*/

	return true;
}

bool BecherMap::LoadObjects(BecherGameLoad & r)
{
	int numobj = r.Chunk().reserved;
	int ver = r.Chunk().ver;
	// read objects
	for (int i=0;i < numobj;i++)
	{
		TObjectSaveStruct s;
		r.Read(&s, sizeof(s));
		BecherObject * bo = this->CreateObject(s.type);
		if (!bo)
		{
			GetCon()->Printf("Bad object type (%d)", s.type);
			return false;
		}
		bo->id = s.id;
		bo->SetAngle(s.angle);
		bo->SetPosition(s.x, s.y);
		bo->Show(true);
		bo->Load(r);
		AddObject(bo);
		if (r.Read<dword>() != 123456789)
		{
			GetCon()->Printf("Corupt file obj(%d)",i);
			return false;
		}
	}

	ComputeLastID();

	return true;
}

bool BecherMap::SaveObjects(BecherGameSave &w)
{
	MapChunk chobj = { ID_CHUNK('O','b','j','s'), 1, GetNumObj()};
	w.Write(&chobj, sizeof(MapChunk));
	for (int i=0;i < GetNumObj();i++)
	{
		BecherObject * bo = GetObj(i);
		TObjectSaveStruct s;
		HoeFileAutoWriter hw(w.GetFile(), &s, sizeof(s));
		s.id = bo->id;
		s.type = bo->GetType();
		s.x = bo->GetPosX();
		s.y = bo->GetPosY();
		s.angle = bo->GetAngle();
		s.advsize = (unsigned long)w.GetFile()->Tell();
		bo->Save(w);
		s.advsize = (unsigned long)w.GetFile()->Tell() - s.advsize;
		hw.Flush();
		// write check
		w.WriteValue<dword>(123456789);
	}
	return true;
}

void BecherMap::AddObject(BecherObject * obj)
{
	assert(m_numobj < 998);
	m_obj[m_numobj++] = obj;
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
