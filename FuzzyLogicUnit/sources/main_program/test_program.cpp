#include <iostream>
#include "gravity_center_defuzzification.h"
#include "rule_base.h"

using namespace std;

void a()
{
	Point p1(0, 0), p2(100, 1), p3(0, 1), p4(100, 0);
	Bond b1(&p1, &p2), b2(&p3, &p4);

	Point* intersection = b1.getIntersection(&b2);

	cout << "intersection: (" << intersection->x << ", " << intersection->y << ")" << endl;

	delete intersection;
}

void b()
{
	Trapezoid tp(0, 0, 200, 300);

	tp.clipTrapezoid(0.5);

	cout << "membership(0): " << tp.getY(0) << endl;
	cout << "membership(75): " << tp.getY(75) << endl;
	cout << "membership(100): " << tp.getY(100) << endl;
	cout << "membership(154): " << tp.getY(154) << endl;
	cout << "membership(200): " << tp.getY(200) << endl;
	cout << "membership(250): " << tp.getY(250) << endl;
	cout << "membership(257): " << tp.getY(257) << endl;
	cout << "membership(265): " << tp.getY(265) << endl;
	cout << "membership(290): " << tp.getY(290) << endl;
	cout << "membership(300): " << tp.getY(300) << endl;
	cout << "membership(350): " << tp.getY(350) << endl;

	Point p1(50, 0.7), p2(75, 0.35), p3(66, 0.15), p4(73, 0.75), p5(269, 0.1), p6(269, 0.37), p7(340, 0.65), p8(355, 0.37);

	cout << "p1(50, 0.7): " << tp.isIn(p1) << endl;
	cout << "p2(75, 0.35): " << tp.isIn(p2) << endl;
	cout << "p3(166, 0.15): " << tp.isIn(p3) << endl;
	cout << "p4(173, 0.75): " << tp.isIn(p4) << endl;
	cout << "p5(269, 0.1): " << tp.isIn(p5) << endl;
	cout << "p6(269, 0.37): " << tp.isIn(p6) << endl;
	cout << "p7(340, 0.65): " << tp.isIn(p7) << endl;
	cout << "p8(355, 0.37): " << tp.isIn(p8) << endl;
}

void c()
{
	vector<vector<TrapezoidTemplate>> trapezoids
	{
		{
			TrapezoidTemplate(50, 50, 200, 400),
			TrapezoidTemplate(300, 450, 550, 800),
			TrapezoidTemplate(700, 900, 1500, 1500)
		}
	};

	vector<vector<float>> accuracies{ {0, 0, 0} };
	vector<float> defuzzifiedOutputs(1);
	
	GravityCenterDefuzzification def(trapezoids, &accuracies, &defuzzifiedOutputs);
	
	vector<vector<bool>> discreteness = def.discreteness[0];
	

	def.defuzzifyOutputs();
	cout << "defuzzified output: " << defuzzifiedOutputs[0] << endl;

	return;

	def.createAndClipTrapezoids();
	vector<Point*> points = def.sortPointsByX(def.mergeSets(0));
	int pointCount = points.size();
	for(int i = 0; i < pointCount; i++)
	{
		cout << "point " << i << ": (" << points[i]->x << ", " << points[i]->y << ")" << endl;
	}

	cout << endl;
	for(int i = 1; i < pointCount; i++)
	{
		GravityCenterXWithArea gWa = def.calculateGravityCenterOfUnitWithArea(points[i - 1], points[i]);
		cout << "area between p" << i - 1 << " with p" << i << ": " << gWa.area << endl;
		cout << "gravity center between p" << i - 1 << " with p" << i << ": " << gWa.gravityCenterX << endl;
		cout << endl;
	}

	cout << endl;
}

void d()
{
	RootTerm* term1 = new RootTerm(); term1->setValue(0.4);
	RootTerm* term2 = new RootTerm(); term2->setValue(0.8);
	RootTerm* term3 = new RootTerm(); term3->setValue(0.9);

	SingleRule rule(
		Conjunction(
			(RuleTerm*)term1, 
			new Conjunction((RuleTerm*)term2, (RuleTerm*)term3, AND),
			OR
		),
		1
	);

	cout << "result of rule: " << rule.evaluateRule() << endl;

	rule.terminateRuleContext();
	delete term1; delete term2; delete term3;
}

