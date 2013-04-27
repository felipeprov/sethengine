solution "SethEngine"
	configurations {"Debug", "Release"}
	location("build/".._ACTION)
	
	project "Demo"
		kind "WindowedApp"
		language "C"
		targetdir "build/bin"
		location("build/".._ACTION .."/demo")
		
		links { "SethEngine", "GLFW", "glew32"}
		
		files{"demo/**.c", "demo/**.h"}
		includedirs {"src", "lib/glfw-2.7.7/include"}
		
		if(_ACTION == "vs2010") then
			libdirs {"lib/glfw-2.7.7/support/msvc110/Debug/Win32"}
			buildoptions  { "/MTd", "/DEBUG" }
			links {"sethEngine","Winmm",  "opengl32", "libpng", "zlib"}
			libdirs { "build/bin", "lib/glew-1.9.0/lib"}
		else
			buildoptions { "-std=gnu++0x"}
			links {"sethEngine", "winmm"}
			libdirs { "build/bin" }
		end
        
	project "SethEngine"
		kind "StaticLib"
		language "C++"
		
		targetname "sethEngine"
		targetdir "build/bin"
		defines {"PLATFORM_WINDOWS"}
		
		files{"src/**.c", "src/**.h"}
		
		includedirs {	"src", 
						"lib/glfw-2.7.7/include", 
						"lib/glew-1.9.0/include", 
						"lib/glm-0.9.4.0/glm",
						"lib/libpng-1.5.13", 
						"lib/zlib-1.2.7"}

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
    
     project "libpng"
		kind "StaticLib"
		language "C"
		
		targetname "libpng"
		targetdir "build/bin"
        location("build/".._ACTION .."/libpng")
		defines {"_VARIADIC_MAX=10","PLATFORM_WINDOWS"}
		
		files{"lib/libpng-1.5.13/*.c", "lib/libpng-1.5.13/*.h"}
        excludes {"lib/libpng-1.5.13/pngtest.c"}
        excludes {"lib/libpng-1.5.13/example.c"}
        
        includedirs {"lib/zlib-1.2.5"}
        
        if(_ACTION == "vs2010") then
			buildoptions  { "/MTd", "/DEBUG" }
		else
			--buildoptions { "-std=gnu++0x"}
		end
        
    project "zlib"
		kind "StaticLib"
		language "C"
		
		targetname "zlib"
		targetdir "build/bin"
        location("build/".._ACTION .."/zlib")
		defines {"_VARIADIC_MAX=10","PLATFORM_WINDOWS"}
		
		files{"lib/zlib-1.2.5/*.c", "lib/zlib-1.2.5/*.h"}
        
        if(_ACTION == "vs2010") then
			buildoptions  { "/MTd", "/DEBUG" }
		else
			--buildoptions { "-std=gnu++0x"}
		end