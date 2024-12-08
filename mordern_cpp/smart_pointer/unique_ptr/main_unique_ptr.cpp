#include <iostream>
#include <memory>

class Investment {
public:
    Investment() {
        std::cout << "Investment constructed!" << std::endl;
    }

    virtual ~Investment() {
        std::cout << "Investment destructed!" << std::endl;
    }
};

class Stock : public Investment {
public:
    Stock(int a, int b) {
        std::cout << "Stock constructed!" << std::endl;
        std::cout << a << " " << b << std::endl;
    }

    ~Stock() override {
        std::cout << "Stock destructed!" << std::endl;
    }
};

class State : public Investment {
public:
    State(int a, int b) {
        std::cout << "State constructed!" << std::endl;
        std::cout << a << " " << b << std::endl;
    }

    ~State() override {
        std::cout << "State destructed!" << std::endl;
    }
};

enum class InvestmentType {
    Stock,
    State
};

template <typename... Ts>
std::unique_ptr<Investment> makeInvstmPtr(InvestmentType investType, Ts&&... params) {
    if (investType == InvestmentType::Stock) {
        return std::make_unique<Stock>(std::forward<Ts>(params)...);
    } else if (investType == InvestmentType::State) {
        return std::make_unique<State>(std::forward<Ts>(params)...);
    }
    throw std::invalid_argument("Invalid InvestmentType");
}

int main() {
    std::cout << "Hello" << std::endl;
    auto invest_1 = makeInvstmPtr(InvestmentType::Stock, 2, 3);
    auto invest_2 = makeInvstmPtr(InvestmentType::State, 2, 3);
    return 0;
}
