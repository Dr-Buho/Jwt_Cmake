file(GLOB INCLUDE_FILES "openssl/*.h" "openssl/*.H")
install(FILES
        ${INCLUDE_FILES}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/openssl)