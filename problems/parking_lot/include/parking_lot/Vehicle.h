#pragma once

#include <string>

#include "VehicleType.h"

class Vehicle {
    std::string vehicleId;
    VehicleType type;

   public:
    Vehicle(std::string vehicleId, VehicleType type);

    [[nodiscard]] const std::string& getVehicleId() const;
    [[nodiscard]] VehicleType getType() const;
};
