local function addfiles(inputpath)
  files
	{ 
		inputPath.."**.h", 
		inputPath.."**.hxx", 
		inputPath.."**.hpp", 
		inputPath.."**.cpp", 
	}
end

local f
local function CreateDirs(filePath, newPathPrefix)
	local pathName = path.rebase(filePath, os.getcwd(), "../../");
	os.mkdir(newPathPrefix..pathName)
	--f:write("Dir : "..filePath.." : "..pathName.."\n")
end

local function CopyFiles(filePath, newPathPrefix)
	local pathName = path.rebase(filePath, os.getcwd(), "../../");
	os.copyfile(filePath, newPathPrefix..pathName)
	--f:write("File : "..filePath.." : "..pathName.."\n")
end

local function CreateProjectForGithub(oldLocation, dirPath, includeEngineSource)

	for _, fname in ipairs(os.matchdirs(dirPath.."/**")) do
		if string.match(fname, ".git") == nil then
			os.rmdir(fname)
		end
	end
	os.mkdir(dirPath)
	--f = io.open("fileList.txt", "w")
	for _, fname in ipairs(os.matchdirs(oldLocation.."/**")) do
		relativePath = string.sub(fname, string.len(oldLocation) + 2)
		if string.match(relativePath, "Engine") ~= nil then
			if string.match(relativePath, "Engine/Doc") ~= nil then
				if string.match(relativePath, "PublicDoc") ~= nil then
					CreateDirs(fname, dirPath.."/")
				end
			elseif string.match(relativePath, "Engine/Source/") ~= nil then
				if includeEngineSource or string.match(relativePath, "Includes") ~= nil then
					CreateDirs(fname, dirPath.."/")
				end
			elseif string.match(relativePath, "Engine/extern/") ~= nil then
				if includeEngineSource or string.match(relativePath, "VisualLeakDetector") ~= nil then
					CreateDirs(fname, dirPath.."/")
				end
			elseif string.match(relativePath, "Engine/Tests") ~= nil then
				if string.match(relativePath, "Obj") == nil or string.find(relativePath, "Projects") == nil then
					CreateDirs(fname, dirPath.."/")
				end
			elseif includeEngineSource or string.match(relativePath, "Engine/Scripts") == nil then
				CreateDirs(fname, dirPath.."/")
			end
		elseif string.match(relativePath, "Tools") ~= nil then
			if string.match(relativePath, "Tools/CppCheck") ~= nil or string.match(relativePath, "Tools/cpplint") ~= nil 
			or string.match(relativePath, "Tools/Scripts") ~= nil then
				CreateDirs(fname, dirPath.."/")
			end
		elseif string.match(relativePath, "Editor") ~= nil then
			if string.match(relativePath, "obj") == nil then
				CreateDirs(fname, dirPath.."/")
			end
		elseif string.match(relativePath, "Obj") == nil and string.match(relativePath, "Scripts") == nil 
			and string.match(fname, "/.git") == nil and string.match(relativePath, "Projects")== nil then
			CreateDirs(fname, dirPath.."/")
		end
	end

	for _, fname in ipairs(os.matchfiles(oldLocation.."/**")) do
		relativePath = string.sub(fname, string.len(oldLocation) + 2)
		if string.match(relativePath, "Debug") ~= nil or string.match(relativePath, "bin") ~= nil then
			if string.match(relativePath, "lib") ~= nil or string.match(relativePath, "dll") ~= nil 
			or string.match(relativePath, "ini") ~= nil or string.match(relativePath, "xsl") ~= nil 
			or string.match(relativePath, "html") ~= nil or string.match(relativePath, "manifest") ~= nil
			or string.match(relativePath, ".exe") ~= nil then
				CopyFiles(fname, dirPath.."/")
			end
		elseif string.match(relativePath, "Engine") ~= nil then
			if string.match(relativePath, "Debug") ~= nil or string.match(relativePath, "bin") ~= nil then
				if string.match(relativePath, "lib") ~= nil or string.match(relativePath, "dll") ~= nil 
				or string.match(relativePath, "ini") ~= nil or string.match(relativePath, "xsl") ~= nil 
				or string.match(relativePath, "html") ~= nil or string.match(relativePath, "manifest") ~= nil 
				or string.match(relativePath, ".exe") ~= nil then
					CopyFiles(fname, dirPath.."/")
				end
			elseif string.match(relativePath, "md") == nil and string.match(relativePath, ".git") == nil then
				CopyFiles(fname, dirPath.."/")
			end
		elseif string.match(relativePath, "patch") == nil and string.match(relativePath, ".git") == nil then
			CopyFiles(fname, dirPath.."/")
		end
	end
	
	-- for _, fname in ipairs(os.matchfiles(oldLocation.."/bin/**")) do
		-- if string.match(fname, "lib") ~= nil or string.match(fname, "dll") ~= nil 
		-- or string.match(fname, "ini") ~= nil or string.match(fname, "xsl") ~= nil 
		-- or string.match(fname, "html") ~= nil then
			-- CopyFiles(fname, dirPath.."/")
		-- end
	-- end
	
	-- for _, fname in ipairs(os.matchfiles(oldLocation.."/Debug/**")) do
		-- if string.match(fname, "lib") ~= nil or string.match(fname, "dll") ~= nil 
		-- or string.match(fname, "ini") ~= nil or string.match(fname, "manifest") ~= nil 
		-- or string.match(fname, "xsl") ~= nil or string.match(fname, "html") ~= nil then
			-- CopyFiles(fname, dirPath.."/")
		-- end
	-- end
	--f:close()
end

return {
  addfiles = addfiles,
  CreateProjectForGithub = CreateProjectForGithub
}