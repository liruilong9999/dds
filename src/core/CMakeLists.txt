# add_subdirectory(LBaseCommon)
# set_property(TARGET LBaseCommon PROPERTY FOLDER "Core")

# add_subdirectory(LBaseGrap)
# set_property(TARGET LBaseGrap PROPERTY FOLDER "Core")

# add_subdirectory(LUserDialog)
# set_property(TARGET LUserDialog PROPERTY FOLDER "Core")

# add_subdirectory(LWebView)
# set_property(TARGET LWebView PROPERTY FOLDER "Core")

add_subdirectory(common)
set_property(TARGET common PROPERTY FOLDER "Core")

add_subdirectory(lbase)
set_property(TARGET lbase PROPERTY FOLDER "Core")

add_subdirectory(pluginmanager)
set_property(TARGET pluginmanager PROPERTY FOLDER "Core")
