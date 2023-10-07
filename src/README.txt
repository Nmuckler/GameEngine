For compiling and running this program, run these commands after the other:

1. make init
2. make clean
3. make
#I usually run/queue step 4 while step 3 is executing
4. ./main

I will reuse steps 2-4 whenever I want to run the program afterward.

After program is running you can:

control the actor with the left and right arrow keys.
Use the P button to pause
Use the space bar to jump
Use the H button to see hit boxes

Press 1 for slow speed
Press 2 for medium speed
Press 3 for fast speed


FOR SECTION 3

Compile each file with 
g++ -Wall -g -o Server PT3Server.cpp -lzmq -pthread
g++ -Wall -g -o Client PT3Client.cpp -lzmq -pthread

** While compiling be sure to run ./Server and ./Client as they are compiling to automatically queue the execution




Credit to for image:
https://www.reddit.com/user/manuadvance/