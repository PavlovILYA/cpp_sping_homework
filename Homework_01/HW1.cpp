#include <iostream>
#include "numbers.dat"
// Data - массив
// Size - размер массива
using namespace std;

bool simple(int P)
{
	if (P == 1)
		return false;
	for (int d = 2; d*d <= P; d++)
	{
		if (P%d == 0)
			return false;
	}

	return true;
}

void segment_search(int begin, int end)
{
	int count = 0;
	for (int j = begin; j <= end; j++)
		if (simple(Data[j]))
			count++;
	cout << count << " " << endl;
	return;
}

int main(int argc, char* argv[])
{
	int begin = -1, end = 0;

	if ((((argc - 1) % 2) == 1) || (argc == 1))
		return -1;

	for (int i = 1; i<argc; i += 2)
	{
		begin = -1;
		end = 0;
		if (atoi(argv[i]) > atoi(argv[i + 1]))
		{
			cout << "0";
			continue;
		}
		int fl = 0, fl_2 = 0; // обнуляем флаги каждый раз для новой пары
		for (int j = 0; j<Size; j++)
		{
			if (Data[j] > atoi(argv[i + 1]))
				break;
			if (begin == -1)
			{
				if (atoi(argv[i]) == Data[j])
				{
					fl++;
					begin = j;
				}
			}
			if (atoi(argv[i + 1]) == Data[j])
			{
				fl_2 = 1;
				end = j;
			}
		}


		if (fl_2 == 1)
			fl++;
		if (fl != 2)
			cout << 0;
		else
		{
			segment_search(begin, end); // функция сама выведет на экран
		}
	}
	return 0;
}