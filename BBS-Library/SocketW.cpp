#include "pch.h"
#include "SocketW.h"

#include <cstdio>

SocketW::SocketW() {
	// Constructor
	this->example_var = 114514;
}

SocketW::~SocketW() {
	// Deconstructor
}

void SocketW::example_func() {
	// Just example func
	this->example_var++;
	printf("%d", this->example_var);
}