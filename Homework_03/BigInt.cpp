#include <iostream>
#include <string> 
#include <cmath>                    // для функции ceil

using namespace std;

int digitCount(int a)
{
	int count = 0;
	if (!a) return 1;
	while (a)
	{
		count++;
		a /= 10;
	}
	return count;
}

int max(int a, int b)
{
	if (a > b) return a;
	else return b;
}

class BigInt
{
private:
	int baseLen = 4;
	int base = 10000;
	int maxLen = 100;
	int* number;
	bool sign;

	void strToLong(string str)  // используется в конструкторе
	{
		if (str[0] != '-')
		{
			number[0] = (int)ceil((double)str.size() / baseLen); // округляет вверх (2.5 ~ 3 - ?), но работает только с double :c
			sign = true;
		}
		else
		{
			sign = false;
			number[0] = (int)ceil((double)(str.size() - 1) / baseLen); // округляет вверх (2.5 ~ 3 - ?), но работает только с double :c
			str.erase(0, 1);
		}
		string cur;
		int i = 0;
		for (i = 1; i <= number[0] - 1; i++)
		{
			cur = str.substr(str.size() - i*baseLen, baseLen);
			number[i] = atoi(cur.c_str()); // c_str добавил, потому что иначе не работало --> https://ru.stackoverflow.com/questions/730050/%D0%A7%D0%B5%D0%BC-%D0%B7%D0%B0%D0%BC%D0%B5%D0%BD%D0%B8%D1%82%D1%8C-%D1%84%D1%83%D0%BD%D0%BA%D1%86%D0%B8%D1%8E-stoi
		}
		cur = str.substr(0, str.size() - (i - 1)*baseLen);
		number[number[0]] = atoi(cur.c_str());
	}

	BigInt longSum(const BigInt &B)  // используется в перегрузке операторов +=, +
	{
		BigInt C;
		C.sign = B.sign;
		int m = max(number[0], B.number[0]);
		int buffer = 0;
		for (int i = 1; i <= m; i++)
		{
			C.number[i] = number[i] + B.number[i] + buffer;
			buffer = 0;
			if (C.number[i] >= base)
			{
				C.number[i] -= base;
				buffer = 1;
			}
			if (!C.number[m + 1])
				C.number[0] = m;
			else
				C.number[0] = m + 1;
		}
		return C;
	}

	BigInt longDif(const BigInt &B)      // используется в перегрузке операторов -, +
	{									// заведомо this больше B !!!  // &B or B ??
		BigInt C;
		C.sign = true;
		int m = number[0];
		int buffer = 0;
		for (int i = 1; i <= m; i++)
		{
			if (number[i] < B.number[i])
			{
				number[i] += base;
				buffer = 1;
			}
			C.number[i] = number[i] - B.number[i];
			number[i + 1] -= buffer;
			buffer = 0;
		}
		if (!C.number[m])
			while (!C.number[m] && m > 1)
				m--;
		C.number[0] = m;
		return C;
	}

	int longCmp(const BigInt &B) const   // используется в перегрузке операторов сравнения
	{
		if ((number[0] == 1) && (number[1] == 0) && (B.number[0] == 1) && (B.number[1] == 0)) return 0; // равенство положительного и отрицательного нуллей
		if (sign && !B.sign) return 1;  //первое "+", второе "-"
		if (!sign && B.sign) return -1; //первое "-", второе "+"
		if (sign && B.sign)             //оба "+"
		{
			if (number[0] > B.number[0]) return 1;
			if (number[0] < B.number[0]) return -1;
			for (int i = number[0]; i >= 1; i--)
			{
				if (number[i] > B.number[i]) return 1;
				if (number[i] < B.number[i]) return -1;
			}
			return 0;
		}
		if (!sign && !B.sign)           //оба "-"
		{
			if (number[0] > B.number[0]) return -1;
			if (number[0] < B.number[0]) return 1;
			for (int i = number[0]; i >= 1; i--)
			{
				if (number[i] > B.number[i]) return -1;
				if (number[i] < B.number[i]) return 1;
			}
			return 0;
		}
	}

public:
	BigInt()
	{
		number = new int[maxLen + 1];
		for (int i = 1; i <= maxLen; i++)
		{
			number[i] = 0;
		}
		number[0] = 1;
	}

	BigInt(int64_t num)
	{
		number = new int[maxLen + 1];
		string str = to_string(num);
		this->strToLong(str);
	}

	~BigInt()
	{
		delete[] number;
	}

	BigInt &operator= (int64_t num)
	{
		string str = to_string(num);
		this->strToLong(str);
		return *this;
	}

	BigInt &operator= (const BigInt &B)  // &?
	{
		number = B.number;
		sign = B.sign;
		return *this;
	}

	BigInt &operator+=(const BigInt &B)
	{
		if (this->sign == B.sign)
			*this = this->longSum(B);
		else
			if ((this->sign == true) && (B.sign == false))
				*this = this->longDif(B);
			else
			{ 
				BigInt C = *this; 
				BigInt D = B;
				*this = D.longDif(C);          // изначально было *this = B.longDif(*this), но ругалось
			}
		return *this;
	}

	BigInt operator-() const
	{
		BigInt C = *this;
		C.sign = !sign;
		return C;
	}

	bool operator<(const BigInt &B) const    // (1)  BigInt
	{
		if (this->longCmp(B) == -1)
			return true;
		else
			return false;
	}

	bool operator<(int64_t num) const    // (2)  int64_t
	{
		BigInt B(num);
		return *this<B;
	}

	bool operator>(const BigInt &B) const    // (1)  BigInt
	{
		if (this->longCmp(B) == 1)
			return true;
		else
			return false;
	}

	bool operator>(int64_t num) const    // (2)  int64_t
	{
		BigInt B(num);
		return *this>B;
	}

	bool operator<=(const BigInt &B) const    // (1)  BigInt
	{
		if (this->longCmp(B) == -1 || this->longCmp(B) == 0)
			return true;
		else
			return false;
	}

	bool operator<=(int64_t num) const    // (2)  int64_t
	{
		BigInt B(num);
		return *this <= B;
	}

	bool operator>=(const BigInt &B) const    // (1)  BigInt
	{
		if (this->longCmp(B) == 1 || this->longCmp(B) == 0)
			return true;
		else
			return false;
	}

	bool operator>=(int64_t num) const    // (2)  int64_t
	{
		BigInt B(num);
		return *this >= B;
	}

	bool operator==(const BigInt &B) const    // (1)  BigInt
	{
		if (this->longCmp(B) == 0)
			return true;
		else
			return false;
	}

	bool operator==(int64_t num) const    // (2)  int64_t
	{
		BigInt B(num);
		return *this == B;
	}

	bool operator!=(const BigInt &B) const    // (1)  BigInt
	{
		if (this->longCmp(B) == 0)
			return false;
		else
			return true;
	}

	bool operator!=(int64_t num) const    // (2)  int64_t
	{
		BigInt B(num);
		return *this != B;
	}


};


int main()
{
	return 0;
}