#pragma once
#include "rule_term.h"
#include <vector>

class RootTerm : public RuleTerm
{
private: 
	float rootValue;

public:
	void terminateTermContext() override;
	float getValue() override;
	void setValue(float value);


	static std::vector<std::vector<RootTerm>>* generateRootTermGroup(std::vector<float> quantityPerProperty, float initValue);
};