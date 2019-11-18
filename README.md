


*John Taylor*
# PES-Project-5 UART and Ring Buffer
## Introduction
This repository includes a drivers for the KL25Z's UART peripheral that utilize ring buffers for both the transmit and receive operation. The UART can operate in either interrupt or blocking mode. The main application can operate in echo mode or in "character count mode" where the device will keep track of all characters that are entered into the terminal. When the escape key is pressed a report of all of the entered characters is displayed. The repository also includes a screen capture of a UART echo operation as well as a FMEA for the system.
## Observations
### MCUXpresso
The entire semester I've had weird issues and annoyances with MCUXpresso. Here are my biggest pet peeves with the IDE
1. The IDE doesn't automatically end the last debug session when you try to start a new one. Instead it gives you an error that a debug session is already running. This makes trying to quickly test new code a huge pain until you get in the habit of always ending the debug session and waiting 3-4 seconds before starting the new one. This should happen automatically, in my opinion. I know it's possible because Code Composer Studio does it.
2. The memory browser defaults to the wrong endianess for our processor. This makes watching your program edit memory really confusing until you figure out how to change the endianess of the memory browser. What's more is that every time you start a new debug session, the setting resets to the wrong endianess. 
3. A lot of times when I hover over a buffer while debugging, the debugger menu will show the first couple of indexes of the buffer and then show them on repeat for the rest of the buffer. For example:
buffer[0] = 0
buffer[1] = 1
buffer[2] = 2
buffer[0] = 0
buffer[1] = 1
buffer[0] = 0
buffer[1] = 1
Then, if you have the audacity to click on the buffer, the IDE goes wild and switches between the two top indexes until you click somewhere else.

These issues and bugs make concepts harder to understand by obfuscating what's actually happening inside the processor. I think the class should switch to a TI based dev board and use Code Composer Studio. TI's Documentation is better, the TI E2E forum is light years ahead of what NXP has going on and their IDE actually works.
## Build Notes
1. Pull the repo from github
2. Import it into MCUXpresso IDE
3. You should be able to run the project easily since we aren't doing any weird cross compilation
### Modes
To run Test mode the top of mode.h should look like
~~~
#define TEST
//#define DB
//#define ECHO
//#define APPLICATION
//#define INTERRUPT
~~~
To run polling echo mode the top of mode.h should look like
~~~
//#define TEST
//#define DB
#define ECHO
//#define APPLICATION
//#define INTERRUPT
~~~
To run interrupt echo mode the top of mode.h should look like
~~~
//#define TEST
//#define DB
#define ECHO
//#define APPLICATION
#define INTERRUPT
~~~
To run polling application mode the top of mode.h should look like
~~~
//#define TEST
//#define DB
//#define ECHO
#define APPLICATION
//#define INTERRUPT
~~~
To run interrupt application mode the top of mode.h should look like
~~~
//#define TEST
//#define DB
//#define ECHO
#define APPLICATION
#define INTERRUPT
~~~
uncomment DB to run debug alongside any other mode
