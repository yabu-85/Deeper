#include "MoneyManager.h"

namespace MoneyManager {
    
    int money_;

    void AddMoney(int i) { money_ += i; }
    int GetMoney() { return money_; }

}


