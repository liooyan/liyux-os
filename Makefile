include config/makefile-command
include config/makefile-config

SOURCE_FILES := $(shell find -name "*.[cS]")
SRC := $(patsubst ./%, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))


INCLUDES := $(patsubst %, -I%, $(INCLUDES_DIR))

all:  $(PROJECT_NAME)

run: bochs_run

## 所有o文件存放路径
$(OBJECT_DIR):
	@mkdir -p $(OBJECT_DIR)


## 所有 link后文件存放路径
$(TARGET_DIR):
	@mkdir -p $(TARGET_DIR)




$(ISO_GRUB_DIR):
	@mkdir -p $(ISO_GRUB_DIR)



# 编译所有.S 文件
$(OBJECT_DIR)/%.S.o: %.S
	@mkdir -p $(@D)
	@echo " BUILD: $<"
	@$(CC) $(INCLUDES) $(DEBUG)    -c $< -o $@


# 编译所有.c 文件
$(OBJECT_DIR)/%.c.o: %.c
	@mkdir -p $(@D)
	@echo " BUILD: $<"
	@$(CC) $(INCLUDES) $(DEBUG)   -c $< -o $@ $(CFLAGS)


# 编译内核文件
$(TARGET_DIR)/$(PROJECT_NAME): $(OBJECT_DIR) $(TARGET_DIR) $(ISO_GRUB_DIR) $(SRC)
	@echo " LINK: $(TARGET_DIR)/$(PROJECT_NAME)"
	@$(CC) -T link/linker.ld -o $(TARGET_DIR)/$(PROJECT_NAME).bin $(SRC) $(LDFLAGS)
	@objcopy --only-keep-debug $(TARGET_DIR)/$(PROJECT_NAME).bin $(TARGET_DIR)/$(PROJECT_NAME).debug

# 使用grub打包
$(PROJECT_NAME) :  $(TARGET_DIR)/$(PROJECT_NAME)
	@./config-grub.sh ${PROJECT_NAME}  $(ISO_GRUB_DIR)/grub.cfg
	@cp $(TARGET_DIR)/$(PROJECT_NAME).bin $(ISO_BOOT_DIR)
	@grub-mkrescue -o $(ISO_DIR)/$(PROJECT_NAME).iso $(ISO_DIR)


bochs_debug: clear $(PROJECT_NAME)
	bochs -q -f ./bochs_debug.cfg


bochs_run: clear $(PROJECT_NAME)
	bochs -q -f ./bochs_run.cfg

clear :
	@rm -rf $(BUILD_DIR)






