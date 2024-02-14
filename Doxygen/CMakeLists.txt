find_package(Doxygen)
if(DOXYGEN_FOUND)
    # Configuration de Doxygen
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Demande à CMake de configurer le Doxyfile en remplaçant les variables
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # Ajoute une target custom pour lancer Doxygen chaque fois que le projet est construit
    add_custom_target(doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        EXCLUDE_FROM_ALL
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Génération de la documentation API avec Doxygen"
        VERBATIM)
else(DOXYGEN_FOUND)
    message("Doxygen doit être installé pour générer la documentation Doxygen")
endif(DOXYGEN_FOUND)
