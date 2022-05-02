#include "mainclass.h"
#include <iostream>
using namespace std;

void cout_fract(fract f, bool abs = 0)
{
    // Просто вывод дроби
    if (abs) {
        cout << qAbs(f.u_num);
    } else {
        cout << f.u_num;
    }
    if (f.d_num != 1) {
        cout << "/" << f.d_num;
    }
}

void cout_matrix(QVector<QVector<fract>> matrix)
{
    // Просто вывод матрицы
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[0].size(); j++) {
            if (matrix[i][j].d_num == 1) {
                cout << matrix[i][j].u_num << "\t";
            } else {
                cout << matrix[i][j].u_num << "/" << matrix[i][j].d_num << "\t";
            }
        }
        cout << endl
             << endl;
    }
    cout << endl;
}

void read_matrix(QVector<QVector<fract>>& matrix)
{
    QFile file(":/matrix");
    if (!file.size()) {
        qDebug() << "File is empty!";
        exit(0);
    }
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QStringList elMat;
        QStringList elMatI;
        QString lineStr;
        QVector<fract> lineEl;
        while (!file.atEnd()) {
            lineStr = file.readLine();
            elMat = lineStr.split("__");
            lineEl.clear();
            for (int i = 0; i < elMat.size(); i++) {
                fract oneEl;
                if (elMat[i].contains('/')) {
                    elMatI = elMat[i].split("/");
                    if (elMatI[1] == "0") {
                        qDebug("Division on null!");
                        exit(0);
                    }
                    oneEl.u_num = elMatI[0].toInt();
                    oneEl.d_num = elMatI[1].toInt();
                    if (oneEl.d_num < 0) {
                        oneEl.d_num *= -1;
                        oneEl.u_num *= -1;
                    }
                } else {
                    oneEl.u_num = elMat[i].toInt();
                    oneEl.d_num = 1;
                }

                lineEl.push_back(oneEl);
            }
            matrix.push_back(lineEl);
        }
    }
}

MainClass::MainClass()
{
    // Подключаем русский на винде.
    system("chcp 65001");
}

void MainClass::init_date()
{
    // Считываем матрицу ограничений
    read_matrix(matrix_lim);
    // Заполняем вектор функции Z
    z << fract(2, 1) << fract(11, 1);
    // Заполняем вектор знаков данными из дано
    sinbol << '>' << '>' << '>';
    // true - max / false - min
    m_nx = false;

    //    if (!m_nx) {
    //        for (int i = 0; i < z.size(); i++) {
    //            z[i].u_num *= -1;
    //        }
    //    }
    cout << endl;
    // Выводим дано
    cout << "Z = ";
    for (int i = 0; i < z.size(); i++) {
        cout_fract(z[i], 1);
        cout << "X" << i + 1;
        if (i < z.size() - 1) {
            if (z[i + 1].u_num >= 0) {
                cout << " + ";
            } else {
                cout << " - ";
            }
        } else {
            cout << " -> ";
            if (m_nx) {
                cout << "max";
            } else {
                cout << "min";
            }
        }
    }
    cout << endl
         << endl;
    if (matrix_lim.isEmpty()) {
        qDebug("Ограничений нет и функция имеет ∞ решений!");
        exit(0);
    }
    cout << "Матрица ограничений:" << endl;
    cout_matrix(matrix_lim);
}

void MainClass::transit_to_can()
{
    // Перевод в каноническую форму
    int count_simb = 0;
    // Определяем кол-во неравенств
    for (int i = 0; i < sinbol.size(); i++) {
        if (sinbol[i] != '=') {
            count_simb++;
        }
    }
    // Копируем матрицу lim в матрицу can без ответов
    copy_matrix_no_horny(matrix_lim, matrix_can);
    // Добавляем свободную переменную взависимости от знака
    for (int i = 0; i < sinbol.size(); i++) {
        if (sinbol[i] == '>') {
            for (int j = 0; j < count_simb; j++) {
                if (i == j || count_simb == 1) {
                    matrix_can[i] << fract(-1, 1);
                } else {
                    matrix_can[i] << fract(0, 1);
                }
            }
        }
        if (sinbol[i] == '<') {
            for (int j = 0; j < count_simb; j++) {
                if (i == j || count_simb == 1) {
                    matrix_can[i] << fract(1, 1);
                } else {
                    matrix_can[i] << fract(0, 1);
                }
            }
        }
        if (sinbol[i] == '=') {
            for (int j = 0; j < count_simb; j++) {
                matrix_can[i] << fract(0, 1);
            }
        }
        // Возвращаем ответ наместо
        matrix_can[i] << matrix_lim[i][matrix_lim[i].size() - 1];
    }
    // Выводим матрицу can
    polar_right_part();
    cout << "Матрица в канонической форме:" << endl;
    cout_matrix(matrix_can);
}

