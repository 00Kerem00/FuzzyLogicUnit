#pragma once
#include "conjunction.h"

class SingleRule
{
	Conjunction conjunctions;
	float weight;

public:
	SingleRule(Conjunction conjunctions, float weight) : conjunctions(conjunctions), weight(weight) 
	{}
	void terminateRuleContext();
	~SingleRule(){}
	float evaluateRuleWithWeight();
	float evaluateRule();
};