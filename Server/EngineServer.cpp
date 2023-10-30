/**
 * Recieved help from the ThreadExample in class
 * Also referenced
 * SOCKET HELP ---  https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/hwserver.cpp
 * PUB SUB HELP ---  https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/wuserver.cpp
 */
#include "../src/Actor.hpp"
#include "../src/Object.hpp"
#include "../src/GameObject.hpp"
#include "../src/Timeline.hpp"
#include "../src/GameManager.hpp"
#include <zmq.hpp>
#include <map>
#include <string>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>

#define sleep(n) Sleep(n)
#endif

bool moveY = true;
float valY = 0;
Timeline timeLine = Timeline();
int64_t currentTime = 0;
int64_t deltatime = 0;

void hoverY(int min, int max, float speed)
{
    // Make sure x1 is the lower value

    float newSpeed = std::abs(speed);

    // True if moving up
    if (moveY)
    {
        if (valY <= min)
        { // Position is lower than the lower bound
            moveY = !moveY;
        }
        else
        {
            valY -= newSpeed;
        }
    }
    else
    {
        if (valY >= max)
        { // Position is higher than the higher bound
            moveY = !moveY;
        }
        else
        {
            // printf("%f is less than %d\n", valY, max);
            valY += newSpeed;
        }
    }
}

std::map<int, std::string> infoMap; // map to keep track of clientID, input
std::map<int, int64_t> timeMap;
int numClients = 0;
int host = 1;
std::vector<int> deltedClientId;
std::string allInfo;
std::string platInfo;


class ThreadRunner
{
    int i; // an identifier

    ThreadRunner *other; // a reference to the "other" thread in this example
    std::mutex *_mutex;  // the object for mutual exclusion of execution

public:
    ThreadRunner(int i, ThreadRunner *other, std::mutex *_mutex)
    {
        this->i = i; // set the id of this thread

        this->_mutex = _mutex;
    }

    void run()
    {

        if (i == 1)
        {
            while (true)
            { // receive direct input
                zmq::context_t context(2);
                zmq::socket_t socket(context, zmq::socket_type::rep);
                socket.bind("tcp://*:8888");
                zmq::message_t request;
                // std::cout << "Waiting to receive Client..." << std::endl; // have server tell that client is here
                socket.recv(request, zmq::recv_flags::none); // wait to recieve a message from a client
                // std::unique_lock<std::mutex> cv_lock(*this->_mutex);
                std::string input(static_cast<char *>(request.data()), request.size()); // parse that as a string
                // Send the client's Id to the client
                const char *input_cstr = input.c_str();
                if (input == "-1")                                           // this should be -1
                {                                                            // Compare clientID as a string
                    std::string sendID = std::to_string(numClients + 1);     // the clients new id (we start at 0)
                    socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send ID to client
                    std::cout << "Received New Client" << std::endl;         // have server tell that client is here

                    infoMap[numClients + 1] = "";

                    // // TEMP
                    numClients++;

                    // put this new character in the array
                    std::cout << "Client: " << numClients << " has been created" << std::endl;
                }
                else if (strlen(input_cstr) == 1 || strlen(input_cstr) == 2)
                {
                    int deleteId;
                    sscanf(input_cstr, "%d", &deleteId);
                    // std::cout << "Received delete message: " << input_cstr << "going to delete client" << deleteId << std::endl; // have server tell that client is here
                    // std::cout << "map was prev: " << map[deleteId] << std::endl; // have server tell that client is here
                    deltedClientId.push_back(deleteId);
                    std::cout << "Client: " << deleteId << " has been deleted" << std::endl;
                }
                else // recieved client info PUBLISH INFOMARION
                {
                    int id;
                    // char input;
                    sscanf(input_cstr, "%d", &id); // scan the first number which is the id
                    // std::cout << "Id is: " << id << std::endl;
                    infoMap[id] = input;              // set the data at that id
                    timeMap[id] = timeLine.getTime(); // set last client report to the current time
                    if (id == host)                   // update the platform whenever the host pings the server; In this case the host is client 1
                    {
                        hoverY(0, 300, .5); //
                    }
                }
            }
        }
        else if (i == 2)
        {
            while (true)
            {
                currentTime = timeLine.getTime();
                for (auto it = timeMap.begin(); it != timeMap.end(); ++it)
                {
                    bool isDeleted = false;
                    for (int i = 0; i < (int)deltedClientId.size(); i++)
                    {
                        if (deltedClientId[i] == it->first)
                            isDeleted = true;
                    }
                    if (!isDeleted)
                    {
                        deltatime = currentTime - it->second;
                        if (deltatime >= 1000 && deltatime <= 3000)
                        {
                            std::cout << "Putting a timeout for: " << it->first << " since the time was: " << deltatime << std::endl;
                            deltedClientId.push_back(it->first);
                        }
                        // else
                        // {
                        //     std::cout << "Not deleting : " << it->first << " since delta time was " << deltaTime << std::endl;
                        // }
                    }
                }
            }
        }
        else if (i == 3)
        {
            while (true)
            {
                for (int i = 0; i < (int)deltedClientId.size(); i++)
                {
                    infoMap[deltedClientId[i]] = "deleted" + std::to_string(deltedClientId[i]) + "|";
                    if (deltedClientId[i] == host)
                    {
                        for (auto it = infoMap.begin(); it != infoMap.end(); ++it)
                        {
                            if (it->second.substr(0, 7) != "deleted")
                            {
                                host = it->first;
                                std::cout << "Setting new host to : " << it->first << std::endl;
                                break;
                            }
                        }
                    }
                }
            }
        }
        else if (i == 4)
        {
            allInfo = "";
            for (int i = 1; i <= numClients; i++)
            {
                if (infoMap[i] != "")
                {
                    allInfo += infoMap[i];
                }
            }

            
        }
        else if (i == 5)
        {
            platInfo = "";
            float newY = 550 - valY;
            float newX = 600 - valY;
            platInfo += "platform1" + std::to_string(600) + "," + std::to_string(newY) + "|";
            platInfo += "platform2" + std::to_string(newX) + "," + std::to_string(200) + "|";
        }
    }
};

/**
 * Wrapper function because threads can't take pointers to member functions.
 */
void run_wrapper(ThreadRunner *fe)
{
    fe->run();
}

int main()
{
    std::mutex m;

    zmq::context_t context(2);
    zmq::socket_t publisher(context, zmq::socket_type::pub);
    zmq::socket_t publisher2(context, zmq::socket_type::pub);

    publisher.bind("tcp://*:6666");
    publisher2.bind("tcp://*:7667");


    std::cout << "Creating the server..." << std::endl;

    ThreadRunner t1(1, NULL, &m);
    std::thread first(run_wrapper, &t1);

    ThreadRunner t2(2, NULL, &m); // check timeouts
    std::thread second(run_wrapper, &t2);

    ThreadRunner t3(3, NULL, &m);
    std::thread third(run_wrapper, &t3);

    ThreadRunner t4(4, NULL, &m);

    ThreadRunner t5(5, NULL, &m);

    while (true)
    {
        std::thread fourth(run_wrapper, &t4);
        std::thread fifth(run_wrapper, &t5);

        fourth.join();
        fifth.join();
        // publish all positions and velicites; will have clients render

        // std::cout << "Published: \n" << allInfo << std::endl;
        publisher.send(zmq::buffer(allInfo), zmq::send_flags::none);

        publisher2.send(zmq::buffer(platInfo), zmq::send_flags::none);


        // tell the clients the positions and velocities
        // maybe have a wait or confirms from all the clients before continuing
    }

    return 0;
}
