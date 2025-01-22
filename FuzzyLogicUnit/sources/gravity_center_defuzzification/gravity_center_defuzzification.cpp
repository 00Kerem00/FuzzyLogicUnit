#include "gravity_center_defuzzification.h"

#pragma region IntersectionInfo

IntersectionInfo::IntersectionInfo(Point* intersection, int indexOfParent1, int indexOfParent2) : intersection(intersection), indexOfParent1(indexOfParent1), indexOfParent2(indexOfParent2)
{}

IntersectionInfo::~IntersectionInfo()
{
	//delete intersection;
}

#pragma endregion

#pragma region Construction

GravityCenterDefuzzification::GravityCenterDefuzzification(
	vector<vector<TrapezoidTemplate>> trapezoidTemplates,
	vector<vector<float>>* accuraciesOfFuzzyPropositions,
	vector<float>* defuzzifiedOutputs
) : trapezoidTemplates(trapezoidTemplates), accuraciesOfFuzzyPropositions(accuraciesOfFuzzyPropositions), defuzzifiedOutputs(defuzzifiedOutputs)
{
	sortTrapezoidTemplatesByXOfPointA();
	generateModifiedOutputs();
	calculateDiscreteness();
}

void GravityCenterDefuzzification::calculateDiscreteness()
{
	int propertyCount = trapezoidTemplates.size();
	discreteness.resize(propertyCount);
	for (int i = 0; i < propertyCount; i++)
	{
		int quantityCount = trapezoidTemplates[i].size() - 1;
		discreteness[i].resize(quantityCount);

		for (int j = 0; j < quantityCount; j++)
		{
			int relationCount = quantityCount - j;
			discreteness[i][j].resize(relationCount);

			for (int k = 0; k < relationCount; k++)
			{
				discreteness[i][j][k] = trapezoidTemplates[i][j].isDiscrete(trapezoidTemplates[i][relationCount - k]);
			}
		}
	}
}

void GravityCenterDefuzzification::generateModifiedOutputs()
{
	int propertyCount = trapezoidTemplates.size();
	modifiedOutputs.resize(propertyCount);
	for (int i = 0; i < propertyCount; i++)
	{
		modifiedOutputs[i].resize(trapezoidTemplates[i].size());
	}
}


void GravityCenterDefuzzification::sortTrapezoidTemplatesByXOfPointA()
{
	int propertyCount = trapezoidTemplates.size();
	for (int i = 0; i < propertyCount; i++)
	{
		int trapezoidCount = trapezoidTemplates[i].size();
		for (int j = 0; j < trapezoidCount; j++)
		{
			int minIdx = j;

			for (int k = j; k < trapezoidCount; k++)
			{
				if (trapezoidTemplates[i][k].a < trapezoidTemplates[i][minIdx].a) { minIdx = k; }
			}

			TrapezoidTemplate temp = trapezoidTemplates[i][j]; trapezoidTemplates[i][j] = trapezoidTemplates[i][minIdx]; trapezoidTemplates[i][minIdx] = temp;
		}
	}
}

#pragma endregion

#pragma region Destruction

GravityCenterDefuzzification::~GravityCenterDefuzzification()
{
	deleteIntersectionPoints();
}
void GravityCenterDefuzzification::deleteIntersectionPoints()
{
	int propertyCount = intersectionPoints.size();
	//if (propertyCount == 0) { return; }

	for (int i = 0; i < propertyCount; i++)
	{
		int intersectionCount = intersectionPoints[i].size();
		for (int j = 0; j < intersectionCount; j++) { if (intersectionPoints[i][j].intersection != nullptr) { delete intersectionPoints[i][j].intersection; } }
		intersectionPoints[i].resize(0);
	}
	intersectionPoints.resize(modifiedOutputs.size());
}

#pragma endregion

void GravityCenterDefuzzification::defuzzifyOutputs()
{
	createAndClipTrapezoids();

	int propertyCount = accuraciesOfFuzzyPropositions->size();
	for(int i = 0; i < propertyCount; i++)
	{
		(*defuzzifiedOutputs)[i] = calculateGravityCenter(i);
	}
}

#pragma region Create And Clip Trapezoids

void GravityCenterDefuzzification::createAndClipTrapezoids()
{
	int propertyCount = trapezoidTemplates.size();
	for (int i = 0; i < propertyCount; i++)
	{
		int countOfSets = trapezoidTemplates[i].size();
		for (int j = 0; j < countOfSets; j++)
		{
			modifiedOutputs[i][j] = trapezoidTemplates[i][j].createInstance();
		}
	}

	for (int i = 0; i < propertyCount; i++)
	{
		int countOfSets = trapezoidTemplates[i].size();
		for (int j = 0; j < countOfSets; j++)
		{
			modifiedOutputs[i][j].clipTrapezoid((*accuraciesOfFuzzyPropositions)[i][j]);
		}
	}
}

#pragma endregion

#pragma region Calculating Gravity Center

float GravityCenterDefuzzification::calculateGravityCenter(int propertyIndex)
{
	vector<Point*> points = sortPointsByX(mergeSets(propertyIndex));
	int pointCount = points.size();

	float total_gXa = 0;
	float totalArea = 0;

	GravityCenterXWithArea gWa;
	for(int i = 1; i < pointCount; i++)
	{
		gWa = calculateGravityCenterOfUnitWithArea(points[i - 1], points[i]);
		total_gXa += gWa.gravityCenterX * gWa.area;
		totalArea += gWa.area;
	}

	return total_gXa / totalArea;
}

