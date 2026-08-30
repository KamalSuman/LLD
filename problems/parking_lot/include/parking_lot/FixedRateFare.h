#include <bits/stdc++.h>

#include "Fare.h"
using namespace std;

class FixedRateFare : public Fare {
    int hourDivisionFee = 4;

   public:
    int calculateFare(int duration) {
        int hours = duration / 60;
        int set = (hours + 3) / 4;
        return set * 10;
    }
};