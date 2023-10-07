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
std::map<int, char> map; // map to keep track of clientID, Iterations
int numClients = 0;
class ClientHandler
{
    GameManager *manager;
    int id;               // an identifier
    ClientHandler *other; // a reference to the "other" thread in this example
    // std::mutex *_mutex;  // the object for mutual exclusion of execution
public:
    ClientHandler(GameManager *manager)
    {
        this->id = 0; // set the id of this thread
        this->manager = manager;

        // this->_mutex = _mutex;
    }

    void run()
    {

        while (true)
        {
            zmq::context_t context(2);
            zmq::socket_t socket(context, zmq::socket_type::rep);
            socket.bind("tcp://*:1235");
            zmq::message_t request;
            std::cout << "Waiting to receive Client..." << std::endl;                  // have server tell that client is here
            socket.recv(request, zmq::recv_flags::none);                               // wait to recieve a message from a client
            std::string clientID(static_cast<char *>(request.data()), request.size()); // parse that as a string

            // Send the client's Id to the client
            if (clientID == "-1") // this should be -1
            {                     // Compare clientID as a string
                std::string sendID = std::to_string(id++);
                socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send ID to client
                std::cout << "Received New Client" << std::endl;         // have server tell that client is here
                map[id] = '~';

                //CREATE A NEW CHARACTER
                 // create a new character for the client
                manager->createCharacter();                                              // put this new character in the array
                numClients++;
                
            }
            else // recieved a num and a char
            {
                int id;
                char input;
                const char* clientID_cstr = clientID.c_str();
                sscanf(clientID_cstr, "%d %c", &id, &input);
                map[id] = input; // set the input of the character
                // std::cout << "Received an odd message...." << std::endl;
            }
        }
    }
};
/**
 * Wrapper function because threads can't take pointers to member functions.
 */
void client_wrapper(ClientHandler *fe)
{
    fe->run();
}
class ThreadRunner
{
    int i;               // an identifier
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

    void run(sf::RenderWindow &window)
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

        else // id == 1
        {

            try
            {
                std::unique_lock<std::mutex> cv_lock(*this->_mutex);
                for (int i = 0; i < numClients; i++)
                {
                    manager->checkInputs(i, map[i]);
                }
            }
            catch (...)
            {
                std::cerr << "Thread " << i << " caught exception." << std::endl;
            }
        }
    }
};

/**
 * Wrapper function because threads can't take pointers to member functions.
 */
void run_wrapper(ThreadRunner *fe, sf::RenderWindow &window)
{
    fe->run(window);
}

int main()
{
    std::mutex m;
    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Engine Window");
    window.setFramerateLimit(60);
    GameManager *manager = GameManager().getInstance();
    manager->initialize(); // initialize

    ThreadRunner t1(0, NULL, &m, manager);
    ThreadRunner t2(1, &t1, &m, manager);

    zmq::context_t context(2);
    zmq::socket_t publisher(context, zmq::socket_type::pub);

    publisher.bind("tcp://*:2346");

    std::cout << "Creating the server..." << std::endl;

    // Create a vector to hold client handler threads
    // std::vector<std::thread> clientThreads;
    ClientHandler t3(manager); // run thread for client searching
    std::thread first(client_wrapper, &t3);

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


        std::thread first(run_wrapper, &t1, std::ref(window));
        std::thread second(run_wrapper, &t2, std::ref(window));

        first.join();
        second.join();

        manager->render(window);
    }

    return 0;
}
