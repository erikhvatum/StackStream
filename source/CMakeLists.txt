set(SOURCE_FILES
        main.cpp
        Image.cpp
        Layer.cpp
        LayerRenderer.cpp)
set(LIBRARIES
        ${OPENGL_LIBRARIES}
        ${FREEIMAGE_LIBRARY}
        ${FREEIMAGEPLUS_LIBRARY})

qt5_add_resources(RESOURCES qml.qrc)
add_executable(StackStream ${SOURCE_FILES} ${RESOURCES})
qt5_use_modules(StackStream Core Gui Qml OpenGL Quick Widgets)
target_link_libraries(StackStream ${LIBRARIES})