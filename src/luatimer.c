/*--
File: luatimer.c
Description: This file defines the interface between C time 
   functions and Lua package 'luatimer'  
Date: 2003, November
Author: Silvana Rossetto, silvana at ic.uff.br
Maintainer: Bruno Silvestre, brunoos at inf.puc-rio.br
--*/

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "luatimer.h"

/* Insert a new timer */
static int _insertTimer(lua_State* L) {
   TimerCell *obj;
   
   double  frequency = lua_tonumber(L, 1);
   obj = (TimerCell *) insertTimer(frequency);
   lua_pushlightuserdata(L, (void*)obj);
   return 1;
}

/* Remove a timer */
static int _removeTimer(lua_State* L) {
   TimerCell* obj;
   int ret;

   obj = (TimerCell *) lua_touserdata(L, 1);
   ret = removeTimer((void*)obj);
   if (ret)
      lua_pushnumber(L, ret);
   else
      lua_pushnil(L);
   return 1;
}

/* Check the timers: returns the first expired timer found in 'tlist' 
 * or nil if was not found expired timer */
static int _timeout(lua_State* L) {
   TimerCell *obj;
   
   obj = (TimerCell *) luatimer_timeout();
   if (obj != NULL) {
      lua_pushlightuserdata(L, (void*)obj);
   } else { 
      lua_pushnil(L);
   }
   return 1;
}

/* Check the timers: returns all expired timers in a table */
static int _timeoutAll(lua_State* L) {
   TimerCell *obj;
   int pos = 1;
   
   // create the table to return
   lua_newtable (L);
   
   obj = (TimerCell *) luatimer_timeout();
   while (obj != NULL) {
      lua_pushlightuserdata(L, (void*)obj);
      lua_rawseti(L, -2, pos);
      pos++;
      obj = (TimerCell *) luatimer_timeout();
   }
   if (pos==1) lua_pushnil(L);
   return 1;
}

/* Return the next expiry time */
static int _getexpiry(lua_State* L) {
   double exp = get_expiry();
   lua_pushnumber(L, exp);
   return 1;
}

/* Return the now time */
static int _gettime(lua_State* L)
{
   double now = get_now();
   lua_pushnumber(L, now);
   return 1;
}

/* Functions in library namespace */
static luaL_reg functions[] = {
   {"getexpiry", _getexpiry},
   {"timeout", _timeout},
   {"timeoutAll", _timeoutAll},
   {"gettime", _gettime},
   {"insertTimer", _insertTimer},
   {"removeTimer", _removeTimer},
   {NULL, NULL}
};

/* Create the timer table and register its functions */
int luaopen_luatimer(lua_State *L)
{
   lua_newtable (L);
   lua_setglobal (L, "luatimer");
   luaL_openlib(L, "luatimer", functions, 0);
   return 0;
}

