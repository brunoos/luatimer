# LuaTimer 
VERSION=2.0

CC=gcc
LD=$(ENV) gcc
CFLAGS=-Wall -O2 -c $(MYCFLAGS) -I$(LUAINC)
LDFLAGS=$(MYLDFLAGS)

OBJS=luatimer.o libtimer.o
LIB=luatimer.so

all: $(LIB)

$(LIB): $(OBJS)
	$(LD) -o $@ $(LDFLAGS) $(OBJS)

clean:
	rm -f *.o *.so

libtimer.o: libtimer.h libtimer.c
luatimer.o: libtimer.h luatimer.c
