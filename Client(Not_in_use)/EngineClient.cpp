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
std::map<int, char> map;
int numClients = 0;
int clientID = -1;

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

        else if (i == 1) // send this clients info to server and wait for publish
        {
            manager->checkInputs(clientID);
        }
        // else if (i == 2)
        // {
        //     while (true)
        //     {
        //     }
        // }
    }
};

/**
 * Wrapper function because threads can't take pointers to member functions.
 */
void run_wrapper(ThreadRunner *fe)
{
    fe->run();
}

// void sendInput(int clientID, std::string const &str1)
// {
//     zmq::context_t context(1);
//     zmq::socket_t socket(context, zmq::socket_type::req);
//     socket.connect("tcp://localhost:8888");
//     std::string strClientID = std::to_string(clientID);
//     std::string sendID = strClientID + str1;
//     socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send that to server
// }

int main()
{
    std::mutex m;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Client Window");

    GameManager *manager = GameManager().getInstance();
    manager->initialize(); // initialize

    // server stuff
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);
    zmq::socket_t subscriber(context, zmq::socket_type::sub);

    std::cout << "Connecting to the server..." << std::endl;
    socket.connect("tcp://localhost:8888");     // connect to server thread
    subscriber.connect("tcp://localhost:6666"); // recieve updates from server

    std::string sendID = "-1";                               // Request a new client ID
    socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send that to server

    zmq::message_t reply;                                              // get reply from server
    socket.recv(reply, zmq::recv_flags::none);                         // receive
    int clientID = std::atoi(static_cast<const char *>(reply.data())); // process as an int

    // NEED TO CREATE CHARACTERS IF ID > 1

    std::cout << "Connected as Client " << clientID << std::endl;
    std::string strClientID = std::to_string(clientID); // create string of ID

    // create clients that were created before it and create new client
    while (numClients < clientID)
    {
        manager->createCharacter();
        numClients++;
    }

    ThreadRunner t1(0, NULL, &m, manager); // dtime stuff
    ThreadRunner t2(1, NULL, &m, manager); // input stuff
    // ThreadRunner t1; // Create a thread
    // std::thread first(run_wrapper, &t1);

    try
    {
        while (window.isOpen())
        // while(true)
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

            socket.send(zmq::buffer(manager->toString(manager->actorObjects[clientID])), zmq::send_flags::none); //send client info to server

            



            manager->render(window);
        }
    }
    catch (const zmq::error_t &e)
    {
        std::cerr << "ZeroMQ error: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.num() << std::endl;
        std::cerr << "Error message: " << zmq_strerror(e.num()) << std::endl;
    }
}
