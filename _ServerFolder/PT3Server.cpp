/**
 * Recieved help from the ThreadExample in class
 * Also referenced
 * SOCKET HELP ---  https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/hwserver.cpp
 * PUB SUB HELP ---  https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/wuserver.cpp
 */
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
std::map<int, int> map; // map to keep track of clientID, Iterations
int numClients = 0;
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
            zmq::context_t context(2);
            zmq::socket_t socket(context, zmq::socket_type::rep);
            socket.bind("tcp://*:1235");
            zmq::message_t request;
            std::cout << "Waiting to recieve Client..." << std::endl;                  // have server tell that client is here
            socket.recv(request, zmq::recv_flags::none);                               // wait to recieve a message from a client
            std::string clientID(static_cast<char *>(request.data()), request.size()); // parse that as a string

            // Send the client's Id to the client
            if (clientID == "-1") // this should be -1
            {                     // Compare clientID as a string
                std::string sendID = std::to_string(++id);
                socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send ID to client
                std::cout << "Received New Client" << std::endl;         // have server tell that client is here
                map[id] = 0;                                             // set iterations to 0
                numClients++;
            }
            else
            {
                std::cout << "Received an odd message...." << std::endl;
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
    zmq::context_t context(2);
    zmq::socket_t publisher(context, zmq::socket_type::pub);

    publisher.bind("tcp://*:2346");

    std::map<int, int> map;
    std::cout << "Creating the server..." << std::endl;

    // Create a vector to hold client handler threads
    // std::vector<std::thread> clientThreads;
    ThreadRunner t1; // Create a thread
    std::thread first(run_wrapper, &t1);

    while (true)
    {
        for (int i = 1; i <= numClients; i++)
        {
            std::string updateMessage = "Client " + std::to_string(i) + ": Iteration " + std::to_string(++map[i]);
            publisher.send(zmq::buffer(updateMessage), zmq::send_flags::none);
        }
        sleep(1);
    }

    return 0;
}
