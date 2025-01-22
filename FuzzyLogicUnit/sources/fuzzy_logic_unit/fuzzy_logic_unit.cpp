#include "fuzzy_logic_unit.h"

FuzzyLogicUnit::FuzzyLogicUnit(
	vector<float>* inputs, 
	vector<float>* outputs, 
	vector<vector<Trapezoid>>* inputSets, 
	vector<vector<TrapezoidTemplate>>* outputSets, 
	InferenceUnit* inferenceUnit
) : inputs(inputs), outputs(outputs), inputSets(inputSets), outputSets(outputSets), inferenceUnit(inferenceUnit)
{
	inferenceUnit->connectToLogicUnit(inputs, outputs, inputSets, outputSets);
}

void FuzzyLogicUnit::terminateContext()
{
	inferenceUnit->terminateInferenceContext();
	delete inferenceUnit;
}

FuzzyLogicUnit::~FuzzyLogicUnit()
{
	// delete inferenceUnit;
}

void FuzzyLogicUnit::evaluate() { inferenceUnit->doInference(); }

#pragma region Mamdani Reference Unit

MamdaniInference::MamdaniInference(RuleBase ruleBase) : inputs(nullptr), outputs(nullptr), inputSets(nullptr), outputSets(nullptr),
	ruleBase(ruleBase)
{}

void MamdaniInference::terminateInferenceContext()
{
	ruleBase.terminateRuleBaseContext();
}

void MamdaniInference::connectToLogicUnit(vector<float>* inputs, vector<float>* outputs, vector<vector<Trapezoid>>* inputSets, vector<vector<TrapezoidTemplate>>* outputSets)
{
	this->inputs = inputs;
	this->outputs = outputs;
	this->inputSets = inputSets;

	this->outputSets = outputSets;
	defuzzification = GravityCenterDefuzzification(*outputSets, &(ruleBase.mergedRuleOutputs), outputs);
}

void MamdaniInference::provideRootTermValues()
{
	int propertyCount = ruleBase.rootTerms->size();
	for (int i = 0; i < propertyCount; i++)
	{
		float input = (*inputs)[i];
		int quantityCount = (*ruleBase.rootTerms)[i].size();
		for (int j = 0; j < quantityCount; j++)
		{
			float y = (*inputSets)[i][j].getY(input);
			(*ruleBase.rootTerms)[i][j].setValue(y);
		}
	}
}

void MamdaniInference::doInference() 
{
	provideRootTermValues();
	ruleBase.evaluateRules();
	defuzzification.defuzzifyOutputs();

}

#pragma endregion