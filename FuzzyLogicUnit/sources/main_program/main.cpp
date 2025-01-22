#include <stdio.h>
#include "iostream"
#include "fuzzy_logic_unit.h"

vector<vector<Trapezoid>> generateInputSets()
{
	vector<vector<Trapezoid>> inputSets;

	// 4 adet giri� �zelli�imiz var; [0]: Alan, [1]: Konum �zellikleri, [2]: Geometrik �ekil, [3]: KAKS
	inputSets.resize(4);

	// Alan �zelli�inin 3 adet nicelik k�mesi bulunuyor; [0]: K���k, [1]: Orta, [2]: B�y�k
	inputSets[0].resize(3);
	inputSets[0][0] = Trapezoid(0, 100, 200, 300);
	inputSets[0][1] = Trapezoid(200, 300, 400, 500);
	inputSets[0][2] = Trapezoid(450, 500, 600, 780);

	// Konum �zelliklerinin 3 adet nicelik k�mesi bulunuyor; [0]: Merkezi De�il, [1]: Az Merkezi, [2]: Merkezi
	inputSets[1].resize(3);
	inputSets[1][0] = Trapezoid(0, 100, 200, 300);
	inputSets[1][1] = Trapezoid(200, 300, 400, 500);
	inputSets[1][2] = Trapezoid(500, 600, 700, 1000);

	// Geometrik �ekil �zelli�inin 3 adet nicelik k�mesi bulunuyor; [0]: D�zg�n De�il, [1]: Az D�zg�n, [2]: D�zg�n
	inputSets[2].resize(3);
	inputSets[2][0] = Trapezoid(0, 100, 200, 300);
	inputSets[2][1] = Trapezoid(250, 400, 550, 600);
	inputSets[2][2] = Trapezoid(550, 600, 700, 740);

	// KAKS �zelli�inin 3 adet nicelik k�mesi bulunuyor; [0]: "0.0-0.5", [1]: "0.5-1.0", [2]: "1.0-2.0"
	inputSets[3].resize(3);
	inputSets[3][0] = Trapezoid(0.0, 0.3, 0.4, 0.5);
	inputSets[3][1] = Trapezoid(0.5, 0.6, 0.7, 1.0);
	inputSets[3][2] = Trapezoid(0.8, 1.2, 1.6, 2.0);

	return inputSets;
}

vector<vector<TrapezoidTemplate>> generateOutputSets()
{
	vector<vector<TrapezoidTemplate>> outputSets;

	// Sadece bir ��kt� �zelli�imiz var: Maliyet
	outputSets.resize(1);

	// Maliyet �zelli�inde 3 adet nicelik k�mesi bulunuyor; [0]: Ucuz, [1]: Ortalama, [2]: Pahal� (bin TL cinsinden)
	outputSets[0].resize(3);
	outputSets[0][0] = TrapezoidTemplate(50, 50, 200, 400);
	outputSets[0][1] = TrapezoidTemplate(300, 450, 550, 800);
	outputSets[0][2] = TrapezoidTemplate(700, 900, 1500, 1500);

	return outputSets;
}

SingleRule produceSingleRule(vector<vector<RootTerm>>* rootTerms, int areaQuantityIdx, int locationQuantityIdx, int geometryQuantityIdx, int kaksQuantityIdx)
{
	return SingleRule(
		Conjunction(
			new Conjunction(
				new Conjunction(
					&(*rootTerms)[0][areaQuantityIdx],
					&(*rootTerms)[1][locationQuantityIdx],
					ConjunctionType::AND
				),
				&(*rootTerms)[2][geometryQuantityIdx],
				ConjunctionType::AND
			),
			&(*rootTerms)[3][kaksQuantityIdx],
			ConjunctionType::AND
		),
		1
	);

}

