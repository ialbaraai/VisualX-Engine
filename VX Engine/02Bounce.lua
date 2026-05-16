-- 02_bouncing_ball.lua
-- Improved: Realistic bouncing, better collision response, energy conservation, no embedding.

local ball, paddle
local vx, vy = 250, 330        -- ball velocity (px/sec)
local paddleSpeed = 350
local bouncedThisFrame = false -- prevent multiple bounces in one frame

local function aabb(ax, ay, aw, ah, bx, by, bw, bh)
    return ax < bx + bw and ax + aw > bx and ay < by + bh and ay + ah > by
end

function Start()
    ball = VX_CreateEntity(
        10, "Ball",
        Position.new(WindowWidth * 0.3, WindowHeight * 0.3),
        Size.new(24, 24),
        Color.new(240, 60, 60, 255),
        SpriteComponent.new(false, "")
    )

    paddle = VX_CreateEntity(
        11, "Paddle",
        Position.new(WindowWidth * 0.5 - 60, WindowHeight - 40),
        Size.new(120, 18),
        Color.new(60, 220, 120, 255),
        SpriteComponent.new(false, "")
    )
end

function Loop(dt)
    if VX_GetKeyDown(Keys.ESC) then VX_Quit(); return end

    -- Reset bounce flag each frame
    bouncedThisFrame = false

    -- Move paddle
    if VX_GetKeyHeld(Keys.ArrowLeft)  then paddle.Position.X = paddle.Position.X - paddleSpeed * dt end
    if VX_GetKeyHeld(Keys.ArrowRight) then paddle.Position.X = paddle.Position.X + paddleSpeed * dt end

    -- Clamp paddle
    if paddle.Position.X < 0 then paddle.Position.X = 0 end
    local maxPaddleX = WindowWidth - paddle.Size.X
    if paddle.Position.X > maxPaddleX then paddle.Position.X = maxPaddleX end

    -- Move ball first
    ball.Position.X = ball.Position.X + vx * dt
    ball.Position.Y = ball.Position.Y + vy * dt

    -- === WALL COLLISIONS ===
    local ballRight = ball.Position.X + ball.Size.X
    local ballBottom = ball.Position.Y + ball.Size.Y

    if ball.Position.X <= 0 then
        ball.Position.X = 0
        vx = -vx
    elseif ballRight >= WindowWidth then
        ball.Position.X = WindowWidth - ball.Size.X
        vx = -vx
    end

    if ball.Position.Y <= 0 then
        ball.Position.Y = 0
        vy = -vy
    elseif ballBottom >= WindowHeight then
        -- Reset ball on bottom hit
        ball.Position = Position.new(WindowWidth * 0.5, WindowHeight * 0.3)
        -- Randomize initial direction slightly for variety
        local angle = math.pi/4 + (math.random() - 0.5) * 0.6  -- ~35° to 55° up
        local speed = 280
        vx = math.cos(angle) * speed * (math.random() < 0.5 and 1 or -1)
        vy = -math.sin(angle) * speed
        bouncedThisFrame = true -- avoid immediate re-collision if spawned near paddle
    end

    -- === PADDLE COLLISION ===
    if not bouncedThisFrame and aabb(
        ball.Position.X, ball.Position.Y, ball.Size.X, ball.Size.Y,
        paddle.Position.X, paddle.Position.Y, paddle.Size.X, paddle.Size.Y
    ) then
        if vy > 0 then
            bouncedThisFrame = true

            -- Place ball above paddle
            ball.Position.Y = paddle.Position.Y - ball.Size.Y - 1

            -- Find hit position relative to paddle center
            local ballCenterX = ball.Position.X + ball.Size.X * 0.5
            local paddleCenterX = paddle.Position.X + paddle.Size.X * 0.5
            local hitOffset = (ballCenterX - paddleCenterX) / (paddle.Size.X * 0.5)
            hitOffset = math.max(-1, math.min(1, hitOffset))

            -- Angle control
            local maxAngle = math.pi / 3
            local angle = hitOffset * maxAngle

            -- === SPEED INCREASE MECHANIC ===
            -- You choose the multiplier; 1.05 is a nice growth rate
            local increaseFactor = 1.07  
            local speed = math.sqrt(vx*vx + vy*vy) * increaseFactor

            -- Hard clamp to avoid instability
            local maxSpeed = 900
            if speed > maxSpeed then speed = maxSpeed end

            -- Apply new velocity using the reflection angle
            vx = math.sin(angle) * speed
            vy = -math.cos(angle) * speed

            -- Paddle flash
            paddle.Color = Color.new(80 + math.random(0, 100), 200, 140, 255)
        end
    end
end