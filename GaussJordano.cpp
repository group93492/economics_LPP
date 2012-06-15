#include "GaussJordano.h"

Subset::Subset(qint8 n, qint8 k)
{
    N = n;
    K = k;
    arr = new qint8[k];
    for (quint8 i = 0; i < K; ++ i)
        arr [i] = i;
}

bool Subset::next()
{
    qint8 i = K - 1;
    while (arr [i] + K - i >= N)
        if (--i < 0)
            return 0;
    qint8 num = arr [i];
    for (; i < K; ++i)
        arr [i] = ++num;
    return 1;
}

qint8 *Subset::GetArr()
{
    return arr;
}

Subset::~Subset()
{
    delete []arr;
}

void Permutations::Swap(qint8 &a, qint8 &b)
{
    qint8 t = a;
    a = b;
    b = t;
}

void Permutations::Reverse(qint8 *v, qint8 start)
{
    for (quint8 i = 0; i < (n - start) / 2; i++)
        Swap(v[start + i], v[n - 1 - i]);
}

Permutations::Permutations(qint8 *V, qint8 i_n)
{
    n = i_n;
    //this->n = n;
    v = new qint8[n];
    //v = V;
    st = new qint8[n];
    for (quint8 i = 0; i < n; i++)
    {
        st[i] = 0;
        v[i] = V[i];
    }
}

bool Permutations::NextPermutation()
{
    if (n < 2)
        return false;
    qint8 pos = /*pos =*/ n - 2;
    while (pos >= 0)
    {
        if (st[pos] < n - 1 - pos)
        {
            Reverse(v, pos + 1);
            for (quint8 i = pos + 1; i < n; )
                st[i++] = 0;
            st[pos]++;
            Swap(v[pos], v[pos + st[pos]]);
            return true;
        }
        pos--;
    }
    return false;
}

qint8 *Permutations::GetVector()
{
    return v;
}

Permutations::~Permutations()
{
    delete []st;
    delete []v;
}

FreeTerm::FreeTerm(double x1, double x2, double c)
{
    m_x1 = x1;
    m_x2 = x2;
    m_c = c;
}

FreeTerm::FreeTerm()
{

}

FreeTerm FreeTerm::operator +(double a)
{
    return FreeTerm (m_x1, m_x2, m_c + a);
}

FreeTerm FreeTerm::operator +(FreeTerm a)
{
    return FreeTerm (m_x1 + a.m_x1, m_x2 + a.m_x2, m_c + a.m_c);
}

FreeTerm FreeTerm::operator -(double a)
{
    return FreeTerm (m_x1, m_x2, m_c - a);
}

FreeTerm FreeTerm::operator -(FreeTerm a)
{
    double newX1 = m_x1 - a.m_x1;
    double newX2 = m_x2 - a.m_x2;
    double newC = m_c - a.m_c;
    return FreeTerm (newX1, newX2, newC);
}

FreeTerm FreeTerm::operator *(double a)
{
    return FreeTerm (a * m_x1, a * m_x2, a * m_c);
}

FreeTerm FreeTerm::operator /(double a)
{
    return FreeTerm (m_x1 / a, m_x2 / a, m_c / a);
}

void FreeTerm::operator =(FreeTerm a)
{
    m_x1 = a.m_x1;
    m_x2 = a.m_x2;
    m_c = a.m_c;
}

bool GaussJordano::Check(quint8 k)
{
    for (quint8 i = k; i < m_row; i++) // i = 0
            if (m_arrayOfXCoef[i][k] != 0)
            {
                return true;
            }
    return false;
}

