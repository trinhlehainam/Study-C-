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
	case 'q':				// for quit
	case ';':				// for print
	case '(': case ')': case '+': case '-': case '*' : case '/':
		return Token(ch);			// let each character present itself
	case '.':
	case '0': case '1': case '2': case '3': case '4': 
	case '5': case '6': case '7': case '8': case '9':
	{
		cin.putback(ch);			// put digit back to input stream
		double val = 0;
		cin >> val;					// read a floating-point number
		return Token('8', val);		// let 8 present a number
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
	case '(': case ')': case '+': case '-': case '*': case '/':
		return Token(ch);        // let each character represent itself
	case '.':
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
	{
		cin.putback(ch);         // put digit back into the input stream
		double val;
		cin >> val;
		/*cin >> val;*/              // read a floating-point number
		return Token('8', val);   // let '8' represent "a number"
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
			left += term();
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
			if (p == 0) error("divide to 0");
			left /= p;
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
	case '8':				// use '8' to identify number
		return t.value;		// return number's value
	default:
		error("Primary expected");
	}
}

/*
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
*/

int main()
{
	try
	{
		double val = 0;
		while (cin)
		{
			Token t = ts.get();
			if (t.kind == 'q') break;
			if (t.kind == ';')
				cout << "=" << val << "\n";
			else
				ts.putback(t);
			val = expression();
		}
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