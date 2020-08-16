#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

bool almost_equal(double num1, double num2)
{
	double dif = num1 > num2 ? num1 - num2 : num2 - num1;
	return dif <= 1.0 / 10;
}

constexpr double cm_per_m = 100.0;
constexpr double cm_per_in = 2.54;
constexpr double in_per_ft = 12.0;
constexpr double in_per_m = cm_per_m / cm_per_in;
constexpr double ft_per_m = in_per_m / in_per_ft;

double convert_to_m(double value, string unit)
{
	if (unit == "cm")
	{
		value = value / cm_per_m;
	}
	else if (unit == "inch")
	{
		value = value / in_per_m;
	}
	else if (unit == "ft")
	{
		value = value / ft_per_m;
	}
	else if (unit == "m")
	{

	}
	else
	{
		cout << "Illegal unit ! " << unit << "\nPlease enter again !\n";
		value = 0.0;
	}
	return value;
}

int main()
{
	vector<double> nums;
	double largest , smallest;
	double num = 0;
	double sum = 0;
	string unit;
	cout << "Please enter the number and unit \n";
	cin >> num >> unit;
	largest = smallest = convert_to_m(num,unit);

	sum += convert_to_m(num, unit);
	nums.push_back(convert_to_m(num, unit));
	for (double num;cin >> num >> unit;)
	{
		
		double val = convert_to_m(num, unit);
		
		if (val != 0.0)
		{
			if (val > largest) largest = val;
			if (val < smallest) smallest = val;
			nums.push_back(val);
		}

		sum += val;
	}
	sort(nums.begin(),nums.end());

	for (double num : nums)
		cout << num << " m\n";
	cout << "Sum : " << sum << " m\n";
	cout << "Largest : " << largest << " m\n";
	cout << "Smallest : " << smallest << " m\n";

}