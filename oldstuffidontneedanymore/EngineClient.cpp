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
            //         manager->checkInputs(clientId, map[i]);
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
                // zmq::context_t context(2);
                // zmq::socket_t socket(context, zmq::socket_type::rep);
                // socket.bind("tcp://*:8888");
                // zmq::message_t request;
                // std::cout << "Waiting to receive Client..." << std::endl; // have server tell that client is here
                // socket.recv(request, zmq::recv_flags::none);              // wait to recieve a message from a client
                // // std::unique_lock<std::mutex> cv_lock(*this->_mutex);
                // std::string clientID(static_cast<char *>(request.data()), request.size()); // parse that as a string

                // Send the client's Id to the client
                if (clientID == "-1")                                        // this should be -1
                {                                                            // Compare clientID as a string
                    // std::string sendID = std::to_string(numClients + 1);     // the clients new id (we start at 0)
                    // socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send ID to client
                    // std::cout << "Received New Client" << std::endl;         // have server tell that client is here

                    // map[numClients + 1] = '~';

                    // // // CREATE A NEW CHARACTER
                    // // //  create a new character for the client

                    // // // TEMP
                    // manager->createCharacter();
                    // numClients++;

                    // // put this new character in the array
                    // std::cout << "Client: " << numClients << " has been created" << std::endl;
                }
                else // recieved a num and a char
                {
                    // std::cout << "Client: " << numClients << " pressed a new input" << std::endl;
                    // int id;
                    // char input;
                    // const char *clientID_cstr = clientID.c_str();
                    // sscanf(clientID_cstr, "%d %c", &id, &input);
                    // std::cout << "Id is: " << id << " and input is " << input << std::endl;
                    // map[id] = input; // set the input of the character
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


void sendInput(int clientID, std::string const &str1)
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);
    socket.connect("tcp://localhost:8888");
    std::string strClientID = std::to_string(clientID);
    std::string sendID = strClientID + str1;
    socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send that to server
}

int main()
{
    std::mutex m;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Client Window");

    GameManager *manager = GameManager().getInstance();
    manager->initialize(); // initialize

    //server stuff
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
    std::cout << "Connected as Client " << clientID << std::endl;
    std::string strClientID = std::to_string(clientID);//create string of ID

    ThreadRunner t1(0, NULL, &m, manager);

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

                // Turn hit boxes on or off
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
                {
                    // std::string sendID = strClientID + " H";
                    // socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send that to server
                    sendInput(clientID, " H");
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
                {
                    // std::string sendID = strClientID + " P";
                    // socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send that to server
                    sendInput(clientID, " P");
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
                {
                    // std::string sendID = strClientID + " 1";
                    // socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send that to server
                    sendInput(clientID, " 1");
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
                {
                    // std::string sendID = strClientID + " 2";
                    // socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send that to server
                    sendInput(clientID, " 2");
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
                {
                    // std::string sendID = strClientID + " 3";
                    // socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send that to server
                    sendInput(clientID, " 3");
                }

                // Controls
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                    // Left key is pressed: move our character to the left
                    // std::string sendID = strClientID + " <";
                    // socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send that to server
                    sendInput(clientID, " <");
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    // Right key is pressed: move our character to the right
                    // Left key is pressed: move our character to the left
                    sendInput(clientID, " >");
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    // // Left key is pressed: move our character to the left
                    // std::string sendID = strClientID + " _";
                    // socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send that to server
                    sendInput(clientID, " _");
                }

                window.clear(sf ::Color::White);
                // manager->render(window);
            }
        }
    }
    catch (const zmq::error_t &e)
    {
        std::cerr << "ZeroMQ error: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.num() << std::endl;
        std::cerr << "Error message: " << zmq_strerror(e.num()) << std::endl;
    }
}
