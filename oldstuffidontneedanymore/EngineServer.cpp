/**
 * Recieved help from the ThreadExample in class
 * Also referenced
 * SOCKET HELP ---  https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/hwserver.cpp
 * PUB SUB HELP ---  https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/wuserver.cpp
 */
#include "Actor.hpp"
#include "Object.hpp"
#include "GameObject.hpp"
#include "Timeline.hpp"
#include "GameManager.hpp"
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
std::map<int, char> map; // map to keep track of clientID, input
int numClients = 0;

class ThreadRunner
{
    int i; // an identifier

    ThreadRunner *other; // a reference to the "other" thread in this example
    std::mutex *_mutex;  // the object for mutual exclusion of execution
    GameManager *manager;

public:
    ThreadRunner(int i, ThreadRunner *other, std::mutex *_mutex, GameManager *gamemanager)
    {
        this->i = i; // set the id of this thread

        this->_mutex = _mutex;
        this->manager = gamemanager;
    }

    void run()
    {

        if (i == 0)
        {
            try
            {
                std::unique_lock<std::mutex> cv_lock(*this->_mutex);
                //...

                // compute deltatime from game manager
                manager->updateDeltaTime();

                // process inputs from game manager
            }
            catch (...)
            {
                std::cerr << "Thread " << i << " caught exception." << std::endl;
            }
        }

        else if (i == 1) // id == 1
        {

            // try
            // {
            //     std::unique_lock<std::mutex> cv_lock(*this->_mutex);
            //     // manager->checkInputs(1, map[i]); // check input at clientID with given map input
            //     for (int i = 1; i <= numClients; i++)
            //     {
            //         // TEMP
            //         manager->checkInputs(i, map[i]);
            //         map[i] = '~'; // reset the input so you dont rubberband anywhere
            //     }
            // }
            // catch (...)
            // {
            //     std::cerr << "Thread " << i << " caught exception." << std::endl;
            // }
        }
        else if (i == 2)
        {
            while (true)
            {
                zmq::context_t context(2);
                zmq::socket_t socket(context, zmq::socket_type::rep);
                socket.bind("tcp://*:8888");
                zmq::message_t request;
                std::cout << "Waiting to receive Client..." << std::endl; // have server tell that client is here
                socket.recv(request, zmq::recv_flags::none);              // wait to recieve a message from a client
                // std::unique_lock<std::mutex> cv_lock(*this->_mutex);
                std::string clientID(static_cast<char *>(request.data()), request.size()); // parse that as a string

                // Send the client's Id to the client
                if (clientID == "-1")                                        // this should be -1
                {                                                            // Compare clientID as a string
                    std::string sendID = std::to_string(numClients + 1);     // the clients new id (we start at 0)
                    socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send ID to client
                    std::cout << "Received New Client" << std::endl;         // have server tell that client is here

                    map[numClients + 1] = '~';

                    // // CREATE A NEW CHARACTER
                    // //  create a new character for the client

                    // // TEMP
                    manager->createCharacter();
                    numClients++;

                    // put this new character in the array
                    std::cout << "Client: " << numClients << " has been created" << std::endl;
                }
                else // recieved client info PUBLISH INFOMARION
                {
                    std::cout << "Client: " << numClients << " pressed a new input" << std::endl;
                    int id;
                    char input;
                    const char *clientID_cstr = clientID.c_str();
                    sscanf(clientID_cstr, "%d %c", &id, &input);
                    std::cout << "Id is: " << id << " and input is " << input << std::endl;
                    map[id] = input; // set the input of the character
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
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Engine Window");
    // window.setFramerateLimit(60);
    GameManager *manager = GameManager().getInstance();
    manager->initialize(); // initialize

    zmq::context_t context(2);
    zmq::socket_t publisher(context, zmq::socket_type::pub);

    publisher.bind("tcp://*:6666");

    std::cout << "Creating the server..." << std::endl;
    ThreadRunner t3(2, NULL, &m, manager);
    std::thread third(run_wrapper, &t3); // third thread that constantly runs

    ThreadRunner t1(0, NULL, &m, manager); //dtime stuff
    ThreadRunner t2(1, NULL, &m, manager); //input stuff


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "Close requested" event: we close the window
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        std::thread first(run_wrapper, &t1);
        std::thread second(run_wrapper, &t2);

        first.join();
        second.join();

        //publish all positions and velicites; will have clients render
        std::string allInfo = manager->allToString(manager->actorObjects);
        publisher.send(zmq::buffer(allInfo), zmq::send_flags::none); //tell the clients the positions and velocities
        //maybe have a wait or confirms from all the clients before continuing 


        manager->render(window);
        
        // if(allinfo != "")
        //     std::cout << "info is: \n" << allinfo << std::endl;


    }

    return 0;
}
