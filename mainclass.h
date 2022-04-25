#ifndef MAINCLASS_H
#define MAINCLASS_H

#include "fraction.h"
#include <QFile>

class MainClass {
public:
    MainClass();
    void init_date();
    void transit_to_can();
    void polar_right_part();
    void check_origin_basis();
    void copy_matrix_no_horny(QVector<QVector<fract>> matrix_in, QVector<QVector<fract>>& matrix_out);
    void back_horny(QVector<QVector<fract>> matrix_no_horny, QVector<QVector<fract>>& matrix_horny);
    void simplex_method();
    void cout_table(QVector<QVector<fract>> table);
    bool check_opti(QVector<QVector<fract>> table);
    bool find_sint_basis();
    void calculate_m_vec(QVector<QVector<fract>> table);
    bool check_opt_m();

private:
    bool m_nx;
    QVector<fract> z;
    QVector<fract> m;
    QVector<QChar> sinbol;
    QVector<QVector<fract>> matrix_lim;
    QVector<QVector<fract>> matrix_can;
    QVector<qint16> gen_basis;
    QVector<qint16> gen_basis_x;
    QVector<qint16> gen_basis_index;
};

#endif // MAINCLASS_H
