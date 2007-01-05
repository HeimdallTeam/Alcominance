
# these are the headers for your project
# noinst_HEADERS = wokno.h

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = ../../alcosim

# the application source, library search path, and link libraries
______alcosim_SOURCES = \ 
../bechapp.cpp \
../buildings.cpp \
../config.cpp \
../console.cpp \
../crr.cpp \
../dialogs.cpp \
../elements.cpp \
../game.cpp \
../idlist.cpp \
../jobs.cpp \
../level.cpp \
main.cpp \
../map.cpp \
../minimap.cpp \
../ministry.cpp \
../obj_coalmine.cpp \
../obj_construct.cpp \
../obj_destilate.cpp \
../obj_factory.cpp \
../obj_farm.cpp \
../obj_herbe.cpp \
../obj_saw.cpp \
../obj_shop.cpp \
../obj_stonemine.cpp \
../obj_store.cpp \
../obj_sugar.cpp \
../obj_waterhole.cpp \
../object.cpp \
../panels.cpp \
../path_engine.cpp \
../phys_prov.cpp \
../screens.cpp \
../sysobjs.cpp \
../terrain.cpp \
../troll.cpp \
../utils.cpp \
../workspace.cpp

______alcosim_CPPFLAGS = -DD_SIM
______alcosim_LDADD = -ll -lHoeGame $(LUA_LIBS) -ldl -lbassmod -lX11 -lfl

