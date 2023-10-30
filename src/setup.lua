player = createEntity()
img = createEntity()
loadImg("square", "C:/Users/gamec/Projects/CS 425/myengine/assets/Untitled.png")
loadImg("img", "C:/Users/gamec/Projects/CS 425/myengine/assets/good.png")
loadSound("boing", "C:/Users/gamec/Projects/CS 425/myengine/assets/boing.wav")
getSprite(player).name = "square"
getSprite(player).xPos = 0
getSprite(player).yPos = 0
getSprite(player).scale = 25.0
getSprite(player).z = 1.0
getPhysics(player).g = -100

getSprite(img).name = "img"
getSprite(img).xPos = 0
getSprite(img).yPos = 0
getSprite(img).scale = 25.0
getSprite(img).z = 2.0

require 'loop'