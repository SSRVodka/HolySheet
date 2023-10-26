#pragma once

class StackNoElement {
	const char* message;
public:
	StackNoElement(const char* msg="No element in the stack.") : message(msg) {}
	const char* what() const { return message; }
};

template <class elemT>
class stack {
public:
	virtual ~stack() {}
	virtual bool isempty() const = 0;
	virtual void push(const elemT& dt) = 0;
	virtual elemT pop() = 0;
	virtual elemT top() const = 0;
};

template <class elemT>
class seqStack : public stack<elemT> {
private:
	elemT* data;
	int maxSize;
	int topIdx;
	void doubleSpace();
public:
	seqStack(int initSize=10) : maxSize(initSize), topIdx(-1) {
		data = new elemT[maxSize];
	}
	~seqStack() { if (data) delete[] data; }
	bool isempty() const override { return topIdx == -1; }
	void push(const elemT& dt) override {
		if (topIdx >= maxSize - 1) 
			doubleSpace(); 
		data[++topIdx] = dt;
	}
	elemT pop() override { 
		if (topIdx == -1) throw StackNoElement();
		return data[topIdx--];
	}
	elemT top() const override { 
		if (topIdx == -1) throw StackNoElement();
		return data[topIdx]; 
	}
};

template <class elemT>
void seqStack<elemT>::doubleSpace() {
	elemT* tmp = data;
	data = new elemT[maxSize * 2];
	for (int i = 0; i <= topIdx; ++i)
		data[i] = tmp[i];
	delete[] tmp;
	maxSize *= 2;
}

template <class elemT>
class linkStack : public stack<elemT> {
private:
	struct Node {
		elemT data;
		Node* nextNode;

		Node() : nextNode(nullptr) {}
		Node(const elemT& dt, Node* next = nullptr)
			: data(dt), nextNode(next) {}
		~Node() {}
	};

	Node* topNode;

public:
	linkStack() : topNode(nullptr) {}
	~linkStack();
	bool isempty() const override {
		return topNode == nullptr;
	}
	void push(const elemT& dt) override {
		topNode = new Node(dt, topNode);
	}
	elemT pop() override {
		if (topNode == nullptr) throw StackNoElement();
		Node* tmp = topNode;
		topNode = topNode->nextNode;
		elemT dt = tmp->data;
		delete tmp;
		return dt;
	}
	elemT top() const override { 
		if (topNode == nullptr) throw StackNoElement();
		return topNode->data; 
	}
};

template <class elemT>
linkStack<elemT>::~linkStack() {
	Node* tmp;
	while (topNode != nullptr) {
		tmp = topNode;
		topNode = topNode->nextNode;
		delete tmp;
	}
}
