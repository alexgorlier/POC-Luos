# POC-Luos
Activate a solenoid with a barometer using Luos

This Proof of concept consist in using Luos in order to reproduce a part of the rocket electronic system I am working on in my association. This Proof of concept will include a barometer used with an SPI driver, a solenoid (also works with every ON/OFF component) and an application that will convert the barometer pressure into an altitude, detect apogee (at the moment I use 32km of altitude as an apogee), and activate a solenoid when apogee is detected. The objective of this system is to determine the altitude reached by the rocket (to have some data to use after the flight) and to initiate a liberation system to free a parachute when needed.
The boards I work on for this project are the NUCLEO-L072RB for the barometer and the application, and an ARDUINO MKRZERO for the solenoid.
Why do I use Luos? To communicate between two boards and to divide my code in what Luos calls services, which basically are small parts of code dedicated to one component only and which can be moved where you want in your system if you want to redesign it or in another system if you use the same component in a different project.
