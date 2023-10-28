//
// Expr.cpp
// LCAS
//
// Created by LeoSM_07 on 10/27/23.
//


#include "Expr.hpp"
#include <stdio.h>

void Expr::simplifyChildren() {
    for (int i = 0; i < subExpr.size(); i++) {
        Expr* sub = subExpr[i];

        if (!sub->simple) {
            sub = sub->simplify();
            subExpr[i] = sub;
        }
    }
}

void Expr::push_back(Expr* e) {
    simple = false;
    subExpr.push_back(e);
}

void Expr::erase(int index) {
    simple = false;
    subExpr.erase(subExpr.begin()+index);
}

void Expr::set(int index, Expr* e) {
    simple = false;
    subExpr.at(index) = e;
}

Expr* Expr::at(int index) {
    return subExpr.at(index);
}

Expr* Expr::first() {
    return subExpr.at(0);
}

size_t Expr::size() {
    return subExpr.size();
}


