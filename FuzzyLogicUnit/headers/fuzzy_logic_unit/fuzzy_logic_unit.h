#pragma once
#include "trapezoid.h"
#include "input_connector.h"
#include "rule_base.h"
#include "gravity_center_defuzzification.h"

using namespace std;

class InferenceUnit
{
public:
	virtual void connectToLogicUnit(vector<float>* inputs, vector<float>* outputs, vector<vector<Trapezoid>>* inputSets, vector<vector<TrapezoidTemplate>>* outputSets) {}
	virtual void terminateInferenceContext() = 0;
	virtual void doInference() {}
};

class MamdaniInference : public InferenceUnit
{
	vector<float>* inputs;								// [clear inputs]
	vector<float>* outputs;								// [clear outputs]

	vector<vector<Trapezoid>>* inputSets;				// [property][quantity set]
	vector<vector<TrapezoidTemplate>>* outputSets;		// [property][quantity set]

	RuleBase ruleBase;									// member of this

	GravityCenterDefuzzification defuzzification;

	void connectToLogicUnit(vector<float>* inputs, vector<float>* outputs, vector<vector<Trapezoid>>* inputSets, vector<vector<TrapezoidTemplate>>* outputSets) override;
	void provideRootTermValues();

public:
	MamdaniInference(RuleBase ruleBase);
	virtual void terminateInferenceContext() override;

	void doInference() override;
};

class FuzzyLogicUnit
{
	vector<float>* inputs;								// [clear inputs]
	vector<float>* outputs;								// [clear outputs]

	vector<vector<Trapezoid>>* inputSets;				// [property][quantity set]
	vector<vector<TrapezoidTemplate>>* outputSets;		// [property][quantity set]

	InferenceUnit* inferenceUnit;

public:
	FuzzyLogicUnit(vector<float>* inputs, vector<float>* outputs, vector<vector<Trapezoid>>* inputSets, vector<vector<TrapezoidTemplate>>* outputSets, InferenceUnit* inferenceUnit);
	void terminateContext();
	~FuzzyLogicUnit();
	void evaluate();
};