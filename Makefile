# LuaTimer 
VERSION = 1.0

# Lua 5.0
LUA_DIR = $(HOME)/lua-5.0
LUA_LIB = $(LUA_DIR)/lib
LUA_INC = $(LUA_DIR)/include

INSTALL_LUATIMER=$(LUA_DIR)/luaTimer

INC = ./include
SRC = ./src
OBJ = ./obj
LIB = ./lib


# the rest of the file should not change
#CC = gcc -Wall -g
CC = /usr/bin/gcc -Wall -g
CFLAGS = -c -I$(INC) -I$(LUA_INC)
LDFLAGS = -L$(LIB) -L$(LUA_LIB) 
LDLIBS  = -llua -llualib -lm
RM = rm
SO = libluatimer.$(VERSION).so

# make all
all: $(OBJ)/timerlib.o $(OBJ)/luatimer.o so 

$(OBJ)/timerlib.o: $(SRC)/timerlib.c $(INC)/timer.h
	$(CC) $(CFLAGS) -o $@ $(SRC)/timerlib.c

$(OBJ)/luatimer.o: $(SRC)/luatimer.c $(INC)/luatimer.h
	$(CC) $(CFLAGS) -o $@ $(SRC)/luatimer.c

# shared libraries (for Linux)
so:
	$(CC) -o $(LIB)/$(SO) -shared $(OBJ)/timerlib.o $(OBJ)/luatimer.o
	cd $(LIB); ln -f -s $(SO) libluatimer.so

# install in INSTALL_LUATIMER
install:
	mkdir -p $(INSTALL_LUATIMER)
	cp $(LIB)/$(SO) $(INSTALL_LUATIMER)
	ln -f -s $(INSTALL_LUATIMER)/$(SO) $(INSTALL_LUATIMER)/libluatimer.so
	cp $(SRC)/luatimer.lua $(INSTALL_LUATIMER)
	cp $(INC)/luatimer.h $(INSTALL_LUATIMER)

# version
version: 
	@echo "luaTimer-"$(VERSION) "    2003,November"

# clean
clean:
	$(RM) -rf $(OBJ)/*.o  $(LIB)/*.so $(INSTALL_LUATIMER)
