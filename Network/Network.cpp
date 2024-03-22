#define _CRT_SECURE_NO_WARNINGS
#include "Network.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <stack>
#include <queue>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <algorithm>


Network::Network() {

}

void Network::process_commands(vector<Client> &clients, vector<string> &commands, int message_limit,
                      const string &sender_port, const string &receiver_port) {
    // TODO: Execute the commands given as a vector of strings while utilizing the remaining arguments.
    /* Don't use any static variables, assume this method will be called over and over during testing.
     Don't forget to update the necessary member variables after processing each command. For example,
     after the MESSAGE command, the outgoing queue of the sender must have the expected frames ready to send. */
    for (auto command : commands) { //
        if (command.find("MESSAGE") != std::string::npos) {
            message(clients, command, message_limit, sender_port, receiver_port);
            is_dropped.push_back(false);
        }
        else if (command == "SEND") {
            send();
            check_next_neighboor(clients);
        }
        else if (command == "RECEIVE") {
            receive();
            if (nearest_neighbor != receiver) {
                //check_next_neighboor(clients);
            }
        }else if (command.find("SHOW_FRAME_INFO") != std::string::npos) {
            show_frame(command, clients);
        }
        else if (command.find("SHOW_Q_INFO") != std::string::npos) {
            show_q(command, clients);
        }
        else if (command.find("PRINT_LOG") != std::string::npos) {
            print_log(command, clients);
        }
        else {
            invalid_command(command);
        }

    }
    //std::cout << "\n";

}

vector<Client> Network::read_clients(const string &filename) {
    vector<Client> clients;
    // TODO: Read clients from the given input file and return a vector of Client instances.
    std::ifstream myFile(filename);
    std::string line;
    std::string variable;
    vector<string> variables;
    if(myFile.is_open()){
        if(std::getline(myFile, line)){
            int total_client = std::stoi(line);
        }
        

        while(std::getline(myFile, line)){
            
            std:stringstream ss(line);
            while(std::getline(ss, variable, ' ')){
                variables.push_back(variable);
            } 

            Client client = Client(variables[0], variables[1], variables[2]);
            clients.push_back(client);
            variables.clear();
            // check if you need to clear vector
        }

    }

    myFile.close();

    return clients;
}

void Network::read_routing_tables(vector<Client> &clients, const string &filename) {
    // TODO: Read the routing tables from the given input file and populate the clients' routing_table member variable.
    std::ifstream myFile(filename);
    std::string line;
    std::string variable;
    vector<string> variables;

    for(auto& client: clients){
        if(myFile.is_open()){
            while(std::getline(myFile, line) && line != "-"){

                std:stringstream ss(line);
                while(std::getline(ss, variable, ' ')){
                    variables.push_back(variable);
                } 

                client.routing_table[variables[0]] = variables[1];
                variables.clear();
            }
            
        }

    }
    myFile.close();
}

// Returns a list of token lists for each command
vector<string> Network::read_commands(const string &filename) {
    vector<string> commands;
    // TODO: Read commands from the given input file and return them as a vector of strings.
    std::ifstream myFile(filename);
    std::string line;

    if (myFile.is_open()) {

        std::getline(myFile, line);

        while (std::getline(myFile, line)) {
            commands.push_back(line);
        }

    }

    return commands;
}

Network::~Network() {
    // TODO: Free any dynamically allocated memory if necessary.
    /*
    for (auto& packet : packets_to_free) {
        delete packet;
    }
    //packets_to_free.clear();*/

}

