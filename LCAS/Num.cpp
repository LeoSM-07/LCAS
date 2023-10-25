//
// Num.cpp
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//


#include "Expr.hpp"
#include "Num.hpp"
#include <iostream>

Num::Num(int val) : value(val) {
    getLatex();
}

Expr* Num::simplify() {
    // Num itself is already in its simplest form
    return this;
}

Expr* Num::copy() {
    return new Num(value);
}

bool Num::equalStruct(Expr* other) {
    Num* otherNum = dynamic_cast<Num*>(other);

    if (otherNum) {
        // If 'other' is also a Num, compare their values
        return this->value == otherNum->value;
    }

    // If 'other' is not a Num, their structures are not equal
    return false;
}

std::string Num::getLatex() {
    return std::to_string(this->value);
}

void Num::print() {
    std::cout << value;
}
