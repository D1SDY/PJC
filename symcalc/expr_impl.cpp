#include "expr_impl.hpp"

#include <iostream>
#include <cmath>
#include <limits>
#define PI 3.14159265


namespace exprs {

    double number::evaluate(const expr_base::variable_map_t &variables) const {
        return num;
    }

    expr number::derive(string const &temp) const {
        throw std::logic_error("not implemented yet tyt1");
    }

    expr number::simplify() const {
        return shared_from_this();
    }

    void number::write(ostream &out, expr_base::WriteFormat fmt) const {
        out << num;
    }

    bool number::equals(const expr_base &variable) const {
        const number *temp = dynamic_cast<number const *>(variable.shared_from_this().get());
        if (temp) {
            if (temp->num == num) {
                return true;
            } else {
                return false;
            }
        }
        return false;
    }


    double variable::evaluate(const expr_base::variable_map_t &variables) const {
//        if(variables.find(name)==variables.end()||variables.empty()){
//            throw unbound_variable_exception("variable wasnt defined");
//        }else{
//            return variables.at(name);
//        }
        try{
            return variables.at(name);
        }catch(exception){
            throw unbound_variable_exception("! variable wasnt defined");
        }
    }

    expr variable::derive(string const &temp) const {
        throw std::logic_error("not implemented yet tut3");
    }

    expr variable::simplify() const {
        return shared_from_this();
    }

    bool variable::equals(const expr_base &var) const {
        const variable* temp = dynamic_cast<variable const *>(var.shared_from_this().get());
        if(temp){
            if(temp->name==name){
                return true;
            }else{
                return false;
            }
        }
        return false;

    }

    void variable::write(ostream &out, expr_base::WriteFormat fmt) const {
        out << name;
    }

    expr_plus::expr_plus(const expr &one, const expr &two) : first(one), second(two) {}
    double expr_plus::evaluate(const expr_base::variable_map_t &variables) const {
       double temp1=first->evaluate(variables);
       double temp2=second->evaluate(variables);
       return temp1+temp2;
    }

    expr expr_plus::derive(string const &temp) const {
        throw std::logic_error("not implemented yet 1");
    }

    expr expr_plus::simplify() const {
        expr temp1=first->simplify();
        expr temp2=second->simplify();
        if(temp1==expr::ZERO){
            return temp2;
        }else if(temp2==expr::ZERO){
            return temp1;
        }
        return temp1+temp2;
    }

    bool expr_plus::equals(const expr_base &variable) const {
        const expr_plus* temp = dynamic_cast<expr_plus const *>(variable.shared_from_this().get());
        if(temp!= nullptr){
            if(second==temp->second){
                if(first==temp->first){
                    return true;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
    }

    void expr_plus::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << " +)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "(" << fmt_expr{first, fmt} << " + " << fmt_expr{second, fmt} << ")";
        } else {
            out << "(+ " << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << ")";
        }
    }

    expr_minus::expr_minus(const expr &one, const expr &two) : first(one), second(two) {}

    double expr_minus::evaluate(const expr_base::variable_map_t &variables) const {
        double temp1=first->evaluate(variables);
        double temp2=second->evaluate(variables);
        return temp1-temp2;
    }

    expr expr_minus::derive(string const &temp) const {
        throw std::logic_error("not implemented yet 3");
    }

    expr expr_minus::simplify() const {
        expr temp1=first->simplify();
        expr temp2=second->simplify();
        if(temp2==expr::ZERO){
            return temp1;
        }
        return temp1-temp2;
    }

    bool expr_minus::equals(const expr_base &variable) const {
        const expr_minus* temp = dynamic_cast<expr_minus const *>(variable.shared_from_this().get());
        if(temp!= nullptr){
            if(second==temp->second){
                if(first==temp->first){
                    return true;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
    }

    void expr_minus::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << " -)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "(" << fmt_expr{first, fmt} << " - " << fmt_expr{second, fmt} << ")";
        } else {
            out << "(- " << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << ")";
        }

    }

    expr_multiply::expr_multiply(const expr &one, const expr &two) : first(one), second(two) {}

    double expr_multiply::evaluate(const expr_base::variable_map_t &variables) const {
        double temp1=first->evaluate(variables);
        double temp2=second->evaluate(variables);
        return temp1*temp2;
    }

    expr expr_multiply::derive(string const &temp) const {
        throw std::logic_error("not implemented yet 5");
    }

    expr expr_multiply::simplify() const {
        expr temp1=first->simplify();
        expr temp2=second->simplify();
        if(temp1==expr::ZERO||temp2==expr::ZERO){
            return expr::ZERO;
        }
        if(temp1==expr::ONE){
            return temp2;
        }else if(temp2==expr::ONE){
            return temp1;
        }
        return temp1*temp2;
    }

    bool expr_multiply::equals(const expr_base &variable) const {
        const expr_multiply* temp = dynamic_cast<expr_multiply const *>(variable.shared_from_this().get());
        if(temp!= nullptr){
            if(second==temp->second){
                if(first==temp->first){
                    return true;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }

    }

    void expr_multiply::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << " *)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "(" << fmt_expr{first, fmt} << " * " << fmt_expr{second, fmt} << ")";
        } else {
            out << "(* " << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << ")";
        }
    }

    expr_divide::expr_divide(const expr &one, const expr &two) : first(one), second(two) {}

    double expr_divide::evaluate(const expr_base::variable_map_t &variables) const {
        double temp1=first->evaluate(variables);
        double temp2=second->evaluate(variables);
        return temp1/temp2;
    }

    expr expr_divide::derive(string const &temp) const {
        throw std::logic_error("not implemented yet 8");
    }

    expr expr_divide::simplify() const {
        expr temp1=first->simplify();
        expr temp2=second->simplify();
        if(temp1==expr::ZERO&&temp2==expr::ZERO){
            return expr::ZERO/expr::ZERO;
        }
        if(temp2==expr::ONE){
            return temp1;
        }
        if(temp1==expr::ZERO){
            return expr::ZERO;
        }

        return temp1/temp2;
    }

    bool expr_divide::equals(const expr_base &variable) const {
        const expr_divide* temp = dynamic_cast<expr_divide const *>(variable.shared_from_this().get());
        if(temp!= nullptr){
            if(second==temp->second){
                if(first==temp->first){
                    return true;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        }else{
            return false;
        }
    }

    void expr_divide::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << " /)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "(" << fmt_expr{first, fmt} << " / " << fmt_expr{second, fmt} << ")";
        } else {
            out << "(/ " << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << ")";
        }
    }

    expr_pow::expr_pow(const expr &one, const expr &two) : first(one), second(two) {}

    double expr_pow::evaluate(const expr_base::variable_map_t &variables) const {
        double temp1=first->evaluate(variables);
        double temp2=second->evaluate(variables);
        return pow(temp1,temp2);
    }

    expr expr_pow::derive(string const &temp) const {
        throw std::logic_error("not implemented yet 9");
    }

    expr expr_pow::simplify() const {
        expr temp1=first->simplify();
        expr temp2=second->simplify();
        if (temp2 == expr::ZERO) {
            return expr::ONE;
        }
        else if (temp2 == expr::ONE) {
            return temp1;
        }
        else if (temp1 == expr::ZERO) {
            return expr::ZERO;
        }
        return pow(temp1, temp2);

    }

    bool expr_pow::equals(const expr_base &variable) const {
        const expr_pow *temp = dynamic_cast<expr_pow const *>(variable.shared_from_this().get());
        if(temp!= nullptr){
            if(first==temp->first){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
        return false;
    }

    void expr_pow::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << " ^)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "(" << fmt_expr{first, fmt} << " ^ " << fmt_expr{second, fmt} << ") ";
        } else {
            out << "(^ " << fmt_expr{first, fmt} << " " << fmt_expr{second, fmt} << ")";
        }
    }

    expr_sin::expr_sin(const expr &one) : first(one) {}

    double expr_sin::evaluate(const expr_base::variable_map_t &variables) const {
        double temp=first->evaluate(variables);
        return sin(temp*PI/180);
    }

    expr expr_sin::derive(string const &temp) const {
        throw std::logic_error("not implemented yet 11");
    }

    expr expr_sin::simplify() const {
        return sin(first->simplify());
    }

    void expr_sin::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " sin)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "sin(" << fmt_expr{first, fmt} << ")";
        } else {
            out << "(sin " << fmt_expr{first, fmt} << ")";
        }

    }

