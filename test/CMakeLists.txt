include_directories(../src)

add_executable(treeedit-test tst_treefile.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core Gui LinguistTools Sql Test Widgets)
target_link_libraries(treeedit-test treeedit-core Qt6::Core Qt6::Gui Qt6::Sql Qt6::Test Qt6::Widgets)

add_test(NAME treeedit-test COMMAND treeedit-test)
