--prevent player from leaving window bounds
if getSprite(player).name == "bunny" then
	if isKeyPressed(KEYBOARD.RIGHT) and getSprite(player).xPos < xBound then
		getSprite(player).xPos = getSprite(player).xPos + speed
	elseif isKeyPressed(KEYBOARD.LEFT) and getSprite(player).xPos > -xBound then
		getSprite(player).xPos = getSprite(player).xPos - speed
	end
end
if getSprite(player).yPos <= -yBound then
	getSprite(player).yPos = -yBound
	getPhysics(player).v.y = 0
elseif getSprite(player).yPos >= yBound then
	getSprite(player).yPos = yBound
	getPhysics(player).v.y = 0
end
if getSprite(player).name == "bunny" then
	if flip then
			if isKeyPressed(KEYBOARD.DOWN) and getSprite(player).yPos >= yBound then
				getPhysics(player).g = gravity
				flip = false
				getSprite(player).scale = -getSprite(player).scale
			end
			if isKeyPressed(KEYBOARD.SPACE) and getSprite(player).yPos >= yBound then
				getPhysics(player).v.y = getPhysics(player).v.y - jumpForce
				playSound("boing")
			end
		else
			if isKeyPressed(KEYBOARD.UP) and getSprite(player).yPos <= -yBound then
				getPhysics(player).g = -gravity
				flip = true
				getSprite(player).scale = -getSprite(player).scale
			end
			if isKeyPressed(KEYBOARD.SPACE) and getSprite(player).yPos <= -yBound then
				getPhysics(player).v.y = getPhysics(player).v.y + jumpForce
				playSound("boing")
			end
		end
end

spikeTimer = spikeTimer - 1
spikeManTimer = spikeManTimer - 1
flyManTimer = flyManTimer - 1

if spikeTimer == 0 then
	getSprite(obstacle1).xPos = xBound + 10
	spikeTimer = 350
end	
if spikeManTimer == 0 then
	getSprite(enemy1).xPos = -xBound - 10
	spikeManTimer = 250
end	
if flyManTimer == 0 then
	getSprite(enemy2).xPos = -xBound - 10
	flyManTimer = 250
end
if getSprite(obstacle2).xPos < -xBound - 10 then
	rand = randInt() % 250
	if rand == 1 then
		getSprite(obstacle2).xPos = xBound + 10
	end
end
getSprite(enemy1).xPos = getSprite(enemy1).xPos + spikeManSpeed
getSprite(obstacle1).xPos = getSprite(obstacle1).xPos - spikeSpeed
getSprite(obstacle2).xPos = getSprite(obstacle2).xPos - spikeSpeed
getSprite(enemy2).xPos = getSprite(enemy2).xPos + flyManSpeed