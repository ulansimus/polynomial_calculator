#include "automaton.hpp"



Monomial *Automaton::handle_sym(char sym){
    Monomial *p = nullptr;
    switch (state) {
        case beg_state:
            p = beg(sym);
            break;
        case sign_state:
            p = sign(sym);
            break;
        case coef_num_state:
            p = coef_num(sym);
            break;
        case mult_state:
            p = mult(sym);
            break;
        case var_state:
            p = var(sym);
            break;
        case pow_state:
            p = pow(sym);
            break;
        case pow_sign_state:
            p = pow_sign(sym);
            break;
        case pow_num_state:
            p = pow_num(sym);
            break;
    }
    return p;
}

int Automaton::append_char_to_int(int num, char sym) {
    num = num * 10 + (sym - '0');
    return num;
}

int Automaton::do_negative(int num) {
    num = -num;
    is_negative = false;
    return num;
}

void Automaton::save_prev(char sym) {
    have_prev_sym = true;
    prev_sym = sym;
    return;
}


Monomial *Automaton::beg(char sym) {
    if (sym == '+' || sym == '-') {
        state = sign_state;
        if (sym == '-') {
            is_negative = true;
        }
    } else if (sym >= '0' && sym <= '9') {
        state = coef_num_state;
        save_prev(sym);
    } else if (sym == 'x') {
        state = var_state;
        tmp_mon.coef = 1;
    } else {
        throw std::string("in symbol ") + std::to_string(sym_counter) +
              std::string(": expected beginning of polynomial(+|-|0-9|x)");
    }
    return nullptr;
}

Monomial *Automaton::sign(char sym) {
    if (sym >= '0' && sym <= '9') {
        state = coef_num_state;
        save_prev(sym);
    } else if (sym == 'x') {
        state = var_state;
        tmp_mon.coef = 1;
        if (is_negative) {
            tmp_mon.coef = do_negative(tmp_mon.coef);
        }
    } else {
        throw std::string("in symbol ") + std::to_string(sym_counter) +
                std::string(": expected coefficient or x(0-9|x)");
    }
    return nullptr;
}

Monomial *Automaton::coef_num(char sym) {
    if (sym == '+' || sym == '-' || sym == '*' || sym == 'x' || sym == '.') {
        if (is_negative) {
            tmp_mon.coef = do_negative(tmp_mon.coef);
        }
    }
    if (sym >= '0' && sym <= '9') {
        tmp_mon.coef = append_char_to_int(tmp_mon.coef, sym);
    } else if (sym == '+' || sym == '-') {
        state = sign_state;
        Monomial *res = new Monomial(tmp_mon.coef, tmp_mon.pow);
        tmp_mon.coef = 0;
        if (sym == '-') {
            is_negative = true;
        }
        return res;
    } else if (sym == '*') {
        state = mult_state;
    } else if (sym == 'x') {
        state = var_state;
    } else if (sym == '.') {
        state = beg_state;
        Monomial *res = new Monomial(tmp_mon.coef, tmp_mon.pow);
        tmp_mon.coef = 0;
        return res;
    } else {
        throw std::string("in symbol ") + std::to_string(sym_counter) +
                std::string(": expected coefficient or x or next monomial(0-9|x|+|-|*)");
    }
    return nullptr;
}

Monomial *Automaton::mult(char sym) {
    if (sym == 'x') {
        state = var_state;
    } else {
        throw std::string("in symbol ") + std::to_string(sym_counter) +
                std::string(": expected x");
    }
    return nullptr;
}

Monomial *Automaton::var(char sym) {
    if (sym == '^') {
        state = pow_state;
        return nullptr;
    } else if (sym == '+' || sym == '-') {
        state = sign_state;
        tmp_mon.pow = 1;
        Monomial *res = new Monomial(tmp_mon.coef, tmp_mon.pow);
        tmp_mon.coef = 0;
        tmp_mon.pow = 0;
        if (sym == '-') {
            is_negative = true;
        }
        return res;
    } else if (sym == '.') {
        state = beg_state;
        tmp_mon.pow = 1;
        Monomial *res = new Monomial(tmp_mon.coef, tmp_mon.pow);
        tmp_mon.coef = 0;
        tmp_mon.pow = 0;
        return res;
    } else {
        throw std::string("in symbol ") + std::to_string(sym_counter) +
                std::string(": expected ^ or next monomial(^|+|-)");
    }
}

Monomial *Automaton::pow(char sym) {
    if (sym >= '0' && sym <= '9') {
        state = pow_num_state;
        save_prev(sym);
    } else if (sym == '+' || sym == '-') {
        state = pow_sign_state;
        if (sym == '-') {
            is_negative = true;
        }
    } else {
        throw std::string("in symbol ") + std::to_string(sym_counter) +
                std::string(": expected pow sign or pow(0-9|+|-)");
    }
    return nullptr;
}

Monomial *Automaton::pow_sign(char sym) {
    if (sym >= '0' || sym <= '9') {
        state = pow_num_state;
        save_prev(sym);
    } else {
        throw std::string("in symbol ") + std::to_string(sym_counter) +
                std::string(": expected pow(0-9)");
    }
    return nullptr;
}

Monomial *Automaton::pow_num(char sym) {
    if (sym >= '0' && sym <= '9') {
        tmp_mon.pow = append_char_to_int(tmp_mon.pow, sym);
        return nullptr;
    } else if (sym == '+' || sym == '-') {
        state = sign_state;
        if (is_negative) {
            tmp_mon.pow = do_negative(tmp_mon.pow);
        }
        Monomial *res = new Monomial(tmp_mon.coef, tmp_mon.pow);
        tmp_mon.coef = 0;
        tmp_mon.pow = 0;
        if (sym == '-') {
            is_negative = true;
        }
        return res;
    } else if (sym == '.') {
        state = beg_state;
        if (is_negative) {
            tmp_mon.pow = do_negative(tmp_mon.pow);
        }
        Monomial *res = new Monomial(tmp_mon.coef, tmp_mon.pow);
        tmp_mon.coef = 0;
        tmp_mon.pow = 0;
        return res;
    } else {
        throw std::string("in symbol ") + std::to_string(sym_counter) +
                std::string(": expected pow or next monomial(0-9|+|-)");
    }
}

Monomial *Automaton::feed_char(char sym) {
    Monomial *p = nullptr;
    ++sym_counter;
    if (sym == '.') {
        sym_counter = 0;
    }
    if (have_prev_sym) {
        p = handle_sym(prev_sym);
        have_prev_sym = false;
    }
    p = handle_sym(sym);
    return p;
}
