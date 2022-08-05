include config/makefile-command
include config/makefile-config

SOURCE_FILES := $(shell find -name "*.[cS]")
SRC := $(patsubst ./%, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))


INCLUDES := $(patsubst %, -I%, $(INCLUDES_DIR))


linux : clean $(PROJECT_NAME)

all: clean  $(TARGET_DIR)/$(PROJECT_NAME)

run: bochs_run

## 所有o文件存放路径
$(OBJECT_DIR):
	@mkdir -p $(OBJECT_DIR) #


## 所有 link后文件存放路径
$(TARGET_DIR):
	@mkdir  -p $(TARGET_DIR) #




$(ISO_GRUB_DIR):
	@mkdir -p $(ISO_GRUB_DIR) #



# 编译所有.S 文件
$(OBJECT_DIR)/%.S.o: %.S
	@mkdir -p $(@D) #
	@echo " BUILD: $<"
	$(CC) $(INCLUDES) $(DEBUG)    -c $< -o $@


# 编译所有.c 文件
$(OBJECT_DIR)/%.c.o: %.c
	@mkdir -p $(@D) #
	@echo " BUILD: $<"
	$(CC) $(INCLUDES) $(DEBUG)   -c $< -o $@ $(CFLAGS)


# 编译内核文件
$(TARGET_DIR)/$(PROJECT_NAME): $(OBJECT_DIR) $(TARGET_DIR) $(ISO_GRUB_DIR) $(SRC)
	@echo " LINK: $(TARGET_DIR)/$(PROJECT_NAME)"
	$(CC) -T link/linker.ld -o $(TARGET_DIR)/$(PROJECT_NAME).bin $(SRC) $(LDFLAGS)
	objcopy --only-keep-debug $(TARGET_DIR)/$(PROJECT_NAME).bin $(TARGET_DIR)/$(PROJECT_NAME).debug

# 使用grub打包
$(PROJECT_NAME) :  $(TARGET_DIR)/$(PROJECT_NAME)
	@./config-grub.sh ${PROJECT_NAME}  $(ISO_GRUB_DIR)/grub.cfg
	@cp $(TARGET_DIR)/$(PROJECT_NAME).bin $(ISO_BOOT_DIR)
	@grub-mkrescue -o $(ISO_DIR)/$(PROJECT_NAME).iso $(ISO_DIR)


win-bochs_debug:
	@bochsdbg -q -f ./bochs/bochs_run.cfg



bochs_debug: clean $(PROJECT_NAME)
	@bochs -q -f ./bochs/bochs_debug.cfg

bochs_gdb: clean $(PROJECT_NAME)
	@bochs-gdb -q -f ./bochs/bochs_gdb.cfg

bochs_run: clean $(PROJECT_NAME)
	bochs -q -f ./bochs/bochs_run.cfg

clean :
	@rm -rf $(BUILD_DIR) #

qemu :
	@qemu-system-i386 -cdrom build/iso/liyux-os.iso  -S -s $(QEMU_OPTIONS)