void MainClass::polar_right_part()
{
    // Если правая часть матрицы отрицательная, то домножаем всю строку на -1
    for (int i = 0; i < matrix_can.size(); i++) {
        if (matrix_can[i][matrix_can[i].size() - 1].u_num < 0) {
            for (int j = 0; j < matrix_can[i].size(); j++) {
                matrix_can[i][j].u_num = matrix_can[i][j].u_num * -1;
            }
        }
    }
}

void MainClass::copy_matrix_no_horny(QVector<QVector<fract>> matrix_in, QVector<QVector<fract>>& matrix_out)
{
    QVector<fract> dima;
    //Копирование матрицы без правой части
    for (int i = 0; i < matrix_in.size(); i++) {
        dima.clear();
        for (int j = 0; j < matrix_in[i].size() - 1; j++) {
            dima << matrix_in[i][j];
        }
        matrix_out << dima;
    }
}

void MainClass::back_horny(QVector<QVector<fract>> matrix_no_horny, QVector<QVector<fract>>& matrix_horny)
{
    QVector<fract> right_part;
    QVector<fract> dima;
    // Соединение матрицы без правой части с правой частью другой матрицы
    for (int i = 0; i < matrix_horny.size(); i++) {
        right_part << matrix_horny[i][matrix_horny[i].size() - 1];
    }
    matrix_horny.clear();
    for (int i = 0; i < matrix_no_horny.size(); i++) {
        dima.clear();
        for (int j = 0; j < matrix_no_horny[i].size(); j++) {
            dima << matrix_no_horny[i][j];
        }
        dima << right_part[i];
        matrix_horny << dima;
    }
}

void MainClass::check_origin_basis()
{
    // Поиск базисов
    QVector<QVector<fract>> matrix;
    // Убераем правую часть
    copy_matrix_no_horny(matrix_can, matrix);
    gen_basis.resize(matrix_can.size(), 0);
    gen_basis_index.resize(matrix_can.size(), 0);
    // Поиск изначальных базисов
    for (int i = 0, count_0 = 0, count_1 = 0, row; i < matrix[0].size(); i++) {
        count_0 = row = count_1 = 0;
        for (int j = 0; j < matrix.size(); j++) {
            if (matrix[j][i].u_num == 1) {
                count_1++;
                row = j;
            }
            if (matrix[j][i].u_num == 0) {
                count_0++;
            }
        }
        // Если в столбце 1 - единица и N-1 - нулей, то это базис
        if (count_0 == matrix.size() - 1 && count_1 == 1) {
            // Записываем, что это изначальный базис и запоменаем его номер в строке
            gen_basis[row] = 1;
            gen_basis_x << 1;
            gen_basis_index[row] = i;
        } else {
            gen_basis_x << 0;
        }
    }
    count_synt_basis = gen_basis.size();
    // Сколько нужно искуственных базисов
    for (int i = 0; i < gen_basis.size(); i++) {
        if (gen_basis[i] == 1) {
            count_synt_basis--;
        }
    }
    // Добавляем искуственные базисы
    for (int i = 0, gena = 0; i < matrix.size(); i++) {
        for (int j = 0; j < count_synt_basis; j++) {
            // Если в этой строке уже есть базис, то пишем нули, иначе создаем искуственный базис
            if (gen_basis[i] != 0) {
                matrix[i] << fract(0, 1);
            } else {
                if (j == gena) {
                    // Сохраняем J-ый индекс базиса в строке
                    gen_basis_index[i] = matrix[i].size();
                    matrix[i] << fract(1, 1);
                    gena++;
                    // Записываем, что это искуственный базис
                    gen_basis_x << -1;
                    gen_basis[i] = -1;
                } else {
                    matrix[i] << fract(0, 1);
                }
            }
        }
    }
    // Возвращаем правую часть
    back_horny(matrix, matrix_can);
    cout << "Добавляем искусственный базис, если это необходимо:" << endl;
    cout_matrix(matrix_can);
}

