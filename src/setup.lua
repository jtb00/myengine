player = createEntity()
loadImg("square", "C:/Users/Jack/Projects/CS 425/myengine/assets/Untitled.png")
getSprite(player).name = "square"
getSprite(player).xPos = 0
getSprite(player).yPos = 0
getSprite(player).scale = 25.0
getSprite(player).z = 1.0
getVelocity(player).x = 0
getVelocity(player).y = 0
getGravity(player).accel = -9.8

require 'loop'