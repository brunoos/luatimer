#include <windows.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#define DllExport __declspec(dllexport)

#define LT_EPOCH (116444736000000000LL)

static int lt_time(lua_State *L)
{
  FILETIME filetime;
  LARGE_INTEGER tmp;
  double now;

  /* FILETIME: Contains a 64-bit value representing the number of
     100-nanosecond intervals since January 1, 1601 (UTC). */
  GetSystemTimeAsFileTime(&filetime);
  tmp.LowPart  = filetime.dwLowDateTime;
  tmp.HighPart = filetime.dwHighDateTime;
	
  /* Convert to microseconds since Epoch, like gettimeofday */
  now = (double)((tmp.QuadPart - LT_EPOCH) / 10LL);
  /* Convert to seconds */
  now /= 1E6f;
  lua_pushnumber(L, now);

  return 1;
}

struct luaL_Reg funcs[] = {
  {"time", lt_time},
  {NULL, NULL}
};

DllExport int luaopen_luatimer_os(lua_State *L)
{
  luaL_register(L, "luatimer.os", funcs);
  return 1;
}
