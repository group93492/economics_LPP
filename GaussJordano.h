#ifndef GAUSSJORDANO_H
#define GAUSSJORDANO_H

#include <QtGlobal>
#include <QObject>
#include <math.h>

class FreeTerm  //класс, в котором хранится свободный член b (правая часть в равенстве(уравнении) в системе)
{
private:
    double m_x1; //коэффициент при x1
    double m_x2; //коэффициент при х2
    double m_c;  //просто число

public:
    FreeTerm(double x1, double x2, double c);
    FreeTerm();
    double x1();
    double x2();
    double c();
    void setX1(double x1);
    void setX2(double x2);
    void setC(double c);
    FreeTerm operator+(double a);
    FreeTerm operator-(double a);
    FreeTerm operator*(double a);
    FreeTerm operator/(double a);
    FreeTerm operator+(FreeTerm a);
    FreeTerm operator-(FreeTerm a);
};

class GaussJordano : public QObject//класс реализующий алгорим Гаусса-Жордано
{
    Q_OBJECT

private:
    double **m_arrayOfXCoef;  //матрица коэффициентов при х
    FreeTerm *m_arrayOfFreeMem; // матрица-столбец свободных членов(содержит х1, х2 и сам свободный член)
    quint8 n;       // количество переменных без х1 и х2, а так же количество уравнений в системе
    bool Check(quint8 k);  // проверка не все ли элементы столбца равны 0
    void Reduction(quint8 k);	// перестановка строк таким образом, чтобы следущий разрешающий элементы стоящий на главной диагонали не был равен 0
    void DeleteAB(); // удаление масивов коэффициентов при х и свободных членов
    //void OutMatr();  // вывод матрицы в консоль
    double **Sample(); // запись в массив результатов вычислений

public:
    GaussJordano(double** m_arrayOfXCoef, double* m_arrayOfFreeMem, quint8 n);
    void Set(double** m_arrayOfXCoef, double* m_arrayOfFreeMem, quint8 n); // установка новых значений переменных a, b и n
    double **Run(); // Выполнение основного алгоритма возращает NULL если в одном солбце все элементы равны 0
    ~GaussJordano();
};

class SystemLinearEquations : public QObject
{
    Q_OBJECT

private:
    double **m_arrayOfXCoef; // массив с индексами при х
    double *m_arrayOfFreeMem; // массив свободных членов
    double *m_arrayOfZCoef; // коэфициеты в фукции z (первые n членов - коэффициенты при х по порядку, а n + 1 член просто число)
    quint8 m_col;  // количество переменных (т.е. количество столбцов массива а)
    quint8 m_row;  //количество уравнений (т.е. количество строк массива а)
    bool Reduction(); // перестановка строк, чтобы можно было выразить все x, если какой-то х нельзя выразить возвращает false иначе true
    double *Sum(double **Variables, quint8 i); // подстановка переменных в уравнения не учавствовавшие в методе гаусса-жордано

public:
    SystemLinearEquations(double *z, double **a, double *b, quint8 n, quint8 m);
    double **Solve(); // Решение системы, если не может решить возвращает NULL
//    void OutSolve(); // вывод ответа в консоль
    ~SystemLinearEquations();

};
#endif // GAUSSJORDANO_H
