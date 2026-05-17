PlayerUp = false
PlayerDown = false
PlayerRight = false
PlayerLeft = false
SpeedMultiplier = 1

function PlayerMovement(player, deltaTime)
	if VX_GetKeyDown(Keys.w) then
		PlayerUp = true
	end
	if VX_GetKeyDown(Keys.s) then
		PlayerDown = true
	end
	if VX_GetKeyDown(Keys.d) then
		PlayerRight = true
	end
	if VX_GetKeyDown(Keys.a) then
		PlayerLeft = true
	end

	if VX_GetKeyUp(Keys.w) then
		PlayerUp = false
	end
	if VX_GetKeyUp(Keys.s) then
		PlayerDown = false
	end
	if VX_GetKeyUp(Keys.d) then
		PlayerRight = false
	end
	if VX_GetKeyUp(Keys.a) then
		PlayerLeft = false
	end

	if VX_GetKeyDown(Keys.LeftShift) then
		SpeedMultiplier = 2
	end

	if VX_GetKeyUp(Keys.LeftShift) then
		SpeedMultiplier = 1
	end

	if player then
		if PlayerUp then
			player.Position.Y = player.Position.Y - 150 * deltaTime * SpeedMultiplier
		end
		if PlayerDown then
			player.Position.Y = player.Position.Y + 150 * deltaTime * SpeedMultiplier
		end
		if PlayerLeft then
			player.Position.X = player.Position.X - 150 * deltaTime * SpeedMultiplier
		end
		if PlayerRight then
			player.Position.X = player.Position.X + 150 * deltaTime * SpeedMultiplier
		end
	end
end

function CheckCollision(A, B)
	if A.IsAlive and B.IsAlive then
		ATop = A.Position.Y
		ABottom = A.Position.Y + A.Size.Y
		ALeft = A.Position.X
		ARight = A.Position.X + A.Size.X

		BTop = B.Position.Y
		BBottom = B.Position.Y + B.Size.Y
		BLeft = B.Position.X
		BRight = B.Position.X + B.Size.X

		return ((ALeft < BRight and ARight > BLeft) and (ABottom > BTop and ATop < BBottom ))
	end
end