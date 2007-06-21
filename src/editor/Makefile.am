
# these are the headers for your project
# noinst_HEADERS = wokno.h

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = ../../BecherEditor

# the application source, library search path, and link libraries
______BecherEditor_SOURCES = \ 
editor.cpp \
editor_map.cpp \
editor_tools.cpp \
help.cpp \
map_properties.cpp \
../buildings.cpp\
../console.cpp\
../crr.cpp\
../elements.cpp\
../idlist.cpp\
../jobs.cpp\
../map.cpp\
../minimap.cpp\
../obj_coalmine.cpp\
../obj_construct.cpp\
../obj_destilate.cpp\
../obj_factory.cpp\
../obj_farm.cpp\
../obj_herbe.cpp\
../obj_saw.cpp\
../obj_shop.cpp\
../obj_stonemine.cpp\
../obj_store.cpp\
../obj_sugar.cpp\
../obj_waterhole.cpp\
../object.cpp\
../sysobjs.cpp\
../terrain.cpp\
../troll.cpp\
../utils.cpp\
../workspace.cpp


______BecherEditor_CPPFLAGS = -DBECHER_EDITOR -I/home/milan/heimdall/Hoe/HoeEditor/include $(WX_CPPFLAGS) $(WX_CXXFLAGS_ONLY) $(WX_CFLAGS_ONLY)

______BecherEditor_LDFLAGS = -ll $(HOE_LIBS) -lHoeEditor -lHoeUtils -lHoeCore $(LUA_LIBS) -ldl -lbassmod -lX11 -lfl $(WX_LIB) -lwx_gtk2_propgrid-2.8

