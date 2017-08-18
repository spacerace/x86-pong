all:
	bcc -i -Md -ansi -0  -I -I. -V -W -x -o pong.com pong.c
	ls -al pong.com
	
test:
	make dosbox
	#make copy to floppy
	#make qemu 
	
dosbox:
	dosbox
	
qemu:
	qemu-system-i386 -m 4 -fda floppy.img -net none -boot a -serial stdio 
	
copy_to_floppy:
	sudo mount -o loop floppy.img loop
	sudo cp pong.com loop/STARTUP.BIN
	sudo umount loop
	
