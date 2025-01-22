#pragma once
#include "rule_term.h"

enum ConjunctionType { AND, OR };

class Conjunction : public RuleTerm
{
private:

	ConjunctionType type;
	RuleTerm* term1 = nullptr;
	RuleTerm* term2 = nullptr;

public:
	Conjunction(RuleTerm* term1, RuleTerm* term2, ConjunctionType type) : term1(term1), term2(term2), type(type){}
	void terminateTermContext() override;
	float getValue() override;

private:
	float min();
	float max();
};