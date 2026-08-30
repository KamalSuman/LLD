#include <bits/stdc++.h>

#include "Payment.h"
using namespace std;
class UpiPayment : public Payment {
    void payment(int amount) { cout << "Choosen method is UPI and the amount paid is " << amount; }
};