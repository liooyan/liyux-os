include config/makefile-command
include config/makefile-config

INIT_SRC := $(patsubst ./%, $(OBJECT_DIR)/%_x86_64.o, $(call SCAN_SRC,./init))
KERNEL_SRC := $(patsubst ./%, $(OBJECT_DIR)/%_x86_64.o, $(call SCAN_SRC,./kernel))
lib_SRC := $(patsubst ./%, $(OBJECT_DIR)/%_x86_64.o, $(call SCAN_SRC,./lib))

SET_UP_SRC :=  $(patsubst ./%, $(OBJECT_DIR)/%_i686.o, $(call SCAN_SRC,./${CORE_BASH_PATH}/boot))
SET_UP_LIB_SRC :=  $(patsubst ./%, $(OBJECT_DIR)/%_i686.o, $(call SCAN_SRC,./arch/lib))


BOOT_SRC := $(patsubst ./%, $(OBJECT_DIR)/%_x86_64.o, $(call SCAN_SRC,./boot))
MM_SRC := $(patsubst ./%, $(OBJECT_DIR)/%_x86_64.o, $(call SCAN_SRC,./mm))
#内核的依赖
INCLUDES := $(patsubst %, -I%, $(INCLUDES_DIR))

#setup的依赖
INCLUDES_SHARED:= $(patsubst %, -I%, $(INCLUDES_SHARED_DIR))
INCLUDES_SETUP := $(patsubst %, -I%, $(INCLUDES_SETUP_DIR))


linux :clean $(PROJECT_NAME)

all: clean $(TARGET_DIR)/boot.elf



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
	$(CC_i686) $(INCLUDES_SHARED)   $(INCLUDES_SETUP)   -c $< -o $@ $(CFLAGS)


$(OBJECT_DIR)/%.c_i686.o: %.c
	@mkdir -p $(@D)
	$(CC_i686) $(INCLUDES_SHARED)   $(INCLUDES_SETUP)    -c $< -o $@ $(CFLAGS)
# 编译setup文件
$(TARGET_DIR)/setup.elf :$(OBJECT_DIR) $(TARGET_DIR) $(ISO_GRUB_DIR) $(SET_UP_SRC) $(SET_UP_LIB_SRC)
	@echo " LINK: $(TARGET_DIR)/setup.elf"
	$(CC_i686)  -T ${CORE_BASH_PATH}/boot/setup.ld  -o $(TARGET_DIR)/setup.elf $(SET_UP_SRC) $(SET_UP_LIB_SRC) $(LDFLAGS)



$(OBJECT_DIR)/%.S_x86_64.o: %.S
	@mkdir -p $(@D)
	$(CC_x86_64) $(INCLUDES)     -c $< -o $@ $(CFLAGS)


$(OBJECT_DIR)/%.c_x86_64.o: %.c
	@mkdir -p $(@D)
	$(CC_x86_64) $(INCLUDES)   -c $< -o $@ $(CFLAGS)



# 编译内核文件
$(TARGET_DIR)/kernel.elf: $(OBJECT_DIR) $(TARGET_DIR) $(ISO_GRUB_DIR) $(INIT_SRC)  $(KERNEL_SRC)  $(lib_SRC)
	@echo " LINK: $(TARGET_DIR)/kernel.elf"
	$(CC_x86_64)  -T init/kernel.ld -o $(TARGET_DIR)/kernel.elf $(INIT_SRC)  $(KERNEL_SRC)  $(lib_SRC) $(LDFLAGS)


# 编译内核文件
$(TARGET_DIR)/boot.elf: $(BOOT_SRC) $(MM_SRC)
	@echo " LINK: $(TARGET_DIR)/boot.elf"
	$(CC_x86_64)  -T boot/boot.ld -o $(TARGET_DIR)/boot.elf $(BOOT_SRC) $(MM_SRC) $(LDFLAGS) -mcmodel=medium



# 使用grub打包
$(PROJECT_NAME) :  $(TARGET_DIR)/kernel.elf $(TARGET_DIR)/setup.elf $(TARGET_DIR)/boot.elf
	@cp GRUB_TEMPLATE  $(ISO_GRUB_DIR)/grub.cfg
	@cp $(TARGET_DIR)/kernel.elf $(ISO_BOOT_DIR)
	@cp $(TARGET_DIR)/setup.elf $(ISO_BOOT_DIR)
	@cp $(TARGET_DIR)/boot.elf $(ISO_BOOT_DIR)
	@grub-mkrescue -o $(ISO_DIR)/$(PROJECT_NAME).iso $(ISO_DIR)






# 虚拟机运行配置
win-bochs_debug:
	@bochsdbg -q -f ./bochs/bochs_run.cfg



bochs_debug: clean $(PROJECT_NAME)
	@bochs -q -f ./bochs/bochs_debug.cfg

bochs_gdb: clean $(PROJECT_NAME)
	@bochs-gdb -q -f ./bochs/bochs_gdb.cfg

bochs_run: clean $(PROJECT_NAME)
	bochs -q -f ./bochs/bochs_run.cfg

clean :
	@rm -rf $(BUILD_DIR)


qemu-32 : clean $(PROJECT_NAME)
	@qemu-system-i386 -cdrom build/iso/liyux-os.iso 	 -s -S -nographic


qemu-64 : clean $(PROJECT_NAME)
	@qemu-system-x86_64 -cdrom build/iso/liyux-os.iso 	 -s -S

