#include "DeathHandler.hpp"

void DeathHandler::onEvent(Event event)
{
    if (event.getEvent() == event.type::DEATH || event.getEventInfo() == "DEATH")
    {
        event.actor->kill();

        //tell the server you're bad at the game
        zmq::context_t context(1);
        zmq::socket_t socket(context, zmq::socket_type::req);

        socket.connect("tcp://localhost:2332"); // connect to server thread

        // send -1 to server to let it know it needs an id
        std::string sendID = std::to_string(event.actor->getId()) + "D";                               // Request a new client ID
        socket.send(zmq::buffer(sendID), zmq::send_flags::none); // send that to server
    }
}
