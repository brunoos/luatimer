#include <sys/time.h>
#include <time.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>


/**
 * Return the time in seconds since Epoch.
 *
 */
static int lt_time(lua_State *L)
{
  struct timeval tvnow;
  if (!gettimeofday(&tvnow, NULL)) {
    double tmp = (double)tvnow.tv_sec + ((double)tvnow.tv_usec/1e6f);
    lua_pushnumber(L, tmp);
  }
  else
    lua_pushnil(L);
  return 1;
}

static luaL_Reg funcs[] = {
  {"time", lt_time},
  {NULL,   NULL}
};

int luaopen_luatimer_os(lua_State *L)
{
  luaL_register(L, "luatimer.os", funcs);
  return 1;
}
