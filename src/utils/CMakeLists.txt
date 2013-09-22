add_definitions(-DXMR_UTILS_LIB)
include_directories(. ..)

set(xmrutils_SRC
	utils_export.h
	xlibhelper.h
	xlibhelper.cpp
)

add_library(xmrutils SHARED ${xmrutils_SRC})
if (WIN32)
	set(xmrutils_LIBS
		Kernel32
	)
else()
	set(xmrutils_LIBS
		dl
	)
endif()
target_link_libraries(xmrutils ${xmrutils_LIBS})

set_target_properties(xmrutils
	PROPERTIES VERSION "1.0.0"
	SOVERSION 1
)

if (NOT WIN32)
	install(TARGETS xmrutils LIBRARY DESTINATION ${libdir})
endif()
