#pragma once

#include <bits/stdc++.h>
using namespace std;
class Payment {
   public:
    virtual ~Payment() = default;
    virtual void payment(int amount) = 0;
};
