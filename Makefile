include config/makefile-command
include config/makefile-config

SOURCE_FILES := $(shell find -name "*.[cS]")
SRC := $(patsubst ./%, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))


INCLUDES := $(patsubst %, -I%, $(INCLUDES_DIR))

all:  $(PROJECT_NAME)



$(OBJECT_DIR):
	@mkdir -p $(OBJECT_DIR)

$(TARGET_DIR):
	@mkdir -p $(TARGET_DIR)

# 编译所有.S 文件
$(OBJECT_DIR)/%.S.o: %.S
	@mkdir -p $(@D)
	@echo " BUILD: $<"
	@$(CC) $(INCLUDES) $(DEBUG)) $(W)  -MMD -c $< -o $@


# 编译所有.c 文件
$(OBJECT_DIR)/%.c.o: %.c
	@mkdir -p $(@D)
	@echo " BUILD: $<"
	@$(CC) $(INCLUDES) $(DEBUG) $(W) -MMD -c $< -o $@ $(CFLAGS)

$(PROJECT_NAME) : clear $(SRC) $(TARGET_DIR)
	$(CC)  $(DEBUG) $(SRC)  $(W) -o $(TARGET_DIR)/$(PROJECT_NAME)
	@objcopy --only-keep-debug $(TARGET_DIR)/$(PROJECT_NAME) $(TARGET_DIR)/$(PROJECT_NAME).debug
# @objcopy --strip-debug $(TARGET_DIR)/$(PROJECT_NAME)


debug_remote : $(PROJECT_NAME)
	gdbserver 0.0.0.0:$(DEBUG_REMOTE_PORT) $(TARGET_DIR)/$(PROJECT_NAME)



clear :
	@rm -rf $(BUILD_DIR)






