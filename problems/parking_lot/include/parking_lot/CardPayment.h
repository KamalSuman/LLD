#pragma once

#include "Payment.h"

class CardPayment : public Payment {
   public:
    void payment(int amount) override;
};
