
#ifndef _STDAFX_H_
#define _STDAFX_H_

//#define _CRT_SECURE_NO_DEPRECATE
#ifdef _WIN32
//#define _WIN32_WINNT 0x0500
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>


#ifdef _LINUX
#include <X11/Xlib.h>
#include <X11/Xutil.h>
//#include <X11/extensions/xf86vmode.h>
#include <X11/keysym.h>
#include <dlfcn.h>
#include <stdarg.h>
#include <string.h>
#endif

#include <assert.h>

#include <hoe3d.h>
#include "../../HoeGame/hoe_utils.h"
#include "../../HoeGame/hoe_console.h"
#include "../../HoeGame/hoe_engine.h"
#include "../../HoeGame/hoe_app.h"
#include "../../HoeGame/hoe_lang.h"
#include "../../HoeGame/hoe_resource_mgr.h"
#include "../../HoeGame/hoe_view.h"
#include "../../HoeGame/lua_script.h"
#include "../../HoeGame/hoe_scene.h"
#include "../../HoeGame/hoe_object.h"
#include "../../HoeGame/hoe_strategy.h"
#include "../../HoeGame/hoe_stratmap.h"
#include "../../HoeGame/hoe_input.h"
#include "../../HoeGame/hoe_music.h"
#include "../../HoeGame/lua_script.h"
#include "../../HoeGame/hoe_timer.h"
#include "../../HoeGame/hoe_vars.h"

typedef HoeGame::CVar CVar;

#include <vector>
#include <list>

#ifdef BECHER_EDITOR
// wxwindows
#include <wx/wxprec.h>
#include <wx/choicebk.h>
#include <wx/propdlg.h>
#include <wx/spinctrl.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/clipbrd.h>
#include <wx/rawbmp.h>
#include <wx/cshelp.h>
#include <wx/dynlib.h>
#include <wx/propgrid/propgrid.h>
#include <wx/bmpbuttn.h>
#include <wx/file.h>
#include <wx/config.h>
#include <wx/splash.h>
#include <wx/splitter.h>

#include <he/app.h>
#include <he/editor.h>
#include <he/engview.h>
#include <he/panels.h>
#include <he/properties.h>
#include <he/resources.h>
#include <he/utils.h>

#endif

#endif // _STDAFX_H_
