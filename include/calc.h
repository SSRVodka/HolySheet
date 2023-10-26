#pragma once
#include "linStack.h"

// My English is poor ~
// @author: SJTU-XHW
// This module acts as a simple calculator,
// which supports +, -, *, /, ^ and () 

class calc {
private:
	int exprLen;
	const char* expr;			// Infix expression
	mutable int hidReader;		// The index for hidExpr (RPN)
	char* hidExpr;				// Reverse Polish Notation (RPN)
	seqStack<double>* numStk;	// The stack for operands
	double result;

	// Infix expression -> Suffix expression
	void mid2hid();

	// Read number from char array.
	double readNumFrom(const char* src) const;
	// Deal with the operators
	void dealOp(char op);
public:
	calc(const char* ex);
	~calc();
	double getResult();
	// Show RPN
	char* getRPNotation() const;
};

// The exception class for illegal expressions or calculator's runtime error
class exprErr {
	const char* message;
public:
	exprErr(const char* msg) : message(msg) {}
	const char* what() const { return message; }
};

