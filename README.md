# x86 pong 
 
This is a text-mode pong game, written in C without using any operating system 
or standard-library. 
It will run on bare metal hardware but also under DOS and DOS emulators. 
 
There are floppy loaders around, that load com files directly. One of these was 
used to create floppy.img. You can boot any PC with it and run pong without 
os or even a hard disk. 
 
It uses bcc/dev86 as compiler and all IO (keyboard, video) is done via 
BIOS-interrupts and inline assembler. See source code for details, it's 
quite easy... 
 
# Running it 
 
On linux-systems you can use my makefile for starting either qemu or dosbox: 
```
$ make qemu 
$ make dosbox 
``` 
If you don't have 'em installed, here's how to do that on debian (ubuntu): 
```
$ apt-get install dosbox qemu
```
 
# Building it 
 
You'll only need "dev86 / bcc" and "make"
```
$ apt-get install bcc make
``` 
There are different target for make: 
- all 
- copy_to_floppy 
- dosbox 
- qemu 

all builds x86-pong. 
copy_to_floppy updates the pc-booter floppy image with the current pong.com. 
dosbox starts dosbox with my provided dosbox.conf 
qemu starts qemu-system-i386 with the pc-booter-image 

# PC-Booter image
Here's the home of the bootloader on the floppy image: 
https://github.com/alexfru/BootProg
 
![ScreenShot](https://raw.githubusercontent.com/spacerace/x86-pong/master/screenshot.png)



