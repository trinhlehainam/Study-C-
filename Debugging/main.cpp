#include <iostream>
#include <string>
#include <vector>

using namespace std;


void error(string message)
{
	throw runtime_error(message);
}

void keep_window_open()
{
	int temp;
	cin >> temp;
}

int area(int width, int length)
// calculate area of rectangle
// pre-condition: both arguments are positive
// post-condition: area is positive
{
	if (width <= 0 || length <= 0) error("area() pre-condition");
	if ((width * length) <= 0) error("area() post-condition");
	return width * length;
}

constexpr double celsius_to_kelvin = 273.15;
double ctok(double c) 
// converts Celsius to Kelvin
// post-contition : converted value >= 0
{
	double k = c + celsius_to_kelvin;
	if (k < 0) error("ctok() post-contition");
	return k;
}

double ktoc(double k)
// convert Kelvin to Celsius
// pre-contition : k >= 0
{
	if (k < 0) error("ktoc() pre-contition");
	double c = k - celsius_to_kelvin;
	return c;
}

int main()
{
	try
	{
		int n;
		vector<double> v;
		double sum = 0.0;
		cout << "Please enter the number of values you want to sum \n";
		cin >> n;
		cout << "Please enter some integers \n";
		for (double value; cin >> value;) v.push_back(value);
		for (int i = 0; i < n; ++i)
		{
			sum += v[i];
		}
		cout << "The sum of the first " << n << " numbers : " << sum;
		return 0;
	}
	catch (exception& e)
	{
		cerr << "error " << e.what() << "\n";
		keep_window_open();
		return 1;
	}
	catch (...)
	{
		cerr << "Unknown error" << "\n";
		keep_window_open();
		return 2;
	}

}