GravityCenterXWithArea GravityCenterDefuzzification::calculateGravityCenterOfUnitWithArea(Point* p0, Point* p1)
{
	GravityCenterXWithArea result;

	float g_triangle;
	float a_triangle;
	float g_rectangle = (p1->x + p0->x) / 2;;
	float a_rectangle;

	if (p0->y > p1->y)
	{
		g_triangle = (p0->x * 2 + p1->x) / 3;
		a_triangle = (p0->y - p1->y) * (p1->x - p0->x) / 2;
		a_rectangle = p1->y * (p1->x - p0->x);
	}
	else
	{
		g_triangle = (p0->x + p1->x * 2) / 3;
		a_triangle = (p1->y - p0->y) * (p1->x - p0->x) / 2;
		a_rectangle = p0->y * (p1->x - p0->x);
	}

	result.area = (a_triangle + a_rectangle);
	if (result.area != 0)
	{
		result.gravityCenterX = ((g_triangle * a_triangle) + (g_rectangle * a_rectangle)) / result.area;
	}
	else { result.gravityCenterX = g_rectangle; }

	return result;
}

// Kümeleri birleþtirme aþamasýnda x'e göre sýralanmýþ bir þekilde oluþturularak bu aþama engellenebilir.
vector<Point*> GravityCenterDefuzzification::sortPointsByX(vector<Point*> points)
{
	// sort points by x
	int pointCount = points.size();
	for (int i = 0; i < pointCount; i++)
	{
		int minIdx = i;

		for (int j = i; j < pointCount; j++)
		{
			if (points[j]->x < points[minIdx]->x) { minIdx = j; }
		}

		Point* temp = points[i]; points[i] = points[minIdx]; points[minIdx] = temp;
	}

	// check successive points. if x values are same, set point as preceding point that y value is grater than value of other point
	for(int i = 1; i < pointCount; i++)
	{
		if(points[i - 1]->x == points[i]->x)
		{
			if (points[i - 1]->y < points[i]->y)
			{
				Point* temp = points[i - 1];
				points[i - 1] = points[i];
				points[i] = temp;
			}
		}
	}

	return points;
}

void GravityCenterDefuzzification::eliminateIntersections(int propertyIndex)
{
	int intersectionCount = intersectionPoints[propertyIndex].size();
	int countOfSets = modifiedOutputs[propertyIndex].size();

	for (int i = 0; i < intersectionCount; i++)
	{
		IntersectionInfo* ii = &intersectionPoints[propertyIndex][i];
		Point* intersection = intersectionPoints[propertyIndex][i].intersection;

		for(int j = 0; j < countOfSets; j++)
		{
			if (ii->indexOfParent1 == j || ii->indexOfParent2 == j) { continue; }

			if(modifiedOutputs[propertyIndex][j].isInDefinitionRange(*intersection))
			{
				if(modifiedOutputs[propertyIndex][j].isIn(*intersection))
				{
					delete intersection; intersectionPoints[propertyIndex][i].intersection = nullptr; break;
				}
			}
		}
	}
}

vector<Point*> GravityCenterDefuzzification::mergeSets(int propertyIndex)
{
	vector<Point*> result;

	// delete intersection points of previous process before combining trapezoids
	deleteIntersectionPoints();

	// combine all non-discrete sets
	int edge = discreteness[propertyIndex].size();
	for (int i = 0; i < edge; i++)
	{
		int relationCount = edge - i;
		for (int j = 0; j < relationCount; j++)
		{
			if (!discreteness[propertyIndex][i][j]) 
			{
				combineTrapezoids(i, edge - j, propertyIndex); 
			}
		}
	}

	// Add unmarked points to result
	int countOfSets = modifiedOutputs[propertyIndex].size();
	for (int i = 0; i < countOfSets; i++)
	{
		Trapezoid* set = &modifiedOutputs[propertyIndex][i];
		for (int j = 0; j < 4; j++)
		{
			if (!set->getPoint(j)->marked) { result.push_back(set->getPoint(j)); }
		}
	}

	// eliminate points that exist inside any trapezoid (delete and set as nullptr)
	eliminateIntersections(propertyIndex);

	// Add non-null intersection points to results
	int intersectionCount = intersectionPoints[propertyIndex].size();
	for (int i = 0; i < intersectionCount; i++)
	{
		if (intersectionPoints[propertyIndex][i].intersection != nullptr) { result.push_back(intersectionPoints[propertyIndex][i].intersection); }
	}

	return result;
}
void GravityCenterDefuzzification::combineTrapezoids(int quantityIndex0, int quantityIndex1, int propertyIndex)
{
	Point* pointHolder;
	Trapezoid* t0 = &modifiedOutputs[propertyIndex][quantityIndex0];
	Trapezoid* t1 = &modifiedOutputs[propertyIndex][quantityIndex1];

	// generate intersection points
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pointHolder = t0->getBond(i)->getIntersection(t1->getBond(j));
			if (pointHolder != nullptr) { intersectionPoints[propertyIndex].push_back(IntersectionInfo(pointHolder, quantityIndex0 ,quantityIndex1)); }
		}
	}

	// mark points those exist in other trapezoid
	for (int i = 0; i < 4; i++) { pointHolder = t0->getPoint(i); if (t1->isIn(*pointHolder)) { pointHolder->marked = true; } }
	for (int i = 0; i < 4; i++) { pointHolder = t1->getPoint(i); if (t0->isIn(*pointHolder)) { pointHolder->marked = true; } }
}

#pragma endregion