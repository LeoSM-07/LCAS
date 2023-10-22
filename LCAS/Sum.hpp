//
// Sum.hpp
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//

#ifndef Sum_hpp
#define Sum_hpp

#include <stdio.h>
#include "Expr.hpp"

class Sum : public Expr {
public:
    Sum();
    Sum(Expr* leftExpr, Expr* rightExpr);

    Expr* simplify() override;

    void addIntegers(Sum* sum);
    void addLikeTerms(Sum* sum);
    Expr* alone(Sum* sum);

    Expr* copy() override;

    bool equalStruct(Expr* other) override;

    void print() override;
};

#endif /* Sum_hpp */
