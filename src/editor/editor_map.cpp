
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
	EditorMap::FileHeader head = { ID_BECHERFILE, ID_BECHERVER, this->GetNumObj()};
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

/*

enum 
{
	PROPERTY_NAME,
	PROPERTY_AUTHOR,
	PROPERTY_TERRAIN_SIZE,
	PROPERTY_TERRAIN_HEIGHTMAP,
	PROPERTY_TERRAIN_REALSIZE,
	PROPERTY_TERRAIN_HEIGHTSCALE,
	PROPERTY_PATH,
	PROPERTY_VIEW_ROT,

	PROPERTY_OBJECT = 200,
	PROPERTY_POSITION,
	PROPERTY_ROTATION,
	PROP_TREE_WOOD,
	PROP_TREE_HEIGHT,
	PROP_TREE_TYPE,
};

typedef wxPixelData<wxBitmap, wxAlphaPixelFormat> PixelData;

/////////////////////////////////////////////////////////////

BecherEditorMap::BecherEditorMap()
{
	m_scene = HoeExtern::IEditor::Get()->GetEngine()->CreateScene(HOETS_TERRAIN);
	m_levelitem = -1;
	Create(m_scene);
	m_mselect = NULL;

	m_mapRealX = 1.f;
	m_mapRealY = 1.f;
	m_mapGridX = 1;
	m_mapGridY = 1;
	m_mapScale = 1.f;
}

void BecherEditorMap::OnSelectItem(int item, void * data)
{
	HoeExtern::IEditor::Get()->GetEngine()->SetActiveScene(m_scene);
	HoeGetRef(HoeExtern::IEditor::Get()->GetEngine())->SetBackgroundColor(0xff008000);
	g_level = this;	
	HoeExtern::IEditor::Get()->ActivateEnginePage(this);

	if (item == m_levelitem)
        SelectRoot();
	else
	{
		LevelEditObject * leo = FindItemObject(item);
		assert(leo != NULL);
		if (leo) SelectObject(leo->object);
		UpdateSelection();
	}
}

int BecherEditorMap::Msg(HoePlugin::EPluginMsg msg, int par1, void * par2)
{
	switch (msg)
	{
	case HoePlugin::EPM_ReloadEngine:
		//m_level.CreateNew();
		break;
	case HoePlugin::EPM_SelectItem:
		GetApp()->Select(this);
		OnSelectItem(par1,par2);
		break;
	case HoePlugin::EPM_UnselectItem:
		GetApp()->Unselect();
		break;
	case HoePlugin::EPM_ChangeProperty:
		OnChangeProperty((HoeExtern::IPropertyItem*)par2);
		break;
	case HoePlugin::EPM_EngKeyDown:
		OnKeyDown(par1);
		break;
	case HoePlugin::EPM_EngMouseWheel:
		if (par1 & HoePlugin::MShift)
			GetView()->Zoom(reinterpret_cast<float*>(par2)[0] * 0.1f);
		else
            GetView()->Rotate(reinterpret_cast<float*>(par2)[0] * 0.001f);
		break;
	case HoePlugin::EPM_EngMouseEnter:
		soc.Show();
		break;
	case HoePlugin::EPM_EngMouseLeave:
		soc.Hide();
		break;
	case HoePlugin::EPM_EngMouseMove: {
		float * m = (float*)par2;
		if (m_mselect)
		{
			m_mselect->UnsetInfo(BecherObject::CanSelect);
			m_mselect = NULL;
		}
		if (soc.IsSelectMode())
		{
			float sx,sy;
			if (GetView()->GetPick(m[2],m[3],&sx,&sy))
				soc.SetPosition(sx,sy);
		}
		else
		{
			m_mselect = reinterpret_cast<BecherObject *>(GetView()->SelObject(m[2],m[3]));
			if (m_mselect)
				m_mselect->SetInfo(BecherObject::CanSelect);
		}
		}break;
	case HoePlugin::EPM_EngMouseUp: {
		float * m = (float*)par2;
		if (soc.IsSelectMode())
		{
            this->AddObject(soc.x,soc.y,soc.Build());
		}
		else if (par1 & HoePlugin::MRight)
		{
			BecherObject * o = reinterpret_cast<BecherObject *>(GetView()->SelObject(m[2],m[3]));
			if (o)
			{
				soc.Set(o);
				RemoveItemObject(o);
			}
		}
		else if (par1 & HoePlugin::MLeft)
		{
			BecherObject * o = reinterpret_cast<BecherObject *>(GetView()->SelObject(m[2],m[3]));
			if (o)
			{
				LevelEditObject * leo = FindItemObject(o);
				assert(leo);
				if (leo)
					HoeExtern::ITreeControl::Get()->SelectItem(leo->item);
			}

		}
		}break;
	case HoePlugin::EPM_CloseItem:

		break;
	};
	return 0;
}

void BecherEditorMap::SelectRoot()
{
	HoeExtern::IProperty * prop = HoeExtern::IProperty::Get();
	prop->Begin(this);
	prop->AppendCategory(__("Level Info"));
	prop->AppendString( PROPERTY_NAME,__("Name"), m_name.c_str(), (m_name.empty()) ? 0:PG_PROP_MODIFIED);
	prop->AppendString( PROPERTY_PATH,__("Path"), m_path.c_str(), PG_PROP_DISABLE);
	prop->AppendCategory(__("Terrain"));
	prop->AppendImageFile(PROPERTY_TERRAIN_HEIGHTMAP,__("HeightMap"),"");	
	prop->AppendSize(PROPERTY_TERRAIN_SIZE,__("Grid Size"),m_mapGridX,m_mapGridY,0);
	prop->AppendSize(PROPERTY_TERRAIN_REALSIZE,__("Real Size"),m_mapRealX,m_mapRealY,0);
	prop->AppendFloat(PROPERTY_TERRAIN_HEIGHTSCALE,__("Height scale"),m_mapScale,0);
	prop->AppendCategory(__("View"));
	prop->AppendAngle(PROPERTY_VIEW_ROT,__("Angle"),GetView()->GetAngle());
	prop->End();
}

void BecherEditorMap::OnChangeProperty(HoeExtern::IPropertyItem * item)
{
	if (item->GetID() < PROPERTY_OBJECT)
	{
		switch (item->GetID())
		{
		case PROPERTY_NAME:
			m_name = HoeCore::GetPropertyString(item);
			//
			HoeExtern::ITreeControl::Get()->SetItemText(this->m_levelitem, m_name.c_str());
			break;
		case PROPERTY_VIEW_ROT:
			GetView()->SetAngle(item->GetAngle());
			break;
		case PROPERTY_TERRAIN_HEIGHTMAP:
			LoadHeightMap(wxString(item->GetFilePath()));
			break;
		case PROPERTY_TERRAIN_SIZE:
			{
			item->GetSize(&this->m_mapGridX,&this->m_mapGridY);
			LoadHeightMap(m_heightpath);
			}break;
		case PROPERTY_TERRAIN_REALSIZE:
			{
				int x,y;
				item->GetSize(&x,&y);
				this->m_mapRealX = x;
				this->m_mapRealY = y;
				LoadHeightMap(m_heightpath);
			}break;
		case PROPERTY_TERRAIN_HEIGHTSCALE:
			this->m_mapScale = item->GetFloat();
			LoadHeightMap(m_heightpath);
			break;
		}
	}
	else
	{
		for (size_t i=0;i < m_selectobjs.size();i++)
		{
			OnChangeObjProperty(item,m_selectobjs[i]);
		}
	}
}

#define MOVE_STEP 1

void BecherEditorMap::OnKeyDown(int key)
{
	switch (key)
	{
	case 0x57: // w
		this->GetView()->Move(MOVE_STEP,0);
		break;
	case 0x41: // a
		this->GetView()->Move(0,-MOVE_STEP);
		break;
	case 0x53: // s
		this->GetView()->Move(-MOVE_STEP,0);
		break;
	case 0x44: // d
		this->GetView()->Move(0,MOVE_STEP);
		break; 
	case 0x2c:
		this->GetView()->Rotate(0.05f);
		break;
	case 0x2e:
		this->GetView()->Rotate(-0.05f);
		break;
	case 0x138:
		this->GetView()->SetDistance(this->GetView()->GetDistance() + 3);
		break;
	case 0x139:
		this->GetView()->SetDistance(this->GetView()->GetDistance() - 3);
		break;
	case 0x7f: // delete
		RemoveSelections();
		break;
	case 0x1b:
		soc.Cancel();
		break;
	default:
		HoeExtern::IEditor::Get()->Log("key: %x",key);
		break;
	};

}

bool BecherEditorMap::Save(HoeExtern::IProjectSave * ps)
{
	if (!m_name.empty())
	{
		ps->AddName("Name");
		ps->SetDataString(m_name.c_str());
	}
	if (!m_path.empty())
	{
		ps->AddName("Path");
		ps->SetDataPath(m_path.c_str());
	}
	/////////////////// save level
	HoeLevel::LevelFile f;
	if (!f.Open(m_path.c_str(),HoeUtils::File::mWrite))
		return false;
	f.WriteFileHeader();

	// zapsat data & teren
	// teren
	if (m_height.LoadFile(m_heightpath,wxBITMAP_TYPE_ANY))//,wxBITMAP_TYPE_BMP_FILE))
	{
		PixelData data(m_height);
		if (data)
		{
			int size = data.GetWidth()*data.GetHeight();
			f.WriteTerrainHeader(m_mapRealX/data.GetWidth(),m_mapRealY/data.GetHeight(),data.GetWidth(),data.GetHeight(), data.GetWidth()/m_mapGridX,data.GetHeight()/m_mapGridY,size*sizeof(float));
			float * ff = new float[size];
			PixelData::Iterator p(data);
			p.Reset(data);
			for ( int i = 0; i < size; i++ )
			{
				ff[i] = (p.Red() + p.Green() + p.Blue()) / 3 * this->m_mapScale;;
				if (ff[i] != 85)
				assert(ff[i] != 85);
			}
			f.Write(ff,sizeof(float)*size);
			delete [] ff;
		}
	}

	for (size_t i=0;i < this->m_objs.size();i++)
	{
		m_objs[i].object->Save(&f);
	}

	f.WriteEnd();
	return true;
}

bool BecherEditorMap::Load(HoeExtern::IProjectLoad * pl)
{
	do {
		if (strcmp(pl->GetName(),"Name") == 0)
		{
			m_name = pl->GetDataString();
		}
		else if (strcmp(pl->GetName(),"Path") == 0)
		{
			char buff[2048];
			pl->GetDataPath(buff,sizeof(buff));
			m_path = buff;
		}

	} while (pl->Next());

	return Open(m_name.c_str(), m_path.c_str());
}

bool BecherEditorMap::Open(const char * name, const char * path)
{
	m_name = name;
	m_path = path;
	HoeExtern::ITreeControl::Get()->SetItemText(this->m_levelitem, m_name.c_str());
	// load
	m_scene->LoadMap(m_path.c_str(),this);
	return true;
}

bool BecherEditorMap::CreateNew(const char * path)
{
	m_path = path;
	if (!HoeUtils::is_ext(path,".bl"))
		m_path += ".bl";
	m_name = HoeUtils::GetFileTitle(m_path);
	return true;
}

void BecherEditorMap::NewSelectObject(unsigned long type)
{
	BecherObject * o = CreateObject(type,m_scene);
	soc.Set(o);
	//o->Load();
	o->Show(true);
}

void BecherEditorMap::OnAddObject(HoeGame::Strategy::StgObject * obj)
{
	LevelEditObject leo;
	leo.object = reinterpret_cast<BecherObject*>(obj);
	if (m_levelitem != -1)
	{
		leo.item = HoeExtern::ITreeControl::Get()->AddItem(m_levelitem,GetTypeName(leo.object->GetType()),GetApp()->GetFileImageIndex(),this,obj);
	}
	else
        leo.item = -1;
	m_objs.push_back(leo);
}

void BecherEditorMap::AddToEditor(int item)
{
	m_levelitem = item;
	HoeExtern::IEditor::Get()->AddEnginePage(this);

	for (size_t i=0;i < m_objs.size();i++)
	{
		if (m_objs[i].item == -1)
			m_objs[i].item = HoeExtern::ITreeControl::Get()->AddItem(m_levelitem,GetTypeName(m_objs[i].object->GetType()),GetApp()->GetFileImageIndex(),this,m_objs[i].object);
	}
}

LevelEditObject * BecherEditorMap::FindItemObject(int item)
{
	for (size_t i=0;i < this->m_objs.size();i++)
	{
		if (m_objs[i].item == item)
			return &(m_objs[i]);
	}
	return NULL;
}

LevelEditObject * BecherEditorMap::FindItemObject(BecherObject * object)
{
	for (size_t i=0;i < this->m_objs.size();i++)
	{
		if (m_objs[i].object == object)
			return &(m_objs[i]);
	}
	return NULL;
}

void BecherEditorMap::OnSelectObject(EObjType type, BecherObject* obj)
{
	HoeExtern::IProperty * prop = HoeExtern::IProperty::Get();
	prop->Begin(this);
	prop->AppendCategory(__("Base"));
	// zakladni vlastnosti
	// pokud single tak dat souradnice
	if (obj)
	{
		float x,y;
		obj->GetCtrl()->GetPosition(&x, NULL, &y);
		prop->AppendPoint(PROPERTY_POSITION, __("Position"),x,y);
		prop->AppendAngle(PROPERTY_ROTATION, __("Orientation"),obj->GetAngle());
	}

	const char * list_tree_types[] = {"Nejvetsi", "mensi", "nejmensi", "stredni", "Warcraft",NULL};
	long tree_type_values[] = { 1,2,3,4,5};

	switch (type)
	{
	case EBO_Tree:
        prop->AppendCategory(__("Tree"));
		prop->AppendList(PROP_TREE_TYPE,__("Type"),list_tree_types,tree_type_values, reinterpret_cast<Tree*>(obj)->GetTypeModel());
		prop->AppendLong(PROP_TREE_WOOD,__("Wood"),(obj) ? reinterpret_cast<Tree*>(obj)->GetWood():0);
		prop->AppendFloat(PROP_TREE_HEIGHT,__("Height"),(obj) ? reinterpret_cast<Tree*>(obj)->GetHeight():0);
		break;
	};
	/*prop->AppendString( PROPERTY_NAME,__("Name"), m_name.c_str(), (m_name.empty()) ? 0:PG_PROP_MODIFIED);
	prop->AppendString( PROPERTY_PATH,__("Path"), m_path.c_str(), PG_PROP_DISABLE);
	prop->AppendCategory(__("Terrain"));
	prop->AppendSize(PROPERTY_TERRAIN_SIZE,__("Grid Size"),100,100,0,__("Velikost terrenu"));*/
	/*prop->End();
}

void BecherEditorMap::OnChangeObjProperty(HoeExtern::IPropertyItem * item, BecherObject * obj)
{
	float x,y;
	switch (item->GetID())
	{
	case PROPERTY_POSITION:
		item->GetPoint(&x,&y);
		obj->GetCtrl()->SetPosition(x, 0, y);
		break;
	case PROPERTY_ROTATION:
		obj->SetAngle(item->GetAngle());
		break;
	case PROP_TREE_WOOD:
		reinterpret_cast<Tree*>(obj)->SetWood(item->GetLong());
		break;
	case PROP_TREE_HEIGHT:
		reinterpret_cast<Tree*>(obj)->SetHeight(item->GetFloat());
		break;
	case PROP_TREE_TYPE:
		reinterpret_cast<Tree*>(obj)->SetTypeModel(item->GetLong());
		break;

	};
}

const char * BecherEditorMap::GetTypeName(EObjType type)
{
	switch (type)
	{
	case EBO_Tree:
		return __("Tree");
	case EBO_Sugar:
		return __("Sugar");
	case EBO_HerbeWoman:
		return __("HerbeWoman");
	case EBO_Farm:
		return __("Farm");
	case EBO_Bridge:
		return __("Bridge");
	case EBO_Destilate:
		return __("Destilate");
	case EBO_WaterHole:
		return __("WaterHole");
	case EBO_Store:
		return __("Store");
	case EBO_Shop:
		return __("Shop");
	case EBO_Factory:
		return __("Factory");
	default:
		return __("UFO");
	};
};

void BecherEditorMap::RemoveItemObject(BecherObject * object)
{
	std::vector<LevelEditObject>::iterator i = m_objs.begin();
	while (i != m_objs.end())
	//for (size_t i=0;i < this->m_objs.size();)
	{
		if ((*i).object == object)
		{
			HoeExtern::ITreeControl::Get()->RemoveItem((*i).item);
			m_objs.erase(i);
		}
		else
			i++;
	}
		
}

void BecherEditorMap::RemoveSelections()
{
	for (size_t i = 0;i < m_selectobjs.size();i++)
	{
		BecherObject * o = m_selectobjs[i];
		RemoveItemObject(o);
		delete o;
	}
	m_selectobjs.clear();
}

void BecherEditorMap::Close()
{
	HoeExtern::ITreeControl::Get()->RemoveItem(this->m_levelitem);
}

void BecherEditorMap::LoadHeightMap(wxString & filename)
{
	m_heightpath = filename;
	if (m_height.LoadFile(m_heightpath,wxBITMAP_TYPE_ANY))//,wxBITMAP_TYPE_BMP_FILE))
	{
		PixelData data(m_height);
		if (data)
		{
			// nahrani terenu do enginu
			IHoeMapElement * elm = this->m_scene->GetMapElement(HME_TERRAIN);
			int size = data.GetWidth()*data.GetHeight();
			float * f = new float[size];
			PixelData::Iterator p(data);
			p.Reset(data);
			for ( int i = 0; i < size; i++ )
			{
				f[i] = (p.Red() + p.Green() + p.Blue()) / 3 * this->m_mapScale;;
			}
			if (elm)
				reinterpret_cast<IHoeTerrain*>(elm)->LoadHeight(
				m_mapRealX/data.GetWidth(),m_mapRealY/data.GetHeight(),
				data.GetWidth(),data.GetHeight(),data.GetWidth()/m_mapGridX,data.GetHeight()/m_mapGridY,f);
			delete [] f;
		}
	}
}*/







