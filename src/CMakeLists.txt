macro(add_pcps2_library lib srcs libs)
	add_library(${lib} STATIC ${srcs})
	target_link_libraries(${lib} ${libs}) 
endmacro(add_pcps2_library)

add_subdirectory(common)
add_subdirectory(core)
add_subdirectory(ui/cli)