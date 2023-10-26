if isKeyPressed(KEYBOARD.RIGHT) then
	getSprite(player).xPos = getSprite(player).xPos + 10
elseif isKeyPressed(KEYBOARD.LEFT) then
	getSprite(player).xPos = getSprite(player).xPos - 10
end
if isKeyPressed(KEYBOARD.SPACE) and getSprite(player).yPos == -80 then
	getPhysics(player).v.y = getPhysics(player).v.y + 100
	playSound("boing")
end