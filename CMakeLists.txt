# preciser la version minimale de cmake requise
cmake_minimum_required(VERSION 3.16.3)
# Nom du projet
project(POO LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE=Release)


set(NB_CLASSE_AGE_CMAKE 0 CACHE STRING "An option for setting NB_CLASSE_AGE")

find_package(GSL REQUIRED)

link_directories(${PROJECT_SOURCE_DIR}/../NOMAD_lib/lib)

include_directories(${PROJECT_SOURCE_DIR}/../NOMAD_lib/include/nomad)
include_directories(${PROJECT_SOURCE_DIR}/../NOMAD_lib/include/sgtelib)

include_directories(headers)

if(NOT (NB_CLASSE_AGE_CMAKE EQUAL 1 OR NB_CLASSE_AGE_CMAKE EQUAL 2 OR NB_CLASSE_AGE_CMAKE EQUAL 4 OR NB_CLASSE_AGE_CMAKE EQUAL 8))
    message(FATAL_ERROR "NB_CLASSE_AGE_CMAKE doit être 1, 2, 4 ou 8. ajouter -DNB_CLASSE_AGE_CMAKE=value")
endif()
message("NB_CLASSE_AGE_CMAKE = ${NB_CLASSE_AGE_CMAKE}")

add_compile_definitions(NB_CLASSE_AGE_CMAKE=${NB_CLASSE_AGE_CMAKE})

# On inclut notre bibliothèque dans le processus de CMake
add_subdirectory(sources)
add_subdirectory(config)



# Crée des variables avec les fichiers à compiler
set(MAIN
    main.cpp
    )



# On indique que l'on veut un exécutable "SIRQD" compilé à partir des fichiers décrits par les variables SRCS et HEADERS


set(EXEC_NAMES Main_classe_1 Main_classe_2 Main_classe_4 Main_classe_8)

if(NB_CLASSE_AGE_CMAKE EQUAL 1)
    set(VALUE 0)
elseif(NB_CLASSE_AGE_CMAKE EQUAL 2)
    set(VALUE 1)
elseif(NB_CLASSE_AGE_CMAKE EQUAL 4)
    set(VALUE 2)
elseif(NB_CLASSE_AGE_CMAKE EQUAL 8)
    set(VALUE 3)
else()
    message(FATAL_ERROR "NB_CLASSE_AGE_CMAKE doit être 1, 2, 4 ou 8.")
endif()

list(GET EXEC_NAMES ${VALUE} NAME_EXEC)

add_executable(${NAME_EXEC} ${MAIN})




#target_compile_options(main PRIVATE -fsanitize=address)
#target_link_options(main PRIVATE -fsanitize=address)
#target_compile_options(main PRIVATE -fsanitize=leak)
#target_link_options(main PRIVATE -fsanitize=leak)
#target_compile_options(main PRIVATE -fsanitize=undefined)
#target_link_options(main PRIVATE -fsanitize=undefined)

target_link_libraries(${NAME_EXEC} PUBLIC sources config_projet GSL::gsl GSL::gslcblas)
target_link_libraries(${NAME_EXEC} PUBLIC nomadAlgos nomadUtils nomadEval sgtelib)


# Et pour que l'exécutable fonctionne, il faut lui indiquer les bibliothèques dont il dépend
#target_link_libraries(main sources1)

#target_compile_options(main PUBLIC -fsanitize=leak -fsanitize=undefined -fsanitize=address -g -Wall -Wextra -pedantic )