#include "polynomials_calculator.h"

PolynomialsCalculator::PolynomialsCalculator(QWidget *parent): QWidget(parent)
{
    input_box = new QLineEdit("Enter your polynomial");
    result_box = new QLineEdit("Result");
    error_box = new QLineEdit("Errors will be displayed here");
    l_polynomials = new QListWidget(this);
    r_polynomials = new QListWidget(this);
    QToolButton *l_delete = new QToolButton;
    QToolButton *r_delete = new QToolButton;

    result_box->setReadOnly(true);
    error_box->setReadOnly(true);

    QPalette *palette = new QPalette();
    palette->setColor(QPalette::Text,Qt::red);
    error_box->setPalette(*palette);

    QAbstractButton *equal_button = createButton(tr("="), SLOT(equalClicked()));
    QAbstractButton *add_polynom_button = createButton(tr("Add"), SLOT(addPolynomClicked()));

    std::vector<QWidget*> operations_widgets = createOperations();
    QGroupBox *operations_group_box = createGroupBox(operations_widgets);


    std::vector<QWidget*> l_pol_widgets({l_polynomials, l_delete});
    std::vector<QWidget*> r_pol_widgets({r_polynomials, r_delete});
    QGroupBox *l_pol_box = createGroupBox(l_pol_widgets);
    QGroupBox *r_pol_box = createGroupBox(r_pol_widgets);
    l_delete->setText(tr("Delete"));
    r_delete->setText(tr("Delete"));
    connect(l_delete, SIGNAL(clicked()), this, SLOT(l_deleteClicked()));
    connect(r_delete, SIGNAL(clicked()), this, SLOT(r_deleteClicked()));

    std::vector<QWidget*> head_widgets({input_box, add_polynom_button});
    std::vector<QWidget*> error_widgets({error_box});
    std::vector<QWidget*> body_widgets({l_pol_box, operations_group_box,
                                        r_pol_box, equal_button, result_box});
    QGroupBox *head_group_box = createGroupBox(head_widgets, true);
    QGroupBox *error_group_box = createGroupBox(error_widgets, true);
    QGroupBox *body_group_box = createGroupBox(body_widgets, true);


    operations_buttons_group = makeButtonsGroup(operations_buttons);

    QVBoxLayout *main_layout = createMainLayout(head_group_box, error_group_box, body_group_box);

    setLayout(main_layout);
    setWindowTitle(tr("Polynomials Calculator"));
}

QGroupBox *PolynomialsCalculator::createGroupBox(const std::vector<QWidget*> &widgets, bool is_horizontal/*= false*/) {
    QBoxLayout *layout;
    QGroupBox *group_box = new QGroupBox;

    if (is_horizontal) {
        layout = new QHBoxLayout;
    } else {
        layout = new QVBoxLayout;
    }

    for (auto it = widgets.begin(); it != widgets.end(); ++it) {
        layout->addWidget(*it);
    }
    group_box->setLayout(layout);
    return group_box;
}


QVBoxLayout *PolynomialsCalculator::createMainLayout(QGroupBox *head_box, QGroupBox *error_box, QGroupBox *body_box) {
    QVBoxLayout *main_layout = new QVBoxLayout;
    main_layout->addWidget(head_box);
    main_layout->addWidget(error_box);
    main_layout->addWidget(body_box);
    return main_layout;
}

void PolynomialsCalculator::addPolynomClicked() {
    try {
        string polynomial = input_box->text().toStdString();
        Automaton autom;
        Polynomial pol;
        Monomial *mon;
        QVariant qpol;
        for (int i = 0; i < polynomial.size(); ++i) {
            if (polynomial[i] == '.'){
                try {
                    mon = autom.feed_char('.');
                } catch(std::string){}
                if (mon) {
                    delete mon;
                }
                throw std::string("'.' is not allowed");

            }
            mon = autom.feed_char(polynomial[i]);
            if (mon) {
                pol = pol + *mon;
            }
        }
        mon = autom.feed_char('.');
        if (mon) {
            pol = pol + *mon;
        }
        qpol = QVariant::fromValue(pol);
        QListWidgetItem *item;
        item = new QListWidgetItem;
        item->setData(Qt::UserRole, qpol);
        item->setText(tr(pol.do_string().c_str()));
        l_polynomials->addItem(item);
        r_polynomials->addItem(new QListWidgetItem(*item));
    } catch(std::string &e) {
        error_box->setText(tr(e.c_str()));
        return;
    }
}


