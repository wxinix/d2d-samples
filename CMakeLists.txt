cmake_minimum_required(VERSION 3.24)
project(d2d_nutshell-samples)

set(CMAKE_CXX_STANDARD 14)

function(AddExecutableForEachCpp SRC_FILES)
	foreach(file_path ${SRC_FILES})
	    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" file_name ${file_path})
	    string(REPLACE ".cpp" "" exe_name ${file_name})
	    add_executable(${exe_name} WIN32 ${file_path})	
	endforeach(file_path ${SRC_FILES})
endfunction()

file(GLOB_RECURSE SRC_FILES src/*.cpp)
AddExecutableForEachCpp("${SRC_FILES}")