void MainClass::cout_table(QVector<QVector<fract>> table)
{
    // Вывод симплексной таблицы
    // Вывод верхней строки
    cout << "B\t";
    for (int i = 0; i < table[0].size(); i++) {
        if (i == table[0].size() - 1) {
            cout << "Bi\t";
        } else {
            cout << "X" << i + 1 << "\t";
        }
    }
    cout << endl
         << endl;
    for (int i = 0; i < table.size(); i++) {
        for (int j = 0; j < table[0].size(); j++) {
            if (j == 0) {
                // Вывод базисных переменных
                if (i < table.size() - 1) {
                    cout << "X" << gen_basis_index[i] + 1 << "\t";
                } else {
                    cout << "Z\t";
                }
            }
            // Вывод значений матрицы
            cout_fract(table[i][j]);
            cout << "\t";
        }
        cout << endl
             << endl;
    }
    // Если М строка существует, то выводим ее
    if (!m.isEmpty()) {
        cout << "M\t";
        for (int i = 0; i < m.size(); i++) {
            cout_fract(m[i]);
            cout << "\t";
        }
        cout << endl
             << endl;
    }
}

bool MainClass::check_opti(QVector<QVector<fract>> table)
{
    // Проверка оптимальности по Z

    for (int i = 0; i < table[table.size() - 1].size() - 1; i++) {
        if (m_nx) {
            if (table[table.size() - 1][i].u_num < 0) {
                return false;
            }
        } else {
            if (table[table.size() - 1][i].u_num > 0) {
                return false;
            }
        }
    }
    return true;
}

bool MainClass::check_opt_m()
{
    // Проверка оптимальности по М
    if (m.isEmpty()) {
        return false;
    }
    for (int i = 0; i < m.size() - 1; i++) {
        if (m[i].u_num < 0) {
            return false;
        }
    }
    return true;
}

bool MainClass::find_sint_basis()
{
    // Остались ли искуственные базисы?
    for (int i = 0; i < gen_basis_x.size(); i++) {
        if (gen_basis_x[i] < 0) {
            return true;
        }
    }
    return false;
}

void MainClass::calculate_m_vec(QVector<QVector<fract>> table)
{
    // Обнуляем М строку
    for (int i = 0; i < m.size(); i++) {
        m[i].u_num = 0;
    }
    // Если есть синтетические базисы, то расчитываем строку, иначе трем вектор
    if (find_sint_basis()) {
        qDebug() << gen_basis_x;
        for (int i = 0; i < table.size() - 1; i++) {
            for (int j = 0; j < table[0].size(); j++) {
                if (gen_basis[i] == -1) {
                    if (j < table[0].size() - 1) {
                        // Если в этом столбце нет базиса, то считаем, иначе записываем ноль
                        if (gen_basis_x[j] == 0) {
                            m[j] = m[j] + table[i][j];
                        } else {
                            m[j] = fract(0, 1);
                        }
                    } else {
                        m[j] = m[j] + table[i][j];
                    }
                }
            }
        }
        // Домножаем все на -1
        for (int i = 0; i < m.size(); i++) {
            m[i].u_num *= -1;
        }
    } else {
        m.clear();
    }
}

void MainClass::show_answer(QVector<QVector<fract>> table)
{
    cout << "Z = (";
    bool flag = false;
    for (int i = 0; i < table[0].size() - 1 - count_synt_basis; i++) {
        for (int j = 0; j < gen_basis_index.size(); j++) {
            if (i == gen_basis_index[j]) {
                for (int g = 0; g < gen_basis_index.size(); g++) {
                    if (table[g][gen_basis_index[j]].u_num == 1) {
                        cout_fract(table[g][table[g].size() - 1]);
                        flag = true;
                    }
                }
            }
        }
        if (!flag) {
            cout << 0;
        } else {
            flag = false;
        }
        if (i < table[0].size() - 2 - count_synt_basis) {
            cout << ", ";
        } else {
            cout << ") = ";
        }
    }
    cout_fract(table[table.size() - 1][table[0].size() - 1]);
    cout << endl;
}

bool MainClass::check_solution(QVector<QVector<fract>> table)
{
    if (check_opt_m()) {
        // Остались ли искуственные базисы
        if (find_sint_basis()) {
            qDebug("Искуственный базис остался, значит функция не имеет решений.");
        } else {
            show_answer(table);
        }
        return false;
    } else {
        if (m.isEmpty() && check_opti(table)) {
            // Остались ли искуственные базисы
            if (find_sint_basis()) {
                qDebug("Искуственный базис остался, значит функция не имеет решений.");
            } else {
                show_answer(table);
            }
            return false;
        }
    }
    return true;
}

