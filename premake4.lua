solution "SethEngine"
	configurations {"Debug", "Release"}
	location("build/".._ACTION)
	
	project "Demo"
		kind "WindowedApp"
		language "C"
		targetdir "build/bin"
		location("build/".._ACTION .."/demo")
		
		links { "SethEngine" }
		
		files{"demo/**.c", "demo/**.h"}
		includedirs {"src"}
		
		if(_ACTION == "vs2010") then
			buildoptions  { "/MTd", "/DEBUG" }
			links {"sethEngine","Winmm"}
			libdirs { "build/bin" }
		else
			buildoptions { "-std=gnu++0x"}
			links {"sethEngine", "winmm"}
			libdirs { "build/bin" }
		end
        
	project "SethEngine"
		kind "StaticLib"
		language "C"
		
		targetname "sethEngine"
		targetdir "build/bin"
		defines {"PLATFORM_WINDOWS"}
		
		files{"src/**.c", "src/**.h"}
		
		includedirs {"src"}
		libdirs { "build/bin" }
		if(_ACTION == "vs2010") then
			buildoptions  { "/MTd", "/DEBUG" }
		else
			buildoptions { "-std=gnu++0x -fpermissive"}
		end
		
		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
			

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }   	
			
	project "UnitTest"
		kind "ConsoleApp"
		language "C++"
		targetdir "build/bin"
		location("build/".._ACTION .."/tests")
		
		links { "SethEngine" }
		includedirs {"src"}
		defines {"_VARIADIC_MAX=10","PLATFORM_WINDOWS"}
		files{"tests/**.cpp", "tests/**.h"}
		
		includedirs {"lib/gtest-1.6.0/include", "src"}
		
		if(_ACTION == "vs2010") then
			buildoptions  { "/MTd" }
			links {"gtestd", "gtest_maind", "sethEngine"}
			libdirs { "lib/gtest-1.6.0/msvc/gtest/Debug", "build/bin" }
		else
			buildoptions { "-std=gnu++0x"}
			links {"gtest_main", "sethEngine"}
			libdirs { "lib/gtest-1.6.0/make", "build/bin" }
		end
    