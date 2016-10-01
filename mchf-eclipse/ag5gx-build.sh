###
###  Installing bootloader
###
##
##  1) Close P6 jumber.
##  2) Press and hold BAND+ button
##  3) Press and hold power button
##  4) Release BAND+ button and keep holding power button (during the whole update process).
##
##      $ dfu-util -d 0483:df11 -D bootloader/bootloader.dfu -R -a 0
##
##  5) Release power button when finished.
##  6) Open P6 jumber and done.
##

###
###  Firmware update
###
##
##  0)See bootloader/readme.txt
##


###
###  Compile
###
make OPT_GCC_ARM=/usr/local/gcc-arm-none-eabi-5_4-2016q2 \
     EXTRACFLAGS=-DMCHF_BOARD_0P5 \
     clean all 
