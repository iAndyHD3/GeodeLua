
include("CreatorLayer")
include("CCDirector")

scene = CreatorLayer.scene()
dir = CCDirector.sharedDirector()
CCDirector.replaceScene(dir, scene)