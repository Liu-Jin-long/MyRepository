KERNELDIR := /home/liu/imx6ull/linux-imx-rel_imx_4.1.15_2.1.0_ga
CURRENT_PATH := $(shell pwd)
obj-m := LED_REGISTER.o

tmp_name := $(patsubst %.o,%,$(obj-m))
build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
	@echo "复制设备驱动文件到开发板根文件系统"
	@cp -v ${tmp_name}.ko /home/liu/imx6ull/nfs/rootfs/lib/modules/4.1.15/
clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
rebuild: clean build

app:
	arm-linux-gnueabihf-gcc ${tmp_name}_app.c -o ${tmp_name}_app
	@echo "复制应用文件到开发板根文件系统"
	@cp -v ${tmp_name}_app /home/liu/imx6ull/nfs/rootfs/lib/modules/4.1.15/
appclean:
	rm ${tmp_name}_app
	
cleanall:clean appclean