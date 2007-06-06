/*--
File: luatimer.h
Description: This file defines the open function for the 'luatimer' package
Date: 2003, November
Author: Silvana Rossetto, silvana at ic.uff.br
Maintainer: Bruno Silvestre, brunoos at inf.puc-rio.br
--*/

#ifndef LUATIMER_H
#define LUATIMER_H

#include <lua.h>
#include <lauxlib.h>

#include "timer.h"

int luaopen_luatimer(lua_State *L);
#endif

