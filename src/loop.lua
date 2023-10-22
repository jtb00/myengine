start = os.time()
if isKeyPressed(KEYBOARD.RIGHT) then
	getSprite(player).xPos = getSprite(player).xPos + 10
elseif isKeyPressed(KEYBOARD.LEFT) then
	getSprite(player).xPos = getSprite(player).xPos - 10
end

dt = os.difftime(os.time(), start)
getVelocity(player).y = getVelocity(player).y + getGravity(player).accel * dt
getSprite(player).yPos = getSprite(player).yPos + getVelocity(player).y * dt