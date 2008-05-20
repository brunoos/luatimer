require("luatimer")
require("socket")

timers = luatimer.createpoll()

t3 = timers:create(3)
t5 = timers:create(5)
t6 = timers:create(6)

print(timers:nextshot())
-- sleep(4 segundos)
socket.sleep(4)

t = timers:fired("one")
print(t)
print(t == t3)

timers:cancel(t3)

print(timers:nextshot())
-- sleep(3 segundos)
socket.sleep(3)

tb = timers:fired("all")
for k, t in ipairs(tb) do
   print(t)
end
timers:cancel("all")
