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
float platX = 600;
float platY = 550;

void hoverY(int y1, int y2, float speed)
{
    // Make sure x1 is the lower value
    int newy1 = std::min(y1, y2);
    int newy2 = std::max(y1, y2);
    float newSpeed = std::abs(speed);

    // True if moving up
    if (moveY)
    {
        if (platY <= newy1)
        { // Position is lower than the lower bound
            moveY = !moveY;
        }
        else
        {
            platY -= newSpeed;
        }
    }
    else
    {
        if (platY >= newy2)
        { // Position is higher than the higher bound
            moveY = !moveY;
        }
        else
        {
            platY += newSpeed;
        }
    }
}

std::map<int, std::string> map; // map to keep track of clientID, input
int numClients = 0;
bool ready = false;

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
                if (input == "-1")                                           // this should be -1
                {                                                            // Compare clientID as a string
                    std::string sendID = std::to_string(numClients + 1);     // the clients new id (we start at 0)
                    socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send ID to client
                    std::cout << "Received New Client" << std::endl;         // have server tell that client is here

                    map[numClients + 1] = "";


                    // // TEMP
                    numClients++;

                    // put this new character in the array
                    std::cout << "Client: " << numClients << " has been created" << std::endl;
                }
                else // recieved client info PUBLISH INFOMARION
                {
                    int id;
                    // char input;
                    const char *input_cstr = input.c_str();
                    sscanf(input_cstr, "%d", &id);
                    // std::cout << "Id is: " << id << std::endl;
                    map[id] = input; // set the input of the character
                    if (id == 1)
                    {
                        hoverY(200, 550, .5);
                    }
                }
            }
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

    publisher.bind("tcp://*:6666");

    std::cout << "Creating the server..." << std::endl;

    ThreadRunner t1(1, NULL, &m);
    std::thread first(run_wrapper, &t1);

    while (true)
    {

        // publish all positions and velicites; will have clients render
        std::string allInfo = "";
        for (int i = 1; i <= numClients; i++)
        {
            if (map[i] != "")
            {
                allInfo += map[i];
            }
        }

        allInfo += "platform" + std::to_string(platX) + "," + std::to_string(platY);
        // std::cout << "Published: \n" << allInfo << std::endl;
        publisher.send(zmq::buffer(allInfo), zmq::send_flags::none);

        // tell the clients the positions and velocities
        // maybe have a wait or confirms from all the clients before continuing
    }

    return 0;
}
