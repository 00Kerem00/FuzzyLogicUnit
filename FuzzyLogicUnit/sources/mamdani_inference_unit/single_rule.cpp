#include "single_rule.h"

void SingleRule::terminateRuleContext()
{
	conjunctions.terminateTermContext();
}

float SingleRule::evaluateRuleWithWeight()
{
	return conjunctions.getValue() * weight;
}

float SingleRule::evaluateRule()
{
	return conjunctions.getValue();
}
