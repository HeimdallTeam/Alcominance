
#ifndef _STDAFX_H_
#define _STDAFX_H_

//#define _CRT_SECURE_NO_DEPRECATE
#ifdef _WIN32
//#define _WIN32_WINNT 0x0500
#include <windows.h>
#define snprintf _snprintf
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include <new>
#include <wchar.h>

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
#ifdef BECHER_EDITOR
// wxwindows
#include <wx/wxprec.h>
#include <wx/bitmap.h>
#include <wx/choicebk.h>
#include <wx/propdlg.h>
#include <wx/spinctrl.h>
#include <wx/listctrl.h>
#include <wx/statline.h>
#include <wx/clipbrd.h>
#include <wx/rawbmp.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/cshelp.h>
#include <wx/dynlib.h>
#include <wx/menu.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/manager.h>
#include <wx/bmpbuttn.h>
#include <wx/file.h>
#include <wx/config.h>
#include <wx/splash.h>
#include <wx/splitter.h>
#include <wx/filesys.h>
#include <wx/fs_zip.h>
#include <wx/html/helpctrl.h>
#include <wx/spinctrl.h>
#include <wx/msgdlg.h>
#include <wx/bmpbuttn.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statusbr.h>
#include <wx/filedlg.h>
#include <wx/log.h>
//TODO #include <wx/aui/aui.h>
#endif

extern "C" {
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include <HoeCore/hoe_types.h>
#include <HoeCore/hoe_stream.h>
#include <HoeCore/hoe_file.h>
#include <HoeCore/hoe_math.h>
#include <HoeCore/hoe_structures.h>
#include <HoeCore/hoe_core.h>

#include <hoe3d.h>

//
#include <HoeGame/hoe_console.h>
#include <HoeGame/hoe_engine.h>
#include <HoeGame/hoe_app.h>
#include <HoeGame/hoe_lang.h>
#include <HoeGame/hoe_resource_mgr.h>
#include <HoeGame/hoe_view.h>
#include <HoeGame/lua_script.h>
#include <HoeGame/hoe_scene.h>
#include <HoeGame/hoe_object.h>
#include <HoeGame/hoe_strategy.h>
#include <HoeGame/hoe_stratmap.h>
#include <HoeGame/hoe_input.h>
#include <HoeGame/hoe_music.h>
#include <HoeGame/lua_script.h>
#include <HoeGame/hoe_timer.h>
#include <HoeGame/hoe_vars.h>
#include <HoeGame/hoe_ai.h>
#include <HoeGame/hoe_land.h>
#include <HoeGame/hoe_mem.h>

typedef HoeGame::CVar CVar;

#ifdef BECHER_EDITOR

#include <HoeEditor/app.h>
#include <HoeEditor/editor.h>
#include <HoeEditor/engview.h>
#include <HoeEditor/panels.h>
#include <HoeEditor/properties.h>
#include <HoeEditor/resources.h>
#include <HoeEditor/utils.h>
#include <HoeEditor/undolist.h>

#endif


#endif // _STDAFX_H_
