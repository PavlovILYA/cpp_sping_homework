#include <iostream>
#include "numbers.dat"
// Data - массив
// Size - размер массива
using namespace std;

bool simple (int P)
{
    if(P==1) 
		return false;
	
	for(int d=2; d*d<=P; d++)
    { 		
		if(P%d==0) 
			return false;
	}
	
	return true;
}

int main(int argc, char* argv[])
{
    int m1=-3, m2=0, count=0;

    if ((((argc-1) % 2) == 1) || (argc == 1))
        return -1;

    for (int i = 1; i<argc; i += 2)
	{
        count = 0;
        m1 = -3;
        m2 = 0;
		if (atoi(argv[i])>atoi(argv[i + 1]))
        {
             cout << "0";
             continue;
        }
		int fl = 0, fl_2 = 0;//обнуляем каждый раз для новой пары
		for (int j = 0; j<Size; j++)
		{
            if (Data[j]>atoi(argv[i+1])) 
                break; 
            if (m1 == -3)
            {
                if (atoi(argv[i]) == Data[j])
                {
                    fl++;
                    m1 = j;
                }
            }
			if (atoi(argv[i + 1]) == Data[j])
			{
				fl_2 = 1;
				m2 = j;
			}
		}
            

		if (fl_2 == 1)
            fl++;
        if (fl != 2)
			cout << 0;
		else
        {  
			for (int j = m1; j<=m2; j++)
				if (simple(Data[j]))
					count++;
		    cout << count << " "<< endl;
        }
	}
    return 0;
}
