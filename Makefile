include config/makefile-command
include config/makefile-config


SRC :=  $(patsubst ./%, $(OBJECT_DIR)/%_i686.o, $(call SCAN_SRC,./src))


#内核的依赖
INCLUDES := $(patsubst %, -I%, $(INCLUDES_DIR))



linux :clean $(PROJECT_NAME)

all: clean $(TARGET_DIR)/setup.elf



## 所有o文件存放路径
$(OBJECT_DIR):
	@mkdir -p $(OBJECT_DIR)


## 所有 link后文件存放路径
$(TARGET_DIR):
	@mkdir  -p $(TARGET_DIR)


$(ISO_GRUB_DIR):
	@mkdir -p $(ISO_GRUB_DIR)


$(OBJECT_DIR)/%.S_i686.o: %.S
	@mkdir -p $(@D)
	$(CC_i686) $(INCLUDES)     -c $< -o $@ $(CFLAGS)


$(OBJECT_DIR)/%.c_i686.o: %.c
	@mkdir -p $(@D)
	$(CC_i686) $(INCLUDES)     -c $< -o $@ $(CFLAGS)
# 编译setup文件
$(TARGET_DIR)/setup.elf :$(OBJECT_DIR) $(TARGET_DIR) $(ISO_GRUB_DIR) $(SRC)
	@echo " LINK: $(TARGET_DIR)/setup.elf"
	$(CC_i686)  -T ./src/setup.ld  -o $(TARGET_DIR)/setup.elf $(SRC)  $(LDFLAGS)




# 使用grub打包
$(PROJECT_NAME) :  $(TARGET_DIR)/setup.elf
	@cp GRUB_TEMPLATE  $(ISO_GRUB_DIR)/grub.cfg
	@cp $(TARGET_DIR)/setup.elf $(ISO_BOOT_DIR)
	@grub-mkrescue -o $(ISO_DIR)/$(PROJECT_NAME).iso $(ISO_DIR)



clean :
	@rm -rf $(BUILD_DIR)


qemu-32 : clean $(PROJECT_NAME)
	@qemu-system-i386 -cdrom build/iso/liyux-os.iso -s -S



