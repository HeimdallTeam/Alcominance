
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
map_properties.cpp

______BecherEditor_CPPFLAGS = -DBECHER_EDITOR -I/home/milan/heimdall/Hoe/HoeEditor/include 

______BecherEditor_LDFLAGS = -ll -lHoeGame -lHoeCore $(LUA_LIBS) -ldl -lbassmod -lX11 -lfl $(WX_LIB)

