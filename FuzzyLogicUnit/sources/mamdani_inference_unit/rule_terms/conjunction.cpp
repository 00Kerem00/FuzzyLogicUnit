#include "conjunction.h"

void Conjunction::terminateTermContext()
{
	if (term1 != nullptr)
	{
		term1->terminateTermContext();
		term1 = nullptr;
	}
	if (term2 != nullptr)
	{
		term2->terminateTermContext();
		term2 = nullptr;
	}
	this->~Conjunction();
}

float Conjunction::getValue()
{
	switch (type)
	{
		case ConjunctionType::AND: return min();
		case ConjunctionType::OR: return max();
		default: return 0;
	}
}

float Conjunction::min()
{
	float v1 = term1->getValue();
	float v2 = term2->getValue();

	return v1 < v2 ? v1 : v2;
}

float Conjunction::max()
{
	float v1 = term1->getValue();
	float v2 = term2->getValue();

	return v1 < v2 ? v2 : v1;
}
