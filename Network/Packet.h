#ifndef PACKET_H
#define PACKET_H

#include <string>
#include <iostream>

using namespace std;

class Packet {
public:
    Packet(int layerID);
    virtual ~Packet();

    int layer_ID;

    friend ostream &operator<<(ostream &os, const Packet &packet);
    virtual void print() {};
    //virtual string return_data() {};
    int number_of_hops = 0;

    //virtual void change_mac_address(string& _sender_MAC, string& _receiver_MAC, int frame) {};
};

#endif // PACKET_H
