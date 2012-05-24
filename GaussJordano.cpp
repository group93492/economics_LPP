#include "GaussJordano.h"

FreeTerm::FreeTerm(double x1, double x2, double c)
{
    m_x1 = x1;
    m_x2 = x2;
    m_c = c;
}

FreeTerm::FreeTerm()
{
}

double FreeTerm::x1()
{
    return m_x1;
}

double FreeTerm::x2()
{
    return m_x2;
}

double FreeTerm::c()
{
    return m_c;
}

void FreeTerm::setX1(double x1)
{
    m_x1 = x1;
}

void FreeTerm::setX2(double x2)
{
    m_x2 = x2;
}

void FreeTerm::setC(double c)
{
    m_c = c;
}

FreeTerm FreeTerm::operator +(double a)
{
    return FreeTerm(m_x1, m_x2, m_c + a);
}

FreeTerm FreeTerm::operator +(FreeTerm a)
{
    return FreeTerm(m_x1 + a.x1(), m_x2 + a.x2(), m_c + a.c());
}

FreeTerm FreeTerm::operator -(double a)
{
    return FreeTerm(m_x1, m_x2, m_c - a);
}

FreeTerm FreeTerm::operator -(FreeTerm a)
{
    return FreeTerm(m_x1 - a.x1(), m_x2 - a.x2(), m_c - a.c());
}

FreeTerm FreeTerm::operator *(double a)
{
    return FreeTerm(a * m_x1, a * m_x2, a * m_c);
}

FreeTerm FreeTerm::operator /(double a)
{
    return FreeTerm(m_x1 / a, m_x2 / a, m_c / a);
}

bool GaussJordano::Check(quint8 k)
{
    for (quint8 i = 0; i < n; i++)
            if (m_arrayOfXCoef[i][k] != 0)
            {
                return true;
            }
    return false;
}

void GaussJordano::Reduction(quint8 k)
{
    for (quint8 i = k; i < n; i++)
    {
        if (m_arrayOfXCoef[i][k] != 0)
        {
            for (quint8 j = 0; j < n; j++)
            {
                double Temp = m_arrayOfXCoef[k][j];
                m_arrayOfXCoef[k][j] = m_arrayOfXCoef[i][j];
                m_arrayOfXCoef[i][j] = Temp;
            }
            FreeTerm Temp = m_arrayOfFreeMem[k];
            m_arrayOfFreeMem[k] = m_arrayOfFreeMem[i];
            m_arrayOfFreeMem[i] = Temp;
            return;
        }
    }
}

void GaussJordano::DeleteAB()
{
    delete [] m_arrayOfFreeMem;
    for (int i = 0; i < n; i++)
        delete [] m_arrayOfXCoef[i];
    delete [] m_arrayOfXCoef;
}

//void GaussJordano::OutMatr()
//{
//    for (int i = 0; i < n; i++)
//    {
//        for (int j = 0; j < n; j++)
//        {
//            printf("%6.2f", a[i][j]);
//            cout << " ";
//        }
//        cout << "| ";
//        printf("%6.2f x1", b[i].x1);
//        cout << " ";
//        printf("%6.2f x2", b[i].x2);
//        cout << " ";
//        printf("%6.2f", b[i].c);
//        cout << endl;
//    }
//    cout << "\n\n";
//}

double **GaussJordano::Sample()
{
    double **Temp = new double*[n];
    for (quint8 i = 0; i < n; i++)
    {
        Temp[i] = new double[3];
        Temp[i][0] = m_arrayOfFreeMem[i].x1();
        Temp[i][1] = m_arrayOfFreeMem[i].x2();
        Temp[i][2] = m_arrayOfFreeMem[i].c();
    }
    return Temp;
}

