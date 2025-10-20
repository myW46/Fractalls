cmake_minimum_required(VERSION 3.20)
project(Fractall_app)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
# Указываем где искать заголовки
include_directories(include)

# Добавляем ВСЕ исходные файлы
add_executable(Fractall_app
    source/Complex.cpp 
    source/MainWindow.cpp
    source/FractallRender.cpp
    source/AppManager.cpp
    source/WinMain.cpp
 "source/BigDouble.cpp")

target_compile_definitions(Fractall_app PRIVATE UNICODE _UNICODE)
# Для Windows
if(WIN32)
    target_link_libraries(Fractall_app)
    set_target_properties(Fractall_app PROPERTIES
        WIN32_EXECUTABLE ON
        LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:wWinMainCRTStartup"
    )
endif()