void Network::message(vector<Client>& clients, string command, int message_limit,
    const string& sender_port, const string& receiver_port) {
    
    std::string word;
    vector<string> command_words;
    vector<string> splitted_sentece;
    std:stringstream ss(command);
    std::string sentence;
    bool is_message = false;
    
    std::string co_word;
    int word_number = 0;
    bool is_message2 = false;
    for (char c : command) {

        if (word_number != 3) {
            if (c != ' ') {
                co_word += c;
            }
            else if (c == ' ') {
                command_words.push_back(co_word);
                co_word.clear();
                word_number++;
            }
            continue;
        }
        

        if (c != '#') {
            sentence += c;
        }
        if (c == '.' || c == '!') {
            break;
        }
    }


    
    //sentences.push_back(sentence);
    
    if (!is_send) {
        sentences.push_back(sentence);
    }
    else {
        sentences.insert(sentences.begin(), sentence);
    }
    

    int start = 0;
    int finish = message_limit;
    for (int i = 0; i < (sentence.length() / message_limit) + 1; i++) {

        if (i == (sentence.length() / message_limit)) {
            splitted_sentece.push_back(sentence.substr(start)); 
            continue;
        }

        if (sentence.length() >= finish + 1) {
            splitted_sentece.push_back(sentence.substr(start, message_limit)); // check errors bounding...
            //continue;
        }

        if (sentence.length() <= message_limit) {
            splitted_sentece.push_back(sentence);
            break;
        }
        
        start = (start == 0) ? start + message_limit  : start + message_limit;
        finish += message_limit;

    }

    //splitted_sentences.push_back(splitted_sentece);
    if (!is_send) {
        splitted_sentences.push_back(splitted_sentece);
    }
    else {
        //splitted_sentences.push_back(splitted_sentece);
        splitted_sentences.insert(splitted_sentences.begin(), splitted_sentece);
    }


    for (auto& client : clients) {
        if (client.client_id == command_words[1]) { // sender
            sender = &client;
            //senders.push_back(sender);

            if (!is_send) {
                senders.push_back(sender);
            }
            else {
                //senders.push_back(sender);
                senders.insert(senders.begin(), sender);
            }
        }
        
        if (client.client_id == command_words[2]) { // recevier
            receiver = &client;
            //receivers.push_back(receiver);

            if (!is_send) {
                receivers.push_back(receiver);
            }
            else {
                //receivers.push_back(receiver);
                receivers.insert(receivers.begin(), receiver);
            }
        }
        
    }

    sender_temp = sender;
    if (!is_send) {
        sender_temps.push_back(sender_temp);
        previous_senders.push_back(sender_temp);
        is_receiveds.push_back(false);
        is_there_neighbors.push_back(false);
    }
    else {
        sender_temps.insert(sender_temps.begin(), sender_temp);
        previous_senders.insert(previous_senders.begin(), sender_temp);
        is_receiveds.insert(is_receiveds.begin(), false);
        is_there_neighbors.insert(is_there_neighbors.begin(), false);
    }
    
    std::string sender_id = sender->client_id;
    std::string reciver_id = receiver->client_id;
    std::string sender_ip = sender->client_ip;
    std::string reciver_ip = receiver->client_ip;
    std::string nearest_neighbor_id;
    bool is_found = false;
    for (auto pair : sender->routing_table) {
        
        if (pair.first == reciver_id) {
            nearest_neighbor_id = pair.second; // or first??
            is_found = true; // do smth if you cant find
        }
    }

    int hops = 0;
    int frames = 0;
    for (auto& client : clients) {
        if (client.client_id == nearest_neighbor_id) {
            nearest_neighbor = &client;

            if (!is_send) {
                nearest_neighbors.push_back(nearest_neighbor);
            }
            else {
                nearest_neighbors.insert(nearest_neighbors.begin(), nearest_neighbor);
            }
            
            

            for (int i = 0; i < command.size() + 9; i++) {
                std::cout << "-";
            }
            std::cout << "\n";
            std::cout << "Command: " << command + "\n";
            for (int i = 0; i < command.size() + 9; i++) {
                std::cout << "-";
            }
            std::cout << "\n";
            std::cout << "Message to be sent: " << "\"" + sentence + "\"" + "\n" + "\n";
            int frame = 0;
            
            int hop = 0;
            for (int i = 0; i < splitted_sentece.size(); i++) {
                frame++;
                ApplicationLayerPacket* applicationLayerPacket = new ApplicationLayerPacket(0, sender_id, reciver_id, splitted_sentece[i]);
                TransportLayerPacket* transportLayerPacket = new TransportLayerPacket(1, sender_port, receiver_port);
                NetworkLayerPacket* networkLayerPacket = new NetworkLayerPacket(2, sender_ip, reciver_ip);
                PhysicalLayerPacket* physicalLayerPacket = new PhysicalLayerPacket(3, sender->client_mac, client.client_mac);
                std::stack<Packet*> stack;
                stack.push(applicationLayerPacket);
                stack.push(transportLayerPacket);
                stack.push(networkLayerPacket);
                stack.push(physicalLayerPacket);
                sender->outgoing_queue.push(stack);

                packets_to_free.push_back(applicationLayerPacket);
                packets_to_free.push_back(transportLayerPacket);
                packets_to_free.push_back(networkLayerPacket);
                packets_to_free.push_back(physicalLayerPacket);

                std::cout << "Frame #" << i + 1<< "\n";
                physicalLayerPacket->print();
                networkLayerPacket->print();
                transportLayerPacket->print();
                applicationLayerPacket->print();
                hop = applicationLayerPacket->number_of_hops;
                hops = hop;
                std::cout << "Number of hops so far: " << applicationLayerPacket->number_of_hops << "\n";
                std::cout << "--------\n";

                //clear stack
                std::stack<Packet*> emptyStack;
                stack.swap(emptyStack);
            }
            frames = frame;
        }
    }

    std::string time = time_stamp(); //time_stamp();
    Log log(time, sentence, frames, hops, sender->client_id, receiver->client_id, true, ActivityType::MESSAGE_SENT);
    sender->log_entries.push_back(log);
}


