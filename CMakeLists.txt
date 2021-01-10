cmake_minimum_required(VERSION 3.15)
project(som_)

set(SDL2_INCLUDE_PATH "C:\\SDL\\SDL2-devel-2.0.5-mingw\\SDL2-2.0.5\\i686-w64-mingw32\\include\\SDL2")
set(SDL2_LIB_PATH "C:\\SDL\\SDL2-devel-2.0.5-mingw\\SDL2-2.0.5\\i686-w64-mingw32\\lib")
set(SDL2_IMAGE_INCLUDE_PATH "C:\\SDL\\SDL2_image-devel-2.0.1-mingw\\SDL2_image-2.0.1\\i686-w64-mingw32\\include\\SDL2")
set(SDL2_IMAGE_LIB_PATH "C:\\SDL\\SDL2_image-devel-2.0.1-mingw\\SDL2_image-2.0.1\\i686-w64-mingw32\\lib")
set(SDL2_MIXER_INCLUDE_PATH "C:\\SDL\\SDL2_mixer-devel-2.0.1-mingw\\SDL2_mixer-2.0.1\\i686-w64-mingw32\\include\\SDL2")
set(SDL2_MIXER_LIB_PATH "C:\\SDL\\SDL2_mixer-devel-2.0.1-mingw\\SDL2_mixer-2.0.1\\i686-w64-mingw32\\lib")
set(SDL2_TTF_INCLUDE_PATH "C:\\SDL\\SDL2_ttf-devel-2.0.14-mingw\\SDL2_ttf-2.0.14\\i686-w64-mingw32\\include\\SDL2")
set(SDL2_TTF_LIB_PATH "C:\\SDL\\SDL2_ttf-devel-2.0.14-mingw\\SDL2_ttf-2.0.14\\i686-w64-mingw32\\lib")

set(CMAKE_CXX_STANDARD 14)

include_directories(${SDL2_INCLUDE_PATH} ${SDL2_IMAGE_INCLUDE_PATH} ${SDL2_MIXER_INCLUDE_PATH} ${SDL2_TTF_INCLUDE_PATH})
link_directories(${SDL2_LIB_PATH} ${SDL2_IMAGE_LIB_PATH} ${SDL2_MIXER_LIB_PATH} ${SDL2_TTF_LIB_PATH})

add_executable(som_ main.cpp include/arreglos.h include/configuracion.h include/CriterioSupervision.h include/distancias.h include/ficherorna.h
        include/gestionadorsom.h include/neurona.h include/NeuronaHex.h include/som.h include/Validacion.h include/valores.h include/Visualizacion.h
        src/configuracion.cpp src/ficherorna.cpp src/gestionadorsom.cpp src/neurona.cpp src/som.cpp src/Validacion.cpp src/Visualizacion.cpp)

target_link_libraries(som_ mingw32 SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf pthread)