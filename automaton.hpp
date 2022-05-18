#ifndef AUTOMATON_HPP_INCLUDED
#define AUTOMATON_HPP_INCLUDED

#include "polynomial.hpp"

class Automaton {
private:
    enum { beg_state, sign_state, coef_num_state,
           mult_state, var_state, pow_state, pow_sign_state, pow_num_state};

    bool have_prev_sym = false;
    char prev_sym;
    bool is_negative = false;

    int state = beg_state;

    int sym_counter = 0;

    Monomial tmp_mon{0, 0};

    Monomial *handle_sym(char sym);

    int append_char_to_int(int numb, char sym);

    int do_negative(int num);

    void save_prev(char sym);


    Monomial *beg(char sym);
    Monomial *sign(char sym);
    Monomial *coef_num(char sym);
    Monomial *mult(char sym);
    Monomial *var(char sym);
    Monomial *pow(char sym);
    Monomial *pow_sign(char sym);
    Monomial *pow_num(char sym);

public:
    Monomial *feed_char(char sym);

};

#endif // AUTOMATON_HPP_INCLUDED
