# Crée des variables avec les fichiers à compiler
set(SOURCES
    
    Data.cpp
    MCMC.cpp
    optimisation_algo.cpp
    fonction_obj.cpp
    fonction_discret.cpp
    Parametres.cpp
    Nomad_file.cpp
    Stats.cpp
    )

add_library(sources ${SOURCES})
