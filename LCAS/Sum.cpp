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

#define LATEX_ADDITION_SIGN "+"

class Prod;

Sum::Sum(){};

Sum::Sum(Expr* leftExpr, Expr* rightExpr) {
    push_back(leftExpr);
    push_back(rightExpr);
}

Expr* Sum::simplify() {
    Expr* toBeSimplified = copy();

    toBeSimplified->simplifyChildren();

    addLikeTerms((Sum*)toBeSimplified);
    addIntegers((Sum*)(toBeSimplified));
    toBeSimplified = alone((Sum*)(toBeSimplified));


    toBeSimplified->simple = true;

    return toBeSimplified;
}

void Sum::addIntegers(Sum* sum) {
    // Pre checks for optimization
    int numCount = 0;
    for (int i = 0; i < size(); i++) {
        Expr* sub = at(i);
        if(Num* expr = dynamic_cast<Num*>(sub)) numCount++;
        if (numCount > 0) break;
    }

    if (numCount == 0) return;

    int total = 0;
    for (int i = 0; i < sum->size(); i++) {
        Num* expr = dynamic_cast<Num*>(sum->at(i));

        if (expr) {
            total = total + expr->value;
            sum->erase(i);
            i--;
        }
    }

    if (total != 0) sum->push_back(new Num(total));
}

void Sum::addLikeTerms(Sum* sum) {
    for(int i = 0; i < sum->size(); i++) {
        Expr* current = sum->at(i)->copy();
        int coef = 1; // the coefficient

        // if the term is a product
        if (dynamic_cast<Prod*>(current)) {
            // loop over each part of the product
            for(int j = 0; j < current->size(); j++) {
                // if the part is a number
                if(dynamic_cast<Num*>(current->at(j))) {
                    coef = ((Num*)(current->at(j)))->value;
                    current->erase(j);
                    current = current->simplify();
                    break;
                }
            }
        }

        bool foundSame = false;
        for(int j = i+1; j < sum->size(); j++) {
            Expr* toComp = sum->at(j)->copy();
            int toCompCoef = 1;

            if (dynamic_cast<Prod*>(toComp)) {
                for(int k = 0; k < toComp->size(); k++) {
                    Expr* partOfProd = toComp->at(k);
                    if (dynamic_cast<Num*>(partOfProd)) {
                        toCompCoef = ((Num*)partOfProd)->value;
                        toComp->erase(k);
                        toComp = toComp->simplify();
                        break;
                    }
                }
            }

            if (current->equalStruct(toComp)) {
                sum->erase(j);
                j--;
                foundSame = true;
                coef = coef + toCompCoef;
            }
        }

        if (foundSame) {
            if (dynamic_cast<Prod*>(current)) {
                current->push_back(new Num(coef));
            } else {
                current = new Prod(current, new Num(coef));
            }

            current = current->simplify();
            sum->set(i, current);
        }
    }
}

Expr* Sum::alone(Sum* sum) {
    if (sum->size() == 1) {
        return sum->at(0);
    } else if (sum->size() == 0) {
        return new Num(0);
    }
    return sum;
}

Expr* Sum::copy() {
    Sum* copySum = new Sum();
    for (int i = 0; i < size(); i++) {
        Expr* sub = at(i);
        copySum->push_back(sub->copy());
    }
    return copySum;
}

long Sum::generateHash() {
    long sum = 1;
    
    for (int i = 0; i < size(); i++) {
        sum += at(i)->generateHash();
    }

    return sum-926481637408623462L;
}

bool Sum::equalStruct(Expr* other) {
    // Check if 'other' is of the same type as Sum
    Sum* otherSum = (Sum*)other;

    if (otherSum) {
        // If 'other' is also a Sum, compare their sub-expressions
        return this->size() == otherSum->size() &&
        this->at(0)->equalStruct(otherSum->at(0)) &&
        this->at(1)->equalStruct(otherSum->at(1));
    }

    // If 'other' is not a Sum, their structures are not equal
    return false;
}

std::string Sum::getLatex() {
    std::string latex = "";
    for (int i = 0; i < size(); i++) {
        Expr* sub = (Expr*) at(i);

        if (sub) {
            latex.append(at(i)->getLatex());
            if (i != (size()-1)) latex.append(LATEX_ADDITION_SIGN);
        }
    }
    return latex;
}

void Sum::print() {
    for (int i = 0; i < size(); i++) {
        Expr* sub = (Expr*) at(i);

        if (sub) {
            at(i)->print();
            if (i != (size()-1)) std::cout << " + ";
        }
    }
}
