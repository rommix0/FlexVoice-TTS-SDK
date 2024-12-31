FlexVocie Speed Test

This program is used to measure the capability of the FlexVoice Server on 
the supplied machine while simulating the possible usage in a client-server
architecture. 
The program could be fine tuned using lot of parameters to emulate the usage
of FlexVoice in every possible circumstances. Hardware and software specific 
issues has high level if influence in the real world FvSpeesTest.exe makes
it possible to set the level of acceptable client number for every machine.

Usage: [Options] <channels> <requests>

By default the program looks for a file called "FVSpeedTest.par", which containes
all the settings, but in options you can override the settings.

-sp           Speaker file
-sf           Sampling frequency [in Hz]
-br           Bit resolution
-rf           Request frequency [in ms]
-pq           Phoneme queue size [in phoneme count]
-wq           Wave queue size [in phoneme count]
-ib           Initial buffer size [in ms]
-bs           Delay buffer size [in ms]
-spd          Generation speed
-tc           Text count

After the run two log files are going to appear:
1. error1.log containing the dealys in msec.
2. the log for every request, when was it issued, when was the first reply
from the server returned and finishing of the reading.