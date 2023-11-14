if isKeyPressed(KEYBOARD.RIGHT) and getSprite(player).xPos < xBound then
	getSprite(player).xPos = getSprite(player).xPos + speed
elseif isKeyPressed(KEYBOARD.LEFT) and getSprite(player).xPos > -xBound then
	getSprite(player).xPos = getSprite(player).xPos - speed
end
if isKeyPressed(KEYBOARD.SPACE) and getSprite(player).yPos == -yBound then
	getPhysics(player).v.y = getPhysics(player).v.y + jumpForce
	playSound("boing")
end
if isKeyPressed(KEYBOARD.R) then
	getSprite(img).scale = getSprite(img).scale - 5
elseif isKeyPressed(KEYBOARD.T) then
	getSprite(img).scale = getSprite(img).scale + 5
end