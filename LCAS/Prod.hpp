//
// Prod.hpp
// LCAS
//
// Created by LeoSM_07 on 10/22/23.
//

#ifndef Prod_hpp
#define Prod_hpp

#include <stdio.h>
#include <string.h>
#include "Expr.hpp"

class Prod : public Expr {
public:
    Prod();
    Prod(Expr* first, Expr* second);

    Expr* simplify() override;

    void multiplyIntegers(Prod* prod);
    void multiplyLikeTerms(Prod* prod);
    Expr* alone(Prod* prod);

    Expr* copy() override;

    long generateHash() override;
    bool equalStruct(Expr* other) override;
    
    std::string getLatex() override;
    void print() override;
};



#endif /* Prod_hpp */
