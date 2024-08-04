KERNELDIR := /home/liu/imx6ull/linux-imx-rel_imx_4.1.15_2.1.0_ga
CURRENT_PATH := $(shell pwd)
obj-m := LED_platform_device.o LED_platform_driver.o 

ko_name := $(patsubst %.o,%.ko,$(obj-m))
app_name := $(patsubst %.o,%,$(obj-m))
app_name := platform_noDTS
build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
	@echo "复制设备驱动文件到开发板根文件系统"
	@cp -v ${ko_name} /home/liu/imx6ull/nfs/rootfs/lib/modules/4.1.15/
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
rebuild: clean build

all: kernel_modules app
app:
	arm-linux-gnueabihf-gcc ${app_name}_app.c -o ${app_name}_app -std=gnu99
	@echo "复制应用文件到开发板根文件系统"
	@cp -v ${app_name}_app /home/liu/imx6ull/nfs/rootfs/lib/modules/4.1.15/
appclean:
	rm ${app_name}_app
	
cleanall:clean appclean