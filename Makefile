# LuaTimer 
VERSION= 1.1

# Lua 5.1
LUA_DIR= $(HOME)/local/lua-5.1
LUA_INC= $(LUA_DIR)/include

INC= ./include
SRC= ./src
OBJ= ./obj
LIB= ./lib

# Linux compilation
CC= gcc
LD= gcc
DEFS= -DLINUX
CFLAGS= -Wall -O2 -fpic -fomit-frame-pointer -c $(DEFS) -I$(INC) -I$(LUA_INC)
LDFLAGS= -O -shared -fpic

# *BSD compilation
# Just comment the DEFS line in the Linux configuration above

# Mac OS X compilation
#CC= gcc
#LD= env MACOSX_DEPLOYMENT_TARGET="10.4" gcc
#CFLAGS= -Wall -O2 -fno-common -c -I$(INC) -I$(LUA_INC)
#LDFLAGS= -bundle -undefined dynamic_lookup


# The rest of the file should not change

SO= luatimer.so

# make all
all: $(LIB)/$(SO)

# shared libraries (for Linux)
$(LIB)/$(SO):  $(OBJ)/timerlib.o $(OBJ)/luatimer.o
	$(LD) $(LDFLAGS) -o $(LIB)/$(SO) $(OBJ)/timerlib.o $(OBJ)/luatimer.o

$(OBJ)/timerlib.o: $(SRC)/timerlib.c $(INC)/timer.h
	$(CC) $(CFLAGS) -o $@ $(SRC)/timerlib.c

$(OBJ)/luatimer.o: $(SRC)/luatimer.c $(INC)/luatimer.h
	$(CC) $(CFLAGS) -o $@ $(SRC)/luatimer.c

# version
version: 
	@echo "luaTimer-"$(VERSION) "    2007, May"

# clean
clean:
	rm -rf $(OBJ)/*.o  $(LIB)/*.so