RuleBase generateRuleBase()
{
	// 4 �zellik
	// [0]: Alan �zelli�inin 3 adet nicelik k�mesi var.
	// [1]: Konum �zelli�inin 3 adet nicelik k�mesi var.
	// [2]: Geometrik �ekil �zelli�inin 3 adet nicelik k�mesi var.
	// [3]: KAKS �zelli�inin 3 adet nicelik k�mesi var.
	vector<vector<RootTerm>>* rootTerms = RootTerm::generateRootTermGroup({ 3, 3, 3, 3 }, 0);

	RuleBase result = RuleBase(
		rootTerms,
		{
			// "Maliyet" �zelli�i ile ilgili kurallar 
			{
				// "Maliyet ucuzdur" ��kt�s�n� veren kurallar 
				{
					// E�er (geometri az d�zg�n ve ((KAKS y�ksek) de�il)) ise MAL�YET UCUZDUR.
					SingleRule(Conjunction(&(*rootTerms)[2][0], new NotTerm(&(*rootTerms)[3][2]), AND), 1),

					// E�er (geometri k�t� ve (alan k���k ve konum merkezi de�il)) ise MAL�YET UCUZDUR.
					SingleRule(Conjunction(&(*rootTerms)[2][0], new Conjunction(&(*rootTerms)[0][0], &(*rootTerms)[1][0], AND), AND), 1),

					// E�er (geometri az d�zg�n ve (((KAKS y�ksek) de�il) ve ((alan b�y�k) de�il))) ise MAL�YET UCUZDUR.
					SingleRule(Conjunction(&(*rootTerms)[2][1], new Conjunction(new NotTerm(&(*rootTerms)[3][2]), new NotTerm(&(*rootTerms)[0][2]), AND), AND), 1),

					// E�er (geometri d�zg�n ve (KAKS d���k ve alan k���k)) ise MAL�YET UCUZDUR.
					SingleRule(Conjunction(&(*rootTerms)[2][2], new Conjunction(&(*rootTerms)[3][0], &(*rootTerms)[0][0], AND), AND), 1),

					// E�er (geometri d�zg�n ve (((KAKS y�ksek) de�il) ve (alan k���k ve ((Konum merkeze yak�n) de�il)))) ise MAL�YET UCUZDUR.
					SingleRule(Conjunction(&(*rootTerms)[2][2], new Conjunction(new NotTerm(&(*rootTerms)[3][2]), new Conjunction(&(*rootTerms)[0][0], new NotTerm(&(*rootTerms)[1][2]), AND), AND), AND), 1),
				},

				// "Maliyet ortalamad�r" ��kt�s�n� veren kurallar 
				{
					// E�er (geometri k�t� ve (KAKS y�ksek ve (((alan b�y�k) de�il) ve ((konum merkeze uzak) de�il)))) ise MAL�YET ORTALAMADIR.
					SingleRule(Conjunction(&(*rootTerms)[2][0], new Conjunction(&(*rootTerms)[3][2], new Conjunction(new NotTerm(&(*rootTerms)[0][2]), new NotTerm(&(*rootTerms)[1][0]), AND), AND), AND), 1),

					// E�er (geometri az d�zg�n ve (KAKS d���k ve alan b�y�k)) ise MAL�YET ORTALAMADIR.
					SingleRule(Conjunction(&(*rootTerms)[2][1], new Conjunction(&(*rootTerms)[3][0], &(*rootTerms)[0][2], AND), AND), 1),

					// E�er (geometri az d�zg�n ve (((KAKS y�ksek) de�il) ve ((konum merkezi) de�il))) ise MAL�YET ORTALAMADIR.
					SingleRule(Conjunction(&(*rootTerms)[2][1], new Conjunction(new NotTerm(&(*rootTerms)[3][2]), new NotTerm(&(*rootTerms)[1][2]), AND), AND), 1),

					// E�er (geometri az d�zg�n ve (KAKS y�ksek ve alan k���k)) ise MAL�YET ORTALAMADIR.
					SingleRule(Conjunction(&(*rootTerms)[2][1], new Conjunction(&(*rootTerms)[3][2], &(*rootTerms)[0][0], AND), AND), 1),

					// E�er (geometri d�zg�n ve alan orta) ise MAL�YET ORTALAMADIR.
					SingleRule(Conjunction(&(*rootTerms)[2][2], &(*rootTerms)[0][1], AND), 1),

					// E�er (geometri d�zg�n ve (KAKS d���k ve alan b�y�k)) ise MAL�YET ORTALAMADIR.
					SingleRule(Conjunction(&(*rootTerms)[2][2], new Conjunction(&(*rootTerms)[3][0], &(*rootTerms)[0][2], AND), AND), 1),

					// E�er (geometri d�zg�n ve (KAKS y�ksek ve alan k���k)) ise MAL�YET ORTALAMADIR.
					SingleRule(Conjunction(&(*rootTerms)[2][2], new Conjunction(&(*rootTerms)[3][2], &(*rootTerms)[0][0], AND), AND), 1),

					// E�er geometri d�zg�n ve KAKS orta ve alan k���k ve konum merkeze uzak ise MAL�YET ORTALAMADIR.
					SingleRule(Conjunction(&(*rootTerms)[2][2], new Conjunction(&(*rootTerms)[3][1], new Conjunction(&(*rootTerms)[0][0], &(*rootTerms)[1][0], AND), AND), AND), 1)
				},

				// "Maliyet pahal�d�r" ��kt�s�n� veren kurallar 
				{
					// E�er (geometri az d�zg�n ve (KAKS y�ksek ve alan b�y�k)) ise MAL�YET PAHALIDIR.
					SingleRule(Conjunction(&(*rootTerms)[2][1], new Conjunction(&(*rootTerms)[3][2], &(*rootTerms)[0][2], AND), AND), 1),

					// E�er (geometri az d�zg�n ve (KAKS orta ve (alan orta ve konum merkezi))) ise MAL�YET PAHALIDIR.
					SingleRule(Conjunction(&(*rootTerms)[2][1], new Conjunction(&(*rootTerms)[3][1], new Conjunction(&(*rootTerms)[0][1], &(*rootTerms)[1][2], AND), AND), AND), 1),

					// E�er (geometri d�zg�n ve (((KAKS d���k) de�il) ve alan b�y�k)) ise MAL�YET PAHALIDIR.
					SingleRule(Conjunction(&(*rootTerms)[2][2], new Conjunction(new NotTerm(&(*rootTerms)[3][0]), &(*rootTerms)[0][2], AND), AND), 1)
				}
			}
		},
		RuleMergingType::MAX
	);

	return result;
}