SingleRule produceSingleRule_(vector<vector<RootTerm>>* rootTerms, int areaQuantityIdx, int locationQuantityIdx, int geometryQuantityIdx, int kaksQuantityIdx)
{
	return SingleRule(
		Conjunction(
			new Conjunction(
				new Conjunction(
					&(*rootTerms)[0][areaQuantityIdx],
					&(*rootTerms)[1][locationQuantityIdx],
					ConjunctionType::OR
				),
				&(*rootTerms)[2][geometryQuantityIdx],
				ConjunctionType::OR
			),
			&(*rootTerms)[3][geometryQuantityIdx],
			ConjunctionType::OR
		),
		1
	);

}
void e() 
{
	vector<vector<RootTerm>>* rootTerms = RootTerm::generateRootTermGroup({ 3, 3, 3, 3 }, 0.5);
	(*rootTerms)[0][1].setValue(0.8);
	(*rootTerms)[1][0].setValue(0.65);
	(*rootTerms)[2][1].setValue(0.7);
	(*rootTerms)[3][2].setValue(0.2);

	RuleBase rulebase(
		rootTerms,
		{
			// "Maliyet" özelliði ile ilgili kurallar 
			{
				// "Maliyet ucuzdur" çýktýsýný veren kurallar 
				{
					// Eðer ((alan orta ve konum az merkezi) ve geometri az düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule_(rootTerms, 0, 1, 1, 0),
					// Eðer ((alan orta ve konum az merkezi) ve geometri az düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule_(rootTerms, 0, 1, 1, 1),
					// Eðer ((alan orta ve konum az merkezi) ve geometri az düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule_(rootTerms, 1, 0, 1, 1),
					// Eðer ((alan orta ve konum az merkezi) ve geometri az düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule_(rootTerms, 1, 1, 0, 1),
					// Eðer ((alan orta ve konum az merkezi) ve geometri az düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule_(rootTerms, 1, 1, 1, 0),
					// Eðer ((alan orta ve konum az merkezi) ve geometri az düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule_(rootTerms, 1, 1, 1, 1),
					// Eðer ((alan büyük ve konum merkezi deðil) ve geometri düzgün deðil) ve kaks "0.0 - 0.5 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule_(rootTerms, 2, 0, 0, 0)
				},

				// "Maliyet ortalamadýr" çýktýsýný veren kurallar 
				{
					// Eðer ((alan orta ve konum merkezi) ve geometri düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ortalamadýr.
					produceSingleRule_(rootTerms, 1, 1, 1, 2),
					// Eðer ((alan orta ve konum merkezi) ve geometri düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ortalamadýr.
					produceSingleRule_(rootTerms, 1, 2, 2, 0),
					// Eðer ((alan orta ve konum merkezi) ve geometri düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ortalamadýr.
					produceSingleRule_(rootTerms, 1, 2, 2, 0),
					// Eðer ((alan orta ve konum merkezi) ve geometri düzgün) ve kaks "1 - 2 aralýðýnda" ise Maliyet ortalamadýr.
					produceSingleRule_(rootTerms, 1, 2, 2, 2),
					// Eðer ((alan orta ve konum merkezi) ve geometri düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule_(rootTerms, 1, 2, 2, 1),
					// Eðer ((alan büyük ve konum merkezi) ve geometri düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ortalamadýr.
					produceSingleRule_(rootTerms, 2, 2, 2, 0),
					// Eðer ((alan büyük ve konum merkezi) ve geometri düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ortalamadýr.
					produceSingleRule_(rootTerms, 0, 2, 2, 2)
				},

				// "Maliyet pahalýdýr" çýktýsýný veren kurallar 
				{
					// Eðer ((alan büyük ve konum merkezi) ve geometri düzgün) ve kaks "1 - 2 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule_(rootTerms, 2, 2, 2, 2),
					// Eðer ((alan büyük ve konum merkezi) ve geometri düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule_(rootTerms, 2, 2, 2, 1),
					// Eðer ((alan büyük ve konum merkezi) ve geometri az düzgün) ve kaks "1 - 2 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule_(rootTerms, 2, 2, 1, 2),
					// Eðer ((alan büyük ve konum az merkezi) ve geometri düzgün) ve kaks "1 - 2 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule_(rootTerms, 2, 1, 2, 2),
					// Eðer ((alan büyük ve konum merkezi) ve geometri düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule_(rootTerms, 2, 1, 2, 1),
					// Eðer ((alan büyük ve konum merkezi) ve geometri düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule_(rootTerms, 2, 2, 1, 1)
				}
			}
		},
		RuleMergingType::MAX
	);

	rulebase.evaluateRules();
	vector<vector<float>>* outputs = &rulebase.mergedRuleOutputs;

	int l = outputs->size();
	for(int i = 0; i < l; i++)
	{
		int ll = (*outputs)[i].size() - 1;
		for(int j = 0; j < ll; j++)
		{
			cout << (*outputs)[i][j] << ", ";
		}
		cout << (*outputs)[i][ll] << endl;
	}
}

