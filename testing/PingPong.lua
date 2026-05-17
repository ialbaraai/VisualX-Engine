PlayerA = nil
PlayerB = nil
Ball = nil

IsChanged = false

-- Input States
PlayerAUp = false
PlayerADown = false
PlayerBUp = false
PlayerBDown = false

-- Game State
PlayerAScore = 0
PlayerBScore = 0
IsSet = false
IsDisabled = false

-- Physics Parameters
PlayerMaxSpeed = 350
PlayerAcceleration = 800
PlayerFriction = 1000

BallMinSpeed = 200
BallMaxSpeed = 600
BallSpeedCap = 800
BallSpeedIncrement = 40

-- Velocity States
PlayerAVelocity = 0.0
PlayerBVelocity = 0.0
BallVelocityX = 0.0
BallVelocityY = 0.0
BallSpin = 0.0
SpinDecay = 200.0

-- Starting position
BallStartingPositionX = 0
BallStartingPositionY = 0

function CheckCollision(A, B)
    if not (A.IsAlive and B.IsAlive) then return false end

    local ATop = A.Position.Y
    local ABottom = A.Position.Y + A.Size.Y
    local ALeft = A.Position.X
    local ARight = A.Position.X + A.Size.X

    local BTop = B.Position.Y
    local BBottom = B.Position.Y + B.Size.Y
    local BLeft = B.Position.X
    local BRight = B.Position.X + B.Size.X

    return (ALeft < BRight and ARight > BLeft) and (ABottom > BTop and ATop < BBottom)
end

-- Push ball out of paddle to prevent sticking
function ResolveCollision(A, B)
    if not (A.IsAlive and B.IsAlive) then return end

    local overlapLeft   = B.Position.X + B.Size.X - A.Position.X
    local overlapRight  = A.Position.X + A.Size.X - B.Position.X
    local overlapTop    = B.Position.Y + B.Size.Y - A.Position.Y
    local overlapBottom = A.Position.Y + A.Size.Y - B.Position.Y

    local minOverlap = math.min(overlapLeft, overlapRight, overlapTop, overlapBottom)

    if minOverlap == overlapLeft then
        A.Position.X = B.Position.X + B.Size.X
    elseif minOverlap == overlapRight then
        A.Position.X = B.Position.X - A.Size.X
    elseif minOverlap == overlapTop then
        A.Position.Y = B.Position.Y + B.Size.Y
    elseif minOverlap == overlapBottom then
        A.Position.Y = B.Position.Y - A.Size.Y
    end
end

function Start()
    VX_SetFPS(VX_SixtyFPS())

    VX_SetThumbnail("PingPonBall.png")

    Ball = Entities.Ball
    PlayerA = Entities.PlayerA
    PlayerB = Entities.PlayerB

    if Ball.IsAlive then
        BallStartingPositionX = Ball.Position.X
        BallStartingPositionY = Ball.Position.Y

        -- Initial serve
        local dir = math.random() > 0.5 and 1 or -1
        BallVelocityX = dir * 250
        BallVelocityY = (math.random() - 0.5) * 200
    end
end

