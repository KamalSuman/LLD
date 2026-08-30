#include "ParkingLot.h"

#include <utility>

void ParkingLot::addParkingFloor(ParkingFloor parkingFloor) {
    parkingFloors.push_back(std::move(parkingFloor));
}

std::vector<ParkingFloor>& ParkingLot::getparkingFloors() { return parkingFloors; }
