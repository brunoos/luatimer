-- This is a simple test program for luaTimer. 
-- It inserts two timer and defines a string to be executed when 
-- the time event occurr.

-- load luatimer library
require ("luatimer")

-- get time now
io.write("time start: "..luatimer.gettime().."\n")

-- register table for timer events  
tab_timer = {}

-- insert timer with frequency 4
tmp_1 = luatimer.insertTimer(4)
-- register a code for this timer   
tab_timer[tmp_1] = "print('Frequency: 4')"

-- insert timer with frequency 3.5
tmp_2 = luatimer.insertTimer(3.5)
-- register a code for this timer   
tab_timer[tmp_2] = "print('Frequency: 3.5')"

-- loop to check timers
i = 1
while i < 10 do
   tmp = luatimer.timeout()  
   if tmp then
      if(tab_timer[tmp]) then  
         -- execute the string defined for timer
         assert(loadstring(tab_timer[tmp]))() 
         i = i + 1
      end
      print("Next expiry: "..luatimer.getexpiry())
   end
end

-- remove timer
luatimer.removeTimer(tmp_1)
luatimer.removeTimer(tmp_2)
io.write("time end: "..luatimer.gettime().."\n")
