#include "TransportLayerPacket.h"

TransportLayerPacket::TransportLayerPacket(int _layer_ID, string _sender_port, string _receiver_port)
        : Packet(_layer_ID) {
    sender_port_number = _sender_port;
    receiver_port_number = _receiver_port;
}

void TransportLayerPacket::print() {
    // TODO: Override the virtual print function from Packet class to additionally print layer-specific properties.
    std::cout << "Sender port number: " + sender_port_number << ", Receiver port number: " + receiver_port_number + "\n";
}

TransportLayerPacket::~TransportLayerPacket() {
    // TODO: Free any dynamically allocated memory if necessary.
}
/*
std::string TransportLayerPacket::return_data() {
    std::string data = "Layer 0 info: Sender port number: " + sender_port_number;
    data += ", Receiver port number: " + receiver_port_number;
    return data;
}*/