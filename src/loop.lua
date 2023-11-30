--prevent player from leaving window bounds
if isKeyPressed(KEYBOARD.RIGHT) and getSprite(player).xPos < xBound then
	getSprite(player).xPos = getSprite(player).xPos + speed
elseif isKeyPressed(KEYBOARD.LEFT) and getSprite(player).xPos > -xBound then
	getSprite(player).xPos = getSprite(player).xPos - speed
end
if getSprite(player).yPos <= -yBound then
	getSprite(player).yPos = -yBound
	getPhysics(player).v.y = 0
elseif getSprite(player).yPos >= yBound then
	getSprite(player).yPos = yBound
	getPhysics(player).v.y = 0
end
if flip then
	if isKeyPressed(KEYBOARD.DOWN) and getSprite(player).yPos >= yBound then
		getPhysics(player).g = gravity
		flip = false
		--getSprite(player).scale = -scale
	end
	if isKeyPressed(KEYBOARD.SPACE) and getSprite(player).yPos >= yBound then
		getPhysics(player).v.y = getPhysics(player).v.y - jumpForce
		playSound("boing")
	end
else
	if isKeyPressed(KEYBOARD.UP) and getSprite(player).yPos <= -yBound then
		getPhysics(player).g = -gravity
		flip = true
		--getSprite(player).scale = -scale
	end
	if isKeyPressed(KEYBOARD.SPACE) and getSprite(player).yPos <= -yBound then
		getPhysics(player).v.y = getPhysics(player).v.y + jumpForce
		playSound("boing")
	end
end

spikeTimer = spikeTimer - 1
spikeManTimer = spikeManTimer - 1
flyManTimer = flyManTimer - 1

if spikeTimer == 0 then
	getSprite(enemy1).xPos = xBound
	spikeTimer = 100
end	
getSprite(enemy1).xPos = getSprite(enemy1).xPos - 1