void Network::send() {
    is_send = true;
    std::cout << "-------------\n";
    std::cout << "Command: SEND\n";
    std::cout << "-------------\n";

    

    for (int i = 0; i < sender_temps.size(); i++) {
        if (is_dropped[i]) {
            continue;
            // print the error..,
        }

        if (sender_temps[i]->outgoing_queue.size() == 0) {
            continue;
        }
        int hops = 0;
        int frame = 0;
        for(int j = 0; j < splitted_sentences[i].size(); j++)
        {
            frame++;
            
            std::cout << "Client " << sender_temps[i]->client_id << " sending frame #" << j + 1 << " to client " << nearest_neighbors[i]->client_id <<"\n";
            nearest_neighbors[i]->incoming_queue.push(sender_temps[i]->outgoing_queue.front());
            sender_temps[i]->outgoing_queue.front().top()->number_of_hops++; // change this, make vector
            hops = sender_temps[i]->outgoing_queue.front().top()->number_of_hops;
            while (!sender_temps[i]->outgoing_queue.front().empty()) {
                sender_temps[i]->outgoing_queue.front().top()->print();
                sender_temps[i]->outgoing_queue.front().pop();
            }
            std::cout << "Number of hops so far: " << hops << "\n";
            std::cout << "--------\n";
            sender_temps[i]->outgoing_queue.pop();
        }
        //nearest_neighbors[i]->incoming_queue.front().top()->number_of_hops++;
        previous_senders[i] = sender_temps[i];
        sender_temps[i] = nearest_neighbors[i]; // i added s at the end of words

        
        ActivityType activityType;
        if (sender_temps[i]->client_id == senders[i]->client_id) {
            activityType = ActivityType::MESSAGE_SENT;
        }
        else {
            activityType = ActivityType::MESSAGE_FORWARDED;
        }
        std::string time = time_stamp(); //time_stamp();
        Log log(time, sentences[i], frame, hops,  senders[i]->client_id, receivers[i]->client_id, true, activityType);
        //sender_temps[i]->log_entries.push_back(log);
    }
}

