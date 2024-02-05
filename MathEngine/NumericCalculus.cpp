#include "pch.h"
#include "exprtk.hpp"
#include <iostream>

using namespace std;
using namespace exprtk;

// Parse and evaluate an expression
double parse_and_evaluate(const string& expression_str, double x) {
	typedef symbol_table<double> symbol_table_t;
	typedef expression<double> expression_t;
	typedef parser<double> parser_t;

	double result = 0.0;
	symbol_table_t symbol_table;
	symbol_table.add_variable("x", x);
	symbol_table.add_constants();

	expression_t expression;
	expression.register_symbol_table(symbol_table);

	parser_t parser;
	if(parser.compile(expression_str, expression)) {
		result = expression.value();
	} else {
		cerr << "Failed to parse expression: " << expression_str << endl;
	}

	return result;
}

// Derivative of a function
double numeric_derivative(const string& expression_str, double x, double h = 0.0001) {
	return (parse_and_evaluate(expression_str, x + h) - parse_and_evaluate(expression_str, x - h)) / (2 * h);
}

// Integral of a function
double numeric_integral(const string& expression_str, double a, double b, int n = 1000) {
	double h = (b - a) / n;
	double sum = 0.5 * (parse_and_evaluate(expression_str, a) + parse_and_evaluate(expression_str, b));

	// sum the rest of the terms
	for(int i = 1; i < n; i++) sum += parse_and_evaluate(expression_str, a + i * h);

	return sum * h;
}

// Exported functions
extern "C" __declspec(dllexport) double derivate_of_expression(const char* expr, double x) {
	return numeric_derivative(string(expr), x);
}

extern "C" __declspec(dllexport) double integral_of_expression(const char* expr, double a, double b) {
	return numeric_integral(string(expr), a, b);
}