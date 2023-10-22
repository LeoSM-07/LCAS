//
// main.cpp
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//

#include <iostream>
#include <vector>
#include <string>

class Expr {
public:
    std::vector<Expr*> subExpr;

    virtual Expr* simplify() = 0;
    virtual Expr* copy() = 0;
    virtual bool equalStruct(Expr* other) = 0;
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

    bool equalStruct(Expr* other) override {
        Num* otherNum = (Num*)other;

        if (otherNum) {
            // If 'other' is also a Num, compare their values
            return this->value == otherNum->value;
        }

        // If 'other' is not a Num, their structures are not equal
        return false;
    }

    void print() override {
        std::cout << value;
    }
};

class Prod: public Expr {
public:
    Prod() { }
    Prod(Expr* first, Expr* second) {
        subExpr.push_back(first);
        subExpr.push_back(second);
    }

    Expr* simplify() override {
        Expr* toBeSimplified = copy();

        multiplyIntegers((Prod*)toBeSimplified);
        toBeSimplified = alone((Prod*)toBeSimplified);
        return toBeSimplified;
    }

    void multiplyIntegers(Prod* prod) {
        // Check to see if there are enough Num to multiply
        int numCount = 0;
        for (Expr* sub : subExpr) {
            if(Num* expr = (Num*)sub) numCount++;
            if (numCount > 0) break;
        }

        if (numCount < 1) return;

        // Preform the operation if there are enough numbers

        int total = 1;

        for (int i = 0; i < prod->subExpr.size(); i++) {
            Num* expr = dynamic_cast<Num*>(prod->subExpr[i]);

            if (expr) {
                total = total * expr->value;
                prod->subExpr.erase(prod->subExpr.begin()+i);
                i--;
            }
        }

        if(total != 1) prod->subExpr.push_back(new Num(total));

    }

    Expr* alone(Prod* prod) {
        // if the product has only 1 element
        if (prod->subExpr.size() == 1) {
            return prod->subExpr[0];
        // if the product is empty, return 1
        } else if (prod->subExpr.size() == 0) {
            return new Num(1);
        }
        return prod;
    }

    Expr* copy() override {
        Expr* prodCopy = new Prod();

        for (Expr* sub : subExpr) {
            prodCopy->subExpr.push_back(sub->copy());
        }

        return prodCopy;
    }

    bool equalStruct(Expr* other) override {
        // Check if 'other' is of the same type as Prod
        Prod* otherProd = (Prod*)other;

        if (otherProd) {
            return this->subExpr.size() == otherProd->subExpr.size() &&
                   this->subExpr[0]->equalStruct(otherProd->subExpr[0]) &&
                   this->subExpr[1]->equalStruct(otherProd->subExpr[1]);
        }

        return false;
    }

    void print() override {
        for (int i = 0; i < subExpr.size(); i++) {
            subExpr[i]->print();
            if (i != (subExpr.size()-1))
                std::cout << " * ";
        }
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
        Expr* toBeSimplified = copy();

        addLikeTerms((Sum*)toBeSimplified);
        addIntegers((Sum*)(toBeSimplified));
        toBeSimplified = alone((Sum*)(toBeSimplified));

        return toBeSimplified;
    }

    void addIntegers(Sum* sum) {
        // Pre checks for optimization
        int numCount = 0;
        for (Expr* sub : subExpr) {
            if(Num* expr = dynamic_cast<Num*>(sub)) numCount++;
            if (numCount > 0) break;
        }

        if (numCount == 0) return;

        int total = 0;
        for (int i = 0; i < sum->subExpr.size(); i++) {
            Num* expr = (Num*)(sum->subExpr[i]);

            if (expr) {
                total = total + expr->value;
                sum->subExpr.erase(sum->subExpr.begin()+i);
                i--;
            }
        }

        if (total != 0) sum->subExpr.push_back(new Num(total));
    }

