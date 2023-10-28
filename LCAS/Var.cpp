//
// Var.cpp
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//

#include <iostream>
#include "Var.hpp"
#include "Expr.hpp"

Var::Var(std::string identifier): name(identifier) {
    simple = true;
}

Expr* Var::simplify() {
    // Nothing to simplify here
    return this;
}

Expr* Var::copy() {
    return new Var(name);
}

long Var::generateHash() {
    long ex = 1;
    long sum = 0;

    for (int i = 0; i < name.length(); i++) {
        sum += (name.at(i)-'0')*ex;
        ex *= 63;
    }

    return sum;
}

bool Var::equalStruct(Expr* other)  {
    Var* otherVar = (Var*)other;

    if (otherVar) {
        // If 'other' is also a Var, compare their values
        return this->name == otherVar->name;
    }

    // If 'other' is not a Var, their structures are not equal
    return false;
}

std::string Var::getLatex() {
    return this->name;
}

void Var::print() {
    std::cout << name;
}
