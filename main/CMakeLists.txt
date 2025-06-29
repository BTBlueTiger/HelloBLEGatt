set(SRCDIR "../src")

set(SOURCES 
    "HELLOBLEGATT.cpp"
    ${SRCDIR}/BLEServer.cpp
    ${SRCDIR}/BLEUtils.cpp
)

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS "." "../include")