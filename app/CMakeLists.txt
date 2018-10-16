#Add executables
add_executable(shell-app main.cpp Files.cpp Cleaner.cpp Thresholder.cpp LanesMarker.cpp RegionMaker.cpp)

#Find packages
find_package(OpenCV REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

#Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${OpenCV_INCLUDE_DIRS})

#Link libraries
target_link_libraries(shell-app ${OpenCV_LIBS} ${Boost_LIBRARIES})
