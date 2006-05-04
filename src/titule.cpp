
#include "StdAfx.h"
#include "hoe3d.h"
#include "hoe_input_codes.h"
#include "becher.h"



void Becher::ToStateTitule()
{
	if (m_state == sRun)
		UnloadMap();

	m_state = sTitule;
}

int Becher::DrawTitule(iHoe2D * hoe2d)
{

	return 0;
}