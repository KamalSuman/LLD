# Parking Lot

## Functional requirements

- Support multiple parking floors.
- Support Motorcycle, Compact, and Large spots.
- Allocate a compatible available spot to an incoming vehicle.
- Generate a unique ticket containing entry time and assigned spot.
- Calculate parking fees using a configurable pricing strategy.
- Process card, cash, and mobile payments using mock processors.
- Release the spot after successful payment and exit.

## Out of scope

- Database persistence
- User interface
- Real payment gateways
- Physical gate, sensor, or display integrations
- Distributed deployment
- Concurrent requests in the initial version

## Assumptions

- A vehicle occupies exactly one parking spot.
- Motorcycles can use Motorcycle, Compact, or Large spots.
- Cars prefer Compact spots but can use Large spots.
- Trucks can use only Large spots.
- The allocation strategy initially returns the first compatible spot.
- Parking duration is rounded up to the next complete hour.
- A spot is released only after successful payment.
- A ticket cannot be used twice.
- All IDs only need to be unique during the current program execution.

## Main use cases

### Park a vehicle

1. A vehicle arrives at an entry gate.
2. The system searches for a compatible available spot.
3. The spot is marked occupied.
4. A ticket is generated and stored.
5. The ticket is returned to the driver.

### Exit the parking lot

1. The driver submits the ticket at an exit gate.
2. The system calculates the parking duration.
3. The configured pricing strategy calculates the fee.
4. Payment is processed.
5. The ticket is closed.
6. The parking spot is released.


problems/parking_lot/
├── README.md
├── CMakeLists.txt
├── include/parking_lot/
│   ├── types.hpp
│   ├── vehicle.hpp
│   ├── parking_spot.hpp
│   ├── parking_floor.hpp
│   ├── parking_ticket.hpp
│   ├── parking_lot.hpp
│   ├── spot_allocation_strategy.hpp
│   ├── pricing_strategy.hpp
│   └── payment_processor.hpp
├── src/
│   ├── parking_spot.cpp
│   ├── parking_floor.cpp
│   ├── parking_lot.cpp
│   ├── spot_allocation_strategy.cpp
│   ├── pricing_strategy.cpp
│   └── payment_processor.cpp
├── app/
│   └── main.cpp
└── tests/
    ├── parking_spot_test.cpp
    ├── parking_lot_test.cpp
    └── pricing_strategy_test.cpp