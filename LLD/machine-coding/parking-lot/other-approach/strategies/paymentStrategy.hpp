#ifndef payment_strategy_hpp
#define payment_strategy_hpp

class PaymentStrategy{
    public:
    virtual double pay() =0;
};


class CreditCard: public PaymentStrategy{
    public:
    double pay(){};
};
class CashPayment: public PaymentStrategy{
    public:
    double pay(){};
};


#endif