void PolynomialsCalculator::equalClicked() {
    QAbstractButton *checked_button = operations_buttons_group->checkedButton();
    if (!checked_button) {
        error_box->setText(tr("Choose operation!"));
        return;
    }
    QString operation = checked_button->text();
    QListWidgetItem *l_item = l_polynomials->currentItem();
    QListWidgetItem *r_item = r_polynomials->currentItem();
    if (!l_item) {
        error_box->setText(tr("Choose polynomial!"));
        return;//TODO WRITE ERROR
    }
    if (!r_item &&
        (
           operation == tr("+") ||
           operation == tr("*") ||
           operation == tr("/")
        )
    )
    {
        error_box->setText(tr("Choose second polynomial!"));
        return;
    }
    Polynomial l_pol = qvariant_cast<Polynomial>(l_item->data(Qt::UserRole));
    Polynomial r_pol;
    if (r_item){
        r_pol = qvariant_cast<Polynomial>(r_item->data(Qt::UserRole));
    }
    if (operation == tr("+")) {
        result_box->setText(tr((l_pol + r_pol).do_string().c_str()));
    } else if (operation == tr("*")) {
        result_box->setText(tr((l_pol * r_pol).do_string().c_str()));
    } else if (operation == tr("/")) {
        result_box->setText(l_item->text() + operation + r_item->text());
    } else if (operation == tr("Roots")) {
        std::vector<int> roots = l_pol.roots();//IF ROOTS EMPTY?
        if (roots.empty()) {
            result_box->setText(tr("there aren't integer roots in polynomial"));
            return;
        }
        std::string res;
        for (auto it = roots.begin(); it != roots.end(); ++it) {
            res += std::to_string(*it) + std::string(", ");
        }
        res = res.substr(0, res.size() - 2);
        result_box->setText(tr(res.c_str()));
    } else if (operation == tr("Evaluate")) {
        bool is_correct_int;
        int point = eval_box->text().toInt(&is_correct_int);
        if (!is_correct_int) {
            error_box->setText(tr("input correct integer for 'Evaluate' function"));
            return;
        }
        double res = l_pol.value_at_point(point);
        result_box->setText(QString::number(res));
    } else if (operation == tr("Derivative")) {
        bool is_correct_int;
        int order = deriv_box->text().toInt(&is_correct_int);
        if (!is_correct_int) {
            error_box->setText(tr("input correct integer for 'Derivative' function"));
            return;
        }
        Polynomial res = l_pol.derivative(order);
        result_box->setText(QString::fromStdString(res.do_string()));
    }

}

void PolynomialsCalculator::l_deleteClicked() {
    QListWidgetItem *cur_item = l_polynomials->currentItem();
    if (!cur_item) {
        error_box->setText(tr("choose polynomial to delete"));
        return;
    }
    delete l_polynomials->takeItem(l_polynomials->row(cur_item));
}

void PolynomialsCalculator::r_deleteClicked() {
    QListWidgetItem *cur_item = r_polynomials->currentItem();
    if (!cur_item) {
        error_box->setText(tr("choose polynomial to delete"));
        return;
    }
    delete r_polynomials->takeItem(r_polynomials->row(cur_item));
}

QAbstractButton *PolynomialsCalculator::createButton(const QString &text, const char *my_slot, bool is_push/* = false*/)
{
    QAbstractButton *button;
    if (is_push) {
        button = new QPushButton;
        button->setCheckable(true);
    } else {
        button = new QToolButton;
    }
    button->setText(text);
    if (my_slot) {
        connect(button, SIGNAL(clicked()), this, my_slot);
    }
    return button;
}

std::vector<QWidget*> PolynomialsCalculator::createOperations() {
    operations_buttons.push_back(createButton(tr("+"), nullptr, true));
    operations_buttons.push_back(createButton(tr("*"), nullptr, true));
    operations_buttons.push_back(createButton(tr("/"), nullptr, true));
    operations_buttons.push_back(createButton(tr("Roots"), nullptr, true));

    std::vector<QWidget*> res(operations_buttons.begin(), operations_buttons.end());

    QHBoxLayout *evaluate_layout = new QHBoxLayout;
    QGroupBox *evaluate = new QGroupBox;
    QAbstractButton* eval_button = createButton(tr("Evaluate"), nullptr, true);
    QLineEdit *eval_point = new QLineEdit;
    evaluate_layout->addWidget(eval_button);
    evaluate_layout->addWidget(eval_point);
    evaluate->setLayout(evaluate_layout);
    res.push_back((QWidget*)evaluate);
    operations_buttons.push_back(eval_button);
    eval_box = eval_point;

    QHBoxLayout *derivative_layout = new QHBoxLayout;
    QGroupBox *derivative = new QGroupBox;
    QAbstractButton* deriv_button = createButton(tr("Derivative"), nullptr, true);
    QLineEdit *deriv_point = new QLineEdit;
    derivative_layout->addWidget(deriv_button);
    derivative_layout->addWidget(deriv_point);
    derivative->setLayout(derivative_layout);
    res.push_back((QWidget*)derivative);
    operations_buttons.push_back(deriv_button);
    deriv_box = deriv_point;

    return res;
}

QButtonGroup *PolynomialsCalculator::makeButtonsGroup(std::vector<QAbstractButton*> &buttons) {
    QButtonGroup *res = new QButtonGroup;

    for (auto it = buttons.begin(); it != buttons.end(); ++it) {
        res->addButton(*it);
    }

    return res;
}
