//
// Power.cpp
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//

#include <iostream>
#include "Power.hpp"
#include "Expr.hpp"
#include "Num.hpp"
#include "Sum.hpp"
#include "Prod.hpp"

void Power::setBase(Expr* base) { subExpr[0] = base; }
void Power::setExpo(Expr* expo) { subExpr[1] = expo; }

Expr* Power::getBase() { return subExpr[0]; }
Expr* Power::getExpo() { return subExpr[1]; }

Power::Power (Expr* base, Expr* expo) {
    subExpr.push_back(base);
    subExpr.push_back(expo);
}

Expr* Power::simplify() {
    return nullptr;
}

Expr* Power::copy() {
    return new Power(getBase()->copy(), getExpo()->copy());
}

bool Power::equalStruct(Expr* other) {
    Power* otherPower = dynamic_cast<Power*>(other);

    if (otherPower) {
        return  getBase()->equalStruct(otherPower->getBase()) && getExpo()->equalStruct(otherPower->getExpo());
    }

    return false;
}

void Power::print() {
    // Rules to use paren on base/expo:
    //  - Base is negative
    //  - Base is a sum, product, or power
    bool useParenthesisOnBase = false;
    bool useParenthesisOnExpo = false;

    if (dynamic_cast<Sum*>(getBase()) || dynamic_cast<Prod*>(getBase()) || dynamic_cast<Power*>(getBase())) useParenthesisOnBase = true;
    if (dynamic_cast<Num*>(getBase())) {
        Num* baseNum = dynamic_cast<Num*>(getBase());
        if (baseNum->value < 0) useParenthesisOnBase = true;
    }

    if (useParenthesisOnBase) std::cout << "(";
    getBase()->print();
    if (useParenthesisOnBase) std::cout << ")";
    std::cout << "^";

    if (dynamic_cast<Sum*>(getExpo()) || dynamic_cast<Prod*>(getExpo()) || dynamic_cast<Power*>(getExpo())) useParenthesisOnExpo = true;
    if (dynamic_cast<Num*>(getExpo())) {
        Num* baseNum = dynamic_cast<Num*>(getExpo());
        if (baseNum->value < 0) useParenthesisOnExpo = true;
    }

    if (useParenthesisOnBase) std::cout << "(";
    getExpo()->print();
    if (useParenthesisOnBase) std::cout << ")";

}