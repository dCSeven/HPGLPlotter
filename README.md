HPGL Plotter
============

A plotter for the ATMega8

It gets the data via the UART (RS232-like and interrupt driven) and plots it out with some steppers (even interrupt driven).
The processing of the Input is done in the main loop.

It is a project for my school, so it should be finnished in April 2014.

The files outside of Main are for testing purposes (or documentation)

----------------------

###Actual status (short summary)

####Stepper movement

completed

####Serial communication

Hardware-Handshaking is finnished (only needs to be simplified)
Software-Handshaking needs testing

####Input processing

not finnished at all
_(if the project will be continued it will be done)_

**To get a more in depth overview what is to be done, look inside the files. There are lots of FIXME, TODO, XXX tags in the header comment of most files**

If someone needs help reading into the project just open up an issue (if there isn't a better way for this)
