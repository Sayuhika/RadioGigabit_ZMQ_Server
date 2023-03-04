#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#include "../TP_ZMQ_Libs/zmq.hpp"
#include "../TP_ZMQ_Libs/json.hpp"

using json = nlohmann::json;

using namespace std;
using namespace zmq;

int main(int argc, char* argv[])
{
    using namespace chrono;

    // initialize the zmq context with a single IO thread
    context_t context{ 1 };

    // construct a REQ (request) socket and connect to interface
    socket_t socket{ context, socket_type::req };
    socket.connect("tcp://localhost:5555");

    json client_message;

    client_message["timestamp"] = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    if (argc > 1) client_message["messageId"] = stoi(string(argv[1]));
    else client_message["messageId"] = 0;

    socket.send(buffer(to_string(client_message)), send_flags::none);

    // wait for reply from server
    message_t reply{};
    socket.recv(reply, recv_flags::none);
    string request_str = reply.to_string();
    client_message = json::parse(request_str);
    request_str = client_message["data"];

    cout << "Received:\n" << request_str << endl << endl;

    cout << "Work with server completed." << endl;

    return 0;
}