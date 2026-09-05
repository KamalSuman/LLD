#include <bits/stdc++.h>
using namespace std;

enum class Coin { NICKEL, DIME, DOLLAR, QUARTER };
double getCoinValue(Coin coin) {
    if (coin == Coin::NICKEL) return 0.05;
    if (coin == Coin::DIME) return 0.10;
    if (coin == Coin::QUARTER) return 0.25;
    return 1.00;
}
class Product {
    string productId;
    int quantity;
    double price;

   public:
    Product(string productId, int quantity, double price) {
        this->productId = productId;
        this->quantity = quantity;
        this->price = price;
    }

    string GetProductId() const { return productId; }

    void SetProductId(string productId) { this->productId = productId; }

    int GetQuantity() const { return quantity; }

    void SetQuantity(int quantity) { this->quantity = quantity; }

    double GetPrice() const { return price; }

    void SetPrice(double price) { this->price = price; }
};

class State {
   public:
    virtual void insertMoney(Coin coin) = 0;
    virtual void selectProduct(string productId, int qty) = 0;
    virtual void dispense() = 0;
    virtual void cancel() = 0;

    virtual ~State() = default;
};

class Inventory {
    unordered_map<string, Product*> products;

   public:
    void addNewProduct(string productId, int quantity, double price) {
        if (products.find(productId) != products.end()) {
            cout << "Product already exists" << endl;
            return;
        }

        products.emplace(productId, new Product(productId, quantity, price));
    }

    Product* getProduct(string productId) {
        auto it = products.find(productId);

        if (it == products.end()) return nullptr;

        return it->second;
    }

    bool allOutOfStock() {
        for (auto& it : products) {
            if (it.second->GetQuantity() > 0) return false;
        }

        return true;
    }

    ~Inventory() {
        for (auto& it : products) {
            delete it.second;
        }
    }
};
class CoinInventory {
    unordered_map<Coin, int> coins;

   public:
    void addCoins(Coin coin, int qty) { coins[coin] += qty; }

    bool removeCoins(Coin coin, int qty) {
        if (coins[coin] < qty) return false;

        coins[coin] -= qty;
        return true;
    }

    int getCoinCount(Coin coin) { return coins[coin]; }
};
// FORWARD DECLARATION

class VendingMachine;

// STATE CLASS DECLARATIONS

class IdleState : public State {
    VendingMachine* vendingMachine;

   public:
    IdleState(VendingMachine* vendingMachine);

    void insertMoney(Coin coin) override;

    void selectProduct(string productId, int qty) override;

    void dispense() override;

    void cancel() override;
};

class HasMoneyState : public State {
    VendingMachine* vendingMachine;

   public:
    HasMoneyState(VendingMachine* vendingMachine);

    void insertMoney(Coin coin) override;

    void selectProduct(string productId, int qty) override;

    void dispense() override;

    void cancel() override;
};

class DispenseState : public State {
    VendingMachine* vendingMachine;

   public:
    DispenseState(VendingMachine* vendingMachine);

    void insertMoney(Coin coin) override;

    void selectProduct(string productId, int qty) override;

    void dispense() override;

    void cancel() override;
};

class OutOfStockState : public State {
    VendingMachine* vendingMachine;

   public:
    OutOfStockState(VendingMachine* vendingMachine);

    void insertMoney(Coin coin) override;

    void selectProduct(string productId, int qty) override;

    void dispense() override;

    void cancel() override;
};

// VENDING MACHINE

class VendingMachine {
    double balance = 0.0;

    Inventory* inventory;
    CoinInventory coinInventory;
    unordered_map<string, int> selectedProducts;

    IdleState* idleState;
    HasMoneyState* hasMoneyState;
    DispenseState* dispenseState;
    OutOfStockState* outOfStockState;

    State* currentState;

   public:
    VendingMachine(Inventory* inventory);
    void acceptCoin(Coin coin) {
        coinInventory.addCoins(coin, 1);
        addBalance(getCoinValue(coin));
    }
    // USER ACTIONS

