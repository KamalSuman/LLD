#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "ParkingSpot.h"
#include "SpotType.h"
using namespace std;

class ParkingFloor {
    int floorNumber;
    unordered_map<string, ParkingSpot> parkingSpots;
    map<SpotType, vector<string>> availableSpots;

   public:
    explicit ParkingFloor(int floorNumber);

    void addParkingSpot(const ParkingSpot& parkingSpot);
    void removeParkingSpot(const std::string& parkingSpotId);
    ParkingSpot* getAvailableSpot(SpotType spotType);
    int getFloorNumber() const;
};
