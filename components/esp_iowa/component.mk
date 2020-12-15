
COMPONENTS := esp_iowa

COMPONENT_SRCDIRS :=    esp32_hal \
                  	iowa_code/iowa/src \
                  	esp32_hal/ \
                  	iowa_code/iowa/src/coap/ \
                  	iowa_code/iowa/src/comm/ \
                  	iowa_code/iowa/src/core/ \
                  	iowa_code/iowa/src/data/ \
                  	iowa_code/iowa/src/logger/ \
                  	iowa_code/iowa/src/lwm2m/ \
                  	iowa_code/iowa/src/misc/ \
                  	iowa_code/iowa/src/oscore/ \
                  	iowa_code/iowa/src/security/ \
                  	iowa_code/iowa/src/objects/


COMPONENT_ADD_INCLUDEDIRS := iowa_code/iowa/include \
                  	iowa_code/iowa/include/objects \

COMPONENT_PRIV_INCLUDEDIRS := 	esp32_hal \
                  	iowa_code/iowa/src/coap/ \
                  	iowa_code/iowa/src/comm/ \
                  	iowa_code/iowa/src/core/ \
                  	iowa_code/iowa/src/data/ \
                  	iowa_code/iowa/src/logger/ \
                  	iowa_code/iowa/src/lwm2m/ \
                  	iowa_code/iowa/src/misc/ \
                  	iowa_code/iowa/src/oscore/ \
                  	iowa_code/iowa/src/security/ \
                  	iowa_code/iowa/src/objects/

COMPONENT_REQUIRES := lwip

CFLAGS += -Wno-implicit-fallthrough

#COMPONENT_ADD_INCLUDEDIRS := $(COMPONENT_SRCDIRS) .