void GaussJordano::Set(double **a, double *b, quint8 n)
{
    if (a != NULL && b != NULL)
        DeleteAB();//протестировать
    this->n = n - 2;
    this->m_arrayOfXCoef = new double*[this->n];
    this->m_arrayOfFreeMem = new FreeTerm[this->n];

    for (quint8 i = 0; i < this->n; i++)
    {
        this->m_arrayOfXCoef[i] = new double[this->n];
        for (int j = 0; j < this->n; j++)
        {
            this->m_arrayOfXCoef[i][j] = a[i][j + 2];
        }
        this->m_arrayOfFreeMem[i].setX1(-a[i][0]);
        this->m_arrayOfFreeMem[i].setX2(-a[i][1]);
        this->m_arrayOfFreeMem[i].setC(b[i]);
    }
}

GaussJordano::GaussJordano(double **a, double *b, quint8 n)
{
    this->m_arrayOfXCoef = NULL;
    this->m_arrayOfFreeMem = NULL;
    Set(a, b, n);
    //OutMatr();
}

double **GaussJordano::Run()
{
    for (quint8 k = 0; k < n; k++)
    {
        if (!Check(k))
            return NULL;
        if (m_arrayOfXCoef[k][k] == 0)
            Reduction(k);

        double** newa = new double*[n]; //создание переменных для следующего массива коэфициентов перед х и сободных членов
        FreeTerm* newb = new FreeTerm[n];

        for (quint8 i = 0; i < n; i++) //инициализация этих переменных
            newa[i] = new double[n];

        for (quint8 i = 0; i < n; i++)
        {
            for(quint8 j = 0; j < n; j++)
                newa[i][j] = m_arrayOfXCoef[i][j];
            newb[i] = m_arrayOfFreeMem[i];
        }

        for (quint8 l = 0; l < n; l++)  //заменна всех элементов столбца кроме разрешающего на 0, а разрешающего на 1
            if (l != k)
                newa[l][k] = 0;
            else
                newa[l][k] = 1;

        for (quint8 i = 0; i < n; i++) // вычисление остальных коэффициентов
        {
            for (quint8 j = k; j < n; j++)
            {
                if (i == k)
                    newa[i][j] = m_arrayOfXCoef[i][j] / m_arrayOfXCoef[k][k];
                else
                {
                    if (j != k)
                    {
                        newa[i][j] = m_arrayOfXCoef[i][j] - m_arrayOfXCoef[i][k] * m_arrayOfXCoef[k][j] / m_arrayOfXCoef[k][k];
                    }
                }
            }
            if (i == k)
                newb[i] = m_arrayOfFreeMem[i] / m_arrayOfXCoef[k][k];
            else
            {
                newb[i] = m_arrayOfFreeMem[i] - m_arrayOfFreeMem[k] * m_arrayOfXCoef[i][k] / m_arrayOfXCoef[k][k];
            }
        }

        DeleteAB(); // удаление прошлых матриц
        m_arrayOfXCoef = newa;
        m_arrayOfFreeMem = newb;
        //OutMatr();
    }
    return Sample();
}

GaussJordano::~GaussJordano()
{
    DeleteAB();
}

bool SystemLinearEquations::Reduction()
{
    for (int i = 0; i < m_col - 2; i++)
    {
        if (m_arrayOfXCoef[i][i + 2] != 0)
            continue;
        int count = i + 1;
        for (int j = i + 1; i < m_row; j++)
        {
            if (m_arrayOfXCoef[j][i + 2] != 0)
            {
                for (int k = 0; k < m_col; k++)
                {
                    double Temp = m_arrayOfXCoef[j][k];
                    m_arrayOfXCoef[j][k] = m_arrayOfXCoef[i][k];
                    m_arrayOfXCoef[i][k] = Temp;
                }
                double Temp = m_arrayOfFreeMem[j];
                m_arrayOfFreeMem[j] = m_arrayOfFreeMem[i];
                m_arrayOfFreeMem[i] = Temp;
                break;
            }
            count++;
        }
        if (count == m_row)
            return false;
    }
    return true;
}

