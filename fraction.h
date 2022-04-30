#ifndef FRACTION_H
#define FRACTION_H
#include <QDebug>
#include <QtMath>

struct fract {
    int u_num;
    int d_num;
    fract();
    fract(int u, int d);

private:
    fract reduct(fract f);

public:
    bool operator<(fract scnd);
    bool operator>(fract scnd);
    bool operator==(fract scnd);
    fract operator+(fract scnd);
    fract operator-(fract scnd);
    fract operator*(fract scnd);
    fract operator/(fract scnd);
};

#endif // FRACTION_H
