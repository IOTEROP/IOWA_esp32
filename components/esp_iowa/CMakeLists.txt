
# Work in progress
set (BASE_IOWA "./iowa_code/iowa/")

idf_component_register(

        SRC_DIRS "./esp32_hal"
                "${BASE_IOWA}src/coap"
                "${BASE_IOWA}src/comm"
                "${BASE_IOWA}src/core"
                "${BASE_IOWA}src/data"
                "${BASE_IOWA}src/logger"
                "${BASE_IOWA}src/lwm2m"
                "${BASE_IOWA}src/misc"
                "${BASE_IOWA}src/objects"
                "${BASE_IOWA}src/oscore"
                "${BASE_IOWA}src/security"

        INCLUDE_DIRS
                "${PROJECT_DIR}/main"
                "${BASE_IOWA}include"
                "${BASE_IOWA}include/objects"

        PRIV_INCLUDE_DIRS
                "./esp32_hal"
                "${BASE_IOWA}src/core"
                "${BASE_IOWA}src/coap"
                "${BASE_IOWA}src/comm"
                "${BASE_IOWA}src/data"
                "${BASE_IOWA}src/logger"
                "${BASE_IOWA}src/lwm2m"
                "${BASE_IOWA}src/misc"
                "${BASE_IOWA}src/objects"
                "${BASE_IOWA}src/oscore"
                "${BASE_IOWA}src/security"

        PRIV_REQUIRES
                lwip

	EXCLUDE_SRCS "mbedtls"
 )

#To avoid conflicting with esp mbedtls:
set(EXCLUDE_COMPONENTS "mbedtls")

#Add iowa_config.h to the build
get_filename_component(iowa_config_file  ${PROJECT_DIR}/"iowa_config.h" ABSOLUTE BASE_DIR)

# Temporary suppress "fallthrough" warnings until they are fixed in iowa repo
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-implicit-fallthrough -Wno-maybe-uninitialized)
