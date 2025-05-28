get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(TARGET_NAME ${PROJECT_NAME})
message("projectname: ${PROJECT_NAME}")

file(GLOB_RECURSE CPP_HEAD_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE CPP_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

message("source dir: ${CMAKE_SOURCE_DIR}")
source_group(
    TREE ${CMAKE_CURRENT_LIST_DIR} 
    FILES ${CPP_HEAD_FILES} ${CPP_SOURCE_FILES}
)

add_executable(${TARGET_NAME} ${CPP_SOURCE_FILES} ${CPP_HEAD_FILES})
set_target_properties(${TARGET_NAME} PROPERTIES FOLDER Apps)

find_package(OpenCV REQUIRED)
target_include_directories(${TARGET_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PRIVATE ${OpenCV_LIBS})
