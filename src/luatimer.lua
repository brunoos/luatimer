--!!
--File: luatimer.lua
--Description: This file links a program with the dynamic C library 'luatimer'
--Date: 2003, November
--Version: 1.0
--Comments to: silvana@inf.puc-rio.br
--!!

-- Define the path of the dynamic C library 'luatimer'
-- !! You should modify it for your configuration -- !!
local libname = os.getenv("HOME").."/luaTimer/lib/libluatimer.so"

-- Define the function name in 'luatimer' to be returned as a C 
-- function 
local libopen = "timer_open"

-- Links a program with the C library 'luatimer'
local init, err1, err2 = loadlib (libname, libopen)
assert (init, (err1 or '')..(err2 or ''))
init()
