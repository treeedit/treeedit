find_package(Qt6 REQUIRED COMPONENTS Core Gui LinguistTools Sql Test Widgets)

add_library(treeedit-core
  treedelegate.h treedelegate.cpp
  treefile.h treefile.cpp
  treeitem.h treeitem.cpp
  treemodel.h treemodel.cpp
)
target_link_libraries(treeedit-core Qt6::Core Qt6::Gui Qt6::Sql Qt6::Widgets)

set(MACOSX_BUNDLE_ICON_FILE treeedit.icns)
set(MACOSX_APP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/treeedit.icns)
set_source_files_properties(${MACOSX_APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

add_executable(treeedit
  MACOSX_BUNDLE
  main.cpp
  mainwindow.h mainwindow.cpp
  mainwindow.qrc
  mainwindow.ui
  ${MACOSX_APP_ICON}
)

target_link_libraries(treeedit treeedit-core Qt6::Core Qt6::Gui Qt6::Sql Qt6::Test Qt6::Widgets)

set_target_properties(treeedit PROPERTIES
  MACOSX_BUNDLE_GUI_IDENTIFIER org.treeedit.app
  MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
  MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
  OUTPUT_NAME "Tree Edit"
  MACOSX_BUNDLE TRUE
  WIN32_EXECUTABLE TRUE
)
