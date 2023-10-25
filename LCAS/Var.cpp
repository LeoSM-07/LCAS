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
    getLatex();
}

Expr* Var::simplify() {
    // Nothing to simplify here
    return this;
}

Expr* Var::copy() {
    return new Var(name);
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
