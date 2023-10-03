#include<iostream>
using std::cin;
using std::cout;
using std::endl;

// returns true if and only if the specified huge integer is zero
bool isZero(int hugeInt[], int size);

// returns true if and only if hugeInt1 < hugeInt2
bool less(int hugeInt1[], int hugeInt2[], int size1, int size2);

// --hugeInt
void decrement(int hugeInt[], int size);

// minuend -= subtrahend provided that minuend > subtrahend
void subtraction(int minuend[], int subtrahend[], int& minuendSize, int subtrahendSize);

// multiplicand *= multiplier
void multiplication(int multiplicand[], int multiplier[], int& multiplicandSize, int multiplierSize);

// quotient = dividend / divisor; remainder = dividend % divisor
void division(int dividend[], int divisor[], int quotient[], int remainder[],
    int dividendSize, int divisorSize, int& quotientSize, int& remainderSize);

// hugeInt /= 10, or equivalently, shifts right by one position
void divideBy10(int hugeInt[], int& size);

const int arraySize = 200;
int f = 0;

int main()
{
    int t, a, b;
    while (cin >> t >> a >> b)
    {
        cout << "(" << t << "^" << a << "-1)/(" << t << "^" << b << "-1) ";

        if (t == 1)
            cout << "is not an integer with less than 100 digits.\n";
        else if (a == b)
            cout << "1\n";
        else if (a < b)
            cout << "is not an integer with less than 100 digits.\n";
        else
        {
            int dividend[arraySize] = {};
            int divisor[arraySize] = {};
            int quotient[arraySize] = {};
            int remainder[arraySize] = {};

            int dividendSize = 1;
            int divisorSize = 1;
            int quotientSize = 1;
            int remainderSize = 1;

            // put all digits of t into base
            int base[arraySize] = {};
            int baseSize = 0;
            for (int i = t; i > 0; i /= 10)
                base[baseSize++] = i % 10;

            // dividend = pow( t, a )
            dividend[0] = 1;
            for (int i = 0; i < a; ++i)
            {
                multiplication(dividend, base, dividendSize, baseSize);
                if (dividendSize > arraySize)
                    break;
            }

            if (dividendSize > arraySize)
                cout << "is not an integer with less than 100 digits.\n";
            else
            {
                // divisor = pow( t, b )
                divisor[0] = 1;
                for (int i = 0; i < b; ++i)
                    multiplication(divisor, base, divisorSize, baseSize);

                decrement(dividend, dividendSize); // --dividend
                decrement(divisor, divisorSize);   // --divisor

                division(dividend, divisor, quotient, remainder,
                    dividendSize, divisorSize, quotientSize, remainderSize);

                // quotient is an integer with less than 100 digits
                if (quotientSize < 100 && isZero(remainder, remainderSize))
                    for (int i = quotientSize - 1; i >= 0; i--)
                        cout << quotient[i];
                else
                    cout << "is not an integer with less than 100 digits.";
                cout << endl;
            }
        }
    }
}

// returns true if and only if the specified huge integer is zero
bool isZero(int hugeInt[], int size)
{
    for (int i = 0; i < size; i++)
        if (hugeInt[i] != 0)
            return false;
    return true;
}

// returns true if and only if hugeInt1 < hugeInt2
bool less(int hugeInt1[], int hugeInt2[], int size1, int size2)
{
    if (size1 < size2) {
        return  true;
    }
    else if (size1 > size2) {
        return false;
    }
    else {
        for (int i = size1 - 1; i >= 0; i--) {
            if (hugeInt1[i] != hugeInt2[i]) {
                if (hugeInt1[i] < hugeInt2[i]) {
                    return true;
                }
                else {
                    return false;
                }
            }
        }
        return false;
    }
}

// --hugeInt
void decrement(int hugeInt[], int size)
{
    int count = 0;
    hugeInt[count]--;
    while (count < size - 1) {
        if (hugeInt[count] < 0) {
            hugeInt[count + 1] -= 1;
            hugeInt[count] += 10;
        }
        count++;
    }
    while (hugeInt[size - 1] == 0 && size != 1)
    {
        size--;
    }
}

// minuend -= subtrahend provided that minuend > subtrahend
void subtraction(int minuend[], int subtrahend[], int& minuendSize, int subtrahendSize)
{
    for (int i = 0; i < minuendSize; i++) {
        minuend[i] = minuend[i] - subtrahend[i];
        if (minuend[i] < 0) {
            minuend[i + 1] -= 1;
            minuend[i] += 10;
        }
    }
    while (minuend[minuendSize - 1] == 0 && minuendSize != 1)
    {
        minuendSize--;
    }

}

// multiplicand *= multiplier
void multiplication(int multiplicand[], int multiplier[], int& multiplicandSize, int multiplierSize)
{
    int tmp[arraySize] = {};
    for (int j = 0; j < multiplierSize; j++) {
        for (int i = 0; i < multiplicandSize; i++) {
            tmp[i + j] += multiplicand[i] * multiplier[j];
        }
    }
    multiplicandSize = multiplicandSize + multiplierSize;
    for (int i = 0; i < multiplicandSize; i++) {
        if (tmp[i] >= 10) {
            tmp[i + 1] += tmp[i] / 10;
            tmp[i] %= 10;
        }
        multiplicand[i] = tmp[i];
    }
    while (multiplicand[multiplicandSize - 1] == 0 && multiplicandSize != 1)
    {
        multiplicandSize--;
    }
}

// quotient = dividend / divisor; remainder = dividend % divisor
void division(int dividend[], int divisor[], int quotient[], int remainder[],
    int dividendSize, int divisorSize, int& quotientSize, int& remainderSize)
{
    remainderSize = dividendSize;
    for (int i = 0; i < remainderSize; i++) {
        remainder[i] = dividend[i];//先讓餘數等於被除數
    }
    int n = dividendSize - divisorSize;//n代表buffer要往右挪幾位
    int buffer[arraySize] = {};
    int bufferSize = dividendSize;
    for (int i = bufferSize - 1; i >= 0; i--) {
        if (i >= n) {
            buffer[i] = divisor[i - n];
        }
        else {
            buffer[i] = 0;//讓0~n-1以後的位數依序等於除數，然後0~n-1放入0
        }
    }
    quotientSize = dividendSize - divisorSize;
    if (less(dividend, buffer, dividendSize, bufferSize)) {//當被除數小於buffer,buffer向右挪一位
        divideBy10(buffer, bufferSize);
    }
    else {
        quotientSize++;//buffer小於或等於被除數要多減一輪，所以商的位數加一
    }
    for (int i = 0; i < quotientSize; i++) {
        quotient[i] = 0;
    }
    for (int k = quotientSize - 1; k >= 0; k--) {
        while (less(remainder, buffer, remainderSize, bufferSize) != true) {//當餘數大於或等於buffer時，進入while迴圈
            subtraction(remainder, buffer, remainderSize, bufferSize);//讓餘數減去buffer
            quotient[k]++;//商的此位數加一(即代表看每位數能讓餘數減去buffer幾次)
            if (isZero(remainder, remainderSize)) {
                return;//當餘數為零(代表整除)直接return
            }
        }
        divideBy10(buffer, bufferSize);//buffer位數向右挪一位，讓餘數可以繼續減buffer
    }
}

// hugeInt /= 10, or equivalently, shifts right by one position
void divideBy10(int hugeInt[], int& size)
{
    if (size == 1)
        hugeInt[0] = 0;
    else
    {
        for (int i = 1; i < size; i++)
            hugeInt[i - 1] = hugeInt[i];

        size--;
        hugeInt[size] = 0;
    }
}