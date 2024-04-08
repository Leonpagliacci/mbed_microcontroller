#!/bin/sh
BINFILE="BUILD/NUCLEO_F446RE/ARMC6/motor-controller.bin"
#echo $BINFILE 
st-flash --connect-under-reset write ${BINFILE} 0x8000000