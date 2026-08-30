#include "Vehicle.h"

#include <utility>

Vehicle::Vehicle(std::string vehicleId, const VehicleType type)
    : vehicleId(std::move(vehicleId)), type(type) {}

const std::string& Vehicle::getVehicleId() const { return vehicleId; }

VehicleType Vehicle::getType() const { return type; }
