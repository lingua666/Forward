
CFLAGS = -c -O2 
	 
MAIN_DIR = .

TARGET_BIN_DIR = $(MAIN_DIR)/bin
TARGET_OBJ_DIR = $(MAIN_DIR)/obj

BIN_NAME  = nkboard

TESTC_DIR = $(MAIN_DIR)/src

INCLUDE_PREFIX = -I

LINK_PREFIX = -L

ALL_PATHS += ./inc
ALL_PATHS += $(LIB_PATH)/usr/include
ALL_PATHS += $(LIB_PATH)/usr/include/sdk_api
ALL_PATHS += $(LIB_PATH)/usr/include/sdk_api/common
ALL_INCLUDES = $(addprefix $(INCLUDE_PREFIX), $(ALL_PATHS))

LIB_DIRS += $(LIB_PATH)/usr/lib
ALL_LINKS += $(addprefix $(LINK_PREFIX), $(LIB_DIRS))

OBJ_CMD = -o 

LD_CMD = -o

TEST_OBJS = $(TARGET_OBJ_DIR)/I2cControl.o \
			$(TARGET_OBJ_DIR)/simcom_common.o \
			$(TARGET_OBJ_DIR)/simcom_test_main.o \
			$(TARGET_OBJ_DIR)/vs1053.o \
			$(TARGET_OBJ_DIR)/RDA5807M.o \
			$(TARGET_OBJ_DIR)/Util.o \
			$(TARGET_OBJ_DIR)/nkboard.o \
			$(TARGET_OBJ_DIR)/main.o

 
LFLAGS +=  -lm -lsdk -lcurl -lcrypto -lssl -lubus -lubox -lrilutil -ljson-c -luci -lpolarssl 

#SIMCOM_TTS_FUNC="TRUE"
ifeq ($(SIMCOM_TTS_FUNC),"TRUE")
	SIMCOM_CFLAGS += -DFEATURE_DEMO_TTS
	LFLAGS += -lhci_curl -lhci_sys -lhci_tts -lhci_tts_local_synth -lacm -lprop2uci -ltinyalsa -llog -laudio-apu -lcutils
endif

CFLAGS +=$(SIMCOM_CFLAGS)

BIN_OBJS = $(TEST_OBJS) 

$(TARGET_OBJ_DIR)/%.o:$(TESTC_DIR)/%.c
	echo ---------------------------------------------------------
	#echo $(ALL_PATHS)
	#echo $(ALL_INCLUDES)
	echo Build OBJECT $(@) from SOURCE $<
	$(CC) $(CFLAGS) $(ALL_INCLUDES) $(OBJ_CMD) $@ $<
	echo ---------------------------------------------------------

$(TARGET_OBJ_DIR)/%.o:$(TESTC_DIR)/%.cpp
	#echo $(ALL_PATHS)
	#echo $(ALL_INCLUDES)
	echo Build OBJECT $(@) from SOURCE $<
	$(CC) $(CFLAGS) $(ALL_INCLUDES) $(OBJ_CMD) $@ $<
	echo ---------------------------------------------------------

.PHONY: all clean

all:prep bin 

prep:
	@if test ! -d $(TARGET_BIN_DIR); then mkdir $(TARGET_BIN_DIR); fi
	@if test ! -d $(TARGET_OBJ_DIR); then mkdir $(TARGET_OBJ_DIR); fi

bin:$(BIN_OBJS)
	@echo Create bin file $(BIN_NAME)
	@$(CXX) $(ALL_LINKS) $(BUILD_LDFLAGS) -o $(TARGET_BIN_DIR)/$(BIN_NAME) $^ $(LFLAGS)
	@echo ---------------------------------------------------------
	
clean:
	@rm -fr $(TARGET_OBJ_DIR) $(TARGET_BIN_DIR) 
	
