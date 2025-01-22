#pragma once
#include "single_rule.h"
#include "root_term.h"
#include "not_term.h"
#include <vector>

using namespace std;
enum RuleMergingType { MIN, MAX, AVERAGE };

class RuleBase
{

	RuleMergingType outputMergingType;						// will be taken as Constructor Parameter
	vector<vector<vector<SingleRule>>> rules;				// [property][fuzzy quantity][related rule], will be taken as Constructor Parameter

	// [property][fuzzy quantity][related rule], kök terim bir girdi kümesinin üyelik deðeridir. uzunluðu girdi kümesi sayýsý kadar olmalýdýr. Kural Tabaný sýnýfýnda bulunmalýdýr ve bu sýnýfta yapýlandýrýlmalýdýr.
	vector<vector<vector<float>>> rawRuleOutputs;			// [property][fuzzy quantity][related rule], will be constructed in this

public:
	vector<vector<RootTerm>>* rootTerms;
	vector<vector<float>> mergedRuleOutputs;				// ordered by target outputs [property][fuzzy quantity], will be constructed in this

	RuleBase() : outputMergingType(RuleMergingType::AVERAGE), rules({}), rootTerms(nullptr), rawRuleOutputs({}), mergedRuleOutputs({}) {}
	void terminateRuleBaseContext();
	RuleBase(vector<vector<RootTerm>>* rootTerms, vector<vector<vector<SingleRule>>> rules, RuleMergingType outputMergingType) : 
		rootTerms(rootTerms), 
		rules(rules), 
		outputMergingType(outputMergingType) 
	{
		// resize rule outputs
		int propertyCount = rules.size();
		rawRuleOutputs.resize(propertyCount);
		mergedRuleOutputs.resize(propertyCount);
		for (int i = 0; i < propertyCount; i++)
		{
			int quantityCount = rules[i].size();
			mergedRuleOutputs[i].resize(quantityCount);
			rawRuleOutputs[i].resize(quantityCount);
			for(int j = 0; j < quantityCount; j++)
			{
				rawRuleOutputs[i][j].resize(rules[i][j].size());
			}
		}
	}
	~RuleBase();
	void evaluateRules();

private:
	void mergeResultOutputs();

	// merge methods
	void minMerge();
	void maxMerge();
	void averageMerge();
};