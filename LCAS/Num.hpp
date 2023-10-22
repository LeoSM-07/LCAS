//
// Num.h
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//

#ifndef Num_h
#define Num_h

#include "Expr.hpp"

class Num : public Expr {
public:
    int value;

    Num(int val);

    Expr* simplify() override;

    Expr* copy() override;

    bool equalStruct(Expr* other) override;

    void print() override;
};

#endif /* Num_h */
