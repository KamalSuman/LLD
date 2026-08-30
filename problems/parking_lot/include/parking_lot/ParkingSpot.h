#pragma once

#include <string>

#include "SpotType.h"
using namespace std;

class ParkingSpot {
   private:
    string spotId;
    SpotType spotType;
    int floorNum;
    bool isOccupied;
    string vehicleId;

   public:
    ParkingSpot(string spotId, SpotType spotType, int floorNum);

    void addVehicle(string vehicleId);
    void releaseVehicle();
    string getSpotId() const;
    SpotType getSpotType() const;
};