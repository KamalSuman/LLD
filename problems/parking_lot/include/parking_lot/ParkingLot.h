#pragma once

#include <vector>

#include "ParkingFloor.h"

class ParkingLot {
    std::vector<ParkingFloor> parkingFloors;

   public:
    void addParkingFloor(ParkingFloor parkingFloor);
    [[nodiscard]] std::vector<ParkingFloor>& getparkingFloors();
};
