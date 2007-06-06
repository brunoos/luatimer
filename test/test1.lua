-- This is a simple test program for luaTimer. It inserts two timer and defines a string to be executed when the time event occurr

-- load luatimer library
require ("../src/luatimer")

-- get time now
io.write("time start: "..luatimer.gettime().."\n")

-- register table for timer events  
tab_timer = {}

-- insert timer with frequency 0.7
tmp_1 = luatimer.insertTimer(0.7)
-- register a code for this timer   
tab_timer[tmp_1] = "print(i..': Frequency: 0.7')"

-- insert timer with frequency 3
tmp_2 = luatimer.insertTimer(3)
-- register a code for this timer   
tab_timer[tmp_2] = "print(i..': Frequency: 3')"

-- loop to check timers
i = 1
while i < 50 do
   tmp = luatimer.timeoutAll() -- get all timers has expired  
   if tmp then
      for j, v in pairs(tmp) do
         if (tab_timer[v]) then  
            -- execute the string defined for timer
            assert(loadstring(tab_timer[v]))() 
            i = i + 1
         end
      end
      print("\nNext expiry: "..luatimer.getexpiry())
   end
end

-- remove timer
for j, v in pairs(tab_timer) do
   luatimer.removeTimer(j)
end
io.write("time end: "..luatimer.gettime().."\n")
