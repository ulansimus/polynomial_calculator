#ifndef POLYNOMIAL_HPP_INCLUDED
#define POLYNOMIAL_HPP_INCLUDED

#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

#include <QMetaType>

using namespace std;


struct DivisionResult;


struct Monomial {
    int coef;
    int pow;

    Monomial(int c, int p) {
        coef = c;
        pow = p;
    }

    Monomial() {
        coef = 0;
        pow = 0;
    }

    string do_string();
};

class Polynomial {
private:
    struct MonomialListNode {
        Monomial val;
        MonomialListNode *next;
        MonomialListNode *prev;
    };
    MonomialListNode *head = nullptr;
    MonomialListNode *tail = nullptr;

    void push_back_(Monomial to_push);

    void delete_MonomialListNode(MonomialListNode *to_delete);

    void ins_bef(MonomialListNode *cur, Monomial ins);

    void delete_MonomialList(MonomialListNode *head);

    vector<int> dividers (int num);

public:

    Polynomial operator+(const Monomial &second);

    const Polynomial &operator=(const Polynomial &b);

    Polynomial();

    Polynomial(const Polynomial &a);

    ~Polynomial();

    string do_string();



    Polynomial operator+(const Polynomial &second);

    double value_at_point(int point);

    Polynomial derivative(int order);

    Polynomial operator*(const Polynomial &second);

    Polynomial operator*(const Monomial &second);

    vector<int> roots();

//    DivisionResult operator/(const Polynomial &second);
};
Q_DECLARE_METATYPE(Polynomial)

struct BasePolynomial {
    Polynomial val;
    BasePolynomial *next;
    BasePolynomial *prev;
};

struct DivisionResult {
    Polynomial quotient;
    Polynomial remainder;
};


#endif // POLYNOMIAL_HPP_INCLUDED
