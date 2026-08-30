#pragma once

#include <algorithm>

#include "Fare.h"

class HourlyRateFare : public Fare {
    int perHourRate;

   public:
    explicit HourlyRateFare(int perHourRate = 10);
    int calculateFare(int duration) override;
};
