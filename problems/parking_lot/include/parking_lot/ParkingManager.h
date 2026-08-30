#pragma once

#include "Fare.h"
#include "ParkingLot.h"
#include "Payment.h"
#include "SpotAllocator.h"
#include "Ticket.h"
#include "Vehicle.h"

class ParkingManager {
    ParkingLot& parkingLot;
    SpotAllocator spotAllocator;
    Fare& fare;
    Payment& payment;

   public:
    ParkingManager(ParkingLot& parkingLot, Fare& fare, Payment& payment);

    [[nodiscard]] Ticket parkVehicle(const Vehicle& vehicle);
    void removeVehicle(Ticket& ticket);
};