void Network::receive() {
    
    
    /*
    for (int i = 0; i < senders.size(); i++) {
        while (!senders[i]->incoming_queue.empty())
        {
            receivers[i]->outgoing_queue.push(senders[i]->incoming_queue.front());
            senders[i]->incoming_queue.pop(); // check sender or senders temps
        }
        senders[i] = receivers[i];
    }*/
    std::cout << "----------------\n";
    std::cout << "Command: RECEIVE\n";
    std::cout << "----------------\n";

    /*
    for (int i = 0; i < sender_temps.size(); i++) {
        if (!is_dropped[i]) {
            return;
            // print the error..,
        }
    }*/

    for (int i = 0; i < sender_temps.size(); i++) {
        int frame2 = 0;
        int hops = 0;
        if (sender_temps[i]->client_id == receivers[i]->client_id) {
            is_receiveds[i] = true;
            for (int j = 0; j < splitted_sentences[i].size(); j++) {
                frame2++;
                std::cout << "Client " << sender_temps[i]->client_id << " receiving frame #" << frame2 << " from client " << previous_senders[i]->client_id << ", originating from client " << senders[i]->client_id << "\n";

                
                hops = sender_temps[i]->incoming_queue.front().top()->number_of_hops;
                while (!sender_temps[i]->incoming_queue.front().empty()) {

                    
                    if (is_receiveds[i]) {
                        //std::cout << "Client: " << sender_temps[i]->client_id << " receiving a message from client " << previous_senders[i]->client_id << ", originating from " << senders[i]->client_id << "\n";
                        sender_temps[i]->incoming_queue.front().top()->print();

                    }
                    sender_temps[i]->incoming_queue.front().pop();
                }
                if (is_receiveds[i] && j != splitted_sentences[i].size() - 1) {
                    std::cout << "Number of hops so far: " << hops << "\n";
                    std::cout << "--------\n";
                }
                else if (is_receiveds[i] && j == splitted_sentences[i].size() - 1) {
                    std::cout << "Number of hops so far: " << hops << "\n";
                }


                sender_temps[i]->incoming_queue.pop();

            }
            std::cout << "--------\n";
            std::cout << "Client " << receivers[i]->client_id << " received the message \"" << sentences[i] << "\" from client " << senders[i]->client_id << ".\n";
            std::cout << "--------\n";

            ActivityType activityType = ActivityType::MESSAGE_RECEIVED;
            std::string time = time_stamp();
            Log log(time, sentences[i], frame2, hops, senders[i]->client_id, receivers[i]->client_id, true, activityType);
            sender_temps[i]->log_entries.push_back(log);

            //delete clients..

            senders.erase(senders.begin() + i);
            sender_temps.erase(sender_temps.begin() + i);
            previous_senders.erase(previous_senders.begin() + i);
            nearest_neighbors.erase(nearest_neighbors.begin() + i);
            receivers.erase(receivers.begin() + i);
            sentences.erase(sentences.begin() + i);
            splitted_sentences.erase(splitted_sentences.begin() + i);
            is_there_neighbors.erase(is_there_neighbors.begin() + i);
            is_dropped.erase(is_dropped.begin() + i);
            is_receiveds.erase(is_receiveds.begin() + i);
        }

    }

    
    for (int i = 0; i < sender_temps.size(); i++) {
        
       // int frame2 = 0;
        //int hops = 0;
        if (!is_receiveds[i]) {
            if (sender_temps[i]->incoming_queue.size() == 0) {
                continue;
            }
            bool is_received = false;
            if (is_dropped[i]) {
                continue;
                // print the error..,
            }
            if (sender_temps[i]->client_id == receivers[i]->client_id) {
                //std::cout << "Client: " << sender_temps[i]->client_id << " receiving a message from client " << previous_senders[i]->client_id << ", originating from " << senders[i]->client_id  << "\n";
                is_received = true;
                is_receiveds[i] = true;
            }
            else if (sender_temps[i]->client_id != receivers[i]->client_id && is_there_neighbors[i]) {
                std::cout << "Client " << sender_temps[i]->client_id << " receiving a message from client " << previous_senders[i]->client_id << ", but intended for client " << receivers[i]->client_id << ". Forwarding... " << "\n";

            }
            else {
                //std::cout << "Client " << sender_temps[i]->client_id << " receiving a frame" <<  " from client " << previous_senders[i]->client_id << ", but intended for client " << receivers[i]->client_id << ". Forwarding... " << "\n";

            }

            if (is_there_neighbors[i] == false) { // check
                //continue;
            }
            int hops = 0;
            int frame2 = 0;
            int frame = 0;
            for (int j = 0; j < splitted_sentences[i].size(); j++)//while (!sender_temps[i]->incoming_queue.empty())
            {
                frame2++;
                hops = sender_temps[i]->incoming_queue.front().top()->number_of_hops;
                if (!is_there_neighbors[i] && !is_received) { //&& !is_received
                    std::cout << "Client " << sender_temps[i]->client_id << " receiving frame #" << j + 1 << " from client " << previous_senders[i]->client_id << ", but intended for client " << receivers[i]->client_id << ". Forwarding... " << "\n";
                    std::cout << "Error: Unreachable destination. Packets are dropped after " << hops << " hops!" << "\n";
                    is_dropped[i] = true; // delete from ...
                    continue;

                }
                sender_temps[i]->outgoing_queue.push(sender_temps[i]->incoming_queue.front());
                //nearest_neighbors[i]->outgoing_queue.push(sender_temps[i]->incoming_queue.front());



                if (is_received) {
                    std::cout << "Client " << sender_temps[i]->client_id << " receiving frame #" << frame2 << " from client " << previous_senders[i]->client_id << ", originating from client " << senders[i]->client_id << "\n";

                }

                while (!sender_temps[i]->incoming_queue.front().empty()) {

                    int layer_id = sender_temps[i]->incoming_queue.front().top()->layer_ID;
                    if (layer_id == 3 & !is_received) {
                        PhysicalLayerPacket* physicalLayerPacket = (PhysicalLayerPacket*)sender_temps[i]->incoming_queue.front().top();
                        frame++;
                        physicalLayerPacket->change_mac_address(sender_temps[i]->client_mac, nearest_neighbors[i]->client_mac, frame);
                    }
                    // sender_temps[i]->incoming_queue.front().top().;
                    if (is_received) {
                        //std::cout << "Client: " << sender_temps[i]->client_id << " receiving a message from client " << previous_senders[i]->client_id << ", originating from " << senders[i]->client_id << "\n";
                        sender_temps[i]->incoming_queue.front().top()->print();

                    }
                    sender_temps[i]->incoming_queue.front().pop();
                }
                if (is_received && j != splitted_sentences[i].size() - 1) {
                    std::cout << "Number of hops so far: " << hops << "\n";
                    std::cout << "--------\n";
                }
                else if (is_received && j == splitted_sentences[i].size() - 1) {
                    std::cout << "Number of hops so far: " << hops << "\n";
                }


                sender_temps[i]->incoming_queue.pop(); // check sender or senders temps
            }
            std::cout << "--------\n";

            if (sender_temps[i]->client_id == receivers[i]->client_id) {
                std::cout << "Client " << receivers[i]->client_id << " received the message \"" << sentences[i] << "\" from client " << senders[i]->client_id << ".\n";
                std::cout << "--------\n";
            }
        
            

            ActivityType activityType; 
            if (sender_temps[i]->client_id == receivers[i]->client_id && is_receiveds[i]) { //&& is_there_neighbors[i]    is_receiveds[i]
                activityType = ActivityType::MESSAGE_RECEIVED;
            }
            else if (!is_there_neighbors[i]) {
                activityType = ActivityType::MESSAGE_DROPPED;
            }
            else if(sender_temps[i]->client_id != receivers[i]->client_id && is_there_neighbors[i]){
                activityType = ActivityType::MESSAGE_FORWARDED;
            }
            std::string time = time_stamp(); //time_stamp();
            if (activityType != ActivityType::MESSAGE_DROPPED) {
                Log log(time, sentences[i], frame2, hops, senders[i]->client_id, receivers[i]->client_id, true, activityType);
                sender_temps[i]->log_entries.push_back(log);
            }
            else {
                Log log(time, sentences[i], frame2, hops, senders[i]->client_id, receivers[i]->client_id, false, activityType);
                sender_temps[i]->log_entries.push_back(log);
            }
            previous_senders[i] = sender_temps[i];
            if (activityType == ActivityType::MESSAGE_RECEIVED) {
                senders.erase(senders.begin() + i);
                sender_temps.erase(sender_temps.begin() + i);
                previous_senders.erase(previous_senders.begin() + i);
                nearest_neighbors.erase(nearest_neighbors.begin() + i);
                receivers.erase(receivers.begin() + i);
                sentences.erase(sentences.begin() + i);
                splitted_sentences.erase(splitted_sentences.begin() + i);
                is_there_neighbors.erase(is_there_neighbors.begin() + i);
                is_dropped.erase(is_dropped.begin() + i);
                is_receiveds.erase(is_receiveds.begin() + i);
            }
        }
        
        //sender_temps[i] = nearest_neighbors[i];
    }
}



