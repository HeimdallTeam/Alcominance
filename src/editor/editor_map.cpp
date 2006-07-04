
#include "../StdAfx.h"
#include "../becher.h"
#include "../buildings.h"
//#include "elements.h"
#include "editor_map.h"
//#include "plugin_editlevel.h"

void EditorMap::CreateNew()
{
	CreateScene();
	GetEngine()->SetActiveScene(m_scene);
	m_mapfilepath = wxT("");
	m_terrain = m_scene->GetSceneEnv()->CreateGridSurface();
	m_terrain->SetTexture(0, "ter_war3", 8, 4);
	m_terrain->Create(300.f, 300.f,20,20);
	// textures
}

bool EditorMap::LoadMap(const wxString &path)
{
	HoeEditor::EditorFile file;
	if (!file.OpenRead(path))
		return false;
	BecherMapLoader r(&file);
	CreateScene();
	GetEngine()->SetActiveScene(m_scene);
	m_terrain = m_scene->GetSceneEnv()->CreateGridSurface();
	m_terrain->SetTexture(0, "ter_war3", 8, 4);
	return Load( r, true);
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
	MapChunk terr = { ID_CHUNK('t','e','r','r'), 0, 0 };
	file.Write(&terr, sizeof(terr));
	m_terrain->Dump(&file);

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




