# -----------------------------------------------------------------------------
# Add module directories
# -----------------------------------------------------------------------------
if (NOT USE_SYSTEM_MALLOC)
    add_subdirectory(bmalloc)
endif ()

add_subdirectory(WTF)

add_subdirectory(JavaScriptCore)

if (WIN32 AND ENABLE_GRAPHICS_CONTEXT_3D)
    add_subdirectory(ThirdParty/ANGLE)
endif ()

if (ENABLE_API_TESTS)
    add_subdirectory(ThirdParty/gtest)
endif ()

if (USE_WOFF2)
    add_subdirectory(ThirdParty/brotli)
    add_subdirectory(ThirdParty/woff2)
endif ()

if (USE_XDGMIME)
    add_subdirectory(ThirdParty/xdgmime)
endif ()

if (ENABLE_WEBCORE)
    add_subdirectory(WebCore)
endif ()

if (ENABLE_WEBKIT_LEGACY)
    add_subdirectory(WebKitLegacy)
endif ()

if (ENABLE_WEBKIT)
    add_subdirectory(WebKit)
endif ()

if (ENABLE_WEBDRIVER)
    add_subdirectory(WebDriver)
endif ()

WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()
