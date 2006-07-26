
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
	}
}

//////////////////////////////////////////////////////
// ToolCreateObject
ToolCreateObject::ToolCreateObject(long type, bool repeat, bool randori)
{
	m_type = type;
	m_repeat = repeat;
	m_rand = randori;
	// vytvorit m_obj
	m_obj = BecherEdit::Get()->GetActMap()->CreateObject(type);
	m_obj->Show(false);
	if (m_rand)
	{
		m_obj->SetAngle((rand() % 628) * 0.01f);
	}
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

void ToolCreateObject::LeftDown(const int x, const int y, const wxMouseEvent &e)
{
	SetPos(x,y);
	BecherEdit::Get()->GetActMap()->AddObject(m_obj);
	if (m_repeat || e.ControlDown())
	{
		m_obj = BecherEdit::Get()->GetActMap()->CreateObject(m_type);
		m_obj->Show(true);
		SetPos(x,y);
		if (m_rand)
		{
			m_obj->SetAngle((rand() % 628) * 0.01f);
		}
	}
	else
	{
		m_obj = NULL;
		BecherEdit::Get()->SetTool(NULL);
	}

}

void ToolCreateObject::LeftUp(const int x, const int y, const wxMouseEvent &e)
{
	/*if (m_map)
		m_map->m_lockobject = NULL;
	*/
}

void ToolCreateObject::RightDown(const int x, const int y, const wxMouseEvent &e)
{
	BecherEdit::Get()->SetTool(NULL);
}

void ToolCreateObject::Wheel( const wxMouseEvent &e)
{
    if (e.ControlDown())
        m_obj->SetAngle(m_obj->GetAngle() + e.GetWheelRotation() / 500.f);
}

//////////////////////////////////////////////////////
// ToolTerrain
ToolTerrain::ToolTerrain(byte set)
{
	m_set = set;
}

void ToolTerrain::LeftDown(const int x, const int y, const wxMouseEvent &e)
{
	float sx,sy;
	if (BecherEdit::Get()->GetActMap()->GetView()->GetPick(x,y,&sx,&sy))
	{
		EditorMap & m = *BecherEdit::Get()->GetActMap();
		IHoeEnv::GridSurface::TGridDesc desc;
		// 300 / 20
		//const uint nx = (uint)((sx+(m.m_sizeX+m.m_distX)*0.5f)/(m.m_distX));
		//const uint ny = (uint)((sy+(m.m_sizeY+m.m_distY)*0.5f)/(m.m_distY));
		const uint nx = (uint)((sx+(m.m_sizeX)*0.5f)/(m.m_distX));
		const uint ny = (uint)((sy+(m.m_sizeY)*0.5f)/(m.m_distY));
		m.GetTerrain()->GetGridDesc(nx,ny,&desc);
		desc.tex2 = m_set;
		m.GetTerrain()->SetGridDesc(nx,ny,&desc);
		m.GetTerrain()->Load();
	}
}

void ToolTerrain::RightDown(const int x, const int y, const wxMouseEvent &e)
{
	BecherEdit::Get()->SetTool(NULL);
}

void ToolTerrain::Move(int relX, int relY, int absX, int absY, const wxMouseEvent & ev)
{
	float sx,sy;
	if (BecherEdit::Get()->GetActMap()->GetView()->GetPick(absX,absY,&sx,&sy))
	{
		EditorMap & m = *BecherEdit::Get()->GetActMap();
		const uint nx = (uint)((sx+(m.m_sizeX)*0.5f)/(m.m_distX));
		const uint ny = (uint)((sy+(m.m_sizeY)*0.5f)/(m.m_distY));
		wxString str;
		str.Printf("x: %f y: %f nx: %d ny: %d", sx, sy, nx, ny);
		BecherEdit::Get()->SetStatus(str);
	}
}

void ToolTerrain::Wheel(const wxMouseEvent &e)
{
	float sx,sy;
	if (BecherEdit::Get()->GetActMap()->GetView()->GetPick(e.GetX(),e.GetY(),&sx,&sy))
	{
		EditorMap & m = *BecherEdit::Get()->GetActMap();
		//const uint nx = (uint)((sx+(m.m_sizeX+m.m_distX)*0.5f)/(m.m_distX));
		//const uint ny = (uint)((sy+(m.m_sizeY+m.m_distY)*0.5f)/(m.m_distY));
		const uint nx = (uint)((sx+(m.m_sizeX)*0.5f)/(m.m_distX));
		const uint ny = (uint)((sy+(m.m_sizeY)*0.5f)/(m.m_distY));
		IHoeEnv::GridSurface::TGridDesc desc;
		m.GetTerrain()->GetGridDesc(nx,ny,&desc);
		int d = desc.x2 * 4 + desc.y2;
		d += e.GetWheelRotation() > 0 ? 31:1;
		d = d % 32;
		desc.x2 = d / 4;
		desc.y2 = d % 4;
		m.GetTerrain()->SetGridDesc(nx,ny,&desc);
        m.GetTerrain()->Load();
	}

}

