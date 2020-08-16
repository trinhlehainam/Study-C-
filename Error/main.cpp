#include <iostream>
#include <string>
#include <vector>
#include <limits>

using namespace std;

void error(string message)
{
	throw runtime_error(message);
}

void keep_window_open()
{
	int open;
	cin >> open;
}

int main()
{
	vector<double> temps;

	double sum, highest, lowest;
	sum = highest = lowest = 0;
	double init_temp;
	cin >> init_temp;
	highest = lowest = init_temp;
	temps.push_back(init_temp);
	for (double temp; cin >> temp;)
	{
		temps.push_back(temp);
		if (temp > highest) highest = temp;
		if (temp < lowest) lowest = temp;
		sum += temp;
	}

	for (double temp : temps)
		cout << temp << "\n";

	cout << "Highest temperature : " << highest
		<< "\nLowest temperature : " << lowest
		<< "\nAverage temperature : " << sum / temps.size();

}