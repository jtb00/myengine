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
loadImg("test", "assets/good.png")
loadSound("boing", "assets/boing.wav")
getSprite(player).name = "square"
getSprite(player).xPos = 0
getSprite(player).yPos = 0
getSprite(player).scale = 25.0
getSprite(player).z = 1.0
getPhysics(player).g = gravity
getHealth(player).percent = health
getBoundingBox(player).x = 0
getBoundingBox(player).y = 0
getBoundingBox(player).width = 25
getBoundingBox(player).height = 25

getSprite(img).name = "test"
getSprite(img).xPos = 0
getSprite(img).yPos = 0
getSprite(img).scale = 25.0
getSprite(img).z = 2.0
getBoundingBox(img).x = 0
getBoundingBox(img).y = 0
getBoundingBox(img).width = 25
getBoundingBox(img).height = 25

require 'loop'