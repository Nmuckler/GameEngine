/**
 * Recieved help from the ThreadExample in class
 * Also referenced
 * SOCKET HELP --- https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/hwclient.cpp#L3
 * PUB SUB HELP --- https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/wuclient.cpp
 */

#include <zmq.hpp>
#include <string>
#include <iostream>
#include <mutex>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
#define sleep(n) Sleep(n)
#endif

int main()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, zmq::socket_type::req);
    zmq::socket_t subscriber(context, zmq::socket_type::sub);

    std::cout << "Connecting to the server..." << std::endl;
    socket.connect("tcp://localhost:1235"); //connect to server thread
    subscriber.connect("tcp://localhost:2346"); //recieve updates from server

    std::string sendID = "-1";                               // Request a new client ID
    socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send that to server

    zmq::message_t reply;                                              // get reply from server
    socket.recv(reply, zmq::recv_flags::none);                         // recieve
    int clientID = std::atoi(static_cast<const char *>(reply.data())); // process as an int
    std::cout << "Connected as Client " << clientID << std::endl;

    // Subscribe to all messages
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "", 0);
    while (true)
    {
        zmq::message_t update; // get the update
        subscriber.recv(update, zmq::recv_flags::none);
        std::string updateMessage(static_cast<const char *>(update.data()), update.size());
        std::cout << "Received: " << updateMessage << std::endl;
    }

    return 0;
}