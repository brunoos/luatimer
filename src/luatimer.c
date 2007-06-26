/*
Author: Silvana Rossetto, silvana at ic.uff.br
Maintainer: Bruno Silvestre, brunoos at inf.puc-rio.br
*/

#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "libtimer.h"


/* Metatables to control the timer types */
#define ACTIVE    "LuaTimer.activetimer"
#define CANCELLED "LuaTimer.cancelledtimer"
/* Metatable where the timers are registered */
#define TIMERS    "LuaTimer.timers"


/**
 * __tostring for active timers.
 */
static int lt_activetostring(lua_State *L)
{
  TimerCell *timer = (TimerCell*) lua_touserdata(L, 1);
  lua_pushfstring(L, "Active timer: %p", timer);
  return 1;
}

/**
 * __tostring for removed timers.
 */
static int lt_cancelledtostring(lua_State *L)
{
  TimerCell *timer = (TimerCell*) lua_touserdata(L, 1);
  lua_pushfstring(L, "Cancelled timer: %p", timer);
  return 1;
}


/**
 * Create a new timer.
 *
 */
static int lt_create(lua_State* L) {
  double frequency;
  TimerCell *timer;

  frequency = luaL_checknumber(L, 1);
  timer = (TimerCell*) lua_newuserdata(L, sizeof(TimerCell));
  if (!timer) {
    lua_pushnil(L);
    lua_pushstring(L, "out of memory");
    return 2;
  }
  
  /* Register the timer to avoid the GC */
  luaL_getmetatable(L, TIMERS);
  lua_pushlightuserdata(L, (void*)timer);
  lua_pushvalue(L, -3);
  lua_rawset(L, -3);
  lua_pop(L, 1);
  /* Set the metatable */
  luaL_getmetatable(L, ACTIVE);
  lua_setmetatable(L, -2);
  /* Add the timer into the list */
  libt_add(timer, frequency);

  return 1;
}

/**
 * Cancel all timers.
 *
 */
static int lt_cancelall(lua_State *L)
{
  TimerCell *timer;
  const char *str = lua_tostring(L, 1);
  if (strcmp(str, "all")) {
    lua_pushstring(L, "bad argument #1: not a timer or 'all'");
    lua_error(L);
    return 0;
  }

  luaL_getmetatable(L, TIMERS);
  lua_pushnil(L);
  while (lua_next(L, -2)) {
    timer = (TimerCell*) lua_touserdata(L, -1);
    /* Set the new metatable */
    luaL_getmetatable(L, CANCELLED);
    lua_setmetatable(L, -2);
    /* Remove the timer from the register */
    lua_pop(L, 1);
    lua_pushnil(L);
    lua_rawset(L, -3);
    /* Remove from the list */
    libt_remove(timer);
    /* next */
    lua_pushnil(L);
  }
  lua_pushboolean(L, 1);
  return 1;
}

/**
 * Cancel a specific timer.
 *
 */
static int lt_cancelone(lua_State *L)
{
  TimerCell *timer = (TimerCell*) lua_touserdata(L, 1);
  /* Remove the timer from the register */
  luaL_getmetatable(L, TIMERS);
  lua_pushlightuserdata(L, (void*)timer);
  lua_pushnil(L);
  lua_rawset(L, -3);
  /* Remove the metatable from the timer */
  luaL_getmetatable(L, CANCELLED);
  lua_setmetatable(L, 1);
  /* Remove the timer from the list */
  lua_pushboolean(L, libt_remove(timer));
  return 1;
}

/**
 * Cancel a specific timer or all timers.
 *
 */
static int lt_cancel(lua_State* L) {
  /* Cancel all timers ... */
  if (lua_isstring(L, 1))
    return lt_cancelall(L);

  /* ... or cancel a specific timer */
  luaL_checkudata(L, 1, ACTIVE);
  return lt_cancelone(L);
}

/**
 * Get the some timer that expired or 'nil' if there is no expired timer.
 *
 */
static int lt_firedone(lua_State* L) {
  TimerCell *timer;
  Iterator *iter;

  iter = libt_newiterator();
  timer = libt_nextfired(iter);
  if (timer) {
    luaL_getmetatable(L, TIMERS);
    lua_pushlightuserdata(L, (void*)timer);
    lua_rawget(L, -2);
  }
  else {
    lua_pushnil(L);
  }
  libt_freeiterator(iter);
  return 1;
}

/**
 * Return an array of expired timers.
 *
 */
static int lt_firedall(lua_State *L) 
{
  double pos = 1;
  TimerCell *timer;
  Iterator *iter;

  /* Register of timers */
  luaL_getmetatable(L, TIMERS);
  /* Create an array */
  lua_newtable(L);

  iter = libt_newiterator();  
  timer = libt_nextfired(iter);
  while (timer) {
    lua_pushlightuserdata(L, (void*)timer);
    lua_rawget(L, -3);
    lua_rawseti(L, -2, pos++);
    timer = libt_nextfired(iter);
  }
  libt_freeiterator(iter);
  return 1;
}

/**
 * Return the expired timer(s).
 *
 */
static int lt_fired(lua_State *L)
{
  const char *type = luaL_checkstring(L, 1);
  if (!strcmp(type, "one"))
    return lt_firedone(L);
  if (!strcmp(type, "all"))
    return lt_firedall(L);
  /* Erro: not a valid request */
  lua_pushstring(L, "bad argument #2: invalid request");
  lua_error(L);
  return 0;
}

/**
 * Return the seconds for the next expiration of the earliest timer.
 *
 */
static int lt_nextshot(lua_State *L) 
{
  lua_pushnumber(L, libt_nextshot());
  return 1;
}

/**
 * Return the time in seconds since the Epoch (01/01/1970).
 *
 */
static int lt_now(lua_State* L)
{
  lua_pushnumber(L, libt_now());
  return 1;
}

/**
 * Functions for the module.
 */
static luaL_Reg funcs[] = {
  {"now",      lt_now},
  {"create",   lt_create},
  {"fired",    lt_fired},
  {"cancel",   lt_cancel},
  {"nextshot", lt_nextshot},
  {NULL,       NULL}
};

/* Create the timer table and register its functions */
int luaopen_luatimer(lua_State *L)
{
  luaL_newmetatable(L, TIMERS);

  luaL_newmetatable(L, ACTIVE);
  lua_pushstring(L, "__tostring");
  lua_pushcfunction(L, lt_activetostring);
  lua_rawset(L, -3);

  luaL_newmetatable(L, CANCELLED);
  lua_pushstring(L, "__tostring");
  lua_pushcfunction(L, lt_cancelledtostring);
  lua_rawset(L, -3);

  luaL_register(L, "luatimer", funcs);
  return 1;
}
