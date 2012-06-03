#ifndef GAUSSJORDANO_H
#define GAUSSJORDANO_H

#include <QtGlobal>
#include <QObject>

class Subset
{
private:
   qint8 N;
   qint8 K;
   qint8 *arr;

public:
    Subset(qint8 n, qint8 k);
    bool next();
    qint8 *GetArr();
    ~Subset();
};

class Permutations
{
    qint8 *st;
    qint8 *v;
    qint8 n;
    void Swap(qint8 &a, qint8 &b);
    void Reverse(qint8 *v, qint8 start);

public:
    Permutations(qint8 *V, qint8 i_n);
    bool NextPermutation();
    qint8 *GetVector();
    //void OutConsole();
    ~Permutations();
};

class FreeTerm   //класс, в котором хранится свободный член b (правая часть в равенстве(уравнении) в системе)
{
public:
    double m_x1; //коэффициент при x1
    double m_x2; //коэффициент при х2
    double m_c;  //просто число

    FreeTerm(double x1, double x2, double c);
    FreeTerm();
    FreeTerm operator+(double a);
    FreeTerm operator+(FreeTerm a);
    FreeTerm operator-(double a);
    FreeTerm operator-(FreeTerm a);
    FreeTerm operator*(double a);
    FreeTerm operator/(double a);
    void operator=(FreeTerm a);
};

class GaussJordano //класс реализующий алгорим Гаусса-Жордано
{
private:
    double **m_arrayOfXCoef;  //матрица коэффициентов при х
    FreeTerm *m_arrayOfFreeMem; // матрица-столбец свободных членов(содержит х1, х2 и сам свободный член)
    quint8 m_row;       // количество переменных без х1 и х2, а так же количество уравнений в системе

    bool Check(quint8 k);  // проверка не все ли элементы столбца равны 0
    void Reduction(quint8 k);	// перестановка строк таким образом, чтобы следущий разрешающий элементы стоящий на главной диагонали не был равен 0
    void DeleteAB(); // удаление масивов коэффициентов при х и свободных членов
    //void OutMatr();  // вывод матрицы в консоль
    double **Sample(); // запись в массив результатов вычислений

public:
    void Set(double** a, double* b, quint8 n); // установка новых значений переменных a, b и n
    GaussJordano(double** a, double* b, quint8 n);
    double **Run(); // Выполнение основного алгоритма возращает NULL если в одном солбце все элементы равны 0
    ~GaussJordano();
};

class SystemLinearEquations
{
private:
    double **m_arrayOfXCoef; // массив с индексами при х
    double *m_arrayOfFreeMem; // массив свободных членов
    double *m_arrayOfZCoef; // коэфициеты в фукции z (первые n членов - коэффициенты при х по порядку, а n + 1 член просто число)
    qint8 m_col;  // количество переменных (т.е. количество столбцов массива а)
    qint8 m_row;  //количество уравнений (т.е. количество строк массива а)

    double *Sum(double **Variables, qint8 i); // подстановка переменных в уравнения не учавствовавшие в методе гаусса-жордано

public:
    SystemLinearEquations(double *z, double **a, double *b, quint8 n, quint8 m);
    double **Solve(); // Решение системы, если не может решить возвращает NULL
    //void OutSolve(); // вывод ответа в консоль
    ~SystemLinearEquations();
};

#endif // GAUSSJORDANO_H
