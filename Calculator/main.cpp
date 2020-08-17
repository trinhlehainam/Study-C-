/*
Simple calculator

Revision history
	Revised by Trinh Le Hai Nam 17th August 2020
	Original written by Trinh Le Hai Nam Summer 2020

The program implements a basic process of calculator
Read input from cin and print out from cout

The grammar for input is:

Calculation:
	Statement
	Quit
	Print
	Calculation Statement
Quit:
	'q'
Print:
	';'
Statement:
	Declaration
	Expression
Declaration:
	"let" Name '=' Declaration
Expression:
	Term
	Term '+' Term
	Term '-' Term
Term:
	Primary
	Term '*' Primary
	Term '/' Primary
	Term '%' Primary
Primary:
	Number
	'(' Expression ')'
	Name
	Name '=' Expression
Number:
	floating-point literal
Name:
	set of characters except
		- the first character is a number
		- is a special character

	Use Token_stream ts to process input from cin
*/

#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

void error(string message)
{
	throw runtime_error(message);
}

void error(string mes1, string mes2)
{
	string message = mes1 + mes2;
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
constexpr char let = 'L';
constexpr char name = 'a';
const string declkey = "let";

struct Token
{
	char kind = 0;
	double value = 0;
	string name = "";
	Token(char ch) :kind(ch), value(0) {};
	Token(char ch, string n) :kind(ch), name(n), value(0) {};
	Token(char ch, double num) :kind(ch), value(num) {};
	Token() = default;
};

class Token_stream
{
public:
	Token get();				// get Token from cin	
	void putback(Token t);
	void ignore(char c);		// discard characters up to c
private:
	bool full{ false };
	Token buffer;				// here where to keep Token buffer using putback()
};

Token Token_stream::get()
{
	// Check if it already has a Token buffer
	if (full)
	{
		full = false;
		return buffer;
	}
	// If it doesn't have a Token, get a new one
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
	case '=':
		return Token(ch);				// let each character present itself
	case '.':
	case '0': case '1': case '2': case '3': case '4': 
	case '5': case '6': case '7': case '8': case '9':
	{
		cin.putback(ch);				// put digit back to input stream
		double val = 0;
		cin >> val;						// read a floating-point number
		return Token(number, val);		// let 8 present a number
	}
	default:
		if (isalpha(ch))
		{
			string s;	
			// Assign first character of a word to s
			s = ch;	

			// Check if the word is ended
			// Add the rest characters of this word to s
			while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch;	
			cin.unget();

			// If s matches declare key
			// Return Token that process declaration
			if (s == declkey) return Token(let);	

			// Else return a variable name to Token_stream
			return Token(name, s);														
		}	
		error("Bad token");
	}
}

void Token_stream::putback(Token t)
{
	if (full) error("\nputback() into a full buffer\n");
	buffer = t;			// copy t to buffer
	full = true;		// buffer is now full
}

void Token_stream::ignore(char c)
// c present the kind of Token
{
	// First check buffer and discard it
	if (full && c == buffer.kind)
	{
		full = false;
		return;
	}
	full = false;
	// Now search input
	// Ignore all input until it found character c
	char ch = 0;
	while (cin >> ch)
		if (c == ch) return;
	
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

struct Variable
{
	string name;
	double value;
	Variable(string n, double v) :name(n), value(v) {};
};

Token_stream ts;		// provide get() and putback()
vector<Variable> var_table;
double statement();		// deal with declaration and expression
double declaration();	// 
double expression();	// deal with + and -
double term();			// deal with * and /
double primary();		// deal with number and parentheses

bool is_declared(string n)
// is var already in var_table
{
	for (const Variable& var : var_table)
		if (var.name == n) return true;
	return false;
}

void define_name(string n, double val)
// add {n,val} to var_table
{
	if (is_declared(n)) error("define_name() declared twice");
	var_table.push_back(Variable{ n,val });
}

double get_value(string n)
{
	for (const Variable& var : var_table)
		if (var.name == n) return var.value;
	error("get_value() variable is not found : ", n);
}

void set_value(string n, double v)
{
	for (Variable& var : var_table)
		if (var.name == n) var.value = v;
	error("set_value() variable is not found : ", n);
}

double statement()
{
	Token t = ts.get();
	switch (t.kind)
	{
	case let:
		return declaration();
	default:
		ts.putback(t);
		return expression();
	}
}
double declaration()
{
	Token t = ts.get();
	if (t.kind != name) error(" name is expected");
	string name = t.name;
	t = ts.get();				// Get next input
	if (t.kind != '=') error(" = is expected");
	double val = expression();
	define_name(name, val);
	return val;
}
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
		return t.value;			// return number's value
	case '-':
		return -primary();
	case '+':
		return primary();
	case print:
		ts.putback(t);			// Save print for calculator() discard
	case name:
	{
		string name = t.name;
		t = ts.get();			// Get next input
		// Name '=' Expression
		if (t.kind == '=')
		{
			double val = expression();
			set_value(name, val);
			return val;
		}
		ts.putback(t);			// If search '=' false, save this Token to other function
		// Name
		return get_value(name);
	}		
	default:
		error("Primary expected");
	}
}

void clean_up_mess()
{
	ts.ignore(print);
}

void calculator()
{
	while (cin)
	{
		try
		{
			cout << prompt;
			Token t = ts.get();
			while (t.kind == print) t = ts.get();		// discard all print
			if (t.kind == quit) return;					// quit program
			ts.putback(t);								// save Token for expression() process
			cout << result << statement() << "\n";
		}
		catch (exception& e)
		{
			cerr << "Error " << e.what() << "\n";
			clean_up_mess();
		}
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