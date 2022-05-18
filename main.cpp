#include <QApplication>

#include "polynomials_calculator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    PolynomialsCalculator calc;
    calc.show();
    return app.exec();
}
