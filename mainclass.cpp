#include "mainclass.h"
#include <iostream>

using namespace std;

void cout_fract(fract f, bool abs = 0){
    // Просто вывод дроби
    if(abs){
        cout << qAbs(f.u_num);
    } else{
        cout << f.u_num;
    }
    if(f.d_num != 1){
        cout << "/" << f.d_num;
    }
}

void cout_matrix(QVector<QVector<fract>> matrix){
    // Просто вывод матрицы
    for(int i = 0; i < matrix.size(); i++){
        for(int j = 0; j <matrix[0].size(); j++){
            if(matrix[i][j].d_num == 1){
                cout<<matrix[i][j].u_num<<"\t";
            }else{
                cout<<matrix[i][j].u_num<<"/"<<matrix[i][j].d_num<<"\t";
            }
        }
        cout << endl << endl;
    }
    cout << endl;
}

void read_matrix(QVector<QVector<fract>>& matrix){
    QFile file(":/matrix");
    if(!file.size()){
        qDebug() << "File is empty!";
        exit(0);
    }
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QStringList elMat;
        QStringList elMatI;
        QString lineStr;
        QVector<fract> lineEl;
        while(!file.atEnd()){
            lineStr = file.readLine();
            elMat = lineStr.split("__");
            lineEl.clear();
            for(int i=0; i<elMat.size(); i++){
                fract oneEl;
                if(elMat[i].contains('/')){
                    elMatI = elMat[i].split("/");
                    if(elMatI[1] == "0"){
                        qDebug("Division on null!");
                        exit(0);
                    }
                    oneEl.u_num = elMatI[0].toInt();
                    oneEl.d_num = elMatI[1].toInt();
                    if(oneEl.d_num < 0){
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

}

void MainClass::init_date(){
    // Считываем матрицу ограничений
    read_matrix(matrix_lim);
    // Заполняем вектор функции Z
    z << fract(1, 1) << fract(1, 1) << fract(-1, 1) << fract(-2, 1);
    // Заполняем вектор знаков данными из дано
    sinbol << '=' << '=' << '<';
    // true - max / false - min
    m_nx = false;
    // Выводим дано
    cout << "Z = ";
    for(int i = 0; i < z.size(); i++){
        cout_fract(z[i], 1);
        cout << "X" << i + 1;
        if(i < z.size() - 1) {
            if(z[i + 1].u_num >= 0){
                cout << " + ";
            } else {
                cout << " - ";
            }
        } else {
            cout << " -> ";
            if(m_nx) {
                cout << "max";
            } else {
                cout << "min";
            }
        }
    }
    cout << endl << endl;
    if(matrix_lim.isEmpty()){
        qDebug("Ограничений нет и функция имеет ∞ решений!");
        exit(0);
    }
    cout << "Матрица ограничений:" << endl;
    cout_matrix(matrix_lim);
}

void MainClass::transit_to_can(){
    // Перевод в каноническую форму
    int count_simb = 0;
    // Определяем кол-во неравенств
    for(int i = 0; i < sinbol.size(); i++){
        if(sinbol[i] != '='){
            count_simb++;
        }
    }
    // Копируем матрицу lim в матрицу can без ответов
    copy_matrix_no_horny(matrix_lim, matrix_can);
    // Добавляем свободную переменную взависимости от знака
    for(int i = 0; i < sinbol.size(); i++){
        if(sinbol[i] == '>'){
            for(int j = 0; j < count_simb; j++){
                if(i == j || count_simb == 1){
                    matrix_can[i] << fract(-1,1);
                } else {
                    matrix_can[i] << fract(0,1);
                }
            }
        }
        if(sinbol[i] == '<'){
            for(int j = 0; j < count_simb; j++){
                if(i == j || count_simb == 1){
                    matrix_can[i] << fract(1,1);
                } else {
                    matrix_can[i] << fract(0,1);
                }
            }
        }
        if(sinbol[i] == '='){
            for(int j = 0; j < count_simb; j++){
                matrix_can[i] << fract(0,1);
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

void MainClass::polar_right_part(){
    // Если правая часть матрицы отрицательная, то домножаем всю строку на -1
    for(int i = 0; i < matrix_can.size(); i++){
        if(matrix_can[i][matrix_can[i].size() - 1].u_num < 0){
            for(int j = 0; j < matrix_can[i].size(); j++){
                matrix_can[i][j].u_num = matrix_can[i][j].u_num * -1;
            }
        }
    }
}

void MainClass::copy_matrix_no_horny(QVector<QVector<fract>> matrix_in, QVector<QVector<fract>> &matrix_out){
    QVector<fract> dima;
    //Копирование матрицы без правой части
    for(int i = 0; i < matrix_in.size(); i++){
        dima.clear();
        for(int j = 0; j < matrix_in[i].size() - 1; j++){
            dima << matrix_in[i][j];
        }
        matrix_out << dima;
    }
}

void MainClass::back_horny(QVector<QVector<fract>> matrix_no_horny, QVector<QVector<fract>> &matrix_horny){
    QVector<fract> right_part;
    QVector<fract> dima;
    // Соединение матрицы без правой части с правой частью другой матрицы
    for(int i = 0; i < matrix_horny.size(); i++){
        right_part << matrix_horny[i][matrix_horny[i].size() - 1];
    }
    matrix_horny.clear();
    for(int i = 0; i < matrix_no_horny.size(); i++){
        dima.clear();
        for( int j = 0; j < matrix_no_horny[i].size(); j++){
            dima << matrix_no_horny[i][j];
        }
        dima << right_part[i];
        matrix_horny << dima;
    }
}

void MainClass::check_origin_basis(){
    // Поиск базисов
    QVector<QVector<fract>> matrix;
    // Убераем правую часть
    copy_matrix_no_horny(matrix_can, matrix);
    gen_basis.resize(matrix_can.size(), 0);
    // Поиск изначальных базисов
    for(int i = 0, count_0 = 0, count_1 = 0, row; i < matrix[0].size(); i++){
        count_0 = row = count_1 = 0;
        for(int j = 0; j < matrix.size(); j++){
            if(matrix[j][i].u_num == 1){
                count_1++;
                row = j;
            }
            if(matrix[j][i].u_num == 0){
                count_0++;
            }
        }
        if(count_0 == matrix.size() - 1 && count_1 == 1){
            gen_basis[row] = 1;
            gen_basis_x << 1;
            gen_basis_index << i;
        } else {
            gen_basis_x << 0;
        }
    }
    int count_synt_basis = gen_basis.size();
    // Сколько нужно искуственных базисов
    for(int i = 0; i < gen_basis.size(); i++){
        if(gen_basis[i] == 1){
            count_synt_basis--;
        }
    }
    // Добавляем искуственные базисы
    for(int i = 0, gena = 0; i < matrix.size(); i++){
        for(int j = 0; j < count_synt_basis; j++){
            if(gen_basis[i] == 1 || gen_basis[i] == -1){
                matrix[i] << fract(0, 1);
            } else {
                if(j == gena){
                    gen_basis_index << matrix[i].size();
                    matrix[i] << fract(1, 1);
                    gena++;
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

void MainClass::cout_table(QVector<QVector<fract>> table){
    // Вывод симплексной таблицы
    // Вывод верхней строки
    cout << "B\t";
    for(int i = 0; i < table[0].size(); i++){
        if(i == table[0].size() - 1){
            cout << "Bi\t";
        } else {
            cout << "X" << i + 1 << "\t";
        }
    }
    cout << endl << endl;
    for(int i = 0; i < table.size(); i++){
        for(int j = 0; j <table[0].size(); j++){
            if(j == 0){
                // Вывод базисных переменных
                if(i < table.size() - 1){
                    cout << "X" << gen_basis_index[i] + 1 << "\t";
                } else {
                    cout << "Z\t";
                }
            }
            // Вывод значений матрицы
            cout_fract(table[i][j]);
            cout << "\t";
//            if(table[i][j].d_num == 1){
//                cout<<table[i][j].u_num<<"\t";
//            }else {
//                cout<<table[i][j].u_num<<"/"<<table[i][j].d_num<<"\t";
//            }
        }
        cout << endl << endl;
    }
    cout << "M\t";
    for(int i = 0; i < m.size(); i++){
        cout_fract(m[i]);
        cout << "\t";
    }
    cout << endl << endl;
}

bool MainClass::check_opti(QVector<QVector<fract>> table){
    for(int i = 0; i < table[table.size() - 1].size(); i++){
        if(table[table.size() - 1][i].u_num < 0){
            return false;
        }
    }
    return true;
}

bool MainClass::find_sint_basis(){
    for(int i = 0; i < gen_basis_x.size(); i++){
        if(gen_basis_x[i] < 0){
            return true;
        }
    }
    return false;
}

void MainClass::simplex_method(){
    int master_col, master_row, table_num = 1;
    // Сам симплекс метод
    QVector<fract> dima;
    Fraction frct;
    QVector<QVector<fract>> table;

    // Копируем матрицу в таблицу
    table = matrix_can;
    // Заполняем строку М
    m.resize(table[0].size(), fract(0, 1));
    qDebug() << gen_basis;
    if(find_sint_basis()){
        for(int i = 0; i < table.size(); i++){
            for(int j = 0; j < table[0].size(); j++){
                if(gen_basis[i] == -1){
                    m[j] = frct.sum(m[j], table[i][j]);
                }
            }
        }
        for(int i = 0; i < m.size(); i++){
            m[i].u_num *= -1;
        }
    }
    // Добавляем строку Z в таблицу
    for(int i = 0; i < table[0].size(); i++){
        if(i < z.size()){
            dima << fract(z[i].u_num * -1, z[i].d_num) ;
        } else {
            dima << fract(0, 1);
        }
    }
    table << dima;
    dima.clear();
    cout << "Симплекс таблица №" << table_num << ":" << endl;
    table_num++;
    cout_table(table);
    fract tim, cur_tim, cur_mult_tim;
    while(!check_opti(table)){
        master_col = master_row = 0;
        // true - max / false - min
        if(m_nx){
            // Если max выбираем наибольший по модулю отрицательный элемент в Z
            // и определяем ведущий столбец
            for(int i = 1; i < table[table.size() - 1].size() - 1; i++){
                if(table[table.size() - 1][i].u_num < 0){
                    if(table[table.size() - 1][master_col] > table[table.size() - 1][i]){
                        master_col = i;
                    }
                }
            }
            // Иначе выбираем наибольший положительный элемент в Z
            // и определяем ведущий столбец
        } else {
            for(int i = 1; i < table[table.size() - 1].size() - 1; i++){
                if(table[table.size() - 1][i].u_num > 0){
                    if(table[table.size() - 1][master_col] < table[table.size() - 1][i]){
                        master_col = i;
                    }
                }
            }
        }
        // Оперделяем ведущую строку
        for(int i = 0; i < table.size() - 1; i++){
            if(table[i][master_col].u_num != 0){
                if(i == 0){
                    tim = frct.mult(table[i][table[i].size() - 1], table[i][master_col], 1);
                } else {
                    cur_tim = frct.mult(table[i][table[i].size() - 1], table[i][master_col], 1);
                }
                if(tim > cur_tim && cur_tim.u_num > 0){
                    master_row = i;
                    tim = cur_tim;
                }
            }
        }
        // Делим строку на ведущий элемент table[master_row][master_col]
        cur_mult_tim = table[master_row][master_col];
        for(int i = 0; i < table[0].size(); i++){
            table[master_row][i] = frct.mult(table[master_row][i], cur_mult_tim, 1);
        }
        // Зануляем все в кроме ведущего элемента в столбце
        for(int i = 0; i < table.size(); i++){
            if(i != master_row){
                cur_mult_tim = table[i][master_col];
                for(int j = 0; j < table[i].size(); j++){
                    table[i][j] = frct.sum(table[i][j], frct.mult(table[master_row][j], cur_mult_tim), 1);
                }
            }
        }
        cout << "Симплекс таблица №" << table_num << ":" << endl;
        table_num++;
        cout_table(table);
        gen_basis_x[gen_basis_index[master_row]] = 0;
        gen_basis_index[master_row] = master_col;
        gen_basis_x[master_col] = 1;
        gen_basis[master_row] = 1;
    }
    // Остались ли искуственные базисы
        if(find_sint_basis()){
            qDebug("Искуственный базис остался, значит функция не имеет решений.");
        }
}
