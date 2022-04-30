#include "fraction.h"

fract::fract() { }

fract::fract(int u, int d)
{
    u_num = u;
    d_num = d;
}

fract fract::reduct(fract f)
{
    int a, b;
    if (f.d_num < 0 && f.u_num < 0) {
        f.d_num *= -1;
        f.u_num *= -1;
    }
    if (f.d_num < 0) {
        f.d_num *= -1;
        f.u_num *= -1;
    }
    a = qAbs(f.u_num);
    b = qAbs(f.d_num);
    while (a && b) {
        if (a > b)
            a %= b;
        else
            b %= a;
    }
    f.u_num /= (a + b);
    f.d_num /= (a + b);
    return f;
}

fract fract::operator+(fract scnd)
{
    fract answer;
    answer.u_num = this->u_num * scnd.d_num + scnd.u_num * this->d_num;
    answer.d_num = this->d_num * scnd.d_num;
    return reduct(answer);
}

fract fract::operator-(fract scnd)
{
    fract answer;
    answer.u_num = this->u_num * scnd.d_num - scnd.u_num * this->d_num;
    answer.d_num = this->d_num * scnd.d_num;
    return reduct(answer);
}

fract fract::operator*(fract scnd)
{
    fract answer;
    answer.u_num = this->u_num * scnd.u_num;
    answer.d_num = this->d_num * scnd.d_num;
    return reduct(answer);
}

fract fract::operator/(fract scnd)
{
    fract answer;
    answer.u_num = this->u_num * scnd.d_num;
    answer.d_num = this->d_num * scnd.u_num;
    return reduct(answer);
}

bool fract::operator<(fract scnd)
{
    this->u_num = scnd.d_num * this->u_num;
    scnd.u_num = this->d_num * scnd.u_num;
    if (this->u_num < scnd.u_num) {
        return true;
    } else {
        return false;
    }
}

bool fract::operator>(fract scnd)
{
    this->u_num = scnd.d_num * this->u_num;
    scnd.u_num = this->d_num * scnd.u_num;
    if (this->u_num > scnd.u_num) {
        return true;
    } else {
        return false;
    }
}

bool fract::operator==(fract scnd)
{
    this->u_num = scnd.d_num * this->u_num;
    scnd.u_num = this->d_num * scnd.u_num;
    if (this->u_num == scnd.u_num) {
        return true;
    } else {
        return false;
    }
}
