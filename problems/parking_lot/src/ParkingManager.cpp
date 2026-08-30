#include <chrono>
#include <stdexcept>
#include <string>

#include "ParkingManager.h"

ParkingManager::ParkingManager(ParkingLot& parkingLot, Fare& fare, Payment& payment)
    : parkingLot(parkingLot), spotAllocator(parkingLot), fare(fare), payment(payment) {}

Ticket ParkingManager::parkVehicle(const Vehicle& vehicle) {
    ParkingSpot* parkingSpot = spotAllocator.getParkingSpot(vehicle);
    if (parkingSpot == nullptr) {
        throw std::runtime_error("No compatible parking spot is available");
    }

    parkingSpot->addVehicle(vehicle.getVehicleId());

    const auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::system_clock::now().time_since_epoch())
                               .count();
    const std::string ticketId = vehicle.getVehicleId() + "_" + std::to_string(timestamp);
    return Ticket(ticketId, vehicle.getVehicleId(), *parkingSpot);
}

void ParkingManager::removeVehicle(Ticket& ticket) {
    ticket.setExittime();
    const int durationParked = ticket.parkingduration();
    const int paymentAmount = fare.calculateFare(durationParked);

    payment.payment(paymentAmount);
    ticket.getParkingSpot().releaseVehicle();
}
