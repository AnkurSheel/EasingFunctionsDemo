-- solution "Common" -- Name for the solution, we'll only use this one as a template so won't be generated because there's no projects inside it
	-- configurations { "Debug", "Release", "ReleaseCandidate" } -- List of all our build types
	-- platforms { "Native", "PS3" } -- Platforms, Native is Win32 in our case

	-- configuration{ "*" } -- Selects all possible configurations
		-- defines { "BOOST_ALL_NO_LIB" }

	-- configuration { "Debug" } -- Will select Win32 | Debug & PS3 | Debug in our case
		-- defines { "_DEBUG" }
		-- flags { "Symbols" } -- Debug symbols
		-- targetsuffix '_d' 
		
	-- configuration { "Release*" } -- Selects both Release & ReleaseCandidate
		-- defines { "NDEBUG", "_SECURE_SCL=0" }
		-- flags { "Optimize" } -- Optimization switches on

	-- configuration { "Native" } -- On Win32
		-- defines{ "WIN32" } -- Define WIN32

	-- configuration { "ReleaseCandidate" } -- All ReleaseCandidates needs a special define
		-- defines { "RELEASE_CANDIDATE" }
	
	-- outputDirectory = "Test"
	
	-- fileSet = 
    -- {
	-- "**.cpp",
	-- "**.hpp",
	-- "**.h",
		-- "**.hxx"
    -- }
	
	-- project "Dummy" 	
ProjectRootLocation = path.getabsolute("../../")
package.path = ProjectRootLocation.."/Engine/Scripts/?.lua;" .. package.path
require('premake_options')
local utils = require('utils')

if _ACTION == "vs2010" then
  platform = 'VS2010'
elseif _ACTION == "vs2013" then
  platform = 'VS2013'
end

libsConfig = { empty = true }

includeEngineSource = false

if _OPTIONS["no_engine_project"] == nil then
	includeEngineSource = true;
	libsConfig.empty = nil
	libsConfig.ai = "project"
	libsConfig.base = "project"
	libsConfig.gamebase = "project"
	libsConfig.graphics = "project"
	libsConfig.physics = "project"
	libsConfig.sound = "project"
	libsConfig.utilities = "project"
end


projectName = "Dummy"
if _OPTIONS["project_name"] then
	projectName = _OPTIONS["project_name"]
end

if _OPTIONS["github"] ~= nil then
	oldLocation = ProjectRootLocation
	ProjectRootLocation = path.getabsolute(ProjectRootLocation.."/../").."/"..projectName.."_Github"
	utils.CreateProjectForGithub(oldLocation, ProjectRootLocation, includeEngineSource)
end

solutionLocation = ProjectRootLocation.."/Projects/".._ACTION
engineLocation = ProjectRootLocation.."/Engine"

local libsDB = require('libsDB')

