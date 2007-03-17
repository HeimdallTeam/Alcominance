
#include "StdAfx.h"
#include "becher.h"
#include "sysobjs.h"

SystemObjectSound::SystemObjectSound(IHoeScene * scn) : BecherSystemObject(scn) 
{
#ifdef BECHER_EDITOR
	SetModel(GetModelForSound());
#endif
}


bool SystemObjectSound::Load(BecherGameLoad &r)
{
	float x,y;
	x = r.Read<float>();
	y = r.Read<float>();
#ifdef BECHER_EDITOR
	this->SetPosition(x,y,0);
	this->Show(true);
#endif
	return true;
}


#ifndef BECHER_EDITOR

bool SystemObjectSound::Save(ChunkDictWrite &w)
{
	assert(!"this code not run");
	return true;
}

#else
void SystemObjectSound::SetPosition(const float x, const float y, const float h)
{
	//light->SetPosition(x,h+20,y);
	BecherObject::SetPosition(x,y,h);
}

bool SystemObjectSound::Save(ChunkDictWrite &w)
{
	// ulozit pozici
	//w.WriteValue<float>(this->GetPosX());
	//w.WriteValue<float>(this->GetPosY());
	return true;
}

IHoeModel * SystemObjectSound::GetModelForSound()
{
	static IHoeModel * mod = NULL;
	if (mod == NULL)
	{
		mod = (IHoeModel *)GetEngine()->Create("generate model box 10");
	}
	return mod;
}

bool SystemObjectSound::Select()
{
	GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Sound"));
	GetProp()->AppendBool(0, _("Loop"), false);
	GetProp()->AppendCategory(_("Test"));
	GetProp()->AppendBool(0, _("Play"), false);
	GetProp()->End();	
	return true;
}

void SystemObjectSound::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}

#endif

///////////////////////////////////////////////////////////////////////////
SystemObjectPlace::SystemObjectPlace(IHoeScene * scn) : BecherSystemObject(scn) 
{
#ifdef BECHER_EDITOR
	SetModel(GetModelForPlace());
#endif
}


bool SystemObjectPlace::Load(BecherGameLoad &r)
{
	float x,y;
	x = r.Read<float>();
	y = r.Read<float>();
#ifdef BECHER_EDITOR
	this->SetPosition(x,y,0);
	this->Show(true);
#endif
	return true;
}


#ifndef BECHER_EDITOR

bool SystemObjectPlace::Save(ChunkDictWrite &w)
{
	assert(!"this code not run");
	return true;
}

#else
void SystemObjectPlace::SetPosition(const float x, const float y, const float h)
{
	//light->SetPosition(x,h+20,y);
	BecherObject::SetPosition(x,y,h);
}

bool SystemObjectPlace::Save(ChunkDictWrite &w)
{
	// ulozit pozici
	//w.WriteValue<float>(this->GetPosX());
	//w.WriteValue<float>(this->GetPosY());
	return true;
}

IHoeModel * SystemObjectPlace::GetModelForPlace()
{
	static IHoeModel * mod = NULL;
	if (mod == NULL)
	{
		mod = (IHoeModel *)GetEngine()->Create("generate model box 10");
	}
	return mod;
}

bool SystemObjectPlace::Select()
{
	GetProp()->Begin(this);
	GetProp()->AppendCategory(_("Sound"));
	GetProp()->AppendBool(0, _("Loop"), false);
	GetProp()->AppendCategory(_("Test"));
	GetProp()->AppendBool(0, _("Play"), false);
	GetProp()->End();	
	return true;
}

void SystemObjectPlace::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
}

#endif

////////////////////////////////////////////////////////////////////////////
SystemObjectSource::SystemObjectSource(IHoeScene * scn) : BecherSystemObject(scn) 
{
#ifdef BECHER_EDITOR
	SetModel(GetModelForSource());
#endif
	m_distance = 100.f;
	m_source = 2000;
}


bool SystemObjectSource::Load(BecherGameLoad &r)
{
	float x,y;
	x = r.Read<float>();
	y = r.Read<float>();
	m_x = x;
	m_y = y;
	m_distance = r.Read<float>();
	m_source = r.Read<uint>();
#ifdef BECHER_EDITOR
	this->SetPosition(x,y,0);
	this->Show(true);
#endif
	return true;
}

float SystemObjectSource::GetDistance(float x, float y)
{
	x -= m_x;
	y -= m_y;
	float d = sqrtf(y*y+x*x);
	d = 1.f - (d / m_distance);
	if (d < 0.f)
		return 0.f;
	if (d > 1.f)
		return 1.f;
	return d;
}

#ifndef BECHER_EDITOR

bool SystemObjectSource::Save(ChunkDictWrite &w)
{
	assert(!"this code not run");
	return true;
}

#else
void SystemObjectSource::SetPosition(const float x, const float y, const float h)
{
	//light->SetPosition(x,h+20,y);
	BecherObject::SetPosition(x,y,h);
}

bool SystemObjectSource::Save(ChunkDictWrite &w)
{
	// ulozit pozici
	/*w.WriteValue<float>(this->GetPosX());
	w.WriteValue<float>(this->GetPosY());
	w.WriteValue<float>(m_distance);
	w.WriteValue<uint>(m_source);*/
	return true;
}

IHoeModel * SystemObjectSource::GetModelForSource()
{
	static IHoeModel * mod = NULL;
	if (mod == NULL)
	{
		mod = (IHoeModel *)GetEngine()->Create("generate model box 10");
	}
	return mod;
}

bool SystemObjectSource::Select()
{
	GetProp()->Begin(this);
	switch (GetType())
	{
	case EBSys_Water:
		GetProp()->AppendCategory(_("Water"));
		break;
	case EBSys_Coal:
		GetProp()->AppendCategory(_("Coal"));
		break;
	case EBSys_Stone:
		GetProp()->AppendCategory(_("Stone"));
		break;
	};
	// vzdalenost
	GetProp()->AppendFloat(0, _("Distance"), m_distance); 
	GetProp()->AppendLong(1, _("Source"), m_source);
	GetProp()->End();	
	return true;
}

void SystemObjectSource::OnChangeProp(int id, const HoeEditor::PropItem & pi)
{
	switch (id)
	{
	case 0:
		m_distance = pi.GetFloat();
		break;
	case 1:
		m_source = pi.GetLong();
		break;
	};
}

#endif


