// TP_ZMQ_Server_RadioGigabite.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <format>

#include "../TP_ZMQ_Libs/zmq.hpp"
#include "../TP_ZMQ_Libs/json.hpp"

using json = nlohmann::json;

using namespace std;
using namespace zmq;

int main()
{
    // initialize the zmq context with a single IO thread
    context_t context{ 1 };

    // construct a REP (reply) socket and bind to interface
    socket_t socket{ context, socket_type::rep };
    socket.bind("tcp://*:5555");

    const string server_name = "Hyperion";

    for (;;)
    {
        using namespace chrono;

        message_t request;

        // receive a request from client
        socket.recv(request, recv_flags::none);

        string request_str = request.to_string();
        cout << "Received\n" << request_str << endl;
        
        json responseJson = json::parse(request_str);

        size_t timestamp  = responseJson["timestamp"];
        size_t messageId  = responseJson["messageId"];
        //string data       = responseJson["data"];

        string message_to_client;
        auto sc_now = system_clock::now();

        uint64_t server_timestamp = duration_cast<milliseconds>(sc_now.time_since_epoch()).count();

        switch (messageId)
        {
        case (1):
            message_to_client = server_name;
            break;
        case (2):
            message_to_client = format("{0:%F %R %Z}", sc_now);
            break;
        case (3):
            return 1;
            break;
        default:
            message_to_client = "Command " + to_string(messageId) + " not found";
            break;
        }
        
        responseJson["timestamp"]   = server_timestamp;
        responseJson["data"]        = message_to_client;

        // simulate work
        // this_thread::sleep_for(1s);

        // send the reply to the client
        socket.send(buffer(to_string(responseJson)), send_flags::none);
    }

    return 0;
}