void bb()
{
	vector<float> inputs{ 250, 700, 350, 1 };						// [0]: Alan, [1]: Konum, [2]: Geometrik Þekil: [3]: KAKS

	vector<vector<Trapezoid>> inputSets;// = generateInputSets();
	int propertyCount = inputSets.size();

	vector<vector<float>> accuracies(propertyCount);

	for (int i = 0; i < propertyCount; i++)
	{
		int quantityCount = inputSets[i].size();
		accuracies[i].resize(quantityCount);

		for (int j = 0; j < quantityCount; j++)
		{
			accuracies[i][j] = inputSets[i][j].getY(inputs[i]);
		}
	}

	for (int i = 0; i < propertyCount; i++)
	{
		int quantityCount = inputSets[i].size() - 1;

		for (int j = 0; j < quantityCount; j++)
		{
			cout << accuracies[i][j] << ", ";
		}
		cout << accuracies[i][quantityCount] << endl;
	}
}


int mainss()
{
	c();

	return 0;
}


/*	int l = discreteness.size();
	for(int i = 0; i < l; i++)
	{
		int ll = discreteness[i].size();
		for(int j = 0; j < ll; j++)
		{
			cout << discreteness[i][j] << " ";
		}
		cout << endl;
	}*/

/*vector<Point*> combineTrapezoids(Trapezoid* t0, Trapezoid* t1)
{
	vector<Point*> result;
	Point* pointHolder;

	// generate intersection points
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			pointHolder = t0->getBond(i)->getIntersection(t1->getBond(j));
			if (pointHolder != nullptr) { result.push_back(pointHolder); }
		}
	}

	// mark points those exist in other trapezoid
	for (int i = 0; i < 4; i++) { pointHolder = t0->getPoint(i); if (t1->isIn(*pointHolder)) { pointHolder->marked = true; } else { result.push_back(pointHolder); } }
	for (int i = 0; i < 4; i++) { pointHolder = t1->getPoint(i); if (t0->isIn(*pointHolder)) { pointHolder->marked = true; } else { result.push_back(pointHolder); } }

	return result;
}
void d()
{
	Trapezoid t1(50, 50, 200, 400);
	Trapezoid t2(300, 450, 550, 800);

	vector<Point*> points = combineTrapezoids(&t1, &t2);
	int pointCount = points.size();

	for(int i = 0; i < pointCount; i++)
	{
		cout << "point " << i << ": (" << points[i]->x << ", " << points[i]->y << ")" << endl;
	}
}*/


