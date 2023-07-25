# Variables for generating version.h

set(PRODUCT_NAME "ECWolf")
set(PRODUCT_IDENTIFIER "org.ecwolf.ECWolf")
if(APPLE OR WIN32)
	set(PRODUCT_DIRECTORY "${PRODUCT_NAME}")
else()
	string(TOLOWER "${PRODUCT_NAME}" PRODUCT_DIRECTORY)
endif()

string(TOLOWER "${PRODUCT_NAME}" ENGINE_BINARY_NAME)

set(VERSION_MAJOR 1)
set(VERSION_MINOR 5)
set(VERSION_PATCH 0)
set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}pre")
