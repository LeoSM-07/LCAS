//
// Sum.cpp
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//

#include <iostream>
#include "Sum.hpp"
#include "Num.hpp"
#include "Prod.hpp"

class Prod;

Sum::Sum(){};

Sum::Sum(Expr* leftExpr, Expr* rightExpr) {
    subExpr.push_back(leftExpr);
    subExpr.push_back(rightExpr);
}

Expr* Sum::simplify() {
    Expr* toBeSimplified = copy();

    addLikeTerms((Sum*)toBeSimplified);
    addIntegers((Sum*)(toBeSimplified));
    toBeSimplified = alone((Sum*)(toBeSimplified));

    return toBeSimplified;
}

void Sum::addIntegers(Sum* sum) {
    // Pre checks for optimization
    int numCount = 0;
    for (Expr* sub : subExpr) {
        if(Num* expr = dynamic_cast<Num*>(sub)) numCount++;
        if (numCount > 0) break;
    }

    if (numCount == 0) return;

    int total = 0;
    for (int i = 0; i < sum->subExpr.size(); i++) {
        Num* expr = dynamic_cast<Num*>(sum->subExpr[i]);

        if (expr) {
            total = total + expr->value;
            sum->subExpr.erase(sum->subExpr.begin()+i);
            i--;
        }
    }

    if (total != 0) sum->subExpr.push_back(new Num(total));
}

void Sum::addLikeTerms(Sum* sum) {
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
                    current = current->simplify();
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

Expr* Sum::alone(Sum* sum) {
    if (sum->subExpr.size() == 1) {
        return sum->subExpr[0];
    } else if (sum->subExpr.size() == 0) {
        return new Num(0);
    }
    return sum;
}

Expr* Sum::copy() {
    Sum* copySum = new Sum();
    for (Expr* sub : subExpr) {
        copySum->subExpr.push_back(sub->copy());
    }
    return copySum;
}

bool Sum::equalStruct(Expr* other) {
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

void Sum::print() {
    for (int i = 0; i < subExpr.size(); i++) {
        Expr* sub = (Expr*)subExpr[i];

        if (sub) {
            subExpr.at(i)->print();
            if (i != (subExpr.size()-1)) std::cout << " + ";
        }
    }
}