/*RuleBase generateRuleBase()
{
	// 4 özellik
	// [0]: Alan özelliðinin 3 adet nicelik kümesi var.
	// [1]: Konum özelliðinin 3 adet nicelik kümesi var.
	// [2]: Geometrik þekil özelliðinin 3 adet nicelik kümesi var.
	// [3]: KAKS özelliðinin 3 adet nicelik kümesi var.
	vector<vector<RootTerm>>* rootTerms = RootTerm::generateRootTermGroup({ 3, 3, 3, 3 }, 0);

	RuleBase result = RuleBase(
		rootTerms,
		{
			// "Maliyet" özelliði ile ilgili kurallar 
			{
				// "Maliyet ucuzdur" çýktýsýný veren kurallar 
				{
					// Eðer ((alan orta ve konum az merkezi) ve geometri az düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule(rootTerms, 0, 1, 1, 0),
					// Eðer ((alan orta ve konum az merkezi) ve geometri az düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule(rootTerms, 0, 1, 1, 1),
					// Eðer ((alan orta ve konum az merkezi) ve geometri az düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule(rootTerms, 1, 0, 1, 1),
					// Eðer ((alan orta ve konum az merkezi) ve geometri az düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule(rootTerms, 1, 1, 0, 1),
					// Eðer ((alan orta ve konum az merkezi) ve geometri az düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule(rootTerms, 1, 1, 1, 0),
					// Eðer ((alan orta ve konum az merkezi) ve geometri az düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule(rootTerms, 1, 1, 1, 1),
					// Eðer ((alan büyük ve konum merkezi deðil) ve geometri düzgün deðil) ve kaks "0.0 - 0.5 aralýðýnda" ise Maliyet ucuzdur.
					produceSingleRule(rootTerms, 2, 0, 0, 0)
				},

				// "Maliyet ortalamadýr" çýktýsýný veren kurallar 
				{
					// Eðer ((alan orta ve konum merkezi) ve geometri düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ortalamadýr.
					produceSingleRule(rootTerms, 1, 1, 1, 2),
					// Eðer ((alan orta ve konum merkezi) ve geometri düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ortalamadýr.
					produceSingleRule(rootTerms, 1, 2, 2, 0),
					// Eðer ((alan orta ve konum merkezi) ve geometri düzgün) ve kaks "1 - 2 aralýðýnda" ise Maliyet ortalamadýr.
					produceSingleRule(rootTerms, 1, 2, 2, 2),
					// Eðer ((alan orta ve konum merkezi) ve geometri düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule(rootTerms, 1, 2, 2, 1),
					// Eðer ((alan büyük ve konum merkezi) ve geometri düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ortalamadýr.
					produceSingleRule(rootTerms, 2, 2, 2, 0),
					// Eðer ((alan büyük ve konum merkezi) ve geometri düzgün) ve kaks "0 - 0.5 aralýðýnda" ise Maliyet ortalamadýr.
					produceSingleRule(rootTerms, 0, 2, 2, 2)
				},

				// "Maliyet pahalýdýr" çýktýsýný veren kurallar 
				{
					// Eðer ((alan büyük ve konum merkezi) ve geometri düzgün) ve kaks "1 - 2 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule(rootTerms, 2, 2, 2, 2),
					// Eðer ((alan büyük ve konum merkezi) ve geometri düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule(rootTerms, 2, 2, 2, 1),
					// Eðer ((alan büyük ve konum merkezi) ve geometri az düzgün) ve kaks "1 - 2 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule(rootTerms, 2, 2, 1, 2),
					// Eðer ((alan büyük ve konum az merkezi) ve geometri düzgün) ve kaks "1 - 2 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule(rootTerms, 2, 1, 2, 2),
					// Eðer ((alan büyük ve konum merkezi) ve geometri düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule(rootTerms, 2, 1, 2, 1),
					// Eðer ((alan büyük ve konum merkezi) ve geometri düzgün) ve kaks "0.5 - 1 aralýðýnda" ise Maliyet yüksetir.
					produceSingleRule(rootTerms, 2, 2, 1, 1)
				}
			}
		},
		RuleMergingType::MAX
	);

	return result;
}

*/