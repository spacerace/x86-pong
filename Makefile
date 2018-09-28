all:
	bcc -i -Md -ansi -0 -O -I -I. -V -W -x -o pong.com pong.c
	ls -al pong.com
	
dosbox:
	dosbox
	
qemu: copy_to_floppy
	qemu-system-i386 -m 4 -fda floppy.img -net none -boot a -serial stdio 
	
copy_to_floppy: all
	sudo mount -o loop floppy.img loop
	sudo cp pong.com loop/STARTUP.BIN
	sudo umount loop
	
