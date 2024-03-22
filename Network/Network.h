#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include "Packet.h"
#include "Client.h"
#include <string>

using namespace std;


class Network {
public:
    Network();
    ~Network();

    // Executes commands given as a vector of strings while utilizing the remaining arguments.
    void process_commands(vector<Client> &clients, vector<string> &commands, int message_limit, const string &sender_port,
                     const string &receiver_port);

    // Initialize the network from the input files.
    vector<Client> read_clients(string const &filename);
    void read_routing_tables(vector<Client> & clients, string const &filename);
    vector<string> read_commands(const string &filename); 

    void message(vector<Client>& clients, string commands, int message_limit,
        const string& sender_port, const string& receiver_port);
    void send();
    void receive();
    void check_next_neighboor(vector<Client>& clients);
    void set_next_neighboor(vector<Client>& clients, string new_nearest_neighbor_id, int client_number);
    std::string time_stamp();
    void show_frame(string command, vector<Client>& clients);
    void show_q(string command, vector<Client>& clients);
    void print_log(string command, vector<Client>& clients);
    void invalid_command(string command);
    
    Client* sender;
    Client* receiver;
    Client* sender_temp;
    Client* nearest_neighbor;
    vector<Client*> senders;
    vector<Client*> receivers;
    vector<Client*> sender_temps;
    vector<Client*> nearest_neighbors;
    vector<Client*> previous_senders;
    vector<string> sentences;
    bool is_there_neighbor = false;
    vector<bool> is_there_neighbors;
    vector<bool> is_dropped;
    vector<bool> is_receiveds;
    vector<Packet*> packets_to_free;
    //int number_of_hops = 0;
    vector<vector<string>> splitted_sentences;
    bool is_send = false;
};

#endif  // NETWORK_H