    void insertMoney(Coin coin) { currentState->insertMoney(coin); }

    void selectProduct(string productId, int qty) { currentState->selectProduct(productId, qty); }

    void dispense() { currentState->dispense(); }

    void cancel() { currentState->cancel(); }

    // STATE RELATED METHODS

    void setState(State* state) { currentState = state; }

    IdleState* getIdleState() { return idleState; }

    HasMoneyState* getHasMoneyState() { return hasMoneyState; }

    DispenseState* getDispenseState() { return dispenseState; }

    OutOfStockState* getOutOfStockState() { return outOfStockState; }

    // BALANCE RELATED METHODS

    double getBalance() { return balance; }

    void addBalance(double amount) { balance += amount; }

    // ACTUAL PRODUCT SELECTION LOGIC

    bool selectProductsInternal(string productId, int qty) {
        Product* product = inventory->getProduct(productId);

        if (product == nullptr) {
            cout << "Product not found" << endl;
            return false;
        }

        if (qty <= 0) {
            cout << "Invalid quantity" << endl;
            return false;
        }

        int alreadySelected = 0;

        auto it = selectedProducts.find(productId);

        if (it != selectedProducts.end()) {
            alreadySelected = it->second;
        }

        int availableQty = product->GetQuantity() - alreadySelected;

        if (availableQty <= 0) {
            cout << "Product is out of stock" << endl;

            return false;
        }

        double selectedCost = 0.0;

        for (auto& it : selectedProducts) {
            Product* selectedProduct = inventory->getProduct(it.first);

            selectedCost += selectedProduct->GetPrice() * it.second;
        }

        double availableBalance = balance - selectedCost;

        int finalQty = min(qty, availableQty);

        if (qty > availableQty) {
            cout << "Only " << availableQty << " more available" << endl;
        }

        double cost = finalQty * product->GetPrice();

        if (availableBalance < cost) {
            cout << "Not enough balance!" << endl;

            return false;
        }

        selectedProducts[productId] += finalQty;

        cout << productId << " selected quantity: " << finalQty << endl;

        return true;
    }

    // ACTUAL DISPENSING LOGIC

    bool dispenseInternal() {
        if (selectedProducts.empty()) {
            cout << "Please select the product first!" << endl;

            return false;
        }

        double totalCost = 0.0;

        for (auto& it : selectedProducts) {
            string productId = it.first;

            Product* product = inventory->getProduct(productId);

            int currentQty = product->GetQuantity();

            product->SetQuantity(currentQty - it.second);

            totalCost += it.second * product->GetPrice();

            cout << "Dispensing " << productId << " x" << it.second << endl;
        }

        double change = balance - totalCost;

        cout << "Please collect the item from the tray" << endl;

        if (change > 0) {
            cout << "Please collect balance: " << change << endl;
        }

        balance = 0.0;

        selectedProducts.clear();

        return true;
    }

    // ACTUAL CANCEL LOGIC

    void cancelInternal() {
        cout << "Transaction cancelled. Please collect " << balance << endl;

        balance = 0.0;

        selectedProducts.clear();
    }

    bool isOutOfStock() { return inventory->allOutOfStock(); }

    ~VendingMachine() {
        delete idleState;
        delete hasMoneyState;
        delete dispenseState;
        delete outOfStockState;
    }
};

// VENDING MACHINE CONSTRUCTOR

VendingMachine::VendingMachine(Inventory* inventory) : inventory(inventory) {
    idleState = new IdleState(this);

    hasMoneyState = new HasMoneyState(this);

    dispenseState = new DispenseState(this);

    outOfStockState = new OutOfStockState(this);

    currentState = idleState;
}

// IDLE STATE

IdleState::IdleState(VendingMachine* vendingMachine) : vendingMachine(vendingMachine) {}

void IdleState::selectProduct(string productId, int qty) { cout << "First insert money!" << endl; }

void IdleState::dispense() { cout << "First insert money and select item!" << endl; }

void IdleState::cancel() { cout << "Nothing to cancel" << endl; }

// HAS MONEY STATE

