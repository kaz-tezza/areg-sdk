20_pubdatarate_BASE       := $(AREG_EXAMPLES)/20_pubdatarate
20_pubdatarate_OUTPUT_OBJ := $(AREG_OUTPUT_OBJ)/20_pubdatarate

20_pubdatarate_CXXFLAGS   = -I$(20_pubdatarate_BASE) $(examples_CXXFLAGS)
20_pubdatarate_LDFLAGS    = -L $(AREG_OUTPUT_LIB) -Wl,-Bstatic -l$(20_generated_PROJECT_NAME) -Wl,-Bdynamic $(examples_LDFLAGS)

# 'generated' have to be first
include $(20_pubdatarate_BASE)/generated/Makefile
include $(20_pubdatarate_BASE)/pubservice/Makefile
include $(20_pubdatarate_BASE)/pubclient/Makefile

20_pubdatarate: $(AREG_OUTPUT_BIN)/$(20_pubclient_TARGET_BIN) $(AREG_OUTPUT_BIN)/$(20_pubservice_TARGET_BIN)

.PHONY: 20_pubdatarate
