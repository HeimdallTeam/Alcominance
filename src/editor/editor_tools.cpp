
#include "../StdAfx.h"
#include "editor_tools.h"
#include "../buildings.h"
#include "../becher.h"
#include "editor_map.h"
#include "editor.h"

//////////////////////////////////////////////////////
// ToolSelect
void ToolSelect::LeftDown(const int x, const int y, wxMouseEvent &e)
{
	if (e.ControlDown())
	{
		//m_map->m_lockobject = m_map->GetObject(x,y);
	}
	else
	{
		assert(BecherEdit::Get()->GetActMap());
		BecherEdit::Get()->GetActMap()->SelectObject(x,y);
		/*IHoeEnv::GridSurface::TGridDesc desc;
		m_map->GetTerrain()->Get()->GetGridDesc(0,0,&desc);
		desc.x1 = (desc.x1+1)%8;
		m_map->GetTerrain()->Get()->SetGridDesc(0,0,&desc);*/
		//m_map->SelectObject(x,y);
	}
}

//////////////////////////////////////////////////////
// ToolCreateObject
ToolCreateObject::ToolCreateObject(long type)
{
	m_type = type;
	// vytvorit m_obj
	m_obj = BecherEdit::Get()->GetActMap()->CreateObject(type);
	m_obj->Show(false);
}

ToolCreateObject::~ToolCreateObject()
{
	SAFE_DELETE(m_obj);
}

void ToolCreateObject::SetPos(int absX, int absY)
{
	float sx,sy;
	BecherEdit::Get()->GetActMap()->GetView()->GetPick(absX,absY,&sx,&sy);
	m_obj->SetPosition(sx,sy); 
}

void ToolCreateObject::Enter(int absX, int absY)
{
	m_obj->Show(true);
	SetPos(absX, absY);
}

void ToolCreateObject::Leave()
{
	m_obj->Show(false);
}

void ToolCreateObject::Move(int relX, int relY, int absX, int absY, const wxMouseEvent &ev)
{
	/*float sx,sy;
	if (m_map && m_map->GetCreatedObject())
	{
		
		if (m_map->GetView()->GetPick(absX,absY,&sx,&sy))
			m_map->GetCreatedObject()->SetPosition(sx,sy); 
	}
	else if (m_map && m_map->m_lockobject)
	{
		if (m_map->GetView()->GetPick(absX,absY,&sx,&sy))
			m_map->m_lockobject->SetPosition(sx,sy); 

	}*/
	SetPos( absX, absY);
}

void ToolCreateObject::LeftDown(const int x, const int y, wxMouseEvent &e)
{
	SetPos(x,y);
	BecherEdit::Get()->GetActMap()->AddObject(m_obj);
	m_obj = NULL;
	/*m_obj = BecherEdit::Get()->GetActMap()->CreateObject(m_type);
	m_obj->Show(true);
	SetPos(x,y);*/
	BecherEdit::Get()->SetTool(NULL);
}

void ToolCreateObject::LeftUp(const int x, const int y, wxMouseEvent &e)
{
	/*if (m_map)
		m_map->m_lockobject = NULL;
	*/
}

void ToolCreateObject::RightDown(const int x, const int y, wxMouseEvent &e)
{
	BecherEdit::Get()->SetTool(NULL);
}

void ToolCreateObject::Wheel(wxMouseEvent &e)
{
	m_obj->SetAngle(m_obj->GetAngle() + e.GetWheelRotation() / 500.f);
}

//////////////////////////////////////////////////////
// ToolTerrain
void ToolTerrain::LeftDown(const int x, const int y, wxMouseEvent &e)
{
	IHoeEnv::GridSurface::TGridDesc desc;
	BecherEdit::Get()->GetActMap()->GetTerrain()->Get()->GetGridDesc(0,0,&desc);
	desc.x1 = (desc.x1+1)%8;
	BecherEdit::Get()->GetActMap()->GetTerrain()->Get()->SetGridDesc(0,0,&desc);
}

void ToolTerrain::RightDown(const int x, const int y, wxMouseEvent &e)
{
	BecherEdit::Get()->SetTool(NULL);
}

