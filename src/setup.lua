player = createEntity()
loadImg("square", "C:/Users/Jack/Projects/CS 425/myengine/assets/Untitled.png")
loadSound("boing", "C:/Users/Jack/Projects/CS 425/myengine/assets/boing.wav")
getSprite(player).name = "square"
getSprite(player).xPos = 0
getSprite(player).yPos = 0
getSprite(player).scale = 25.0
getSprite(player).z = 1.0
getPhysics(player).g = -75

require 'loop'