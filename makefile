OUTPUT_DIR = .
TMP_DIR = ./tmp

OUTPUT_BINARY = $(OUTPUT_DIR)/Becher

CC=gcc
GCC=g++
C_FLAGS= -D_LINUX -ggdb3 -I../Hoe/Hoe3D/include -I../Hoe/HoeGame/include
LINK=g++
L_FLAGS= -L/usr/X11R6/lib -L../Hoe/bin
LINK_LIBRARIES = -ll -lHoeGame -llua -ldl -lbassmod -lX11 -lfl

OBJ_FILES = $(TMP_DIR)/bechapp.o $(TMP_DIR)/buildings.o $(TMP_DIR)/main.o \
$(TMP_DIR)/becher.o $(TMP_DIR)/game.o \
$(TMP_DIR)/level.o $(TMP_DIR)/console.o $(TMP_DIR)/object.o $(TMP_DIR)/elements.o \
$(TMP_DIR)/troll.o $(TMP_DIR)/jobs.o $(TMP_DIR)/panels.o $(TMP_DIR)/idlist.o \
$(TMP_DIR)/map.o $(TMP_DIR)/config.o $(TMP_DIR)/obj_sugar.o $(TMP_DIR)/obj_farm.o \
 $(TMP_DIR)/obj_store.o  $(TMP_DIR)/obj_destilate.o $(TMP_DIR)/obj_shop.o \
$(TMP_DIR)/obj_factory.o



all:	BUILD editor

$(TMP_DIR):
	mkdir -p $(TMP_DIR)

$(OUTPUT_DIR):
	mkdir -p $(OUTPUT_DIR)

BUILD:		$(TMP_DIR) $(OUTPUT_DIR) $(OBJ_FILES) 
	$(LINK) $(L_FLAGS) -o $(OUTPUT_BINARY) $(OBJ_FILES) $(LINK_LIBRARIES)
editor:
	make -f makeplugin

clean:
	rm -f $(OBJ_FILES) $(OUTPUT_BINARY)

#
# Generic rules to build object files
#

$(TMP_DIR)/%.o:		src/%.cpp
	$(GCC) $(C_FLAGS) -c $< -o $@

$(TMP_DIR)/%.o:		src/%.c
	$(CC) $(C_FLAGS) -c $< -o $@



