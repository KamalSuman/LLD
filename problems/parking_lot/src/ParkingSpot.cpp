#include "ParkingSpot.h"

#include <stdexcept>
#include <utility>
using namespace std;

ParkingSpot::ParkingSpot(std::string spotId, const SpotType spotType, const int floorNum)
    : spotId(std::move(spotId)), spotType(spotType), floorNum(floorNum) {}

void ParkingSpot::addVehicle(std::string vehicleId) {
    if (isOccupied) {
        throw std::logic_error("Parking spot is already occupied");
    }

    this->vehicleId = std::move(vehicleId);
    isOccupied = true;
}

void ParkingSpot::releaseVehicle() {
    vehicleId.clear();
    isOccupied = false;
}

string ParkingSpot::getSpotId() const { return spotId; }

SpotType ParkingSpot::getSpotType() const { return spotType; }
