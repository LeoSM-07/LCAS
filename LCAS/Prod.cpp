//
// Prod.cpp
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//

#include <iostream>
#include <vector>
#include "Prod.hpp"
#include "Num.hpp"
#include "Var.hpp"
#include "Power.hpp"
#include "Sum.hpp"

#define LATEX_MULTIPLICATION_SIGN "\\cdot "

Prod::Prod() { }

Prod::Prod(Expr* first, Expr* second) {
    push_back(first);
    push_back(second);
    getLatex();
}

Expr* Prod::simplify() {
    Expr* toBeSimplified = copy();
    
    multiplyLikeTerms((Prod*)toBeSimplified);
    multiplyIntegers((Prod*)toBeSimplified);
    toBeSimplified = alone((Prod*)toBeSimplified);
//    toBeSimplified->setLatex();

    return toBeSimplified;
}

void Prod::multiplyIntegers(Prod* prod) {
    // Check to see if there are enough Num to multiply
    int numCount = 0;
    for (int i = 0; i < size(); i++) {
        Expr* sub = at(i);
        if(Num* expr = (Num*)sub) numCount++;
        if (numCount > 0) break;
    }

    if (numCount < 1) return;

    // Preform the operation if there are enough numbers

    int total = 1;

    for (int i = 0; i < prod->size(); i++) {
        Num* expr = dynamic_cast<Num*>(prod->at(i));

        if (expr) {
            total = total * expr->value;
            prod->erase(i);
            i--;
        }
    }

    if(total != 1) prod->push_back(new Num(total));
}

void Prod::multiplyLikeTerms(Prod* prod) {
    for (int i = 0; i < prod->size(); i++) {
        Expr* current = prod->at(i);
        Expr* expo = new Sum();

        Power* currentPower = dynamic_cast<Power*>(current);
        if (currentPower) {
            current = currentPower->getBase();
            expo->push_back(currentPower->getExpo());
        } else expo->push_back(new Num(1));

        bool found = false;
        for (int j = i+1; j < prod->size(); j++) {
            Expr* other = prod->at(j);

            if (other->equalStruct(current)) {
                expo->push_back(new Num(1L));
                prod->erase(j);
                j--;
                found = true;
            } else if (dynamic_cast<Power*>(other)) {
                Power* otherPower = dynamic_cast<Power*>(other);

                if (otherPower->getBase()->equalStruct(current)) {
                    expo->push_back(otherPower->getExpo());
                    prod->erase(j);
                    j--;
                    found = true;
                }
            }
        }

        if (found) {
            expo = expo->simplify();
            prod->set(i, new Power(current, expo));
        }
        
    }
}

Expr* Prod::alone(Prod* prod) {
    // if the product has only 1 element
    if (prod->size() == 1) {
        return prod->at(0);
    // if the product is empty, return 1
    } else if (prod->size() == 0) {
        return new Num(1);
    }
    return prod;
}

Expr* Prod::copy() {
    Expr* prodCopy = new Prod();

    for (int i = 0; i < size(); i++) {
        Expr* sub = at(i);
        prodCopy->push_back(sub->copy());
    }

    return prodCopy;
}

bool Prod::equalStruct(Expr* other) {
    // Check if 'other' is of the same type as Prod
    Prod* otherProd = (Prod*)other;

    if (otherProd) {
        return this->size() == otherProd->size() &&
               this->at(0)->equalStruct(otherProd->at(0)) &&
               this->at(1)->equalStruct(otherProd->at(1));
    }

    return false;
}

std::string Prod::getLatex() {
    std::vector<Num*> nums;
    std::vector<Var*> vars;
    std::string latex = "";

    for (int i = 0; i < size(); i++) {
        if (dynamic_cast<Var*>(at(i))) vars.push_back(dynamic_cast<Var*>(at(i)));
        else if (dynamic_cast<Num*>(at(i))) nums.push_back(dynamic_cast<Num*>(at(i)));
    }

    // Special ordering for single coefficients on variables
    if (vars.size() == 1 && nums.size() == 1 && size() == 2) {
        for (Num* num : nums) {
            latex.append(num->getLatex());
        }
        for (Var* var : vars) {
            latex.append(var->getLatex());
        }
    } else {
        for (int i = 0; i < size(); i++) {
            latex.append(at(i)->getLatex());
            if (i != (size()-1))
                latex.append(LATEX_MULTIPLICATION_SIGN);
        }
    }

    return latex;
}

void Prod::print() {
    for (int i = 0; i < size(); i++) {
        at(i)->print();
        if (i != (size()-1))
            std::cout << "*";
    }
}
