
#include "../StdAfx.h"
#include "../becher.h"
#include "../buildings.h"
//#include "elements.h"
#include "editor_map.h"
//#include "plugin_editlevel.h"

EditorMap * EditorMap::s_actmap = NULL;

void EditorMap::CreateNew()
{
	this->Create(GetEngine()->CreateScene(HOETS_GRAPH));
	GetEngine()->SetActiveScene(m_scene);
	HoeGetRef(GetEngine())->SetBackgroundColor(0xffb060ff);
	m_mapfilepath = wxT("");
	m_terrain = m_scene->GetSceneEnv()->CreateGridSurface();
	m_terrain->SetSize(3000.f, 3000.f);
	m_terrain->GenerateHeight(50,50);

}

void EditorMap::AddNewObject(unsigned long type)
{
	SAFE_DELETE(m_createobject);

	m_createobject = CreateObject(type);
	m_createobject->Show(false);
	//m_createobject->SetPosition(0,0,0);
	//m_createobject->SetOrientation(0,1,0,rand());
}

void EditorMap::LeaveObject()
{
	SAFE_DELETE(m_createobject);
}

void EditorMap::BuildObject()
{
	AddObject(m_createobject);
	m_createobject->Select();
	m_createobject = NULL;
}

bool EditorMap::SaveMap(const wxString &path)
{
	// open file for write
	HoeEditor::EditorFile file;
	if (!file.OpenWrite(path))
	{
		wxLogMessage(_("Failed open file %s for writing."), path.c_str());
		return false;
	}

	// write nejakej header
	MapChunk head = { ID_BECHERFILE, ID_BECHERVER, 0 };
	file.Write(&head, sizeof(head));

	// zakladni data (teren atd)
	

	// objekty
	HoeFileWriter w(&file);
	SaveObjects(w);

	return true;
}

bool EditorMap::SaveMap()
{
	assert(!m_mapfilepath.IsEmpty());
	return SaveMap(m_mapfilepath);
}

void EditorMap::SelectObject(const int x, const int y)
{
	BecherObject * o = GetObject(x,y);
	if (o)
		o->Select();
	else
		GetProp()->Begin(NULL);
}

void EditorMap::SetFilePath(const wxString &path)
{
	m_mapfilepath = path;
}

wxString EditorMap::GetFilePath()
{
	return this->m_mapfilepath;
}

wxString EditorMap::GetTitle()
{
	return this->m_mapfilepath;
}




