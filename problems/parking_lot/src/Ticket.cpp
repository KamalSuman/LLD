#include "Ticket.h"

#include <utility>

Ticket::Ticket(std::string ticketId, std::string vehicleId, ParkingSpot& parkingSpot)
    : ticketId(std::move(ticketId)),
      vehicleId(std::move(vehicleId)),
      entryTime(std::chrono::system_clock::now()),
      exitTime(entryTime),
      parkingSpot(&parkingSpot) {}

void Ticket::setExittime() { exitTime = std::chrono::system_clock::now(); }

int Ticket::parkingduration() const {
    return static_cast<int>(
        std::chrono::duration_cast<std::chrono::minutes>(exitTime - entryTime).count());
}

const std::string& Ticket::getTicketId() const { return ticketId; }

const std::string& Ticket::getVehicleId() const { return vehicleId; }

ParkingSpot& Ticket::getParkingSpot() const { return *parkingSpot; }
