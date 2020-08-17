/*
Simple calculator

Revision history
	Revised by Trinh Le Hai Nam 17th August 2020
	Original written by Trinh Le Hai Nam Summer 2020

The program implements a basic process of calculator
Read input from cin and print out from cout

The grammar for input is:

Statement:
	Expression
	Quit
	Print
Quit:
	'q'
Print:
	';'
Expression:
	Term
	Expression '+' Term
	Expression '-' Term
Term:
	Primary
	Term '*' Primary
	Term '/' Primary
	Term '%' Primary
Primary:
	Number
	'('Expression ')'
Number:
	floating-point literal

	Use Token_stream ts to process input from cin
*/

#include <iostream>
#include <string>

using namespace std;

void error(string message)
{
	throw runtime_error(message);
}

void keep_window_open()
{
	int any;
	cout << "\nEnter any key to end program\n";
	cin >> any;
}

template<class R, class A> 
R narrow_cast(const A& a)
{
	R r = R(a);								// convert A value to R value;
	if (A(r) != a) error("Data lost");		// convert value back to A to check if data is lost
	return r;
}

constexpr char prompt = '>';
constexpr char result = '=';
constexpr char number = '8';
constexpr char print = ';';
constexpr char quit = 'q';

struct Token
{
	char kind;
	double value;
	Token(char ch) :kind(ch), value(0) {};
	Token(char ch, double num) :kind(ch), value(num) {};
	Token() = default;
};

class Token_stream
{
public:
	Token get();
	void putback(Token t);
private:
	bool full{ false };
	Token buffer;				// here where to keep Token buffer using putback()
};

Token Token_stream::get()
{
	if (full)
	{
		full = false;
		return buffer;
	}
	char ch;
	cin >> ch;
	switch (ch)
	{
	case quit:				
	case print:				
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
		return Token(ch);			// let each character present itself
	case '.':
	case '0': case '1': case '2': case '3': case '4': 
	case '5': case '6': case '7': case '8': case '9':
	{
		cin.putback(ch);			// put digit back to input stream
		double val = 0;
		cin >> val;					// read a floating-point number
		return Token(number, val);		// let 8 present a number
	}
	default:
		error("Bad token");
	}
}

void Token_stream::putback(Token t)
{
	if (full) error("\nputback() into a full buffer\n");
	buffer = t;			// copy t to buffer
	full = true;		// buffer is now full
}


// get next token
Token get_token()    // read a token from cin
{
	char ch;
	cin >> ch;    // note that >> skips whitespace (space, newline, tab, etc.)

	switch (ch) {
		//not yet   case ';':    // for "print"
		//not yet   case 'q':    // for "quit"
	case '(': case ')': case '+': case '-': case '*': case '/': case '%':
		return Token(ch);        // let each character represent itself
	case '.':
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	{
		cin.putback(ch);         // put digit back into the input stream
		double val;
		cin >> val;
		/*cin >> val;*/              // read a floating-point number
		return Token(number, val);   // let '8' represent "a number"
	}
	default:
		error("Bad token");
	}
}

Token_stream ts;	// provide get() and putback()
double expression(); // deal with + and -
double term();		 // deal with * and /
double primary();	 // deal with number and parentheses

double expression()
{
	double left = term();
	Token t = ts.get();
	while (true)
	{
		switch (t.kind)
		{
		case '+':
			left += term();
			t = ts.get();
			break;
		case '-':
			left -= term();
			t = ts.get();
			break;
		default:
			ts.putback(t);
			return left;
		}
	}
}
double term()
{
	double left = primary();
	Token t = ts.get();
	while (true)
	{
		switch (t.kind)
		{
		case '*':
			left *= primary();
			t = ts.get();
			break;
		case '/':
		{
			double p = primary();
			if (p == 0) error("/: divide by zero");
			left /= p;
			t = ts.get();
			break;
		}
		case '%':
		{
			int i1 = narrow_cast<int>(left);
			int i2 = narrow_cast<int>(primary());
			if (i2 == 0) error("%: divide by zero");
			left = i1 % i2;
			t = ts.get();
			break;

		}
		default:
			ts.putback(t);
			return left;
		}
	}
}

double primary()
{
	Token t = ts.get();
	switch (t.kind)
	{
	case '(':				// handle '(' expression ')'
	{
		double value = expression();
		t = ts.get();
		if (t.kind != ')') error(") expected");
		return value;
	}
	case number:				// use '8' to identify number
		return t.value;		// return number's value
	case '-':
		return -primary();
	case '+':
		return primary();
	default:
		error("Primary expected");
	}
}

void calculator()
{
	while (cin)
	{
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) t = ts.get();		// eat print
		if (t.kind == quit) return;					// quit program
		ts.putback(t);								// save Token for expression() process
		cout << result << expression() << "\n";
	}
}

int main()
{
	try
	{
		calculator();
		keep_window_open();
		return 0;
	}
	catch (exception& e)
	{
		cerr << "Error : " << e.what() << "\n";
		keep_window_open();
		return 1;
	}
	catch (...)
	{
		cerr << "Unexpected error \n";
		keep_window_open();
		return 2;
	}
}