HasMoneyState::HasMoneyState(VendingMachine* vendingMachine) : vendingMachine(vendingMachine) {}

void IdleState::insertMoney(Coin coin) {
    vendingMachine->acceptCoin(coin);
    vendingMachine->setState(vendingMachine->getHasMoneyState());
}

void HasMoneyState::insertMoney(Coin coin) { vendingMachine->acceptCoin(coin); }

void HasMoneyState::selectProduct(string productId, int qty) {
    vendingMachine->selectProductsInternal(productId, qty);
}

void HasMoneyState::dispense() {
    vendingMachine->setState(vendingMachine->getDispenseState());

    vendingMachine->dispense();
}

void HasMoneyState::cancel() {
    vendingMachine->cancelInternal();

    vendingMachine->setState(vendingMachine->getIdleState());
}

// DISPENSE STATE

DispenseState::DispenseState(VendingMachine* vendingMachine) : vendingMachine(vendingMachine) {}

void DispenseState::insertMoney(Coin coin) {
    cout << "Cannot add money during dispensing!" << endl;
}

void DispenseState::selectProduct(string productId, int qty) {
    cout << "Cannot select product during dispensing!" << endl;
}

void DispenseState::dispense() {
    bool success = vendingMachine->dispenseInternal();

    if (!success) {
        vendingMachine->setState(vendingMachine->getHasMoneyState());

        return;
    }

    if (vendingMachine->isOutOfStock()) {
        vendingMachine->setState(vendingMachine->getOutOfStockState());

    } else {
        vendingMachine->setState(vendingMachine->getIdleState());
    }
}

void DispenseState::cancel() { cout << "Cannot cancel during dispensing!" << endl; }

// OUT OF STOCK STATE

OutOfStockState::OutOfStockState(VendingMachine* vendingMachine) : vendingMachine(vendingMachine) {}

void OutOfStockState::insertMoney(Coin coin) { cout << "Machine is out of stock!" << endl; }

void OutOfStockState::selectProduct(string productId, int qty) {
    cout << "Machine is out of stock!" << endl;
}

void OutOfStockState::dispense() { cout << "Nothing to dispense!" << endl; }

void OutOfStockState::cancel() { cout << "Nothing to cancel!" << endl; }

int main() {
    Inventory inventory;

    inventory.addNewProduct("COKE", 5, 2.0);
    inventory.addNewProduct("CHIPS", 3, 1.5);
    inventory.addNewProduct("WATER", 2, 1.0);

    VendingMachine machine(&inventory);

    cout << "\n--- TEST 1: Normal Purchase ---\n";

    for (int i = 0; i < 10; i++) {
        machine.insertMoney(Coin::DOLLAR);
    }

    machine.selectProduct("COKE", 2);   // cost = 4
    machine.selectProduct("CHIPS", 2);  // cost = 3

    machine.dispense();

    // total cost = 7
    // expected change = 3

    cout << "\n--- TEST 2: Insufficient Balance ---\n";

    machine.insertMoney(Coin::DOLLAR);
    machine.insertMoney(Coin::DOLLAR);

    machine.selectProduct("COKE", 2);  // needs 4

    machine.cancel();

    // expected refund = 2

    cout << "\n--- TEST 3: Invalid Product ---\n";

    for (int i = 0; i < 5; i++) {
        machine.insertMoney(Coin::DOLLAR);
    }

    machine.selectProduct("PEPSI", 1);

    machine.cancel();

    cout << "\n--- TEST 4: Selecting More Than Stock ---\n";

    for (int i = 0; i < 20; i++) {
        machine.insertMoney(Coin::DOLLAR);
    }

    machine.selectProduct("WATER", 5);

    // only 2 available

    machine.dispense();

    cout << "\n--- TEST 5: Same Product Multiple Times ---\n";

    for (int i = 0; i < 20; i++) {
        machine.insertMoney(Coin::DOLLAR);
    }

    machine.selectProduct("COKE", 2);
    machine.selectProduct("COKE", 2);

    machine.dispense();

    return 0;
}