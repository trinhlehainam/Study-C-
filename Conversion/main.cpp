#include <iostream>

int square(int x)
{
	int sum = 0;
	for (int i = 0; i < x; ++i)
	{
		sum += x;
	}
	return sum;
}

int main()
{
	int i = 0;
	while (i < 100)
	{
		std::cout << i << "\t" << square(i) << "\n";
		++i;
	}
}