void Network::check_next_neighboor(vector<Client>& clients) {
    for (int i = 0; i < senders.size(); i++) {
        std::string receiver_id = receivers[i]->client_id;
        std::string nearest_neighbor_id = nearest_neighbors[i]->client_id;
        std::string new_nearest_neighbor_id;

        is_there_neighbor = false;

        if (receiver_id == nearest_neighbor_id) {

            //return;
        }
        bool is_there = false;
        for (auto pair : nearest_neighbors[i]->routing_table) {
            if (pair.first == receiver_id) { //pair.second != "X"
                for (auto& client : clients) {
                    if (client.client_id == pair.second) {
                        is_there = true;
                    }
                }

                if (!is_there) {
                    break;
                }


                if (pair.first == pair.second) {
                    nearest_neighbor = receiver;
                    //nearest_neighbors.push_back(nearest_neighbor);
                    nearest_neighbors[i] = nearest_neighbor;
                    is_there_neighbor = true;
                }
                else {
                    is_there_neighbor = true;
                    new_nearest_neighbor_id = pair.second;
                    set_next_neighboor(clients, new_nearest_neighbor_id, i);
                }
                break;
            }
        }
        //is_there_neighbors.push_back(is_there_neighbor);
        
        if (is_there_neighbors.size() > i) {
            is_there_neighbors[i] = is_there_neighbor;
        }
        else
        {
            is_there_neighbors.push_back(is_there_neighbor);
        }
        
    }
    
    
}

