require("luatimer.os")

module("luatimer", package.seeall)

-- Max double
local dblmax = 1.79e308

--------
--- Metatables.
--
local activepoll  -- define below

local activetimer = { 
   __tostring = function(t)
      return "Timer (frequency = " .. t.frequency .. ")"
   end
}

--------
--- Find the next expiration time.
--
local function findmin(timers)
   local min = dblmax
   for t in pairs(timers) do
      if t.expiration < min then
         min = t.expiration
      end
   end
   return min
end


--------
--- Select one fired timer.
--
local function firedone(poll)
   local resp
   local now = luatimer.os.time()
   for t in pairs(poll._timers) do
      if t.expiration < now then
         resp = t
         t.expiration = t.frequency + now
         break
      end
   end
   if resp then
      poll._mintimer = findmin(poll._timers)
   end
   return resp
end

--------
--- Return all fired timers.
--
local function firedall(poll)
   local resp = {}
   local now = luatimer.os.time()
   for t in pairs(poll._timers) do
      if t.expiration <= now then
         table.insert(resp, t)
         t.expiration = t.frequency + now
      end
   end
   if #resp > 0 then
      poll._mintimer = findmin(poll._timers)
   end
   return resp
end

--------
--- Return fired timers.
--
local function fired(poll, req)
   if type(poll) ~= "table" or getmetatable(poll) ~= activepoll then
      error("invalid timer poll", 2)
   end
   if req == "one" then
      return firedone(poll)
   elseif req == "all" then
      return firedall(poll)
   end
   error("invalid request", 2)
end

--------
--- Return the seconds for the next timer fires.
--
local function nextshot(poll)
   if type(poll) ~= "table" or getmetatable(poll) ~= activepoll then
      error("invalid timer poll", 2)
   end
   local tmp = poll._mintimer - luatimer.os.time()
   if tmp > 0 then
      return tmp
   end
   return 0
end

--------
--- Cancel the timer from the poll.
--
local function cancel(poll, t) 
   if type(poll) ~= "table" or getmetatable(poll) ~= activepoll then
      error("invalid timer poll", 2)
   end
   if t == "all" then
      poll._size = 0
      poll._timers = {}
      poll._mintimer = dblmax
   elseif poll._timers[t] then
      poll._timers[t] = nil
      poll._size = poll._size - 1
      poll._mintimer = findmin(poll._timers)
   else 
      error("invalid timer", 2)
   end
end

--------
--- Create a new poll of timers.
--
local function create(poll, f)
   if type(poll) ~= "table" or getmetatable(poll) ~= activepoll then
      error("invalid timer poll", 2)
   end
   local now = luatimer.os.time()
   local timer = { 
      frequency = f,
      expiration = f+now,
   }
   setmetatable(timer, activetimer)
   poll._timers[timer] = true
   poll._size = poll._size + 1
   if timer.expiration < poll._mintimer then
      poll._mintimer = timer.expiration
   end
   return timer
end

--------
--- The number of timers into the poll.
--
local function size(poll)
   return poll._size
end

--------
--- Metatable for the timers.
--
activepoll = {
   __tostring = function()
      return "Timer poll"
   end,
   __index = {
      size = size,
      fired = fired,
      create = create,
      cancel = cancel,
      nextshot = nextshot,
   }
}

--------
--- Create a new timer poll.
--
function createpoll()
   local poll = {
      _size = 0,
      _timers = {},
      _mintimer = dblmax,
   }
   setmetatable(poll, activepoll)
   return poll
end
