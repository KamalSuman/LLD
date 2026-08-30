#pragma once

#include <bits/stdc++.h>
using namespace std;
class Fare {
   public:
    virtual ~Fare() = default;
    virtual int calculateFare(int amount) = 0;
};
