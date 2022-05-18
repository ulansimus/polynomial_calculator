#ifndef POLYNOMIALSCALCULATOR_H
#define POLYNOMIALSCALCULATOR_H

#include <QtWidgets>
#include "automaton.hpp"

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

class PolynomialsCalculator : public QWidget
{
    Q_OBJECT
public:
    explicit PolynomialsCalculator(QWidget *parent = 0);

private slots:
    void addPolynomClicked();
    void equalClicked();
    void l_deleteClicked();
    void r_deleteClicked();

private:
    QLineEdit *input_box;
    QLineEdit *result_box;
    QButtonGroup *operations_buttons_group;
    std::vector<QAbstractButton*> operations_buttons;
    QListWidget *l_polynomials;
    QListWidget *r_polynomials;
    QLineEdit *eval_box;
    QLineEdit *deriv_box;
    QLineEdit *error_box;


    QAbstractButton *createButton(const QString &text, const char *my_slot, bool is_push = false);
    QButtonGroup *makeButtonsGroup(std::vector<QAbstractButton*> &buttons);
    std::vector<QWidget*> createOperations();

    QGroupBox *createGroupBox(const std::vector<QWidget*> &widgets, bool is_horizontal = false);
    QVBoxLayout *createMainLayout(QGroupBox *head_box, QGroupBox *error_box, QGroupBox *body_box);

};

#endif // POLYNOMIALSCALCULATOR_H
