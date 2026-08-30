#include "ParkingFloor.h"

#include <iostream>

ParkingFloor::ParkingFloor(int floorNumber) : floorNumber(floorNumber) {}

void ParkingFloor::addParkingSpot(const ParkingSpot& parkingSpot) {
    parkingSpots.emplace(parkingSpot.getSpotId(), parkingSpot);

    availableSpots[parkingSpot.getSpotType()].push_back(parkingSpot.getSpotId());
}

void ParkingFloor::removeParkingSpot(const std::string& parkingSpotId) {
    auto it = parkingSpots.find(parkingSpotId);

    if (it == parkingSpots.end()) {
        std::cout << "Invalid parking spot id\n";
        return;
    }

    SpotType spotType = it->second.getSpotType();

    auto& spots = availableSpots[spotType];

    for (int i = 0; i < spots.size(); i++) {
        if (spots[i] == parkingSpotId) {
            spots.erase(spots.begin() + i);
            break;
        }
    }

    parkingSpots.erase(it);
}

ParkingSpot* ParkingFloor::getAvailableSpot(SpotType type) {
    auto& spotIds = availableSpots[type];

    if (spotIds.empty()) {
        return nullptr;
    }

    std::string spotId = spotIds.front();

    auto it = parkingSpots.find(spotId);

    if (it == parkingSpots.end()) {
        return nullptr;
    }

    return &(it->second);
}

int ParkingFloor::getFloorNumber() const { return floorNumber; }