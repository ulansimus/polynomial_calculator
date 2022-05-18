#include "polynomial.hpp"

using namespace std;

//Monomial

string Monomial::do_string() {
    if (coef == 1) {
        if (pow == 0) {
            return string("1");
        } else if (pow == 1) {
            return string("x");
        } else {
            return (string("x^")+to_string(pow));
        }
    } else if (coef == -1) {
        if (pow == 0) {
            return string("-1");
        } else if (pow == 1) {
            return string("-x");
        } else {
            return (string("-x^")+to_string(pow));
        }
    } else {
        if (pow == 0) {
            return to_string(coef);
        } else if (pow == 1) {
            return (to_string(coef)+string("x"));
        } else {
            return (to_string(coef)+string("x^")+to_string(pow));
        }
    }
}



//Polynomial

//private:

void Polynomial::push_back_(Monomial to_push) {
    if (!head && !tail) {
        head = new MonomialListNode;
        tail = head;
        head->val = to_push;
        head->prev = nullptr;
        head->next = nullptr;
        return;
    }
    tail->next = new MonomialListNode;
    tail->next->val = to_push;
    tail->next->next = nullptr;
    tail->next->prev = tail;
    tail = tail->next;
}

void Polynomial::delete_MonomialListNode(MonomialListNode *to_delete) {
    if (to_delete->prev && to_delete->next) {
        to_delete->prev->next = to_delete->next;
        to_delete->next->prev = to_delete->prev;
        delete to_delete;
        return;
    }
    if (!to_delete->next) {
        tail = tail->prev;
        if (tail) {
            tail->next = nullptr;
        }
    }
    if (!to_delete->prev) {
        head = head->next;
        if (head) {
            head->prev = nullptr;
        }
    }
    delete to_delete;
}

void Polynomial::ins_bef(MonomialListNode *cur, Monomial to_ins) {
    if (!cur->prev) {
        cur->prev = new MonomialListNode;
        head = head->prev;
        head->val = to_ins;
        head->next = cur;
        return;
    }
    MonomialListNode *tmp = new MonomialListNode;
    tmp->val = to_ins;
    tmp->next = cur;
    tmp->prev = cur->prev;
    cur->prev = tmp;
    tmp->prev->next = tmp;
    return;
}

void Polynomial::delete_MonomialList(MonomialListNode *head) {
    while (head){
        MonomialListNode *to_delete = head;
        head = head->next;
        delete to_delete;
    }
}

vector<int> Polynomial::dividers (int num) {
    if (num < 0) {
        num = -num;
    }
    vector<int> res;
    for (int i = 1; i * i <= num; ++i) {
        if (num % i == 0) {
            if (i * i < num) {
                res.push_back(i);
                res.push_back(num / i);
                res.push_back(-i);
                res.push_back(-num / i);
            } else {
                res.push_back(i);
                res.push_back(-i);
            }
        }
    }
    return res;
}



//public

Polynomial Polynomial::operator+(const Monomial &second) {
    Polynomial res(*this);
    MonomialListNode *cur = res.head;

    while (cur != nullptr && cur->val.pow > second.pow)
        cur = cur->next;

    if (cur == nullptr) {
        res.push_back_(second);
    } else {
        if (cur->val.pow == second.pow) {
            cur->val.coef += second.coef;
            if (cur->val.coef == 0) {
                res.delete_MonomialListNode(cur);
            }
        } else {
            res.ins_bef(cur, second);
        }
    }
    return res;
}

const Polynomial &Polynomial::operator=(const Polynomial &second) {
    MonomialListNode *cur_second = second.head;

    delete_MonomialList(head);
    head = nullptr;
    tail = nullptr;
    while (cur_second != nullptr){
        push_back_(cur_second->val);
        cur_second = cur_second->next;
    }
    return *this;
}

Polynomial::Polynomial() {
    head = nullptr;
    tail = nullptr;
}

Polynomial::Polynomial(const Polynomial &second){
    *this = second;
}

Polynomial::~Polynomial(){
    delete_MonomialList(head);
}

string Polynomial::do_string() {
    MonomialListNode *cur = head;
    string res;
    if (!cur) {
        return std::string("0");
    }
    res += cur->val.do_string();
    cur = cur->next;
    while (cur){
        if (cur->val.coef > 0) {
            res += string("+");
        }
        res += cur->val.do_string();
        cur = cur->next;
    }
    return res;
}



Polynomial Polynomial::operator+(const Polynomial &second) {//TODO O(n)
    Polynomial res = *this;
    MonomialListNode *cur = second.head;
    while (cur) {
        res = res + cur->val;
        cur = cur->next;
    }
    return res;
}
/*
Polynomial Polynomial::operator+(const Polynomial &second) {
    Polynomial res;
    MonomialListNode *cur_first = this->head, *cur_second = second.head;
    while (cur_first && cur_second) {
        if (cur_first->val.pow >= cur_second->val.pow) {
            if (res.tail->val.pow)
            res.push_back_(cur_first->val);
        } else {
        }
    }
}*/

double Polynomial::value_at_point(int point) {
    double res = 0;
    MonomialListNode *cur = head;
    if (point == 0) {
        if (tail->val.pow > 0) {
            return 0;
        } else if (tail->val.pow == 0) {
            return tail->val.coef;
        } else {
            throw "outside of area of allowable values";
        }
    }
    while (cur) {
        res = res + (double)cur->val.coef * pow(point, cur->val.pow);
        cur = cur->next;
    }
    return  res;
}

Polynomial Polynomial::derivative(int order) {
    Polynomial res;
    MonomialListNode *cur = head;
    while (cur) {
        if (cur->val.pow >= order || cur->val.pow <= -1) {
            Monomial to_ins;
            to_ins.coef = cur->val.coef;
            for (int i = 0; i < order; ++i) {
                to_ins.coef *= cur->val.pow - i;
            }
            to_ins.pow = cur->val.pow - order;
            res.push_back_(to_ins);
        }
        cur = cur->next;
    }
    return res;
}

Polynomial Polynomial::operator*(const Polynomial &second) {
    Polynomial res;
    MonomialListNode *cur = second.head;
    while (cur) {
        res = res + (*this) * cur->val;
        cur = cur->next;
    }
    return res;
}

Polynomial Polynomial::operator*(const Monomial &second) {
    Polynomial res = (*this);
    MonomialListNode *cur = res.head;
    while (cur) {
        cur->val.pow += second.pow;
        cur->val.coef *= second.coef;
        cur = cur->next;
    }
    return res;
}

vector<int> Polynomial::roots() {
    vector<int> res;
    if (this->tail->val.pow > 0) {
        res.push_back(0);
    }
    vector<int> divs = dividers(this->tail->val.coef);
    for (int i = 0; i < divs.size(); ++i) {
        if (value_at_point(divs[i]) == 0) {
            res.push_back(divs[i]);
        }
    }
    sort(res.begin(), res.end());
    return res;
}

/*
DivisionResult Polynomial::operator/(const Polynomial &second) {
    DivisionResult res;
    if (head->val.pow < second.head.val.pow) {
        res.quotient();
        res.remainder = (*this);
        return res;
    }

}
*/
