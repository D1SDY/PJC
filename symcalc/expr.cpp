#include "expr.hpp"
#include "expr_impl.hpp"
#include "tokenizer.hpp"
#include <stdexcept>
#include <vector>
#include <queue>

const expr expr::ZERO = expr::number(0.0);
const expr expr::ONE = expr::number(1.0);

// TODO: overloaded operators +, -, *, /, functions pow, log, sin, cos,
//       expr::number, expr::variable, operator==, operator<<,
//       create_expression_tree
//empty
//size
//front
//back
//push_back
//pop_front
class queue_expr{
private:
//    queue <expr> out;
    expr first;
    expr second;
public:
//    queue_expr(){
//        out=queue<expr>();
//    }
//    bool empty(){
//        if(out.empty()==true){
//            return true;
//        }else{
//            return false;
//        }
//    }
//    int size(){
//        return out.size();
//    }
//    expr front(){
//        return out.front();
//    }
//    expr back(){
//        return out.back();
//    }
//    void push_back(const expr& a){
//        out.push(a);
//    }
//    expr pop_front(){
//        out.pop();
//    }
    vector<expr> out;
    bool empty(){
        if(out.size()==0){
            return true;
        }else{
            return false;
        }
    }
    int size(){
        return out.size();
    }
    expr front(){
        return out[0];
    }
    expr back(){
        return out[out.size()];
    }
    void push_back(const expr& a){
        out.push_back(a);
    }
    expr pop_front(){
        out.erase(out.begin());
    }
    void pop_two_args_from_queue(){
        second=out.back();
        //out.pop_front();
        pop_front();
        first=out.back();
        //out.pop_front();
        pop_front();
    }
    void find_Token(Token& token){
        if(token.id==TokenId::Plus){
            if(out.size()>2){
                pop_two_args_from_queue();
                //out.push(first+second);
                push_back(first+second);
            }else{
                throw parse_error("ammout of args is lover then 2");
            }
        }
        if(token.id==TokenId::Minus){
            if(out.size()>2){
                pop_two_args_from_queue();
                //out.push(first-second);
                push_back(first-second);
            }else{
                throw parse_error("ammout of args is lover then 2");
            }
        }
        if(token.id==TokenId::Multiply){
            if(out.size()>2){
                pop_two_args_from_queue();
                //out.push(first*second);
                push_back(first*second);
            }else{
                throw parse_error("ammout of args is lover then 2");
            }
        }
        if(token.id==TokenId::Divide){
            if(out.size()>2){
                pop_two_args_from_queue();
                //out.push(first/second);
                push_back(first/second);
            }else{
                throw parse_error("ammout of args is lover then 2");
            }
        }
        if(token.id==TokenId::Power){
            if(out.size()>2){
                pop_two_args_from_queue();
                //out.push(pow(first,second));
                push_back(pow(first,second));
            }else{
                throw parse_error("ammout of args is lover then 2");
            }
        }
        if(token.id==TokenId::Identifier){
            if(out.empty()){
                throw parse_error("ammout of args is lover then 2");
            }else{
                first=out.back();
                pop_front();
                if(token.identifier=="sin"){
                    //out.push(sin(first));
                    push_back(sin(first));
                }else if(token.identifier=="cos"){
                    //out.push(cos(first));
                    push_back(cos(first));
                }else if(token.identifier=="log"){
                    //out.push(log(first));
                    push_back(log(first));
                }
            }
        }

    }
};
//we can change queue to vector???
expr create_expression_tree(const std::string& expression) {
//    while(true){
//
//    }
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

