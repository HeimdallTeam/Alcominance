
#include "StdAfx.h"
#include "becher.h"
#include "buildings.h"
#include "elements.h"
#include "level.h"
#include "game.h"
#include "troll.h"

BecherLevel::BecherLevel()
{
	this->m_build = NULL;
	m_filename[0] = '\0';
	m_select = NULL;
}

void BecherLevel::Update(float time)
{
	m_timer.Update(time);
	// update objects
	for (int i=0;i < this->m_numobj;i++)
	{
		this->m_obj[i]->Update(time);
	}
}

void BecherLevel::MouseUpdate(float x, float y)
{
	if (m_build)
	{
		float X,Y;
		m_build->Show(true);
		if (GetView()->GetPick(x,y,&X,&Y))
			m_build->SetPosition(X,Y);
	}
	else // show select
	{
	}
}

void BecherLevel::MouseLeave() 
{
	if (m_build)
		m_build->Show(false);
}

void BecherLevel::SetBuildObject(BecherObject * bo, int gold, int wood, int stone)
{
	if (m_build)
		delete m_build;

	m_build = bo;
	m_buildgold = gold;
	m_buildwood = wood;
	m_buildstone = stone;
	m_buildinit = m_select;
	bo->Show(false);
}

void BecherLevel::Start()
{
	m_timer.Set(HoeGame::Timer::tmClock, 0);
	m_timer.Start();
}

void BecherLevel::MouseLeftDown(float x, float y)
{
	if (m_build)
	{
		//m_buildcandidate.wood,m_buildcandidate.stone);
		//assert((this->GetSelectionCount() == 1));
		//assert(this->GetSelectedObject(0)->GetType() == EBO_Troll);
		//reinterpret_cast<Troll*>(this->GetSelectedObject(0))->Build(reinterpret_cast<BecherBuilding*>(o));
		float X,Y;
		if (GetView()->GetPick(x,y,&X,&Y))
		{
			// build
			m_build->SetPosition(X,Y);
			assert(GetObjectClass(m_build->GetType()));
			if (dynamic_cast<BecherBuilding*>(m_build)->StartBuilding(m_buildgold, m_buildwood, m_buildstone))
			{
				this->AddObject(m_build);
				//if (m_buildinit && m_buildinit->GetType() == EBO_Troll)
				//	dynamic_cast<BecherBuilding*>(m_build)->Assign(dynamic_cast<Troll*>(m_buildinit));
				m_build = NULL;
			}
		}
		return;
	}

    BecherObject * o = dynamic_cast<BecherObject *>(GetView()->SelObject(x,y));
	if (o)
		SelectObject(o);
	else
		SelectObject(NULL);
}

void BecherLevel::SelectObject(BecherObject * so)
{
	if (m_select)
		m_select->Unselect();
	if (!so || so->Select())
		m_select = so;
}

bool BecherLevel::SaveGame(const char * path)
{
	HoeGame::HoeFile file;
	if (!file.Open(path, hftWrite))
		return false;
	BecherGameSave w(&file);

	MapChunk head = {ID_BSAVE, ID_BECHERVER, GetNumObj()};
	w.Write(&head, sizeof(head));

	// save map info (ktera mapa)
	size_t sfn = strlen(m_filename);
	w.Write(&sfn, sizeof(sfn));
	w.Write(m_filename, sfn);

	// save timer
	m_cash.Save(w);
	m_timer.Save(w);

	// save view
	float view[4];
	GetView()->GetTargetPosition( &view[0], &view[1]);
	view[2] = GetView()->GetAngle();
	view[3] = GetView()->GetDistance();
	w.Write(view, sizeof(view));

	// save objects
	SaveAllObjects(w);

	return true;
}

bool BecherLevel::LoadGame(BecherGameLoad &r)
{
	int ver = r.Chunk().ver;
	size_t sfn;
	r.Read(&sfn, sizeof(sfn));
	r.Read(m_filename, sfn);
	m_filename[sfn] = '\0';

	HoeGame::HoeFile file;
	file.Open(m_filename);
	BecherGameLoad rr(&file);
	Load( rr, true);

	// load timer and casch
	m_cash.Load( ver, r);
	m_timer.Load( ver, r);

	float view[4];
	r.Read(view, sizeof(view));
	GetView()->SetTargetPosition( view[0], view[1]);
	GetView()->SetAngle(view[2]);
	GetView()->SetDistance(view[3]);

	r.ReadNext();
	//LoadObjects(r);
	assert(0);

	return true;
}

bool BecherLevel::LoadGame(const char *path)
{
	Create(CreateScene());
	CreateTerrain()

	HoeGame::HoeFile file;
	if (!file.Open(path))
	{
		GetCon()->Printf("Open file %s failed.", path);
		return false;
	}

	BecherGameLoad r(&file);
	if (!r.ReadHeader())
		return false;
	if (!r.IsSaveGame())
	{
		strncpy(m_filename, path, sizeof(m_filename));
		if (!Load( r, false))
			return false;		
	}
	else 
	{
		if (!LoadGame(r))
			return false;
	}

	return true;
}

Store * BecherLevel::FindBestStore()
{
	assert(!"FindBestStore");
	return NULL;
}

////////////////////////////////////////////////
bool BecherTime::Save(HoeFileWriter &w)
{
	return true;
}

bool BecherTime::Load(int ver, HoeFileReader &r)
{
	return true;
}

BecherCash::BecherCash()
{
	Set(0,-100000);
}

void BecherCash::Set(int cash, int limit)
{
	m_limit = limit;
	m_cash = cash;
}

bool BecherCash::Save(HoeFileWriter &w)
{
	//w.Write<int>(m_cash);
	//w.Write<int>(m_limit);
	return true;
}

bool BecherCash::Load(int ver, HoeFileReader &r)
{
	//m_cash = r.Read<int>();
	//m_limit = r.Read<int>();
	Set(0,-100000);
	return true;
}

void BecherCash::Paint(IHoe2D * d2)
{
		// draw digit
	static IHoePicture * p = GetResMgr()->Get<IHoePicture>(ID_DIGITFONT);
	d2->SetRect(20,20);
	const int np = 7;
	const int rl = 18;
	const int ll = 15;

	int i = GetValue();
	bool sign = false;
	if (i < 0)
	{
		sign = true;
		i = -i;
	}

	const float pp = ((float)(rl-ll)) / np;
	float up = rl;
	if (i == 0)
	{
		THoeRect r = {up-pp, 0, up, 1};
		THoeRect r2={ 0, 0, 1*(60.0/64.0/4), 1*(60.0/64.0/3)};
		d2->Blt(&r, p, &r2);
		i = i / 10;
		up -= pp;
	}
	while (i != 0)
	{
		THoeRect r = {up-pp, 0, up, 1};
		int t=i%10;
		THoeRect r2={ (t%4)*(60.0f/64.0f/4), (t/4)*(60.0f/64.0f/3), (t%4+1)*(60.0f/64.0f/4), (t/4+1)*(60.0f/64.0f/3)};
		d2->Blt(&r, p, &r2);
		i = i / 10;
		up -= pp;
	}
	if (sign)
	{
		THoeRect r = {up-pp, 0, up, 1};
		THoeRect r2={ 2*(60.0/64.0/4), 2*(60.0/64.0/3), 3*(60.0/64.0/4), 3*(60.0/64.0/3)};
		d2->Blt(&r, p, &r2);
		i = i / 10;
		up -= pp;
	}
}













