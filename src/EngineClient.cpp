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
std::map<int, char> map;

class ThreadRunner
{
    int i; // an identifier

    ThreadRunner *other; // a reference to the "other" thread in this example
    std::mutex *_mutex;  // the object for mutual exclusion of execution
    GameManager *manager;
    int clientID;

public:
    ThreadRunner(int i, ThreadRunner *other, std::mutex *_mutex, GameManager *gamemanager, int ID)
    {
        this->i = i; // set the id of this thread

        this->_mutex = _mutex;
        this->manager = gamemanager;
        this->clientID = ID; // client id
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
                // manager->updateDeltaTime();

                zmq::context_t context(1);
                zmq::socket_t subscriber(context, zmq::socket_type::sub);
                subscriber.setsockopt(ZMQ_CONFLATE, 1);
                // subscriber.set(zmq::sockopt::subscribe, "", 0);
                subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
                subscriber.connect("tcp://localhost:6666");

                zmq::message_t update; // get the update
                subscriber.recv(update, zmq::recv_flags::none);
                std::string updateMessage(static_cast<const char *>(update.data()), update.size());
                // std::cout << "received: \n"
                //           << updateMessage << std::endl;
                manager->parsePos(updateMessage);

                // process inputs from game manager
            }
            catch (...)
            {
                std::cerr << "Thread " << i << " caught exception." << std::endl;
            }
        }
        else if (i == 1) // send this clients info to server and wait for publish
        {
            std::unique_lock<std::mutex> cv_lock(*this->_mutex);
            manager->updateDeltaTime();
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
    sf::RenderWindow window(sf::VideoMode(800, 600), "Client Window");
    window.setFramerateLimit(60);

    sf::View view(sf::Vector2f(100.0f, 300.0f), sf::Vector2f(800.0f, 600.0f));
    // window.setView(view);

    GameManager *manager = GameManager().getInstance();
    manager->initialize(&view); // initialize

    // server stuff
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);

    std::cout << "Connecting to the server..." << std::endl;
    socket.connect("tcp://localhost:8888"); // connect to server thread

    // send -1 to server to let it know it needs an id
    std::string sendID = "-1";                               // Request a new client ID
    socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send that to server

    zmq::message_t reply;                                              // get reply from server
    socket.recv(reply, zmq::recv_flags::none);                         // receive
    int clientID = std::atoi(static_cast<const char *>(reply.data())); // process as an int
    socket.close();

    // NEED TO CREATE CHARACTERS IF ID > 1

    std::cout << "Connected as Client " << clientID << std::endl;
    std::string strClientID = std::to_string(clientID); // create string of ID

    // create clients that were created before it and create new client
    while (manager->numClients < clientID)
    {
        manager->createCharacter(clientID);
    }
    manager->setBounds();

    ThreadRunner t1(0, NULL, &m, manager, clientID); // dtime stuff
    ThreadRunner t2(1, NULL, &m, manager, clientID); // input stuff
    try
    {
        while (window.isOpen())
        {
            zmq::context_t context(1);
            zmq::socket_t socket(context, zmq::socket_type::req);
            socket.connect("tcp://localhost:8888"); // connect to server thread
            zmq::socket_t subscriber(context, zmq::socket_type::sub);
            subscriber.setsockopt(ZMQ_CONFLATE, 1);
            // subscriber.set(zmq::sockopt::subscribe, "", 0);
            subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);

            subscriber.connect("tcp://localhost:6666"); // recieve updates from server

            sf::Event event;
            while (window.pollEvent(event))
            {
                // "Close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                {
                    zmq::socket_t cancel(context, zmq::socket_type::req);
                    cancel.connect("tcp://localhost:8888"); // connect to server thread
                    cancel.send(zmq::buffer(std::to_string(clientID)), zmq::send_flags::none);
                    std::cout << "Disconnecting to the server with ID: " << clientID << std::endl;
                    window.close();
                }
            }

            // wait for threads to finish
            std::thread first(run_wrapper, &t1);
            std::thread second(run_wrapper, &t2);

            if (clientID > 0)
                manager->checkInputs(&window);

            std::string newPos = manager->toString(clientID); // send out position

            socket.send(zmq::buffer(newPos), zmq::send_flags::none); // send client info to server

            first.join();  // receive data
            second.join(); // update delatime
            // manager->updateView();
            manager->render(window);
        }
    }
    catch (const zmq::error_t &e) // testing for zmq errors
    {
        std::cerr << "ZeroMQ error: " << e.what() << std::endl;
        std::cerr << "Error code: " << e.num() << std::endl;
        std::cerr << "Error message: " << zmq_strerror(e.num()) << std::endl;
    }
}
