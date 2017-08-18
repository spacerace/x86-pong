# x86 pong

This is a text-mode pong game, written in C without using any operating system
or standard-library. 
It will run on bare metal hardware but also under DOS and DOS emulators.

There are floppy loaders around, that load com files directly. One of these was
used to create floppy.img. You can boot any PC with it and run pong without
os or even a hard disk.
See ..... for more details on that floppy loader.

It uses bcc/dev86 as compiler and all IO (keyboard, video) is done via
BIOS-interrupts and inline assembler. See source code for details, it's
quite easy...

Beware of the spaghetti and ugly code...


