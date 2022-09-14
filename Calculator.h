// Copyright(c) 2021-2022 aslze
// Licensed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef ASL_CALCULATOR_H
#define ASL_CALCULATOR_H

#include <asl/String.h>
#include <asl/Array.h>
#include <asl/Map.h>
#include <asl/Stack.h>

/**
A Calculator to compute the value of math expressions including functions and variables.

~~~
Calculator calc("-3.5*cos(i*5)+pi*(-2+sqrt(i*pi^2))/3");
double y = calc.set("i", 25.5).compute();
~~~
*/

class Calculator
{
public:
	/**
	Creates a calculator
	*/
	Calculator();
	/**
	Creates a calculator and parses the given expression
	*/
	Calculator(const asl::String& e) { init(); parse(e); }
	/**
	Parses an expression to prepare for computation 
	*/
	void parse(const asl::String& e);
	/**
	Computes the value of a previously parsed expression
	*/
	double compute() const;
	/**
	Parses and computes the value of an expression, optionally using a set of variable values
	*/
	static double eval(const asl::String& e, const asl::Dic<double>& vars = asl::Dic<double>());
	/**
	Sets the value of a variable for use in computation of an expression
	*/
	Calculator& set(const asl::String& var, double val) { _variables[var] = val; return *this; }

private:
	typedef double(*Function)(double);
	enum TokenType { NONE, OP, SEP, NAME, NUMBER };
	struct Token {
		double x;
		asl::String val;
		TokenType type;
		Token() {}
		Token(char c) : type(OP), val(c) {}
		Token(TokenType t, const asl::String& v) : type(t), val(v) {}
		Token(double v) : type(NUMBER), x(v) {}
		Token(asl::String& v) : type(NAME), val(v) {}
	};
	asl::Array<Token> _input, _output;
	asl::Stack<Token> _operators;
	mutable asl::Stack<double> _operands;
	asl::Dic<double> _variables;
	asl::Dic<Function> _natfunctions;
	void init();
	double value(const Token& t) const;
};

#endif