double *SystemLinearEquations::Sum(double **Variables, quint8 i)
{
    double *Temp = new double[3];
    Temp[0] = m_arrayOfXCoef[i][0];
    Temp[1] = m_arrayOfXCoef[i][1];
    Temp[2] = -m_arrayOfFreeMem[i];
    for (quint8 j = 2; j < m_col; j++)
    {
        Temp[0] = Temp[0] + Variables[j - 2][0] * m_arrayOfXCoef[i][j];
        Temp[1] = Temp[1] + Variables[j - 2][1] * m_arrayOfXCoef[i][j];
        Temp[2] = Temp[2] + Variables[j - 2][2] * m_arrayOfXCoef[i][j];
    }
    return Temp;
}

SystemLinearEquations::SystemLinearEquations(double *z, double **a, double *b, quint8 n, quint8 m)
{
    m_arrayOfZCoef = z;
    m_arrayOfXCoef = a;
    m_arrayOfFreeMem = b;
    m_col = n;
    m_row = m;
}

double **SystemLinearEquations::Solve()
{
    if (!Reduction()) // Переставляем строки и проверяем можно ли решить систему
        return NULL;

    double *bForGJ = new double[m_col - 2]; //переменная b для метода Гаусса-Жордано
    double **aForGJ = new double*[m_col - 2]; //переменная a для метода Гаусса-Жордано

    for (quint8 i = 0; i < m_col - 2; i++) //инициализация массивов a и b для метода Гаусса-Жордано
    {
        bForGJ[i] = m_arrayOfFreeMem[i];
        aForGJ[i] = new double[m_col];
        for (quint8 j = 0; j < m_col; j++)
        {
            aForGJ[i][j] = m_arrayOfXCoef[i][j];
        }
    }

    GaussJordano Gauss(aForGJ, bForGJ, m_col);
    double **Variables = new double*[m_col - 2]; //массив хранящий выраженные переменные
    for (quint8 i = 0; i < m_col - 2; i++)
        Variables[i] = new double[3];
    Variables = Gauss.Run();

    double **Result = new double*[m_row + 1]; // подстановка переменных в уравнения не учавствовавшие в методе Гаусса-Жордано
    for (quint8 i = 0; i < m_row; i++)
    {
        Result[i] = new double[3];
        if (i < m_col - 2)
            for (quint8 j = 0; j < 3; j++)
                Result[i][j] = Variables[i][j];
        else
            Result[i] = Sum(Variables, i);
    }

    Result[m_row] = new double[3];
    Result[m_row][0] = m_arrayOfZCoef[0];
    Result[m_row][1] = m_arrayOfZCoef[1];
    Result[m_row][2] = m_arrayOfZCoef[m_col];
    for (quint8 j = 2; j < m_col; j++) // подстановка переменных в функцию z
    {
        Result[m_row][0] = Result[m_row][0] + Variables[j - 2][0] * m_arrayOfZCoef[j];
        Result[m_row][1] = Result[m_row][1] + Variables[j - 2][1] * m_arrayOfZCoef[j];
        Result[m_row][2] = Result[m_row][2] + Variables[j - 2][2] * m_arrayOfZCoef[j];
    }

    return Result;
}

//void SystemLinearEquations::OutSolve()
//{
//    double **Result = Solve();
//    if (Result == NULL)
//    {
//        cout << "error";
//        return;
//    }
//    for (quint8 i = 0; i < m_row + 1; i++)
//    {
//        if (i == m_col - 2 || i == m_row)
//            cout << "------------------------\n\n";
//        for (quint8 j = 0; j < 3; j++)
//        {
//            printf("%6.2f ", Result[i][j]);
//        }
//        cout << "\n\n";
//    }
//    cout << "\n\n";
//}

SystemLinearEquations::~SystemLinearEquations()
{
    for (quint8 i = 0; i < m_row; i++)
        delete[] m_arrayOfXCoef[i];
    delete[] m_arrayOfXCoef;
    delete[] m_arrayOfFreeMem;
    delete[] m_arrayOfZCoef;
}
