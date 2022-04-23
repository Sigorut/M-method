#ifndef FRACTION_H
#define FRACTION_H
#include <QtMath>
#include <QDebug>

struct fract{
    int u_num;
    int d_num;
    fract(){}
    fract(int u, int d){
        u_num = u;
        d_num = d;
    }
    bool operator<(fract scnd) {
        this->u_num = scnd.d_num * this->u_num;
        scnd.u_num = this->d_num * scnd.u_num;
        if(this->u_num < scnd.u_num){
            return true;
        } else {
            return false;
        }
    }
    bool operator>(fract scnd) {
        this->u_num = scnd.d_num * this->u_num;
        scnd.u_num = this->d_num * scnd.u_num;
        if(this->u_num > scnd.u_num){
            return true;
        } else {
            return false;
        }
    }
    bool operator==(fract scnd) {
        this->u_num = scnd.d_num * this->u_num;
        scnd.u_num = this->d_num * scnd.u_num;
        if(this->u_num == scnd.u_num){
            return true;
        } else {
            return false;
        }
    }
};

class Fraction
{
public:
    Fraction();
    fract sum(fract frst, fract scnd, bool p_m = 0);
    fract mult(fract frst, fract scnd, bool d_m = 0);
    fract abs(fract f);
    bool comparison(fract frst, fract scnd);
private:
    fract reduct(fract);
    short int check_null(fract frst, fract scnd);

};

#endif // FRACTION_H
