#include <exception>
#include <iostream>
#include <utility>

#include "../include/parking_lot/CardPayment.h"
#include "../include/parking_lot/HourlyRateFare.h"
#include "../include/parking_lot/ParkingFloor.h"
#include "../include/parking_lot/ParkingLot.h"
#include "../include/parking_lot/ParkingManager.h"
#include "../include/parking_lot/ParkingSpot.h"
#include "../include/parking_lot/SpotType.h"
#include "../include/parking_lot/Vehicle.h"
#include "../include/parking_lot/VehicleType.h"

int main() {
    // Build the parking-lot configuration before accepting vehicles.
    ParkingFloor groundFloor(0);
    groundFloor.addParkingSpot(ParkingSpot("G-M1", SpotType::MOTORCYCLE, 0));
    groundFloor.addParkingSpot(ParkingSpot("G-C1", SpotType::COMPACT, 0));
    groundFloor.addParkingSpot(ParkingSpot("G-L1", SpotType::LARGE, 0));

    ParkingFloor firstFloor(1);
    firstFloor.addParkingSpot(ParkingSpot("F1-C1", SpotType::COMPACT, 1));
    firstFloor.addParkingSpot(ParkingSpot("F1-L1", SpotType::LARGE, 1));

    ParkingLot parkingLot;
    parkingLot.addParkingFloor(std::move(groundFloor));
    parkingLot.addParkingFloor(std::move(firstFloor));

    // Inject replaceable policies into the application service.
    HourlyRateFare fare(10);
    CardPayment payment;
    ParkingManager manager(parkingLot, fare, payment);

    try {
        const Vehicle car("KA-01-AB-1234", VehicleType::Car);
        Ticket ticket = manager.parkVehicle(car);

        std::cout << "Ticket generated: " << ticket.getTicketId() << '\n'
                  << "Allocated spot: " << ticket.getParkingSpot().getSpotId() << '\n';

        manager.removeVehicle(ticket);
        std::cout << "Vehicle exited and spot released.\n";
    } catch (const std::exception& error) {
        std::cerr << "Parking operation failed: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
