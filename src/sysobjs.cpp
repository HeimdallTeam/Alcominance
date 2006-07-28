
#include "StdAfx.h"
#include "becher.h"
#include "sysobjs.h"

SystemObjectSound::SystemObjectSound(IHoeScene * scn) : BecherSystemObject(scn) 
{
#ifdef BECHER_EDITOR
	SetModel(GetModelForSound());
#endif
}

#ifndef BECHER_EDITOR

#else

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

