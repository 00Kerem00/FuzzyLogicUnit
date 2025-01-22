#pragma once
#include "trapezoid.h"
#include "trapezoid_template.h"
#include <vector>

using namespace std;
struct GravityCenterXWithArea
{
	float gravityCenterX;
	float area;
};

struct IntersectionInfo
{
	Point* intersection;
	int indexOfParent1;
	int indexOfParent2;

	IntersectionInfo() : intersection(nullptr), indexOfParent1(0), indexOfParent2(0) {};
	IntersectionInfo(Point* intersection, int indexOfParent1, int indexOfParent2);
	~IntersectionInfo();
};

class GravityCenterDefuzzification
{
public:

	vector<vector<TrapezoidTemplate>> trapezoidTemplates;			// [property][fuzzy quantity],		will be taken as constructor parameter
	vector<vector<Trapezoid>> modifiedOutputs;						// [property][fuzzy quantity],		will be constructed in this with every defuzzification process

	// dynamic inputs & outputs
	vector<vector<float>>* accuraciesOfFuzzyPropositions;			// [property][fuzzy quantity],		will be taken as constructor parameter
	vector<float>* defuzzifiedOutputs;								// [property],						will be taken as constructor parameter and calculated in defuzzification process

	vector<vector<vector<bool>>> discreteness;						// [property][edge 1][edge 2],		will be constructed in constructor of this
	vector<vector<IntersectionInfo>> intersectionPoints;			// [property][intersection],		will be constructed in this with every defuzzification process
	


	#pragma region Destruction

	~GravityCenterDefuzzification();
	void deleteIntersectionPoints();

#pragma endregion

	#pragma region Create And Clip Trapezoids

	void createAndClipTrapezoids();

#pragma endregion

	#pragma region Calculating Gravity Center

	float calculateGravityCenter(int propertyIndex);										// Single property
	vector<Point*> sortPointsByX(vector<Point*> points);									// Single property
	vector<Point*> mergeSets(int propertyIndex);											// Single property
	void eliminateIntersections(int propertyIndex);											// Single property
	void combineTrapezoids(int quantityIndex0, int quantityIndex1, int propertyIndex);				// Single property

	GravityCenterXWithArea calculateGravityCenterOfUnitWithArea(Point* p0, Point* p1);				// Single property

#pragma endregion

	#pragma region Construction

	GravityCenterDefuzzification() : trapezoidTemplates({}), modifiedOutputs({}), accuraciesOfFuzzyPropositions(nullptr), defuzzifiedOutputs(nullptr), discreteness({}), intersectionPoints({}) {}
	GravityCenterDefuzzification(vector<vector<TrapezoidTemplate>> outputSets, vector<vector<float>>* accuraciesOfFuzzyPropositions, vector<float>* defuzzifiedOutputs);
	
	void calculateDiscreteness();
	void generateModifiedOutputs();
	void sortTrapezoidTemplatesByXOfPointA();

#pragma endregion 
	void defuzzifyOutputs();										// Main method
};