void GaussJordano::Reduction(quint8 k)
{
    for (quint8 i = k; i < m_row; i++)
    {
        if (m_arrayOfXCoef[i][k] != 0)
        {
            for (quint8 j = 0; j < m_row; j++)
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
    delete []m_arrayOfFreeMem;
    for (quint8 i = 0; i < m_row; i++)
        delete []m_arrayOfXCoef[i];
    delete []m_arrayOfXCoef;
}

double **GaussJordano::Sample()
{
    double **Temp = new double*[m_row];
    for (quint8 i = 0; i < m_row; i++)
    {
        Temp[i] = new double[3];
        Temp[i][0] = m_arrayOfFreeMem[i].m_x1;
        Temp[i][1] = m_arrayOfFreeMem[i].m_x2;
        Temp[i][2] = m_arrayOfFreeMem[i].m_c;
    }
    return Temp;
}

void GaussJordano::Set(double **a, double *b, quint8 n)
{
    if ((this->m_arrayOfXCoef != NULL) && (this->m_arrayOfFreeMem != NULL))
        DeleteAB();
    this->m_row = n - 2;
    this->m_arrayOfXCoef = new double*[this->m_row];
    this->m_arrayOfFreeMem = new FreeTerm[this->m_row];

    for (quint8 i = 0; i < this->m_row; i++)
    {
        this->m_arrayOfXCoef[i] = new double[this->m_row];
        for (quint8 j = 0; j < this->m_row; j++)
        {
            this->m_arrayOfXCoef[i][j] = a[i][j + 2];
        }
        this->m_arrayOfFreeMem[i].m_x1 = -a[i][0];
        this->m_arrayOfFreeMem[i].m_x2 = -a[i][1];
        this->m_arrayOfFreeMem[i].m_c = b[i];
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
    qint8 *StartValuesPerm = new qint8[m_row];
    for (quint8 i = 0; i < m_row; i++)
        StartValuesPerm[i] = i;
    Permutations Perm(StartValuesPerm, m_row);
    bool NextPermExist = true;

    double **BackupA = new double*[m_row]; // сохранение исходной системы
    FreeTerm *BackupB = new FreeTerm[m_row];
    for (quint8 i = 0; i < m_row; i++)
    {
        BackupA[i] = new double[m_row];
        BackupB[i] = m_arrayOfFreeMem[i];
        for (quint8 j = 0; j < m_row; j++)
            BackupA[i][j] = m_arrayOfXCoef[i][j];
    }

    while (NextPermExist)
    {
        qint8 *Indexes = Perm.GetVector();
        for (quint8 i = 0; i < m_row; i++)
        {
            m_arrayOfFreeMem[i] = BackupB[Indexes[i]];
            for (quint8 j = 0; j < m_row; j++)
                m_arrayOfXCoef[i][j] = m_arrayOfXCoef[Indexes[i]][j];
        }

        bool OperationCompleted = true;

        for (quint8 k = 0; k < m_row; k++)
        {
            if (!Check(k))
            {
                OperationCompleted = false;
                break;
            }
            if (m_arrayOfXCoef[k][k] == 0)
                Reduction(k);

            double** newa = new double*[m_row]; //создание переменных для следующего массива коэфициентов перед х и сободных членов
            FreeTerm* newb = new FreeTerm[m_row];

            for (quint8 i = 0; i < m_row; i++) //инициализация этих переменных
                newa[i] = new double[m_row];

            for (quint8 i = 0; i < m_row; i++)
            {
                for(quint8 j = 0; j < m_row; j++)
                    newa[i][j] = m_arrayOfXCoef[i][j];
                newb[i] = m_arrayOfFreeMem[i];
            }

            for (quint8 l = 0; l < m_row; l++)  //заменна всех элементов столбца кроме разрешающего на 0, а разрешающего на 1
                if (l != k)
                    newa[l][k] = 0;
                else
                    newa[l][k] = 1;

            for (quint8 i = 0; i < m_row; i++) // вычисление остальных коэффициентов
            {
                for (quint8 j = k; j < m_row; j++)
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

        if (OperationCompleted) // проверка смог ли алгоритм решить систему
        {
            for (quint8 i = 0; i < m_row; i++) // Протестировать
                delete []BackupA[i];
            delete []BackupA;
            delete []BackupB;
            return Sample(); //возращение результата
        }
        else
            NextPermExist = Perm.NextPermutation(); // генерация следующей перестановки если она есть
    }
    return NULL;
}

GaussJordano::~GaussJordano()
{
    DeleteAB();
    m_arrayOfXCoef = NULL;
    m_arrayOfFreeMem = NULL;
}

double *SystemLinearEquations::Sum(double **Variables, qint8 i)
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
    m_col = n;
    m_row = m;
    this->m_arrayOfZCoef = new double[m_col + 1];
    this->m_arrayOfFreeMem = new double[m_row];
    this->m_arrayOfXCoef = new double*[m_row];

    for (quint8 i = 0; i < m_col + 1; i++)
        this->m_arrayOfZCoef[i] = z[i];
    for (quint8 i = 0; i < m_row; i++)
    {
        this->m_arrayOfXCoef[i] = new double[m_col];
        this->m_arrayOfFreeMem[i] = b[i];
        for (quint8 j = 0; j < m_col; j++)
            this->m_arrayOfXCoef[i][j] = a[i][j];
    }

}

double **SystemLinearEquations::Solve()
{
    bool NextSubsetExist = true;  // Есть ли еще подмножества
    double **Result = NULL;
    Subset subset(m_row, m_col - 2);

    while (NextSubsetExist)  // пока есть еще варианты подмножеств
    {
        qint8 *Indexes = subset.GetArr();  // получение подмножества индексов следующей системы уравнений

        double *bForGJ = new double[m_col - 2]; //переменная b для метода Гаусса-Жордано
        double **aForGJ = new double*[m_col - 2]; //переменная a для метода Гаусса-Жордано

        for (quint8 i = 0; i < m_col - 2; i++) //инициализация массивов a и b для метода Гаусса-Жордано
        {
            bForGJ[i] = m_arrayOfFreeMem[Indexes[i]];
            aForGJ[i] = new double[m_col];
            for (quint8 j = 0; j < m_col; j++)
            {
                aForGJ[i][j] = m_arrayOfXCoef[Indexes[i]][j];
            }
        }

        GaussJordano Gauss(aForGJ, bForGJ, m_col);
        double **Variables = new double*[m_col - 2]; //массив хранящий выраженные переменные
        for (quint8 i = 0; i < m_col - 2; i++)
            Variables[i] = new double[3];

        Variables = Gauss.Run();

        if (!Variables)  //Проверка смог ли класс решить посланную систему, если нет то очищаем массивы и генерируем следующее подмножество
        {
            delete []bForGJ;
            for (quint8 i = 0; i < m_col - 2; i++)
                delete []aForGJ[i];
            delete []aForGJ;
            NextSubsetExist = subset.next();
            continue;
        }

        bool *NotConsidered = new bool[m_row]; // массив указывающий какие индексы были переданы для решения методом гаусса-жордано
        for (quint8 i = 0; i < m_row; i++)
        {
            bool Considered = false;
            for (quint8 j = 0; j < m_col - 2; j++)
                if (Indexes[j] == i)
                {
                    Considered = true;
                    break;
                }
            if (Considered)
                NotConsidered[i] = false;
            else
                NotConsidered[i] = true;
        }

        qint8 *NotConsideredIndexes = new qint8[m_row - m_col + 2]; //выделение в отдельный массив самих индексов
        qint8 counter = 0;
        for(quint8 i = 0; i < m_row; i++)
            if(NotConsidered[i])
                NotConsideredIndexes[counter++] = i;

        counter = 0;

        Result = new double*[m_row + 1]; // подстановка переменных в уравнения не учавствовавшие в методе Гаусса-Жордано
        for (quint8 i = 0; i < m_row; i++)
        {
            Result[i] = new double[3];
            if (i < m_col - 2)
                for (quint8 j = 0; j < 3; j++)
                    Result[i][j] = Variables[i][j];
            else
                Result[i] = Sum(Variables, NotConsideredIndexes[counter++]);				//Result[i] = Sum(Variables, i);
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


        delete []NotConsidered;
        delete []NotConsideredIndexes;

        break;
    }
    return Result;
}

SystemLinearEquations::~SystemLinearEquations()
{
    for (quint8 i = 0; i < m_row; i++)
        delete[] m_arrayOfXCoef[i];
    delete[] m_arrayOfXCoef;
    delete[] m_arrayOfFreeMem;
    delete[] m_arrayOfZCoef;
}
