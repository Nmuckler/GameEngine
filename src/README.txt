For compiling and running this program, run these commands after the other:

1. make init
2. make clean
3. make
#I usually run/queue step 4 while step 3 is executing
4. ./main

You will want to run EngineClient.cpp in the src folder and
EngineServer.cpp in the Server folder. You will use the instructions above for both.
Create run another EngineClient.cpp to add another client.
I cant create clients while another is building since I use make clean

I will reuse steps 2-4 whenever I want to run the program afterward.

After program is running you can:

control the actor with the left and right arrow keys.

Press P button to pause
Press space bar to jump
Press H button to see hit boxes // this is here for fun
Press 1 for slow speed
Press 2 for medium speed
Press 3 for fast speed


Bugs:
- do not be moving in one client while another is joining, clients will keep their velocity and not update to normal for some reason. I am still resolving this
 (you can still join at any time though!!)
- jumping can mess with velocity when changing time when it's slow,
 but you can still move left and right and change speed with no issie

FOR SECTION 3

Compile each file with 
----------------------------------------------------
g++ -Wall -g -o Server PT3Server.cpp -lzmq -pthread
./Server
----------------------------------------------------
g++ -Wall -g -o Client PT3Client.cpp -lzmq -pthread
./Client
----------------------------------------------------
** While compiling be sure to run ./Server and ./Client as they are compiling to automatically queue the execution




Credit to for image:
https://www.reddit.com/user/manuadvance/