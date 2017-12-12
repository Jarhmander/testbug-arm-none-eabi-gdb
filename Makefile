test: all
	arm-none-eabi-gdb -ex "target remote | openocd -f /usr/share/openocd/scripts/board/stm32f4discovery.cfg -c 'gdb_port pipe'" \
		-ex 'mon reset halt' -ex load -ex 'mon reset halt' -ex c -- a.out

all:
	arm-none-eabi-gcc -c -g -mthumb -march=armv7e-m -mfpu=fpv4-sp-d16 -mfloat-abi=hard test.c
	arm-none-eabi-ld  -g -e _reset test.o -Ttext=0x08000040 -Tbss=0x20000000 -section-start=isr_vector=0x08000000

