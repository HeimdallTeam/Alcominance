
#include "../StdAfx.h"
#include "../becher.h"
#include "../buildings.h"
//#include "elements.h"
#include "editor_map.h"
//#include "plugin_editlevel.h"

void EditorMap::CreateNew()
{
	m_numX = 4;
	m_numY = 4;
	m_sizeX = 80.f;
	m_sizeY = 80.f;
	m_distX = m_sizeX / m_numX;
	m_distY = m_sizeY / m_numY;
	CreateScene();
	GetEngine()->SetActiveScene(m_scene);
	m_mapfilepath = wxT("");
	m_terrain = m_scene->GetSceneEnv()->CreateGridSurface();
	m_terrain->SetTexture(0, "trava", 4, 4);
	m_terrain->SetTexture(1, "ter_war3", 8, 4);
	m_terrain->Create(m_sizeX, m_sizeY,m_numX,m_numY);

	// vytvorit
	for (uint x=0;x < 4;x++)
	for (uint y=0;y < 4;y++)
	{
		IHoeEnv::GridSurface::TGridDesc desc;
		m_terrain->GetGridDesc(x,y,&desc);
		desc.x2 = x;
		desc.y2 = y;
		desc.tex2 = 1;
		m_terrain->SetGridDesc(x,y,&desc);
	}

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
	m_terrain->SetTexture(0, "trava", 4, 4);
	m_terrain->SetTexture(1, "ter_war3", 8, 4);
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




