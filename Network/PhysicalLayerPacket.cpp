#include "PhysicalLayerPacket.h"

PhysicalLayerPacket::PhysicalLayerPacket(int _layer_ID, const string& _sender_MAC, const string& _receiver_MAC)
        : Packet(_layer_ID) {
    sender_MAC_address = _sender_MAC;
    receiver_MAC_address = _receiver_MAC;
}

void PhysicalLayerPacket::print() {
    // TODO: Override the virtual print function from Packet class to additionally print layer-specific properties.
    std::cout << "Sender MAC address: " + sender_MAC_address << ", Receiver MAC address: " + receiver_MAC_address + "\n";
    
}

void PhysicalLayerPacket::change_mac_address(string& _sender_MAC, string& _receiver_MAC, int frame) {
    sender_MAC_address = _sender_MAC;
    receiver_MAC_address = _receiver_MAC;
    std::cout << "Frame #" << frame << " MAC address change: New sender MAC " << sender_MAC_address << ", new receiver MAC " << receiver_MAC_address << "\n";
    
}

PhysicalLayerPacket::~PhysicalLayerPacket() {
    // TODO: Free any dynamically allocated memory if necessary.
}
/*
std::string PhysicalLayerPacket::return_data() {
    std::string data = "Layer 3 info: Sender MAC address: " + sender_MAC_address;
    data += ", Receiver MAC address: " + receiver_MAC_address;
    return data;
}*/