#pragma once
#include "rule_term.h"

class NotTerm : public RuleTerm
{
private:
	RuleTerm* term;

public:
	NotTerm(RuleTerm* term) : term(term){}
	void terminateTermContext() override;
	float getValue() override;
};