require("luatimer")
require("socket")

local timers = luatimer.createpoll()

t1 = timers:create(1)
print(t1)

print(timers:fired("one"))
socket.sleep(2)
t2 = timers:fired("one")
print(t2)

timers:cancel(t2)
print(t2)
