#include "root_term.h"

void RootTerm::terminateTermContext()
{
	//rootValue = -1001;
}

float RootTerm::getValue()
{
	return rootValue;
}

void RootTerm::setValue(float value)
{
	this->rootValue = value;
}

std::vector<std::vector<RootTerm>>* RootTerm::generateRootTermGroup(std::vector<float> quantityPerProperty, float initValue)
{
	int propertyCount = quantityPerProperty.size();
	std::vector<std::vector<RootTerm>>* result = new std::vector<std::vector<RootTerm>>(propertyCount);
	for (int i = 0; i < propertyCount; i++) 
	{ 
		int quantityCount = quantityPerProperty[i];
		(*result)[i].resize(quantityCount);

		for(int j = 0; j < quantityCount; j++)
		{
			(*result)[i][j].setValue(initValue);
		}
	}

	return result;
}
