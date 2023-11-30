xBound = 170
yBound = 90
speed = 10
jumpForce = 150
gravity = -500
flip = false

health = 3
health2 = 1

player = createEntity()
platform1 = createEntity()
platform2 = createEntity()
platform3 = createEntity()
platform4 = createEntity()
platform5 = createEntity()
platform6 = createEntity()
platform7 = createEntity()
platform8 = createEntity()
platform9 = createEntity()
platform10 = createEntity()
enemy1 = createEntity()
enemy2 = createEntity()
enemy3 = createEntity()
enemy4 = createEntity()
background = createEntity()
obstacle1 = createEntity()
obstacle2 = createEntity()

loadImg("bunny", "assets/bunny_stand.png")
loadImg("ground_grass", "assets/ground_grass.png")
loadImg("background1", "assets/background.png")
loadImg("spikeMan", "assets/spikeMan_stand.png")
loadImg("flyMan", "assets/flyMan_still_stand.png")
loadImg("ground_stone", "assets/ground_stone.png")
loadImg("ground_wood", "assets/ground_wood.png")
loadImg("spike1", "assets/spike.png")
loadImg("spike2", "assets/spikes.png")

loadSound("boing", "assets/boing.wav")

getSprite(player).name = "bunny"
getSprite(player).xPos = 0
getSprite(player).yPos = 0
getSprite(player).scale = 10.0
getSprite(player).z = 1.0
getPhysics(player).g = gravity
getHealth(player).percent = health
scale = getSprite(player).scale/100.0
getBoundingBox(player).x = 0
getBoundingBox(player).y = 0
getBoundingBox(player).width = math.floor(getWidth("bunny") * scale)
getBoundingBox(player).height = math.floor(getHeight("bunny") * scale)

getSprite(enemy1).name = "spikeMan"
getSprite(enemy1).xPos = 50
getSprite(enemy1).yPos = 0
getSprite(enemy1).scale = 10.0
getSprite(enemy1).z = 1.0
getPhysics(enemy1).g = gravity
getHealth(enemy1).percent = health2
scale = getSprite(enemy1).scale/100.0
getBoundingBox(enemy1).x = 0
getBoundingBox(enemy1).y = 0
getBoundingBox(enemy1).width = math.floor(getWidth("spikeMan") * scale)
getBoundingBox(enemy1).height = math.floor(getHeight("spikeMan") * scale)

getSprite(enemy2).name = "flyMan"
getSprite(enemy2).xPos = 100
getSprite(enemy2).yPos = 0
getSprite(enemy2).scale = 10.0
getSprite(enemy2).z = 1.0
getPhysics(enemy2).g = gravity
getHealth(enemy2).percent = health2
scale = getSprite(enemy2).scale/100.0
getBoundingBox(enemy2).x = 0
getBoundingBox(enemy2).y = 0
getBoundingBox(enemy2).width = math.floor(getWidth("flyMan") * scale)
getBoundingBox(enemy2).height = math.floor(getHeight("flyMan") * scale)

getSprite(obstacle1).name = "spike1"
getSprite(obstacle1).xPos = 0
getSprite(obstacle1).yPos = 0
getSprite(obstacle1).scale = 10.0
getSprite(obstacle1).z = 1.0
getPhysics(obstacle1).g = gravity
scale = getSprite(obstacle1).scale/100.0
getBoundingBox(obstacle1).x = 0
getBoundingBox(obstacle1).y = 0
getBoundingBox(obstacle1).width = math.floor(getWidth("spike1") * scale)
getBoundingBox(obstacle1).height = math.floor(getHeight("spike1") * scale)

getSprite(obstacle2).name = "spike2"
getSprite(obstacle2).xPos = 0
getSprite(obstacle2).yPos = 0
getSprite(obstacle2).scale = 10.0
getSprite(obstacle2).z = 1.0
getPhysics(obstacle2).g = gravity
scale = getSprite(obstacle2).scale/100.0
getBoundingBox(obstacle2).x = 0
getBoundingBox(obstacle2).y = 0
getBoundingBox(obstacle2).width = math.floor(getWidth("spike2") * scale)
getBoundingBox(obstacle2).height = math.floor(getHeight("spike2") * scale)

getSprite(background).name = "background1"
getSprite(background).xPos = 0
getSprite(background).yPos = 0
getSprite(background).scale = 200.0
getSprite(background).z = 3.0
scale = getSprite(background).scale/100.0
getBoundingBox(background).x = 0
getBoundingBox(background).y = 0
getBoundingBox(background).width = math.floor(getWidth("background1") * scale)
getBoundingBox(background).height = math.floor(getHeight("background1") * scale)


require 'loop'