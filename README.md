# GeodeLua

WIP lua bindings for geode

Example:

```lua
include("CreatorLayer")
include("CCDirector")

scene = CreatorLayer.scene()
dir = CCDirector.sharedDirector()
CCDirector.replaceScene(dir, scene)
```

this code is currently hardcoded to run on `MenuLayer::onMoreGames` however hooks are planned and it would look like this:

```lua
include("CreatorLayer")
include("CCDirector")

function MenuLayer_onMoreGames(self, sender)
	scene = CreatorLayer.scene()
	dir = CCDirector.sharedDirector()
	CCDirector.replaceScene(dir, scene)
end
```

