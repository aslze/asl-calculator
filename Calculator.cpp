// Copyright(c) 2021 aslze
// Licensed under the MIT License (http://opensource.org/licenses/MIT)

#include "Calculator.h"
#include <math.h>
#include <ctype.h>

using namespace asl;


Calculator::Calculator()
{
	init();
}

void Calculator::init()
{
	_variables.set("e", 2.718281828459045);
	_variables.set("pi", 3.141592653589793);

	_natfunctions.set("abs", (Function)fabs);
	_natfunctions.set("acos", (Function)acos);
	_natfunctions.set("asin", (Function)asin);
	_natfunctions.set("atan", (Function)atan);
	_natfunctions.set("cos", (Function)cos);
	_natfunctions.set("exp", (Function)exp);
	_natfunctions.set("floor", (Function)floor);
	_natfunctions.set("ln", (Function)log);
	_natfunctions.set("log", (Function)log10);
	_natfunctions.set("sin", (Function)sin);
	_natfunctions.set("sqrt", (Function)sqrt);
	_natfunctions.set("tan", (Function)tan);
}

double Calculator::eval(const String& e, const Dic<double>& vars)
{
	Calculator calc(e);
	foreach2(String& name, double value, vars)
		calc.set(name, value);
	return calc.compute();
}

inline int precedence(const String& o)
{
	if (o == '+' || o == '-')
		return 1;
	else if (o == '*' || o == '/')
		return 2;
	else
		return 3;
}

// produce token list

void Calculator::parse(const String& e)
{
	_input.clear();
	_output.clear();
	String e1 = e + ' ';
	char* p = e1;
	String name;
	TokenType type = NONE;
	while (char c = *p++)
	{
		switch (type) {
		case NONE:
			switch (c)
			{
			case '+': case '-': case '*': case '/': case '^':
				_input << c;
				break;
			case '(': case ')':
				_input << Token(SEP, String(c));
				break;
			default:
				if ((c >= '0' && c <= '9') || c == '.' || c == '-') {
					type = NUMBER;
					name = c;
				}
				else if (isalpha(c)) {
					type = NAME;
					name = c;
				}
				break;
			}
			break;
		case NUMBER:
			if (!((c >= '0' && c <= '9') || c == '.' || c == 'e' || (c=='-' && *(p-2)=='e'))) {
				_input << Token((double)name);
				type = NONE;
				p--;
				break;
			}
			else
				name << c;
			break;
		case NAME:
			if (isalnum(c))
				name << c;
			else
			{
				_input << Token(NAME, name);
				p--;
				type = NONE;
			}
			break;
		default:
			break;
		}
	}

	// build reverse

	foreach2(int i, const Token& token, _input)
	{
		char op;
		switch (token.type)
		{
		case NUMBER: _output << token;
			break;
		case NAME:
			if (_input[i + 1].type == SEP && _input[i + 1].val == '(')
				_operators.push(token);
			else
				_output << token;
			break;
		case SEP:
			op = token.val[0];
			switch (op)
			{
			case '(': _operators.push(op);
				break;
			case ')':
				while (_operators.top().val != '(')
					_output << _operators.popget();
				_operators.pop();
				break;
			}
			break;

		case OP:
			op = token.val[0];
			if (op == '-' && (i == 0 || _input[i - 1].type == OP || _input[i - 1].val == '('))
			{
				_operators.push('_');
				break;
			}
			while (_operators.length() > 0 && _operators.top().val != '(' && (
				(op != '^' && precedence(token.val) <= precedence(_operators.top().val)) ||
				(op == '^' && precedence(token.val) < precedence(_operators.top().val))))
				_output << _operators.popget();
			_operators.push(token);
		default:
			break;
		}
	}

	while (_operators.length() > 0)
	{
		if (_operators.top().val == '(' || _operators.top().val == ')') {
			printf("Mismatch\n");
			return;
		}
		else
			_output << _operators.popget();
	}
}

double Calculator::value(const Calculator::Token& t)
{
	double def = 0.0;
	if (t.type == NAME)
		return _variables.get(t.val, def);
	else
		return (double)t.x;
}

double Calculator::compute()
{
	_operands.clear();

	// execute

	foreach(const Token& token, _output)
	{
		double x1, x2;
		switch (token.type)
		{
		case NUMBER:
			_operands.push(value(token));
			break;
		case OP:
			switch (token.val[0])
			{
			case '_':
				_operands.top() = -_operands.top();
				break;
			case '+':
				x2 = _operands.popget();
				x1 = _operands.top();
				_operands.top() = x1 + x2;
				break;
			case '-':
				x2 = _operands.popget();
				x1 = _operands.top();
				_operands.top() = x1 - x2;
				break;
			case '*':
				x2 = _operands.popget();
				x1 = _operands.top();
				_operands.top() = x1 * x2;
				break;
			case '/':
				x2 = _operands.popget();
				x1 = _operands.top();
				_operands.top() = x1 / x2;
				break;
			case '^':
				x2 = _operands.popget();
				x1 = _operands.top();
				_operands.top() = x1 + x2;
				if (x2 == 2)
					_operands.top() = x1 * x1;
				else
					_operands.top() = pow(x1, x2);
				break;
			}
			break;
		case NAME:
			if (const Function* f = _natfunctions.find(token.val))
			{
				x1 = _operands.top();
				_operands.top() = (*f)(x1);
			}
			/*else if (_functions.has(token.val))
			{
				x1 = _operands.popget();
				_operands.push(_functions(token.val, x1));
			}*/
			else
				_operands.push(value(token));
			break;
		default:
			break;
		}
	}

	return _operands.top();
}
