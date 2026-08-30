#include <bits/stdc++.h>

#include "Payment.h"
using namespace std;
class Netbanking : public Payment {
    void payment(int amount) {
        cout << "Choosen method is NetBanking and the amount paid is " << amount;
    }
};