    void addLikeTerms(Sum* sum) {
        for(int i = 0; i < sum->subExpr.size(); i++) {
            Expr* current = sum->subExpr[i]->copy();
            int coef = 1; // the coefficient

            // if the term is a product
            if (dynamic_cast<Prod*>(current)) {
                // loop over each part of the product
                for(int j = 0; j < current->subExpr.size(); j++) {
                    // if the part is a number
                    if(dynamic_cast<Num*>(current->subExpr[j])) {
                        coef = ((Num*)(current->subExpr[j]))->value;
                        current->subExpr.erase(current->subExpr.begin()+j);
                        current->simplify();
                        break;
                    }
                }
            }

            bool foundSame = false;
            for(int j = i+1; j < sum->subExpr.size(); j++) {
                Expr* toComp = sum->subExpr[j]->copy();
                int toCompCoef = 1;
                

                if (dynamic_cast<Prod*>(toComp)) {
                    for(int k = 0; k < toComp->subExpr.size(); k++) {
                        Expr* partOfProd = toComp->subExpr[k];
                        if (dynamic_cast<Num*>(partOfProd)) {
                            toCompCoef = ((Num*)partOfProd)->value;
                            toComp->subExpr.erase(toComp->subExpr.begin()+k);
                            toComp = toComp->simplify();
                            break;
                        }
                    }
                }

                if (current->equalStruct(toComp)) {
                    sum->subExpr.erase(sum->subExpr.begin()+j);
                    j--;
                    foundSame = true;
                    coef = coef + toCompCoef;
                }
            }

            if (foundSame) {
                if (dynamic_cast<Prod*>(current)) {
                    current->subExpr.push_back(new Num(coef));
                } else {
                    current = new Prod(current, new Num(coef));
                }

                current = current->simplify();
                sum->subExpr[i] = current;
            }
        }
    }

    Expr* alone(Sum* sum) {
        if (sum->subExpr.size() == 1) {
            return sum->subExpr[0];
        } else if (sum->subExpr.size() == 0) {
            return new Num(0);
        }
        return sum;
    }

    Expr* copy() override {
        Sum* copySum = new Sum();
        for (Expr* sub : subExpr) {
            copySum->subExpr.push_back(sub->copy());
        }
        return copySum;
    }

    bool equalStruct(Expr* other) override {
        // Check if 'other' is of the same type as Sum
        Sum* otherSum = (Sum*)other;

        if (otherSum) {
            // If 'other' is also a Sum, compare their sub-expressions
            return this->subExpr.size() == otherSum->subExpr.size() &&
                   this->subExpr[0]->equalStruct(otherSum->subExpr[0]) &&
                   this->subExpr[1]->equalStruct(otherSum->subExpr[1]);
        }

        // If 'other' is not a Sum, their structures are not equal
        return false;
    }

    void print() override {
        for (int i = 0; i < subExpr.size(); i++) {
            Expr* sub = (Expr*)subExpr[i];

            if (sub) {
                subExpr.at(i)->print();
                if (i != (subExpr.size()-1)) std::cout << " + ";
            }
        }
    }

};

class Var: public Expr {
public:
    std::string name;

    Var(std::string identifier): name(identifier) { }

    Expr* simplify() override {
        // Nothing to simplify here
        return this;
    }

    Expr* copy() override {
        return new Var(name);
    }

    bool equalStruct(Expr* other) override {
        Var* otherVar = (Var*)other;

        if (otherVar) {
            // If 'other' is also a Var, compare their values
            return this->name == otherVar->name;
        }

        // If 'other' is not a Var, their structures are not equal
        return false;
    }

    void print() override {
        std::cout << name;
    }

};



int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    Expr* result;

    Sum sumExpr2(new Var("x"), new Prod(new Num(2), new Var("x")));
    result = sumExpr2.simplify();
    sumExpr2.print();
    std::cout << " = ";
    result->print();
    std::cout << std::endl;

    return 0;
}
