
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
#include <hoe_types.h>
#include <hoe_math.h>
#include <hoe_structures.h>

#include <hoe3d.h>
#include "../../Hoe/HoeGame/include/hoe_utils.h"
#include "../../Hoe/HoeGame/include/hoe_console.h"
#include "../../Hoe/HoeGame/include/hoe_engine.h"
#include "../../Hoe/HoeGame/include/hoe_app.h"
#include "../../Hoe/HoeGame/include/hoe_lang.h"
#include "../../Hoe/HoeGame/include/hoe_resource_mgr.h"
#include "../../Hoe/HoeGame/include/hoe_view.h"
#include "../../Hoe/HoeGame/include/lua_script.h"
#include "../../Hoe/HoeGame/include/hoe_scene.h"
#include "../../Hoe/HoeGame/include/hoe_object.h"
#include "../../Hoe/HoeGame/include/hoe_strategy.h"
#include "../../Hoe/HoeGame/include/hoe_stratmap.h"
#include "../../Hoe/HoeGame/include/hoe_input.h"
#include "../../Hoe/HoeGame/include/hoe_music.h"
#include "../../Hoe/HoeGame/include/lua_script.h"
#include "../../Hoe/HoeGame/include/hoe_timer.h"
#include "../../Hoe/HoeGame/include/hoe_vars.h"


typedef HoeGame::CVar CVar;

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
#include <wx/filesys.h>
#include <wx/fs_zip.h>
#include <wx/html/helpctrl.h>
#include <wx/spinctrl.h>

#include <he/app.h>
#include <he/editor.h>
#include <he/engview.h>
#include <he/panels.h>
#include <he/properties.h>
#include <he/resources.h>
#include <he/utils.h>
#include <he/undolist.h>

#endif

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#endif // _STDAFX_H_
