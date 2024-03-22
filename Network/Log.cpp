//
// Created by alperen on 2.10.2023.
//

#include "Log.h"

Log::Log(const string &_timestamp, const string &_message, int _number_of_frames, int _number_of_hops, const string &_sender_id,
         const string &_receiver_id, bool _success, ActivityType _type) {
    timestamp = _timestamp;
    message_content = _message;
    number_of_frames = _number_of_frames;
    number_of_hops = _number_of_hops;
    sender_id = _sender_id;
    receiver_id = _receiver_id;
    success_status = _success;
    activity_type = _type;
}

Log::~Log() {
    // TODO: Free any dynamically allocated memory if necessary.
}

void Log::print() {
    std::string type;
    std::string success;
    if(activity_type == ActivityType::MESSAGE_RECEIVED){
        type = "Message Received";
    }
    else if (activity_type == ActivityType::MESSAGE_DROPPED) {
        type = "Message Dropped";
    }
    else if (activity_type == ActivityType::MESSAGE_SENT) {
        type = "Message Sent";
    }
    else if (activity_type == ActivityType::MESSAGE_FORWARDED) {
        type = "Message Forwarded";
    }
    if (success_status) {
        success = "Yes";
    }
    else {
        success = "No";
    }

    std::cout << "Activity: " << type << "\n";

    std::cout << "Timestamp: " << timestamp << "\n";
    std::cout << "Number of frames: " << number_of_frames << "\n";
    std::cout << "Number of hops: " << number_of_hops << "\n";
    std::cout << "Sender ID: " << sender_id << "\n";
    std::cout << "Receiver ID: " << receiver_id << "\n";
    std::cout << "Success: " << success << "\n";
    if (activity_type == ActivityType::MESSAGE_SENT || activity_type == ActivityType::MESSAGE_RECEIVED) {
        std::cout << "Message: \"" << message_content << "\"\n";
    }
    
}