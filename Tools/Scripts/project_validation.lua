local fw = io.open(solutionLocation.."/postbuild-event.bat", "w")
fw:write("echo off\n\n");

fw:write("set CPPCHECK_TEMPLATE=-j 4 --template=\"{file}({line}): error: ({severity}:{id}) {message}\" --enable=style,warning,performance --inline-suppr -i "..ProjectRootLocation.."/Engine/extern/ --std=c++11\n")
fw:write("set CPPLINT_FILTERS=--filter=+,-legal,-runtime/threadsafe_fn,-runtime/arrays,-runtime/references"..
	",-whitespace/tab,-whitespace/braces,-whitespace/line_length"..
	",-build/include,-build/namespaces,-build/include_alpha,-readability/streams,-runtime/indentation_namespace\n\n"
)

fw:write("cd "..ProjectRootLocation.."\n");
fw:write("\"C:/Program Files/Git/bin/git.exe\" status --porcelain > \""..solutionLocation.."/changed_files.txt".."\"\n");
fw:write("cd "..solutionLocation.."\n");


fw:write("for /F \"tokens=1,2\" %%A in ('FINDSTR \"\\.h\" \""..solutionLocation.."/changed_files.txt".."\"') do (\n");
fw:write("IF NOT \"%%A\" == \"D\" ") 
fw:write("\""..ProjectRootLocation.."/Tools/cppcheck/cppcheck.exe\" %CPPCHECK_TEMPLATE% \""..ProjectRootLocation.."/%%B\"\n")
fw:write(")\n\n");

fw:write("for /F \"tokens=1,2\" %%A in ('FINDSTR \"\\.cpp\" \""..solutionLocation.."/changed_files.txt".."\"') do (\n");
fw:write("IF NOT \"%%A\" == \"D\" ") 
fw:write("\""..ProjectRootLocation.."/Tools/cppcheck/cppcheck.exe\" %CPPCHECK_TEMPLATE% \""..ProjectRootLocation.."/%%B\"\n")
fw:write(")\n\n");

fw:write("for /F \"tokens=1,2\" %%A in ('FINDSTR \"\\.cpp\" \""..solutionLocation.."/changed_files.txt".."\"') do (\n");
fw:write("IF NOT \"%%A\" == \"D\" ") 
fw:write(
	"\""..ProjectRootLocation.."/Tools/cpplint/cpplint.py".."\" ".. 
	"--output=vs7 --extensions=h,cpp,hxx %CPPLINT_FILTERS% "..ProjectRootLocation.."/%%B\n")
fw:write(")\n\n");
	
fw:write("for /F \"tokens=1,2\" %%A in ('FINDSTR \"\\.h\" \""..solutionLocation.."/changed_files.txt".."\"') do (\n");
fw:write("IF NOT \"%%A\" == \"D\" ") 
fw:write(
	"\""..ProjectRootLocation.."/Tools/cpplint/cpplint.py".."\" ".. 
	"--output=vs7 --extensions=h,cpp,hxx %CPPLINT_FILTERS% "..ProjectRootLocation.."/%%B\n")
fw:write(")\n");

--Engine
fw:write("\n\n\n");
fw:write("cd "..ProjectRootLocation.."/Engine\n");
fw:write("\"C:/Program Files/Git/bin/git.exe\" status --porcelain > \""..solutionLocation.."/changed_files.txt".."\"\n");
fw:write("cd "..solutionLocation.."\n");


fw:write("for /F \"tokens=1,2\" %%A in ('FINDSTR \"\\.h\" \""..solutionLocation.."/changed_files.txt".."\"') do (\n");
fw:write("IF NOT \"%%A\" == \"D\" ") 
fw:write("\""..ProjectRootLocation.."/Tools/cppcheck/cppcheck.exe\" %CPPCHECK_TEMPLATE% \""..ProjectRootLocation.."/Engine/%%B\"\n")
fw:write(")\n\n");

fw:write("for /F \"tokens=1,2\" %%A in ('FINDSTR \"\\.cpp\" \""..solutionLocation.."/changed_files.txt".."\"') do (\n");
fw:write("IF NOT \"%%A\" == \"D\" ") 
fw:write("\""..ProjectRootLocation.."/Tools/cppcheck/cppcheck.exe\" %CPPCHECK_TEMPLATE% \""..ProjectRootLocation.."/Engine/%%B\"\n")
fw:write(")\n\n");

fw:write("for /F \"tokens=1,2\" %%A in ('FINDSTR \"\\.cpp\" \""..solutionLocation.."/changed_files.txt".."\"') do (\n");
fw:write("IF NOT \"%%A\" == \"D\" ") 
fw:write(
	"\""..ProjectRootLocation.."/Tools/cpplint/cpplint.py".."\" ".. 
	"--output=vs7 --extensions=h,cpp,hxx %CPPLINT_FILTERS% "..ProjectRootLocation.."/Engine/%%B\n")
fw:write(")\n\n");
	
fw:write("for /F \"tokens=1,2\" %%A in ('FINDSTR \"\\.h\" \""..solutionLocation.."/changed_files.txt".."\"') do (\n");
fw:write("IF NOT \"%%A\" == \"D\" ") 
fw:write(
	"\""..ProjectRootLocation.."/Tools/cpplint/cpplint.py".."\" ".. 
	"--output=vs7 --extensions=h,cpp,hxx %CPPLINT_FILTERS% "..ProjectRootLocation.."/Engine/%%B\n")
fw:write(")\n");

 -- Need this so that batch file doesnt return an error code in post build command vs
fw:write("\nREM\n");

fw:close();
