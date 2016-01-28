-- Format: 
-- { 
-- releaseStaticPath,
-- debugStaticPath,
-- projectPath,
-- premakescript
-- } 
return {
	ai = {
		VS2010 = { 
			nil,
			nil,
			engineLocation.."/Source/AI",
			"ai",
		},
		VS2013 = { 
			nil,
			nil,
			engineLocation.."/Source/AI",
			"ai",
		}
	},
	base = {
		VS2010 = { 
			nil,
			nil,
			engineLocation.."/Source/Base",
			"base",
		},
		VS2013 = { 
			nil,
			nil,
			engineLocation.."/Source/Base",
			"base",
		}
	},
	gamebase = {
		VS2010 = { 
			nil,
			nil,
			engineLocation.."/Source/GameBase",
			"gamebase",
		},
		VS2013 = { 
			nil,
			nil,
			engineLocation.."/Source/GameBase",
			"gamebase",
		}
	},
	utilities = {
		VS2010 = { 
			nil,
			nil,
			engineLocation.."/Source/Utilities",
			"utilities",
		},
		VS2013 = { 
			nil,
			nil,
			engineLocation.."/Source/Utilities",
			"utilities",
		}
	},
	graphics = {
		VS2010 = { 
			nil,
			nil,
			engineLocation.."/Source/GraphicsEngine",
			"graphics",
		},
		VS2013 = { 
			nil,
			nil,
			engineLocation.."/Source/GraphicsEngine",
			"graphics",
		}
	},
	physics = {
		VS2010 = { 
			nil,
			nil,
			engineLocation.."/Source/Physics",
			"physics",
		},
		VS2013 = { 
			nil,
			nil,
			engineLocation.."/Source/Physics",
			"physics",
		}
	},
	sound = {
		VS2010 = { 
			nil,
			nil,
			engineLocation.."/Source/Sound",
			"sound",
		},
		VS2013 = { 
			nil,
			nil,
			engineLocation.."/Source/Sound",
			"sound",
		}
	},
}
