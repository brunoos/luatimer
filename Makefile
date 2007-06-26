# Lua 5.1 headers
LUAINC=/home/brunoos/local/lua-5.1/include

# linux bsd macosx
PLATFORM=linux

# For Mac OS X
MACOSX_VERSION=10.4


#----------------------
# Do not edit this part

BSD_CFLAGS=-fomit-frame-pointer
BSD_LDFLAGS=-O -shared -fpic

LNX_CFLAGS=-fomit-frame-pointer
LNX_LDFLAGS=-O -shared -fpic

MAC_ENV=env MACOSX_DEPLOYMENT_TARGET=$(MACOSX_VERSION)
MAC_CFLAGS=-fno-common
MAC_LDFLAGS=-bundle -undefined dynamic_lookup

all: $(PLATFORM)

linux:
	@echo "Build for Linux"
	@echo "---------------"
	@cd src ; make LUAINC=$(LUAINC) MYCFLAGS="$(LNX_CFLAGS)" MYLDFLAGS="$(LNX_LDFLAGS)"

bsd:
	@echo "Build for *BSD"
	@echo "--------------"
	@cd src ; make LUAINC=$(LUAINC) MYCFLAGS="$(BSD_CFLAGS)" MYLDFLAGS="$(BSD_LDFLAGS)"

macosx:
	@echo "Build for Mac OS X $(MACOSX_VERSION)"
	@echo "-------------------------"
	@cd src ; make LUAINC=$(LUAINC) ENV="$(MAC_ENV)" MYCFLAGS="$(MAC_CFLAGS)" MYLDFLAGS="$(MAC_LDFLAGS)"

none:
	@echo "Usage: make {linux | bsd | macosx}"

clean:
	@cd src ; make clean

#----------------------
