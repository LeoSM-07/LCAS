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

    simple = true;
}

void Expr::sort() {
    for (int i = 0; i < subExpr.size() - 1; i++) {
        for (int j = 0; j < subExpr.size() - 2 - i; j++) {
            if (subExpr[i]->generateHash() > subExpr[j + 1]->generateHash()) {
                Expr* temp = subExpr[j];
                subExpr[j] = subExpr[j + 1];
                subExpr[j + 1] = temp;
            }
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


