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

class ThreadRunner
{
    int id;              // an identifier
    ThreadRunner *other; // a reference to the "other" thread in this example
    // std::mutex *_mutex;  // the object for mutual exclusion of execution
public:
    ThreadRunner()
    {
        this->id = 0; // set the id of this thread

        // this->_mutex = _mutex;
    }

    void run()
    {

        while (true)
        {
            // // Turn hit boxes on or off
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
            // {
            // }
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            // {
            // }
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            // {
            //     printf("one pressed");
            // }
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            // {
            //     printf("two pressed");
            // }
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            // {
            //     printf("three pressed");
            // }

            // // Controls
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            // {
            //     // Left key is pressed: move our character to the left
            // }
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            // {
            //     // Right key is pressed: move our character to the right
            // }
            // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            // {
            // }
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
// Timeline characterTimeline = Timeline(&timeline, 1); //WILL NEED TO CREATE ARRAY OF TIMELINES

// sf::CircleShape newActorCircle(35.f);

// sf::RectangleShape line(sf::Vector2f(400.f, 500.f));
// sf::RectangleShape movingLine(sf::Vector2f(250.f, 30.f));

// Actor character = Actor(newActorCircle, sf::Vector2f(100, 500), "hamster.png");
// GameObject _floor = GameObject(line, sf::Vector2f(0, 550), "");
// GameObject platform = GameObject(movingLine, sf::Vector2f(600, 550), "");

int main()
{
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

    // ThreadRunner t1; // Create a thread
    // std::thread first(run_wrapper, &t1);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Client Window");
    window.setFramerateLimit(60);
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
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            {
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
            {
                printf("one pressed");
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
            {
                printf("two pressed");
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
            {
                printf("three pressed");
            }

            // Controls
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                // Left key is pressed: move our character to the left
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                // Right key is pressed: move our character to the right
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
            }
        }
    }
}
