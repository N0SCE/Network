#include "ApplicationLayerPacket.h"

ApplicationLayerPacket::ApplicationLayerPacket(int _layer_ID, const string &_sender_ID, const string &_receiver_ID, const string& _message_data) : Packet(_layer_ID) {
    sender_ID = _sender_ID;
    receiver_ID = _receiver_ID;
    message_data = _message_data;
}

void ApplicationLayerPacket::print() {
    // TODO: Override the virtual print function from Packet class to additionally print layer-specific properties.
    std::cout << "Sender ID: " + sender_ID << ", Receiver ID: " + receiver_ID + "\n";
    std::cout << "Message chunk carried: " << "\"" + message_data << "\"\n" ;
}

ApplicationLayerPacket::~ApplicationLayerPacket() {
    // TODO: Free any dynamically allocated memory if necessary.
}

/*
string ApplicationLayerPacket::return_data() {
    std::string data = "Carried Message: \"" + message_data + "\"\n";
    data += "Layer 0 info: ";
    data += "Sender ID: " + sender_ID;
    data += ", Receiver ID: " + receiver_ID;
    return data;
}*/
