#pragma once

#include "ParkingLot.h"
#include "Vehicle.h"

class SpotAllocator {
    ParkingLot& parkinglot;

   public:
    explicit SpotAllocator(ParkingLot& parkinglot);
    [[nodiscard]] ParkingSpot* getParkingSpot(const Vehicle& vehicle);
};
