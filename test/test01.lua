require("luatimer")
require("posix")

local poll = luatimer.createpoll()
print(poll)

poll:create(1)
poll:create(1)
poll:create(1)
poll:create(1)
poll:create(1)
poll:create(1)

print("poll size: ", poll:size())

print(poll:nextshot())

posix.sleep(3)

t = poll:fired("all")
print(#t)

print(t[3])
poll:cancel("all")
print(t[3])
posix.sleep(3)
t = poll:fired("all")
print(#t)
