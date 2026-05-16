--- Window Height
WindowHeight = 0

--- Window Width
WindowWidth = 0

--- Keys Table
Keys = {}

--- Mouse Table
Mouse = {}

--- Mouse X Position
Mouse.X = 0

--- Mouse Y Position
Mouse.Y = 0


--- Entities Table
Entities = {}


--- @class Position
Position = {}

--- Position Constructor
--- @param X number X Coordinate
--- @param Y number Y Coordinate
--- @return Position
function Position.new(X, Y) end

--- Position X
Position.X = 0

--- Position Y
Position.Y = 0


--- @class Size
Size = {}

--- Size Constructor
--- @param X number Width
--- @param Y number Height
--- @return Size
function Size.new(X, Y) end

--- Size Width
Size.X = 0

--- Size Height
Size.Y = 0


--- @class Color
Color = {}

--- Color Constructor
--- @param R number Red
--- @param G number Green
--- @param B number Blue
--- @param A number Alpha
--- @return Color
function Color.new(R, G, B, A) return Color end


--- Color Constructor
--- @return Color
function Color.new() return Color end

--- Color Red
Color.Red = 0

--- Color Green
Color.Green = 0

--- Color Blue
Color.Blue = 0

--- Color Alpha
Color.Alpha = 0

--- @SpriteComponent class
SpriteComponent = {}


--- @class Entity
Entity = {}

--- Entity Name
Entity.Name = ""

--- Entity Id
Entity.Id = 0

--- Entity IsAlive
Entity.IsAlive = true

--- Entity Position
Entity.Position = Position.new()

--- Entity Size
Entity.Size = Size.new()

--- Entity Color
Entity.Color = Color.new()


--- Entity Constructor
--- @param id number Entity Id
--- @param name string Entity Name
--- @param position Position Entity Position
--- @param size Size Entity Size
--- @param color Color Entity Color
--- @return Entity
function VX_CreateEntity(id, name, position, size, color) end

--- Find Entity By Id
--- @param id number Entity Id
--- @return Entity
function VX_FindEntityById(id) end

--- Get left mouse button down
--- @return boolean
function VX_IsMouseLeftDown() end

--- Get left mouse button up
--- @return boolean
function VX_IsMouseLeftUp() end

--- Get right mouse button down
--- @return boolean
function VX_IsMouseRightDown() end

--- Get right mouse button up
--- @return boolean
function VX_IsMouseRightUp() end

--- Get middle mouse button down
--- @return boolean
function VX_IsMouseMiddleDown() end

--- Get middle mouse button up
--- @return boolean
function VX_IsMouseMiddleUp() end


--- Get key down
--- @param key number SDL key code
--- @return boolean
function VX_GetKeyDown(key) end

--- Get key up
--- @param key number SDL key code
--- @return boolean
function VX_GetKeyUp(key) end

--- Get key held
--- @param key number SDL key code
--- @return boolean
function VX_GetKeyHeld(key) end

--- Get time since start
--- @return number
function VX_GetRuntime() end

--- Pause execution for a duration
--- @param duration number Duration in seconds
function VX_Wait(duration) end

--- Quit VX Engine
function VX_Quit() end