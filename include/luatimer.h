/*--
File: luatimer.h
Description: This file defines the open function for the 'luatimer' package
Date: 2003, November
Version: 1.0
Comments to: silvana@inf.puc-rio.br
--*/

#ifndef LUATIMER_H
#define LUATIMER_H

#include <lua.h>
#include <lauxlib.h>

#include "timer.h"

int timer_open(lua_State *L);
#endif

