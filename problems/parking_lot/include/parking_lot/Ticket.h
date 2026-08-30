#pragma once

#include <chrono>
#include <string>

#include "ParkingSpot.h"

class Ticket {
    std::string ticketId;
    std::string vehicleId;
    std::chrono::system_clock::time_point entryTime;
    std::chrono::system_clock::time_point exitTime;
    ParkingSpot* parkingSpot;

   public:
    Ticket(std::string ticketId, std::string vehicleId, ParkingSpot& parkingSpot);

    void setExittime();
    [[nodiscard]] int parkingduration() const;
    [[nodiscard]] const std::string& getTicketId() const;
    [[nodiscard]] const std::string& getVehicleId() const;
    [[nodiscard]] ParkingSpot& getParkingSpot() const;
};