solution (projectName)
	outputDirectory = solutionLocation
	location (outputDirectory)
	language "C++"
	objdir (ProjectRootLocation.."/Obj/")
	--configurations {"Release", "Debug", "ReleaseCandidate"}
	configurations {"Release", "Debug"}
	pchheader "stdafx.h"
	
	configuration { "Debug" } -- Will select Win32 | Debug in our case
		defines { "_DEBUG", "SPASSERT_DEBUG_MODE" }
		flags { "Symbols" } -- Debug symbols
		targetsuffix '_d' 
		targetdir (ProjectRootLocation.."/Debug")
		libdirs {ProjectRootLocation.."/Debug"}
		
	configuration { "Release*" } -- Selects both Release
		defines { "NDEBUG" }
		-- flags { "Optimize", "Symbols" } -- Optimization switches on
		flags { "Optimize"} -- Optimization switches on
		targetdir (ProjectRootLocation.."/bin")
		libdirs {ProjectRootLocation.."/bin"}
		
	-- configuration { "ReleaseCandidate" } -- All ReleaseCandidates needs a special define
		-- defines { "RELEASE_CANDIDATE" }

	configuration { "Native" } -- On Win32
		defines{ "WIN32" } -- Define WIN32

	-- fileSet = 
    -- {
	-- "*.cpp",
	-- "*.hpp",
	-- "*.h",
		-- "*.hxx"
    -- }

	configuration ()
		buildoptions
		{
			-- '/Wall',
			'/wd4061', -- enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
			'/wd4100', -- 'identifier' : unreferenced formal parameter
			'/wd4127', -- conditional expression is constant
			'/wd4201', -- nonstandard extension used 
			'/wd4355', -- 'this' : used in base member initializer list
			'/wd4820', -- 'bytes' bytes padding added after construct 'member_name'
			'/we4191', -- 'operator/operation' : unsafe conversion from 'type of expression' to 'type required' (NB: prevents dumb function pointer casts)
			'/we4715', -- 'function' : not all control paths return a value
			'/we4553', -- 'operator' : operator has no effect; did you intend 'operator'? (NB: usually signals a real error)
			'/we4098', -- 'function' : void function returning a value
		}
		linkoptions 
		{
			'/ignore:4099', -- PDB 'vc100.pdb' was not found with 'name.lib(name.obj)' or at 'path'; linking object as if no debug info
		}
	vpaths 
	{
		["Interfaces"] = {ProjectRootLocation.."/**.hxx"},
		["Headers"] = {ProjectRootLocation.."/**.h"},
		["Sources"] = {ProjectRootLocation.."/**.c", ProjectRootLocation.."/**.cpp"}
	}

	mainProjectName = projectName.."Main"
	project (mainProjectName)
		kind "WindowedApp"
		location = outputDirectory
		inputPath = ProjectRootLocation.."/Source/"..mainProjectName.."/"
		utils.addfiles(inputPath)
		files
		{ 
			ProjectRootLocation.."/Debug/**.ini", 
			ProjectRootLocation.."/bin/**.ini", 
		}
		vpaths 
		{
			["DebugConfig"] = {ProjectRootLocation.."/Debug/**.ini"},
			["RelaseConfig"] = {ProjectRootLocation.."/bin/**.ini"},
		}

		includedirs
		{
			ProjectRootLocation.."/Source/"..projectName.."/Includes",
			ProjectRootLocation.."/Engine/Source/Base/Includes",
			ProjectRootLocation.."/Engine/Source/Utilities/Includes",
			ProjectRootLocation.."/Engine/Source/GameBase/Includes",
			inputPath.."src",
			ProjectRootLocation.."/Engine/extern/Include/VisualLeakDetector"
		}
		libdirs 
		{
			ProjectRootLocation.."/Engine/extern/Lib/VisualLeakDetector/Win32"
		}
		pchsource (inputPath.."src/stdafx.cpp")
		flags{"WinMain"}
		configuration ("Debug")
			debugdir (ProjectRootLocation.."/Debug")
			links { projectName.."_d", "Base_d", "Utilities_d" }
		configuration ("Release")
			debugdir (ProjectRootLocation.."/bin")
			links { projectName, "Base", "Utilities" }

	project (projectName)
		kind "SharedLib"
		location = outputDirectory
		inputPath = ProjectRootLocation.."/Source/"..projectName.."/"
		utils.addfiles(inputPath)
		files
		{ 
			ProjectRootLocation.."/Assets/**.xml", 
			ProjectRootLocation.."/Assets/**.lvl", 
			ProjectRootLocation.."/Assets/**.ini"
		}
		vpaths 
		{
			["Assets"] = {ProjectRootLocation.."/Assets/**.xml"},
			["Assets"] = {ProjectRootLocation.."/Assets/**.lvl"},
			["Assets"] = {ProjectRootLocation.."/Assets/**.ini"}
		}
		
		includedirs
		{
			ProjectRootLocation.."/Engine/Source/Base/Includes",
			ProjectRootLocation.."/Engine/Source/Utilities/Includes",
			ProjectRootLocation.."/Engine/Source/AI/Includes",
			ProjectRootLocation.."/Engine/Source/GraphicsEngine/Includes",
			ProjectRootLocation.."/Engine/Source/Physics/Includes",
			ProjectRootLocation.."/Engine/Source/Sound/Includes",
			ProjectRootLocation.."/Engine/Source/GameBase/Includes",
			ProjectRootLocation.."/Engine/extern/Include/VisualLeakDetector",
			inputPath.."Includes",
			--inputPath.."src"
		}	
		libdirs 
		{
			ProjectRootLocation.."/Engine/extern/Lib/VisualLeakDetector/Win32"
		}		
		pchsource (inputPath.."src/stdafx.cpp")
		defines("BTDEMO_EXPORTS")
		configuration ("Debug")
			links { "Base_d", "Utilities_d", "AI_d", "Graphics_d", "Physics_d", "Sound_d", "Gamebase_d" }
		configuration ("Release")
			links { "Base", "Utilities", "AI", "Graphics", "Physics", "Sound", "Gamebase" }
		configuration {}
			postbuildcommands 
			{ 
				solutionLocation.."/postbuild-event.bat"
			}

	if _OPTIONS["no_editor"] == nil then
		editorProjectName = projectName.."Editor"
		project (editorProjectName)
			kind "SharedLib"
			location = outputDirectory
			inputPath = ProjectRootLocation.."/Source/"..editorProjectName.."/"
			utils.addfiles(inputPath) 
			includedirs
			{
				ProjectRootLocation.."/Source/"..projectName.."/Includes",
				ProjectRootLocation.."/Engine/Source/Base/Includes",
				ProjectRootLocation.."/Engine/Source/Utilities/Includes",
				ProjectRootLocation.."/Engine/Source/GraphicsEngine/Includes",
				ProjectRootLocation.."/Engine/Source/GameBase/Includes",
				ProjectRootLocation.."/Engine/extern/Include/VisualLeakDetector",
			}
			libdirs 
			{
				ProjectRootLocation.."/Engine/extern/Lib/VisualLeakDetector/Win32"
			}
			 pchsource (inputPath.."src/stdafx.cpp")
			 defines("EDITOR_EXPORTS")
			 configuration ("Debug")
				links
				{ 
					projectName.."_d",
					"Base_d",
					"Utilities_d",
					"Graphics_d",
					"Gamebase_d" 
				}
			 configuration ("Release")
				links 
				{ 
					projectName,
					"Base",
					"Utilities",
					"Graphics",
					"Gamebase"
				}
	end

	require("project_validation")

	if _OPTIONS["no_engine_project"] == nil then
		for name, _ in pairs(libsConfig) do
			local libDef = libsDB[name]
			assert(libDef ~= nil, 'Unknown library name: ' .. name )
			local libPlatformInfo = libDef[platform]
			assert(libPlatformInfo ~= nil, 'Unknown platform for library name: ' .. name..", "..platform)
			local releaseStaticPath, debugStaticPath, projectPath, premakescript = unpack(libPlatformInfo)
			if(premakescript ~= nil) then
				require(premakescript).run(engineLocation, outputDirectory)
			end
		end
	end
	
	if _OPTIONS.rebuild then
		print ("rebuild")
		local msbuildPath = ""
		if platform == "VS2010" then
			msbuildPath = "%SystemRoot%/Microsoft.NET/Framework/v4.0.30319/MSBuild.exe"
		elseif platform == "VS2013" then
			msbuildPath = "\"C:/Program Files (x86)/MSBuild/12.0/Bin/MSBuild.exe\""
		end
		local rebuildCmdline = msbuildPath.." "..solutionLocation.."/"..projectName..".sln"
		local rebuildDebugCmdline = rebuildCmdline.." /p:Configuration=Debug"
		local rebuildReleaseCmdline = rebuildCmdline.." /p:Configuration=Release"
		print (rebuildDebugCmdline )
		os.execute(rebuildDebugCmdline )
		print (rebuildReleaseCmdline)
		os.execute(rebuildReleaseCmdline)
		-- local setCl = "SET CL="
		-- for name, _ in pairs(libsConfig) do
			-- local libDef = libsDB[name]
			-- assert(libDef ~= nil, 'Unable to rebuild, unknown library name: ' .. name )
			-- local libPlatformInfo = libDef["VS2010"]
			-- assert(libPlatformInfo ~= nil, 'Unable to rebuild, unknown library name: ' .. name..", VS2010" )
			-- local releaseStaticPath, debugStaticPath, projectPath, premakescript = unpack(libPlatformInfo)
			-- if(projectPath ~= nil) then
				-- local rebuildDebugCmdline   = setCl .. ' && ' .. msbuildPath .. ' "' .. projectPath

				-- -- local rebuildDebugCmdline   = setCl .. ' && ' .. msbuildPath .. ' "' .. projectPath .. '" /p:Configuration=Debug   /property:OutDir="' .. path.getabsolute(debugLibDir)   .. '/"'
				-- print(rebuildDebugCmdline)
			-- end
		-- end
	end

 