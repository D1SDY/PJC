#include "expr.hpp"
#include "expr_impl.hpp"
#include <stdexcept>

const expr expr::ZERO = expr::number(0.0);
const expr expr::ONE = expr::number(1.0);

// TODO: overloaded operators +, -, *, /, functions pow, log, sin, cos,
//       expr::number, expr::variable, operator==, operator<<,
//       create_expression_tree

expr create_expression_tree(const std::string& expression) {
   
}

expr expr::number(double n) {
    return make_shared<exprs::number>(exprs::number(n));
}
expr expr::variable(std::string name) {
    return make_shared<exprs::variable>(exprs::variable(name));
}
expr operator+(expr a, expr b){
    return make_shared<exprs::expr_plus>(exprs::expr_plus(a,b));
}
expr operator-(expr a,expr b){
    return make_shared<exprs::expr_minus>(exprs::expr_minus(a,b));
}
expr operator*(expr a, expr b){
    return make_shared<exprs::expr_multiply>(exprs::expr_multiply(a,b));
}
expr pow(expr m, expr e){
    return make_shared<exprs::expr_pow>(exprs::expr_pow(m,e));
}
expr operator/(expr a, expr b){
    return make_shared<exprs::expr_divide>(exprs::expr_divide(a,b));
}
expr sin(expr e){
    return make_shared<exprs::expr_sin>(exprs::expr_sin(e));
}
expr cos(expr e){
    return make_shared<exprs::expr_cos>(exprs::expr_cos(e));
}
expr log(expr e){
    return make_shared<exprs::expr_log>(exprs::expr_log(e));
}

bool operator==(const expr &a, const expr &b) {
    if(a->equals(b->shared_from_this().operator*())==true){
        return true;
    }else{
        return false;
    }
}
std::ostream& operator<<(std::ostream &os, const expr &e) {
    e->write(os, expr::WriteFormat::Prefix);
    return os;
}
std::ostream& operator<<(std::ostream &os, const fmt_expr &fmt) {
    fmt.e->write(os, fmt.fmt);
    return os;
}

