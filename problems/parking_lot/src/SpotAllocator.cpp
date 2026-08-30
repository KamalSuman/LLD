#include "SpotAllocator.h"

#include "SpotType.h"
#include "VehicleType.h"

SpotAllocator::SpotAllocator(ParkingLot& parkinglot) : parkinglot(parkinglot) {}

ParkingSpot* SpotAllocator::getParkingSpot(const Vehicle& vehicle) {
    auto& parkingFloors = parkinglot.getparkingFloors();
    ParkingSpot* spot = nullptr;

    for (auto& parkingFloor : parkingFloors) {
        if (vehicle.getType() == VehicleType::Bike) {
            spot = parkingFloor.getAvailableSpot(SpotType::MOTORCYCLE);
        } else if (vehicle.getType() == VehicleType::Car) {
            spot = parkingFloor.getAvailableSpot(SpotType::COMPACT);
            if (spot == nullptr) {
                spot = parkingFloor.getAvailableSpot(SpotType::LARGE);
            }
        } else if (vehicle.getType() == VehicleType::Truck) {
            spot = parkingFloor.getAvailableSpot(SpotType::LARGE);
        }

        if (spot != nullptr) {
            break;
        }
    }

    return spot;
}
