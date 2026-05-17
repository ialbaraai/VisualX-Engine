-- 01_player_move.lua
-- Demo: Move a player rectangle with WASD. A tiny "cursor" square follows the mouse.
-- Keys: WASD to move, ESC to quit.

local player, cursor
local speed = 220 -- px/sec

function Start()
    -- Create a blue player box
    player = VX_CreateEntity(
        1,                                      -- Id
        "Player",                               -- Name (Entities["Player"])
        Position.new(WindowWidth * 0.5, WindowHeight * 0.5),
        Size.new(60, 60),
        Color.new(40, 120, 255, 255),
        SpriteComponent.new(false, "")              -- solid color rectangle
    )

    -- Create a small white square that tracks the mouse
    cursor = VX_CreateEntity(
        2,
        "Cursor",
        Position.new(0, 0),
        Size.new(10, 10),
        Color.new(255, 255, 255, 255),
        SpriteComponent.new(false, "")
    )
end

function Loop(dt)
    -- Quit
    if VX_GetKeyDown(Keys.ESC) then
        VX_Quit()
        return
    end

    -- Move with WASD
    local dx, dy = 0, 0
    if VX_GetKeyHeld(Keys.w) then dy = dy - speed end
    if VX_GetKeyHeld(Keys.s) then dy = dy + speed end
    if VX_GetKeyHeld(Keys.a) then dx = dx - speed end
    if VX_GetKeyHeld(Keys.d) then dx = dx + speed end

    -- Update player position (modify fields directly)
    if player.IsAlive then
        if dx ~= 0 or dy ~= 0 then
            player.Position.X = player.Position.X + dx * dt
            player.Position.Y = player.Position.Y + dy * dt
        end

         -- Keep player inside window
        local px, py = player.Position.X, player.Position.Y
        local pw, ph = player.Size.X, player.Size.Y
        if px < 0 then player.Position.X = 0 end
        if py < 0 then player.Position.Y = 0 end
        if px + pw > WindowWidth  then player.Position.X = WindowWidth - pw end
        if py + ph > WindowHeight then player.Position.Y = WindowHeight - ph end
    end

   if cursor.IsAlive then
        -- Mouse follower
        cursor.Position.X = Mouse.X - cursor.Size.X * 0.5
        cursor.Position.Y = Mouse.Y - cursor.Size.Y * 0.5
    end
end
