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

this code is currently hardcoded to run on `MenuLayer::onMoreGames`, but hooks are planned so it would look like this:

```lua
include("CreatorLayer")
include("CCDirector")

function MenuLayer_onMoreGames(self, sender)
	scene = CreatorLayer.scene()
	dir = CCDirector.sharedDirector()
	CCDirector.replaceScene(dir, scene)
end
```

TODO in order:
- Hooks
- Handle overloaded functions (eg `FLAlertLayer::create`)
- Bind class members


codegen.cpp takes a few minutes because of templates, so be careful and avoid editing codegen.cpp accidentally