//
// main.cpp
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//

#include <iostream>
#include <vector>

class Expr {
public:
    std::vector<Expr*> subExpr;

    virtual Expr* simplify() = 0;
    virtual Expr* copy() = 0;
    virtual void print() = 0;

    void println() {
        print();
        std::cout << std::endl;
    }

};


class Num : public Expr {
public:
    int value;

    Num(int val) : value(val) {}

    Expr* simplify() override {
        // Num itself is already in its simplest form
        return this;
    }

    Expr* copy() override {
        return new Num(value);
    }

    void print() override {
        std::cout << value;
    }
};

class Sum: public Expr {
public:
    Sum() { };

    Sum(Expr* leftExpr, Expr* rightExpr) {
        subExpr.push_back(leftExpr);
        subExpr.push_back(rightExpr);
    }

    Expr* simplify() override {
        if (subExpr.size() != 2) {
            // Invalid sum expression, return as is
            return this;
        }

        Expr* simplifiedLeft = subExpr[0]->simplify();
        Expr* simplifiedRight = subExpr[1]->simplify();

        // If both sub-expressions are numerical, evaluate the sum
        if (dynamic_cast<Num*>(simplifiedLeft) && dynamic_cast<Num*>(simplifiedRight)) {
            int sum = dynamic_cast<Num*>(simplifiedLeft)->value + dynamic_cast<Num*>(simplifiedRight)->value;
            return new Num(sum);
        }

        Sum* simplifiedSum = new Sum(simplifiedLeft, simplifiedRight);
        return simplifiedSum;
    }

    Expr* copy() override {
        Sum* copySum = new Sum(nullptr, nullptr);
        for (Expr* sub : subExpr) {
            copySum->subExpr.push_back(sub->copy());
        }
        return copySum;
    }

    void print() override {
        if (subExpr.size() != 2) {
            // Invalid sum expression, print as is
            std::cout << "Invalid Sum Expression";
            return;
        }

        subExpr[0]->print();
        std::cout << " + ";
        subExpr[1]->print();

    }

};

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";

    Num num1(2);
    Num num2(23);
    Sum sumExpr(&num1, &num2);

    Expr* result = sumExpr.simplify();

    sumExpr.print();  // Output: 20 + 22
    std::cout << " = ";
    result->print();  // Output: 42
    std::cout << "\n";


    return 0;
}