function Loop(deltaTime)
    -- Prevent physics explosion
    deltaTime = math.min(deltaTime, 0.02)

    -- === PLAYER A MOVEMENT (W/S + Shift/Control) ===
    if PlayerA.IsAlive then
        local upHeld = VX_GetKeyHeld(Keys.w)
        local downHeld = VX_GetKeyHeld(Keys.s)
        local boosting = VX_GetKeyHeld(Keys.LeftShift)
        local slowing = VX_GetKeyHeld(Keys.LeftControl)
        local boost = boosting and 1.8 or (slowing and 0.6 or 1.0)

        -- Accelerate
        if upHeld and not downHeld then
            PlayerAVelocity = PlayerAVelocity - PlayerAcceleration * deltaTime * boost
        elseif downHeld and not upHeld then
            PlayerAVelocity = PlayerAVelocity + PlayerAcceleration * deltaTime * boost
        else
            -- Apply friction
            local drag = PlayerFriction * deltaTime
            if math.abs(PlayerAVelocity) < drag then
                PlayerAVelocity = 0
            elseif PlayerAVelocity > 0 then
                PlayerAVelocity = PlayerAVelocity - drag
            else
                PlayerAVelocity = PlayerAVelocity + drag
            end
        end

        -- Cap speed
        PlayerAVelocity = math.max(-PlayerMaxSpeed, math.min(PlayerMaxSpeed, PlayerAVelocity))

        -- Update position
        PlayerA.Position.Y = PlayerA.Position.Y + PlayerAVelocity * deltaTime

        -- Boundaries
        if PlayerA.Position.Y < 10 then
            PlayerA.Position.Y = 10
            PlayerAVelocity = 0
        end
        if PlayerA.Position.Y + PlayerA.Size.Y > WindowHeight - 10 then
            PlayerA.Position.Y = WindowHeight - PlayerA.Size.Y - 10
            PlayerAVelocity = 0
        end
    end

    -- === PLAYER B MOVEMENT (Arrows + RShift/RControl) ===
    if PlayerB.IsAlive then
        local upHeld = VX_GetKeyHeld(Keys.ArrowUp)
        local downHeld = VX_GetKeyHeld(Keys.ArrowDown)
        local boosting = VX_GetKeyHeld(Keys.RightShift)
        local slowing = VX_GetKeyHeld(Keys.RightControl)
        local boost = boosting and 1.8 or (slowing and 0.6 or 1.0)

        if upHeld and not downHeld then
            PlayerBVelocity = PlayerBVelocity - PlayerAcceleration * deltaTime * boost
        elseif downHeld and not upHeld then
            PlayerBVelocity = PlayerBVelocity + PlayerAcceleration * deltaTime * boost
        else
            local drag = PlayerFriction * deltaTime
            if math.abs(PlayerBVelocity) < drag then
                PlayerBVelocity = 0
            elseif PlayerBVelocity > 0 then
                PlayerBVelocity = PlayerBVelocity - drag
            else
                PlayerBVelocity = PlayerBVelocity + drag
            end
        end

        PlayerBVelocity = math.max(-PlayerMaxSpeed, math.min(PlayerMaxSpeed, PlayerBVelocity))

        PlayerB.Position.Y = PlayerB.Position.Y + PlayerBVelocity * deltaTime

        if PlayerB.Position.Y < 10 then
            PlayerB.Position.Y = 10
            PlayerBVelocity = 0
        end
        if PlayerB.Position.Y + PlayerB.Size.Y > WindowHeight - 10 then
            PlayerB.Position.Y = WindowHeight - PlayerB.Size.Y - 10
            PlayerBVelocity = 0
        end

        -- Disable sprite after 5s, re-enable with image after 10s
        if VX_GetRuntime() >= 5 and not IsDisabled then
            PlayerB.SpriteComponent.IsImage = false
            IsDisabled = true
        end

        if VX_GetRuntime() >= 10 and not IsSet then
            -- We assume the image file "red.png" exists and is valid
            if VX_SetImage(PlayerB.Id, "red.png") then
                IsSet = true
            end
        end
    end

    -- === BALL PHYSICS ===
    if Ball.IsAlive then
        -- Apply spin to vertical velocity
        BallVelocityY = BallVelocityY + BallSpin * deltaTime
        BallSpin = BallSpin - math.sin(BallSpin) * SpinDecay * deltaTime
        if math.abs(BallSpin) < 5 then BallSpin = 0 end

        -- Update position
        Ball.Position.X = Ball.Position.X + BallVelocityX * deltaTime
        Ball.Position.Y = Ball.Position.Y + BallVelocityY * deltaTime

        -- Top/Bottom Wall Collision
        if Ball.Position.Y <= 0 then
            Ball.Position.Y = 0
            BallVelocityY = -BallVelocityY * 0.9  -- Energy loss
        elseif Ball.Position.Y + Ball.Size.Y >= WindowHeight then
            Ball.Position.Y = WindowHeight - Ball.Size.Y
            BallVelocityY = -BallVelocityY * 0.9
        end

        -- Paddle A Hit
        if CheckCollision(PlayerA, Ball) then
            ResolveCollision(Ball, PlayerA)
            BallVelocityX = math.abs(BallVelocityX)  -- Move right

            -- Relative hit position: -1 (top) to +1 (bottom)
            local hitPos = (Ball.Position.Y + Ball.Size.Y / 2) - (PlayerA.Position.Y + PlayerA.Size.Y / 2)
            local relHit = hitPos / (PlayerA.Size.Y / 2)

            BallVelocityY = relHit * 300
            BallSpin = relHit * 150  -- Impart spin

            -- Speed up ball slightly
            local speed = math.sqrt(BallVelocityX^2 + BallVelocityY^2)
            speed = math.min(speed + BallSpeedIncrement, BallSpeedCap)
            local angle = math.atan2(BallVelocityY, BallVelocityX)
            BallVelocityX = math.cos(angle) * speed
            BallVelocityY = math.sin(angle) * speed
        end

        -- Paddle B Hit
        if CheckCollision(PlayerB, Ball) then
            ResolveCollision(Ball, PlayerB)
            BallVelocityX = -math.abs(BallVelocityX)  -- Move left

            local hitPos = (Ball.Position.Y + Ball.Size.Y / 2) - (PlayerB.Position.Y + PlayerB.Size.Y / 2)
            local relHit = hitPos / (PlayerB.Size.Y / 2)

            BallVelocityY = relHit * 300
            BallSpin = relHit * 150

            local speed = math.sqrt(BallVelocityX^2 + BallVelocityY^2)
            speed = math.min(speed + BallSpeedIncrement, BallSpeedCap)
            local angle = math.atan2(BallVelocityY, BallVelocityX)
            BallVelocityX = math.cos(angle) * speed
            BallVelocityY = math.sin(angle) * speed
        end

        -- Scoring
        if Ball.Position.X + Ball.Size.X < 0 then
            PlayerBScore = PlayerBScore + 1
            ResetBall(1)
        elseif Ball.Position.X > WindowWidth then
            PlayerAScore = PlayerAScore + 1
            ResetBall(-1)
        end
    end
end

function ResetBall(direction)
    Ball.Position.X = BallStartingPositionX
    Ball.Position.Y = BallStartingPositionY

    BallVelocityX = direction * 250
    BallVelocityY = (math.random() - 0.5) * 200
    BallSpin = 0

    print("Score! A: " .. PlayerAScore .. " | B: " .. PlayerBScore)
end

-- Math clamp helper
function math.clamp(x, min, max)
    if x < min then return min end
    if x > max then return max end
    return x
end