void Network::set_next_neighboor(vector<Client>& clients, string new_nearest_neighbor_id, int client_number) {
    for (auto& client : clients) {
        if (client.client_id == new_nearest_neighbor_id) {
            nearest_neighbor = &client;
            //nearest_neighbors.push_back(nearest_neighbor);
             
            //nearest_neighbors.erase(nearest_neighbors.begin() + client_number);
            //nearest_neighbors.insert(nearest_neighbors.begin() + client_number, nearest_neighbor);
            //nearest_neighbors[client_number] = nullptr;
            //nearest_neighbors[client_number] = nearest_neighbor;
            nearest_neighbors[client_number] = nearest_neighbor;
        }
    }
}
/*
std::string time_stamp() {
    auto currentTimePoint = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(currentTimePoint);
    std::tm* timeInfo = std::localtime(&timeT);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);

    return std::string(buffer);
}*/

std::string Network::time_stamp()
{
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);

    tm* tmStruct = localtime(&currentTime);
    ostringstream oss;
    oss << std::put_time(tmStruct, "%Y-%m-%d %H:%M:%S");

    return oss.str();
}

void Network::show_frame(string command, vector<Client>& clients) {
    std:stringstream ss(command);
    vector<string> command_words;
    
    std::string word;
    std::string output;
    std::string hypens;
    vector<string> layer_prints;

    for (int i = 0; i < 9 + command.size(); i++) {
        hypens += "-";
    }
    std::cout << hypens << "\n";
    std::cout << "Command: " << command << "\n";
    std::cout << hypens << "\n";
    
    


    while (std::getline(ss, word, ' ')) {
        command_words.push_back(word);
    }

    std::string id = command_words[1];
    int frame_number = std::stoi(command_words[3]);

    //std::cout << "Current Frame #" << frame_number << " ";
    
    queue<stack<Packet*>> queue_normal;
    queue<stack<Packet*>> queue_to_reversed;

    bool is_there_frame = true;

    vector<string> datas;
    for (auto& client : clients) {
        if (client.client_id == id) {
            if (command_words[2] == "in") {

                queue_normal = client.incoming_queue;
                if (queue_normal.size() == 0) {
                    is_there_frame = false;
                    std::cout << "No such frame.\n";
                }
                else {
                    // output += " on the incoming queue of client " + id;
                    std::cout << "Current Frame #" << frame_number << " " << "on the incoming queue of client " << id << "\n";
                }
               
            }
            else {
                queue_normal = client.outgoing_queue;
                if (queue_normal.size() == 0) {
                    is_there_frame = false;
                    std::cout << "No such frame.\n";
                }
                else {
                    //output += " on the outgoing queue of client " + id;
                    std::cout << "Current Frame #" << frame_number << " " << "on the outgoing queue of client " << id << "\n";
                }
                
            }
        }
        
    }
    if (!is_there_frame) {
        return;
    }

    
    int frame = 1;
    int hops = 0;
    while (!queue_normal.empty() && !(frame > frame_number) ) {
        if (frame == frame_number) { //frame == 3
            while (!queue_normal.front().empty())
            {
                if (queue_normal.front().top()->layer_ID == 0) {
                    ApplicationLayerPacket* applicationLayerPacket = (ApplicationLayerPacket*)queue_normal.front().top();
                    datas.push_back(applicationLayerPacket->receiver_ID);
                    datas.push_back(applicationLayerPacket->sender_ID);
                    datas.push_back(applicationLayerPacket->message_data);
                    hops = applicationLayerPacket->number_of_hops;
                }
                else if (queue_normal.front().top()->layer_ID == 1) {
                    TransportLayerPacket * transportLayerPacket = (TransportLayerPacket*)queue_normal.front().top();
                    datas.push_back(transportLayerPacket->receiver_port_number);
                    datas.push_back(transportLayerPacket->sender_port_number);
                }
                else if (queue_normal.front().top()->layer_ID == 2) {
                    NetworkLayerPacket * networkLayerPacket = (NetworkLayerPacket*)queue_normal.front().top();
                    datas.push_back(networkLayerPacket->receiver_IP_address);
                    datas.push_back(networkLayerPacket->sender_IP_address);
                }
                else if (queue_normal.front().top()->layer_ID == 3) {
                    PhysicalLayerPacket *physicalLayerPacket = (PhysicalLayerPacket*)queue_normal.front().top();
                    datas.push_back(physicalLayerPacket->receiver_MAC_address);
                    datas.push_back(physicalLayerPacket->sender_MAC_address);
                }
                //std::string data = (queue_normal.front().top()->return_data());
                //datas.push_back(data);
                queue_normal.front().pop();
            }
            
        }
        queue_normal.pop();
        frame++;
    }
    
  
    std::cout << "Carried Message: \"" << datas[8] << "\"" << "\n";
    std::cout << "Layer 0 info: Sender ID: " << datas[7] << ", Receiver ID: " << datas[6] << "\n";
    std::cout << "Layer 1 info: Sender port number: " << datas[5] << ", Receiver port number: " << datas[4] << "\n";
    std::cout << "Layer 2 info: Sender IP address: " << datas[3] << ", Receiver IP address: " << datas[2] << "\n";
    std::cout << "Layer 3 info: Sender MAC address: " << datas[1] << ", Receiver MAC address: " << datas[0] << "\n";
    std::cout << "Number of hops so far: " <<hops <<"\n";

    /*
    * for (int i = 0; i < datas.size(); i++) {
        std::cout << datas[datas.size() - 1 - i] << "\n";

    }
    * 
    while (!queue_to_reversed.empty()) {
        queue_normal.push(queue_to_reversed.front());
        // queue_normal.front().top()->print();
        queue_to_reversed.pop();
    }

    while (!queue_normal.front().empty()) {
        queue_normal.front().top()->print();
        queue_normal.front().pop();
    }*/
}



