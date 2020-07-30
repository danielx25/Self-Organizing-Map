cmake_minimum_required(VERSION 3.15)
project(som_)

set(CMAKE_CXX_STANDARD 14)

add_executable(som_ main.cpp include/arreglos.h include/configuracion.h include/CriterioSupervision.h include/distancias.h include/ficherorna.h
        include/gestionadorsom.h include/neurona.h include/NeuronaHex.h include/som.h include/Validacion.h include/valores.h
        src/configuracion.cpp src/ficherorna.cpp src/gestionadorsom.cpp src/neurona.cpp src/som.cpp src/Validacion.cpp)

target_link_libraries(som_ pthread)