    bool expr_sin::equals(const expr_base &variable) const {
        const expr_sin *temp = dynamic_cast<expr_sin const *>(variable.shared_from_this().get());
        if(temp!= nullptr){
            if(first==temp->first){
                return true;
            }else{
                return false;
            }
        }
        return false;
    }

    expr_cos::expr_cos(const expr &one) : first(one) {}

    double expr_cos::evaluate(const expr_base::variable_map_t &variables) const {
        double temp=first->evaluate(variables);
        return cos(temp*PI/180);
    }

    expr expr_cos::derive(string const &temp) const {
        throw std::logic_error("not implemented yet 13");
    }

    expr expr_cos::simplify() const {
        return cos(first->simplify());
    }

    void expr_cos::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " cos)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "cos(" << fmt_expr{first, fmt} << ")";
        } else {
            out << "(cos " << fmt_expr{first, fmt} << ")";
        }
    }

    bool expr_cos::equals(const expr_base &variable) const {
        const expr_cos *temp = dynamic_cast<expr_cos const *>(variable.shared_from_this().get());
        if(temp!= nullptr){
            if(first==temp->first){
                return true;
            }else{
                return false;
            }
        }
        return false;

    }

    expr_log::expr_log(const expr &one) : first(one) {}

    double expr_log::evaluate(const expr_base::variable_map_t &variables) const {
        double temp=first->evaluate(variables);
        if(temp<=0){
            throw domain_exception("lower or euqals 0");
        }else{
            return log(temp);
        }
    }

    expr expr_log::derive(string const &temp) const {
        throw std::logic_error("not implemented yet 15");
    }

    expr expr_log::simplify() const {
        expr temp = first->simplify();

        if (temp == expr::ONE) {
            return expr::ZERO;
        }
        return log(temp);

    }

    void expr_log::write(ostream &out, expr_base::WriteFormat fmt) const {
        if (fmt == WriteFormat::Postfix) {
            out << "(" << fmt_expr{first, fmt} << " log)";
        }
        if (fmt == WriteFormat::Infix) {
            out << "log(" << fmt_expr{first, fmt} << ")";
        } else {
            out << "(log " << fmt_expr{first, fmt} << ")";
        }
    }

    bool expr_log::equals(const expr_base &variable) const {
        const expr_log *temp = dynamic_cast<expr_log const *>(variable.shared_from_this().get());
        if(temp!= nullptr){
            if(first==temp->first){
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
        return false;
    }

} // namespace exprs
