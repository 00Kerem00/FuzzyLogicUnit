#include "not_term.h"

void NotTerm::terminateTermContext()
{
	if (term != nullptr)
	{
		term->terminateTermContext();
		term = nullptr;
	}
	this->~NotTerm();
}

float NotTerm::getValue()
{
	return 1 - term->getValue();
}
