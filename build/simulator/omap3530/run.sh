#!/bin/sh

QEMUCMMOND="qemu-system-arm"
MACHINE="beagle"
#MACHINE="mw71"
NANDIMG="omap3-nand.img"

./create_nandflash.py -i g-bios-th.bin -o ${NANDIMG}
./create_nandflash.py -i g-bios-bh.bin -o ${NANDIMG}

#sudo ${QEMUCMMOND} -M ${MACHINE} -mtdblock ${NANDIMG} -serial stdio -net nic -net tap -sd sd.img
${QEMUCMMOND} -M ${MACHINE} -mtdblock ${NANDIMG} -serial stdio
