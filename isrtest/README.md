# Stand Alone C examples for SmartFusion

requires ARM Toolchain
https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
tested with Version 9-2019-q4-major

## isrtest - console/ISR example

* simplified build structure compared to SmartConsole example:
  * All build controls in Makefile
* source code is in main.c, CMSIS/,drivers/
* there are two build options
  * Makefile -- For load via JLink
    * generate isrtest.elf,isrtest.hex
  * Makefile.release -- for inclusion in Libero build of design -- cannot load via JLink
    * generate isrtest_release.elf,isrtest_release.hex
  


### to build

```
make clean
make

or
make -f Makefile.release  clean
make -f Makefile.release
```
ISR examples

open terminal session

screen /dev/ttyUSB0 57600
 
press RESET -- terminal should show
```
blinky time
```
commands - press Key
* 1 - configure Timer1
* 2 - stop Timer1
* 3 - Enable Timer1 IRQ
* 4 - Disable Timer2 IRQ
* 5 - send Major Frame Interrupt
* 6 - send Minor Frame Interrupt
* 7 - send both Major and Minor Frame Interrupt
* w - Trigger Watchdog Interrupt (after 5 seconds)

When enabled, Timer1 will blink the LEDS once per second

Major Frame Interrpt will toggle LEDS 0xaa

Minor Frame Interrupt will toggle LEDs 0x55
 
 
## Loading via JLink:
connect terminal session via USB
screen /dev/ttyUSB0 57600

### JLink EDU (full sized)
```
JLinkExe
connect
Device: A2F200M3F
Target Interface: JTAG -- accept default
JTConf - Auto-Detect -- accept default
Speed 4000 kHz  -- accept default
 after connect:
 loadfile <filename>.hex
 setPC 0
 g
 code should now be running
```

### JLink mini EDU
```
JLinkExe
connect
Device: A2F200M3F
Target Interface: SWD 
Speed 4000 kHz  -- accept default
 after connect:
 loadfile <filename>.hex
 setPC 0
 g
 code should now be running
``` 

## using GDB

connect terminal session via USB
screen /dev/ttyUSB0 57600

### full sized EDU
```
JLinkGDBServer -i JTAG -device A2F2000M3F
```
### mini EDU
```
JLinkGDBServer -i SWD -device A2F2000M3F
```
it will connect and listen on port 2331

open another session
```
arm-none-eabi-gdb
tar extended-remote:2331
file <filename>.elf
(respond y - to prompts for loading file and symbols)
load
c
```

the code should be running in your terminal window.
