#!/bin/bash 

echo "Borrando Archivos"
rm bootload
rm floppya.img
rm kernel
rm kernel.o
rm kernel_asm.o

echo "Compilando Bootloader"
nasm bootload.asm 

echo "Creando FLoppy Disk"
dd if=/dev/zero of=floppya.img bs=512 count=2880 
dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc seek=0 

echo "Compilando Kernel"
bcc -ansi -c -o kernel.o kernel.c 
as86 kernel.asm -o kernel_asm.o 
ld86 -o kernel -d kernel.o kernel_asm.o 

echo "Cargando Kernel"
dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3 

echo "Procesos Finalizados."
