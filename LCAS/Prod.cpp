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
    subExpr.push_back(first);
    subExpr.push_back(second);
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

void Prod::multiplyLikeTerms(Prod* prod) {
    for (int i = 0; i < prod->subExpr.size(); i++) {
        Expr* current = prod->subExpr[i];
        Expr* expo = new Sum();

        Power* currentPower = dynamic_cast<Power*>(current);
        if (currentPower) {
            current = currentPower->getBase();
            expo->subExpr.push_back(currentPower->getExpo());
        } else expo->subExpr.push_back(new Num(1));

        bool found = false;
        for (int j = i+1; j < prod->subExpr.size(); j++) {
            Expr* other = prod->subExpr[j];

            if (other->equalStruct(current)) {
                expo->subExpr.push_back(new Num(1L));
                prod->subExpr.erase(prod->subExpr.begin() + j);
                j--;
                found = true;
            } else if (dynamic_cast<Power*>(other)) {
                Power* otherPower = dynamic_cast<Power*>(other);

                if (otherPower->getBase()->equalStruct(current)) {
                    expo->subExpr.push_back(otherPower->getExpo());
                    prod->subExpr.erase(prod->subExpr.begin() + j);
                    j--;
                    found = true;
                }
            }
        }

        if (found) {
            expo = expo->simplify();
            prod->subExpr[i] = new Power(current, expo);
        }
        
    }
}

Expr* Prod::alone(Prod* prod) {
    // if the product has only 1 element
    if (prod->subExpr.size() == 1) {
        return prod->subExpr[0];
    // if the product is empty, return 1
    } else if (prod->subExpr.size() == 0) {
        return new Num(1);
    }
    return prod;
}

Expr* Prod::copy() {
    Expr* prodCopy = new Prod();

    for (Expr* sub : subExpr) {
        prodCopy->subExpr.push_back(sub->copy());
    }

    return prodCopy;
}

bool Prod::equalStruct(Expr* other) {
    // Check if 'other' is of the same type as Prod
    Prod* otherProd = (Prod*)other;

    if (otherProd) {
        return this->subExpr.size() == otherProd->subExpr.size() &&
               this->subExpr[0]->equalStruct(otherProd->subExpr[0]) &&
               this->subExpr[1]->equalStruct(otherProd->subExpr[1]);
    }

    return false;
}

std::string Prod::getLatex() {
    std::vector<Num*> nums;
    std::vector<Var*> vars;
    std::string latex = "";

    for (int i = 0; i < subExpr.size(); i++) {
        if (dynamic_cast<Var*>(subExpr[i])) vars.push_back(dynamic_cast<Var*>(subExpr[i]));
        else if (dynamic_cast<Num*>(subExpr[i])) nums.push_back(dynamic_cast<Num*>(subExpr[i]));
    }

    // Special ordering for single coefficients on variables
    if (vars.size() == 1 && nums.size() == 1 && subExpr.size() == 2) {
        for (Num* num : nums) {
            latex.append(num->getLatex());
        }
        for (Var* var : vars) {
            latex.append(var->getLatex());
        }
    } else {
        for (int i = 0; i < subExpr.size(); i++) {
            latex.append(subExpr[i]->getLatex());
            if (i != (subExpr.size()-1))
                latex.append(LATEX_MULTIPLICATION_SIGN);
        }
    }

    return latex;
}

void Prod::print() {
    for (int i = 0; i < subExpr.size(); i++) {
        subExpr[i]->print();
        if (i != (subExpr.size()-1))
            std::cout << "*";
    }
}
