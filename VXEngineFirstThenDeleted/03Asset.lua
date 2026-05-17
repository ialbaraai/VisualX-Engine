-- 03_image_swapper.lua
-- Demo: Toggle an entity image using VX_SetImage; drag the image with left mouse.
-- Keys: Space toggles sprite; ESC to quit.
-- NOTE: Place these two files in your assets: "assets/crate.png", "assets/star.png"

local sprite
local useA = true
local id = 21
local pathA = "assets/crate.jpg"
local pathB = "assets/star.png"

function Start()
    -- Start with a colored placeholder; we'll swap to an image immediately
    sprite = VX_CreateEntity(
        id, "Sprite",
        Position.new(WindowWidth * 0.5 - 32, WindowHeight * 0.5 - 32),
        Size.new(64, 64),
        Color.new(255, 255, 255, 255),
        SpriteComponent.new(false, "")
    )

    -- Try to set initial image
    local ok = VX_SetImage(id, pathA)
    VX_SetThumbnail(pathB)

    if ok then
        sprite.SpriteComponent.IsImage = true
        useA = true
    else
        -- If image fails, keep colored fallback (helps debugging)
        sprite.SpriteComponent.IsImage = false
    end
end

function Loop(dt)
    if VX_GetKeyDown(Keys.ESC) then VX_Quit(); return end

    -- Toggle image on Space
    if VX_GetKeyDown(Keys.Space) then
        local newPath = useA and pathB or pathA
        local prevPath = useA and pathA or pathB
        if VX_SetImage(id, newPath) then
            VX_SetThumbnail(prevPath)
            sprite.SpriteComponent.IsImage = true
            useA = not useA
        else
            -- Fallback to a color if set failed
            sprite.SpriteComponent.IsImage = false
            sprite.Color = Color.new(255, 180, 60, 255)
        end
    end

    -- Drag with left mouse: center the sprite on the cursor
    if VX_IsMouseLeftDown() or VX_GetKeyHeld(Keys.LeftShift) then
        sprite.Position.X = Mouse.X - sprite.Size.X * 0.5
        sprite.Position.Y = Mouse.Y - sprite.Size.Y * 0.5
    end
end
