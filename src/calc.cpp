#include "calc.h"
#include <cmath>

calc::calc(const char* ex): expr(ex) {
	exprLen = 0;
	for (; expr[exprLen]; ++exprLen);
	hidExpr = nullptr;
	result = hidReader = 0;
	numStk = new seqStack<double>;

	mid2hid();
}

calc::~calc() {
	if (hidExpr) delete[] hidExpr;
	delete numStk;
}

void calc::mid2hid() {
	hidExpr = new char[3 * exprLen] {0};
	seqStack<char> opStk;			// The stack for operators
	int numStart = 0, numEnd = 0;	// The index of the beginning / endding for the current number
	int idx = 0;					// The index of 'expr' for reading
	int hidWriter = 0;				// The index of 'hidExpr' for writing

	while (idx < exprLen) {
		bool hasNum = 0;			// whether detected number(s) in this loop or not
		while (expr[idx] >= '0' && expr[idx] <= '9'
			|| expr[idx] == '.') {
			if (!hasNum) numStart = idx;
			++idx; hasNum = 1;
		}
		numEnd = idx;
		// If the selected area is a number, just write it down on the 'hidExpr'.
		while (hasNum && numStart < numEnd) {
			hidExpr[hidWriter] = expr[numStart];
			++numStart; ++hidWriter;
		}
		// If we write a number down, we must add a space(' ') behind it
		// in order to distinguish it from other numbers in 'hidExpr'.
		if (hasNum) {
			hidExpr[hidWriter] = ' '; ++hidWriter;
		}
		// If '(' is detected, push it into the stack for operators immediately
		if (expr[idx] == '(') opStk.push(expr[idx]);
		// If '+' or '-' is detected, which has the lowest priority, then any operators
		// between any left bracket an it can be written down into 'hidExpr'.
		//
		// **ATTENTION**: '+' / '-' can be either a binary operator or a monadic operator,
		// so the usage of either of them varies depending on their context.
		// 
		// If '+' / '-' is regarded as a binary operator, then the specified symbols (numbers, 
		// right brackets and space (dual)) in front of it can be written down into 'hidExpr' immediately.
		// If '+' / '-' is regarded as a monadic operator, then the other part of symbols (left bracket,
		// invalid symbols and 'idx == 0'(the beginning of the expr)) in front of it can be written down
		// into 'hidExpr' immediately.
		else if (expr[idx] == '+' || expr[idx] == '-') {
			if (idx == 0 || expr[idx - 1] == '('
				&& expr[idx - 1] != ')' && expr[idx - 1] != '*'
				&& expr[idx - 1] != '/' && expr[idx - 1] != '^'
				&& !(expr[idx - 1] >= '0' && expr[idx - 1] <= '9')) {
				hidExpr[hidWriter] = '0';
				hidExpr[hidWriter + 1] = ' ';
				hidWriter += 2;
			}
			while (!opStk.isempty() && opStk.top() != '(') {
				hidExpr[hidWriter] = opStk.pop();
				++hidWriter;
			}
			opStk.push(expr[idx]);
		}
		// '*' or '/' detected. Specified symbols between left bracket (included)
		// and '*', '/', '^' can be written down into 'hidExpr' immediately.
		else if (expr[idx] == '*' || expr[idx] == '/') {
			while (!opStk.isempty() && opStk.top() != '('
				&& opStk.top() != '+' && opStk.top() != '-') {
				hidExpr[hidWriter] = opStk.pop();
				++hidWriter;
			}
			opStk.push(expr[idx]);
		}
		// '^' detected. We cannot deal with any symbols in front of it.
		// The only thing we can do is to push it into the stack for operators.
		else if (expr[idx] == '^') opStk.push(expr[idx]);
		// ')' detected. Keep writing all the symbols in front of it down to 'hidExpr'
		// until we meet a left bracket (excluded).
		// Remember: LIFO && discard the brackets
		else if (expr[idx] == ')') {
			while (!opStk.isempty() && opStk.top() != '(') {
				hidExpr[hidWriter] = opStk.pop();
				++hidWriter;
			}
			if (!opStk.isempty() && opStk.top() == '(')
				opStk.pop();
			else throw exprErr("Missing opening bracket(s).");
		}
		++idx;
	}
	// After finishing reading 'expr'(infix expression),
	// pop all the operators out of the stack and append them to 'hidExpr'.
	while (!opStk.isempty()) {
		hidExpr[hidWriter] = opStk.pop();
		if (hidExpr[hidWriter] == '(')
			throw exprErr("Missing closing bracket(s).");
		++hidWriter;
	}
}

double calc::readNumFrom(const char* src) const {
	double currentNum = 0;
	// Record whether or not it is after the decimal point,
	// record how many digits after the decimal point
	int flag = 0;
	while (*src >= '0' && *src <= '9' || *src == '.') {
		if (*src == '.') flag = 1;
		else {
			if (flag) ++flag;
			currentNum = (currentNum * 10 + *src - '0');
		}
		++src; ++hidReader;
	}
	while (--flag > 0) currentNum /= 10;
	return currentNum;
}

void calc::dealOp(char op) {
	double res = 0;
	double former, latter;
	try {
		latter = numStk->pop();
		former = numStk->pop();
	}
	catch (StackNoElement) {
		throw exprErr(
			"There is no enough"\
			" number in the expression."
		);
	}
	switch (op) {
	case '+': res = former + latter; break;
	case '-': res = former - latter; break;
	case '*': res = former * latter; break;
	case '/': res = former / latter; break;
	case '^': res = pow(former, latter);
	}
	numStk->push(res);
	++hidReader;
}

char* calc::getRPNotation() const { return hidExpr; }

double calc::getResult() {
	while (hidExpr[hidReader]) {
		if (hidExpr[hidReader] >= '0'
			&& hidExpr[hidReader] <= '9'
			|| hidExpr[hidReader] == '.') {
			numStk->push(readNumFrom(hidExpr + hidReader));
		}
		else if (hidExpr[hidReader] == '+'
			|| hidExpr[hidReader] == '^'
			|| hidExpr[hidReader] == '/'
			|| hidExpr[hidReader] == '*'
			|| hidExpr[hidReader] == '-')
			dealOp(hidExpr[hidReader]);
		else ++hidReader;
	}
	try {
		result = numStk->pop();
	}
	catch (StackNoElement) {
		throw exprErr(
			"Input Expression Illegal."
		);
	}
	if (!numStk->isempty()) 
		throw exprErr(
			"Input Expression Illegal."
		);
	return result;
}
