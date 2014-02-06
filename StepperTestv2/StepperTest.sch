v 20110115 2
C 40000 40000 0 0 0 title-B.sym
C 43800 48900 1 0 0 2phase-stepper-1.sym
{
T 44000 50700 5 10 0 0 0 0 1
device=2PHASE-STEPPER-4LEADS
T 43800 50500 5 10 1 1 0 0 1
refdes=U?
}
T 41900 48050 8 10 0 0 0 0 1
device=ATtiny25_45_85
T 41900 48250 8 10 0 0 0 0 1
footprint=DIP8
T 41900 48450 8 10 0 0 0 0 1
net=GND:4
T 41900 48650 8 10 0 0 0 0 1
net=Vcc:8
T 41900 48850 8 10 0 0 0 0 1
description=8-bit RISC micro controller (Atmel)
T 41900 49050 8 10 0 0 0 0 1
numslots=0
T 41900 49250 8 10 0 0 0 0 1
author=Purdea Andei <andrei -at- purdea.ro>, inspired by Werner Hoch <werner.hoATgmx.de>
T 41895 47854 8 10 0 0 0 0 1
documentation=http://www.atmel.com/Images/doc2586.pdf
N 44900 47600 44900 48900 4
T 52100 41000 9 10 1 0 0 0 1
TCA 3727 symbol needed
N 45100 46400 45100 48900 4
N 44300 48900 44300 48500 4
N 44300 48500 43500 48500 4
N 44100 48900 44100 48700 4
N 44100 48700 43300 48700 4
C 44900 47800 1 180 0 header20-1.sym
{
T 44700 48050 5 10 0 1 180 0 1
device=HEADER20
T 44300 43700 5 10 1 1 180 0 1
refdes=J?
}
N 45100 46400 44900 46400 4
N 43500 48500 43500 47600 4
N 43300 48700 43300 46400 4
N 43300 46400 43500 46400 4
C 45300 46700 1 0 0 resistor-1.sym
{
T 45600 47100 5 10 0 0 0 0 1
device=RESISTOR
T 45500 47000 5 10 1 1 0 0 1
refdes=R?
}
C 42200 46700 1 0 0 resistor-1.sym
{
T 42500 47100 5 10 0 0 0 0 1
device=RESISTOR
T 42400 47000 5 10 1 1 0 0 1
refdes=R?
}
N 43100 46800 43500 46800 4
N 44900 46800 45300 46800 4
C 46500 46700 1 90 0 gnd-1.sym
C 41900 46900 1 270 0 gnd-1.sym
T 42500 46500 9 10 1 0 0 0 1
3R9
T 45600 46500 9 10 1 0 0 0 1
3R9
C 45200 45500 1 90 0 gnd-1.sym
C 45200 45900 1 90 0 gnd-1.sym
C 43200 46100 1 270 0 gnd-1.sym
C 43200 45700 1 270 0 gnd-1.sym
N 44900 44800 47200 44800 4
T 46200 44500 9 10 1 0 0 0 1
to MCU
T 47400 44700 9 10 1 0 0 0 1
Phase 1 Input
T 47400 43400 9 10 1 0 0 0 1
Phase 2 Input
N 43500 44800 42900 44800 4
N 42900 43300 42900 44800 4
N 42900 43300 46100 43300 4
N 46100 43300 46100 43500 4
N 46100 43500 47200 43500 4
T 45500 42000 9 10 1 0 0 0 1
/Chip Enable
N 43500 45200 42600 45200 4
N 42600 45200 42600 42100 4
N 42600 42100 45000 42100 4
C 45000 42300 1 270 0 vcc-1.sym
N 44900 44000 47200 44000 4
N 44900 44400 47200 44400 4
T 47400 44300 9 10 1 0 0 0 1
Power Control Line 1 Phase 1
T 47400 43900 9 10 1 0 0 0 1
Power Control Line 2 Phase 1
T 47400 43000 9 10 1 0 0 0 1
Power Control Line 1 Phase 2
T 47400 42600 9 10 1 0 0 0 1
Power Control Line 2 Phase 2
N 43500 44400 43100 44400 4
N 43100 44400 43100 43100 4
N 43100 43100 47200 43100 4
N 43500 44000 43300 44000 4
N 43300 44000 43300 42700 4
N 43300 42700 47200 42700 4
C 45100 45000 1 0 0 capacitor-1.sym
{
T 45300 45700 5 10 0 0 0 0 1
device=CAPACITOR
T 45300 45500 5 10 1 1 0 0 1
refdes=C?
T 45300 45900 5 10 0 0 0 0 1
symversion=0.1
}
N 45100 45200 44900 45200 4
C 46300 45100 1 90 0 gnd-1.sym
C 51600 44100 1 0 0 vcc-1.sym
C 52000 43200 1 90 0 diode-1.sym
{
T 51400 43600 5 10 0 0 90 0 1
device=DIODE
T 51500 43500 5 10 1 1 90 0 1
refdes=D?
}
C 52000 43200 1 180 0 5V-plus-1.sym
C 45200 47400 1 270 0 vcc-1.sym
C 43100 47000 1 90 0 vcc-1.sym
N 43100 47200 43500 47200 4
N 44900 47200 45200 47200 4
T 46500 45100 9 10 1 0 0 0 2
For multiple Steppers the oszillators should be synchronized
(by using a simple 25kHz circuit (<1V OFF, >2.6V ON)
T 46400 50200 9 10 1 0 0 0 2
Stepper Speed (30 steps) vs uC-Timer speed: (uC Clk 1 MHz)
10 RPS: 300 steps/s --> uC-Timer speed 300Hz (Prescaler  val needed)