void Network::show_q(string command, vector<Client>& clients) {
    std:stringstream ss(command);
    vector<string> command_words;
    std::string output;
    std::string word;

    for (int i = 0; i < 9 + command.size(); i++) {
        output += "-";
    }
    output += "\nCommand: " + command + "\n";
    for (int i = 0; i < 9 + command.size(); i++) {
        output += "-";
    }
    output += "\n";

    std::cout << output;
    while (std::getline(ss, word, ' ')) {
        command_words.push_back(word);
    }

    std::string id = command_words[1];


    for (auto& client : clients) {
        if (client.client_id == id) {
            if (command_words[2] == "out") {
                std::cout << "Client " << id << " Outgoing Queue Status\n";
                std::cout << "Current total number of frames: " << client.outgoing_queue.size() << "\n";
            }
            else {
                std::cout << "Client " << id << " Incoming Queue Status\n";
                std::cout << "Current total number of frames: " << client.incoming_queue.size() << "\n";
            }
        }
    }
}


void Network::print_log(string command, vector<Client>& clients) {
    std::string hypens;
    for (int i = 0; i < (command.size() + 9); i++) {
        hypens += "-";
    }
    std::cout << hypens << "\n";
    std::cout << "Command: " << command << "\n";
    std::cout << hypens << "\n";

    std:stringstream ss(command);
    vector<string> command_words;
    std::string output;
    std::string word;

    while (std::getline(ss, word, ' ')) {
        command_words.push_back(word);
    }

    std::string id = command_words[1];

   
    int log_numbers = 1;

    for (auto & client : clients) {
        if (client.client_id == id) {
            if (client.log_entries.size() == 0) {
                continue;
            }
            std::cout << "Client " << id << " Logs:" << "\n";
            std::cout << "--------------\n";
            for (auto log : client.log_entries) {
                if (log_numbers >= 2) {
                    std::cout << "--------------\n";
                }
                std::cout << "Log Entry #" << log_numbers <<":\n";
                log.print();
                log_numbers++;
            }

        }
    }

}

void Network::invalid_command(string command) {
    std::string hypens;
    for (int i = 0; i < (command.size() + 9); i++) {
        hypens += "-";
    }
    std::cout << hypens << "\n";
    std::cout << "Command: " << command << "\n";
    std::cout << hypens << "\n";
    std::cout << "Invalid command.\n";
}