void testWithSampleValues()
{
	//vector<float> inputs { 635, 700, 350, 1 };
	vector<float> inputs(4);									// [0]: Alan, [1]: Konum, [2]: Geometrik �ekil, [3]: KAKS
	vector<float> outputs(1);									// [0]: Maliyet
	vector<vector<Trapezoid>> inputSets = generateInputSets();
	vector<vector<TrapezoidTemplate>> outputSets = generateOutputSets();
	RuleBase ruleBase = generateRuleBase();

	FuzzyLogicUnit flu = FuzzyLogicUnit(&inputs, &outputs, &inputSets, &outputSets, new MamdaniInference(ruleBase));

	inputs = { 250, 700, 350, 1 };
	flu.evaluate();
	cout << "Alan: 250, Konum: 700, Geometrik Sekil: 350, KAKS: 1     --------> Maliyet: " << outputs[0] << endl;

	inputs = { 90, 240, 150, 0.7 };
	flu.evaluate();
	cout << "Alan: 90,  Konum: 240, Geometrik Sekil: 150, KAKS: 0.7   --------> Maliyet: " << outputs[0] << endl;

	inputs = { 425, 345, 565, 0.3 };
	flu.evaluate();
	cout << "Alan: 425, Konum: 345, Geometrik Sekil: 565, KAKS: 0.3   --------> Maliyet: " << outputs[0] << endl;

	inputs = { 522, 815, 380, 1.6 };
	flu.evaluate();
	cout << "Alan: 522, Konum: 815, Geometrik Sekil: 380, KAKS: 1.6   --------> Maliyet: " << outputs[0] << endl;

	inputs = { 410, 530, 190, 0.9 };
	flu.evaluate();
	cout << "Alan: 410, Konum: 530, Geometrik Sekil: 190, KAKS: 0.9   --------> Maliyet: " << outputs[0] << endl;

	flu.terminateContext();
}

void testWithUserGivenValues()
{
	vector<float> inputs(4);						// [0]: Alan, [1]: Konum, [2]: Geometrik �ekil: [3]: KAKS
	vector<float> outputs(1);						// [0]: Maliyet
	vector<vector<Trapezoid>> inputSets = generateInputSets();
	vector<vector<TrapezoidTemplate>> outputSets = generateOutputSets();
	RuleBase ruleBase = generateRuleBase();


	FuzzyLogicUnit flu = FuzzyLogicUnit(&inputs, &outputs, &inputSets, &outputSets, new MamdaniInference(ruleBase));

	int exitKey = 1;

	while (exitKey != 0)
	{
		cout << "Alani Girin (m�): ";
		cin >> inputs[0];

		cout << "Konum Ozelliklerini Girin (0 ile 1000 arasi puan, 0: merkezi degil, 1000 merkezi): ";
		cin >> inputs[1];

		cout << "Geometrik Sekil Ozelligini Girin (0 ile 740 arasi puan, 0: duzgun degil, 740 duzgun ): ";
		cin >> inputs[2];

		cout << "KAKS Degerini Girin (0 ile 2 arasi): ";
		cin >> inputs[3];
		cout << endl;

		flu.evaluate();

		cout << "Maliyet: " << outputs[0] << endl;

		cout << "Programdan Cikmak Istiyorsaniz Programa 0 Girin...";
		cin >> exitKey;
	}

	flu.terminateContext();
}


int main()
{
	cout << "Hazir degerlerle hesaplama yap: 0" << endl << "Kullanici girisi degerlerle hesaplama yap: 1" << endl;
	int f;
	cin >> f;
	system("cls");

	if (f == 0) 
	{
		testWithSampleValues();
	}
	else if(f == 1)
	{
		testWithUserGivenValues();
	}

	return 0;
}