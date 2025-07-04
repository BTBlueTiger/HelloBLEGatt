set(SRCDIR "../src")
set(SERVICEDIR "../src/Service")

set(SOURCES 
    "HELLOBLEGATT.cpp"
    ${SRCDIR}/BLEServer.cpp
    ${SRCDIR}/BLEUtils.cpp

    ${SERVICEDIR}/LedService.cpp
)

idf_component_register(SRCS ${SOURCES}
                    INCLUDE_DIRS "." "../include" "../include/Service" "../Configurations"
                 
                    )