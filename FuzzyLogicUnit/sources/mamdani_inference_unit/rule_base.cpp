#include "rule_base.h"

void RuleBase::terminateRuleBaseContext()
{
	int propertyCount = rules.size();
	for (int i = 0; i < propertyCount; i++)
	{
		int quantityCount = rules[i].size();
		for (int j = 0; j < quantityCount; j++)
		{
			int ruleCount = rules[i][j].size();
			for (int k = 0; k < ruleCount; k++)
			{
				rules[i][j][k].terminateRuleContext();
			}
		}
	}

	delete rootTerms;
}

RuleBase::~RuleBase()
{
}

void RuleBase::evaluateRules()
{
	int propertyCount = rawRuleOutputs.size();
	for(int i = 0; i < propertyCount; i++)
	{
		int quantityCount = rawRuleOutputs[i].size();
		for(int j = 0; j < quantityCount; j++)
		{
			int ruleCount = rawRuleOutputs[i][j].size();
			for(int k = 0; k < ruleCount; k++)
			{
				rawRuleOutputs[i][j][k] = rules[i][j][k].evaluateRuleWithWeight();
			}
		}
	}
	mergeResultOutputs();
}

void RuleBase::mergeResultOutputs()
{
	switch (outputMergingType)
	{
		case RuleMergingType::MIN: minMerge(); break;
		case RuleMergingType::MAX: maxMerge(); break;
		case RuleMergingType::AVERAGE: averageMerge(); break;
	}
}

void RuleBase::minMerge()
{
	int propertyCount = rules.size();
	for(int i = 0; i < propertyCount; i++)
	{
		int outputSetCount = rules.size();
		for (int j = 0; j < outputSetCount; j++)
		{
			int ruleCountForOutput_i = rules[j].size();
			float min = rawRuleOutputs[i][j][0];
			for (int k = 1; k < ruleCountForOutput_i; k++)
			{
				if (min > rawRuleOutputs[i][j][k]) { min = rawRuleOutputs[i][j][k]; }
			}

			mergedRuleOutputs[i][j] = min;
		}
	}
}

void RuleBase::maxMerge()
{
	int propertyCount = rules.size();
	for (int i = 0; i < propertyCount; i++)
	{
		int outputSetCount = rules[i].size();
		for (int j = 0; j < outputSetCount; j++)
		{
			int ruleCountForOutput_i = rules[i][j].size();
			float max = rawRuleOutputs[i][j][0];
			for (int k = 1; k < ruleCountForOutput_i; k++)
			{
				if (max < rawRuleOutputs[i][j][k]) { max = rawRuleOutputs[i][j][k]; }
			}

			mergedRuleOutputs[i][j] = max;
		}
	}
}

void RuleBase::averageMerge()
{
	int propertyCount = rules.size();
	for (int i = 0; i < propertyCount; i++)
	{
		int outputSetCount = rules.size();
		for (int j = 0; j < outputSetCount; j++)
		{
			int ruleCountForOutput_i = rules[j].size();
			float total = rawRuleOutputs[i][j][0];
			for (int k = 1; k < ruleCountForOutput_i; k++)
			{
				total += rawRuleOutputs[i][j][k];
			}

			mergedRuleOutputs[i][j] = total / ruleCountForOutput_i;
		}
	}
}
