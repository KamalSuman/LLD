#include "HourlyRateFare.h"

#include <algorithm>

HourlyRateFare::HourlyRateFare(const int perHourRate) : perHourRate(perHourRate) {}

int HourlyRateFare::calculateFare(const int duration) {
    const int chargedHours = std::max(1, (duration + 59) / 60);
    return chargedHours * perHourRate;
}