void MainClass::simplex_method()
{
    int master_col, master_row, table_num = 1;
    // Сам симплекс метод
    QVector<fract> dima;
    QVector<QVector<fract>> table;
    // Копируем матрицу в таблицу
    table = matrix_can;
    // Заполняем строку М
    m.resize(table[0].size(), fract(0, 1));
    qDebug() << gen_basis;
    // Добавляем строку Z в таблицу
    for (int i = 0; i < table[0].size(); i++) {
        if (i < z.size()) {
            dima << fract(z[i].u_num * -1, z[i].d_num);
        } else {
            dima << fract(0, 1);
        }
    }
    table << dima;
    dima.clear();
    // Считаем М строку
    calculate_m_vec(table);
    cout << "Симплекс таблица №" << table_num << ":" << endl;
    table_num++;
    cout_table(table);
    fract tim, cur_tim, cur_mult_tim;
    while (check_solution(table)) {
        master_col = master_row = 0;
        // true - max / false - min
        if (!m.isEmpty()) {
            for (int i = 1; i < m.size() - 1; i++) {
                if (m[i].u_num < 0) {
                    if (m[master_col] > m[i]) {
                        master_col = i;
                    }
                }
            }

        } else if (m_nx) {
            // Если max выбираем наибольший по модулю отрицательный элемент в Z
            // и определяем ведущий столбец
            for (int i = 1; i < table[table.size() - 1].size() - 1; i++) {
                if (table[table.size() - 1][i].u_num < 0) {
                    if (table[table.size() - 1][master_col] > table[table.size() - 1][i]) {
                        master_col = i;
                    }
                }
            }
            // Иначе выбираем наибольший положительный элемент в Z
            // и определяем ведущий столбец
        } else {
            for (int i = 1; i < table[table.size() - 1].size() - 1; i++) {
                if (table[table.size() - 1][i].u_num > 0) {
                    if (table[table.size() - 1][master_col] < table[table.size() - 1][i]) {
                        master_col = i;
                    }
                }
            }
        }
        // Опрtделяем ведущую строку
        tim = cur_tim = fract(0, 1);
        //Находим первое неотрицательное отношение B и ведущего столбца
        for (int i = 0; i < table.size() - 1; i++) {
            if (table[i][master_col].u_num != 0 && (table[i][table[i].size() - 1].u_num * table[i][master_col].u_num) > 0) {
                tim = table[i][table[i].size() - 1] / table[i][master_col];
                master_row = i;
                cout << "MASTER_ROW = " << master_row << endl;
                break;
            }
        }
        cout_fract(tim);
        //Проходим по всем оставшимся строкам, выбирая наименьшее положительное отношение B и ведущего столбца
        for (int i = master_row; i < table.size() - 1; i++) {

            if (table[i][table[i].size() - 1].u_num * table[i][master_col].u_num > 0) {
                cur_tim = table[i][table[i].size() - 1] / table[i][master_col];
                if (cur_tim < tim) {
                    master_row = i;
                    tim = cur_tim;
                }
            }
        }
        // Делим строку на ведущий элемент table[master_row][master_col]
        cur_mult_tim = table[master_row][master_col];
        for (int i = 0; i < table[0].size(); i++) {
            table[master_row][i] = table[master_row][i] / cur_mult_tim;
        }
        // Зануляем все в кроме ведущего элемента в столбце
        for (int i = 0; i < table.size(); i++) {
            if (i != master_row) {
                cur_mult_tim = table[i][master_col];
                for (int j = 0; j < table[i].size(); j++) {
                    table[i][j] = table[i][j] - (table[master_row][j] * cur_mult_tim);
                }
            }
        }
        //        cout << "MASTER_row = " << master_row << " MASTER_col = " << master_col << endl;
        if (!m.isEmpty()) {
            cur_mult_tim = m[master_col];
            for (int j = 0; j < m.size(); j++) {
                m[j] = m[j] - (table[master_row][j] * cur_mult_tim);
            }
        }

        //Зануляем в таблице столбик по исключенному базису
        if (gen_basis_x[gen_basis_index[master_row]] == -1) {
            for (int i = 0; i < table.size(); i++) {
                table[i][gen_basis_index[master_row]] = fract(0, 1);
            }
            m[gen_basis_index[master_row]] = fract(0, 1);
        }
        //Исключение искусственного базиса из списка
        gen_basis_x[gen_basis_index[master_row]] = 0;
        gen_basis_index[master_row] = master_col;
        gen_basis_x[master_col] = 1;
        gen_basis[master_row] = 1;
        calculate_m_vec(table);

        cout << "Симплекс таблица №" << table_num << ":" << endl;
        table_num++;
        cout_table(table);
    }
}
