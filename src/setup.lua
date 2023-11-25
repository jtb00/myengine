xBound = 110
yBound = 80
speed = 10
jumpForce = 250
gravity = -500
flip = false
health = 3

player = createEntity()
img = createEntity()
loadImg("square", "assets/Untitled.png")
loadSound("boing", "assets/boing.wav")
getSprite(player).name = "square"
getSprite(player).xPos = 0
getSprite(player).yPos = 0
getSprite(player).scale = 25.0
getSprite(player).z = 1.0
getPhysics(player).g = gravity
getHealth(player).percent = health
setBoundingBox(player, {x = 0, y = 0, width = 25, height = 25})

require 'loop'