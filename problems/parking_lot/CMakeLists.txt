add_library(
    parking_lot
    src/CardPayment.cpp
    src/HourlyRateFare.cpp
    src/ParkingFloor.cpp
    src/ParkingLot.cpp
    src/ParkingManager.cpp
    src/ParkingSpot.cpp
    src/SpotAllocator.cpp
    src/Ticket.cpp
    src/Vehicle.cpp
)

target_include_directories(parking_lot PUBLIC include/parking_lot)
target_link_libraries(parking_lot PUBLIC lld_project_options)

add_executable(parking_lot_app app/main.cpp)
target_link_libraries(parking_lot_app PRIVATE parking_lot)
