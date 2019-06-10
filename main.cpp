#include <iostream>
#include <random>
#include <chrono>
#include <vector>

#include "function.h"


using functionPtr = Vector2D (*) (std::size_t index);

////////////////////////////////////////////////////////////
std::vector<std::size_t> generateRandomSequence(const std::size_t numberOfElements)
{
	std::vector<std::size_t> sequence;
	sequence.reserve(numberOfElements);
	const std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 randomGenerator(seed);
	std::uniform_int_distribution<std::size_t> random3(0,3);

	for (std::size_t i = 0; i < numberOfElements; ++i)
	{
		const std::size_t random = random3(randomGenerator);
		sequence.push_back(random);
	}
	return sequence; // {0, 0, 2, 2, 0, 1, 2, 3, 2, 1, 1, 3}
}

////////////////////////////////////////////////////////////
std::vector<std::size_t> generatePredictableSequence_1(const std::size_t numberOfElements)
{
	std::vector<std::size_t> sequence;
	sequence.reserve(numberOfElements);

	for (std::size_t i = 0; i < numberOfElements; ++i)
	{
		sequence.push_back(i%4);
	}
	return sequence;  // {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3}
}

////////////////////////////////////////////////////////////
std::vector<std::size_t> generatePredictableSequence_2(const std::size_t numberOfElements)
{
	std::vector<std::size_t> sequence;
	sequence.reserve(numberOfElements);
	const std::size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 randomGenerator(seed);
	std::uniform_int_distribution<std::size_t> random3(0,3);

	const std::size_t predictableNumber = random3(randomGenerator);

	for (std::size_t i = 0; i < numberOfElements; ++i)
	{
		sequence.push_back(predictableNumber);
	}
	return sequence;  // {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}
}

////////////////////////////////////////////////////////////
void printNElements(const std::vector<std::size_t>& sequence, std::size_t n)
{
	std::cout << "sequence: ";
	for (std::size_t elem : sequence)
	{
		if (n == 0) break;
		std::cout << elem << ", ";
		--n;
	}
	if (n == 0) std::cout << "...";
	std::cout << "\n";
}

////////////////////////////////////////////////////////////
void testFunction(functionPtr testedFunction)
{
	for (int i = 0; i < 4; ++i)
	{
		Vector2D result = testedFunction(i);
		std::cout << "f(" << i << ") = " << "(" << result.x << ", " << result.y << ")\n";
	}	
}

////////////////////////////////////////////////////////////
void benchmarkWithSequence(const std::vector<std::size_t>& sequence, functionPtr testedFunction)
{
	Vector2D result{0,0};

	auto start = std::chrono::steady_clock::now();
	for (auto index : sequence)
	{
		result += testedFunction(index);
	}
	auto end = std::chrono::steady_clock::now();

	std::cout << "Result: " << "(" << result.x << "," << result.y << "), " 
			  << "Elapsed time: "
			  << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
			  << " ms\n";
}

////////////////////////////////////////////////////////////
int main()
{
	std::cout << "Branch version: \n";
	testFunction(getPointBranch);
	std::cout << "-------------------------------------\n";
	std::cout << "BranchFree version: \n";
	testFunction(getPointBranchFree);
	std::cout << "\n";

	const std::size_t numberOfIterations = 3 * 10000000;
	const int numberOfRepetition = 5;
	std::vector<std::size_t> sequence;

	std::cout << "SIZE OF SEQUENCE: " << numberOfIterations << "\n";	
	sequence = generateRandomSequence(numberOfIterations);
	std::cout << "RANDOM SEQUENCE BENCHMARK TO COMPLICATE A CPU BRANCH PREDICTION\n";
	std::cout << "===============================================================\n";
	printNElements(sequence, 15);
	std::cout << "Branch version:\n";
	for (int i = 0; i < numberOfRepetition; ++i)
	{
		benchmarkWithSequence(sequence, getPointBranch);
	}

	std::cout << "-------------------------------------\n";

	std::cout << "Branch free version:\n";
	for (int i = 0; i < numberOfRepetition; ++i)
	{
		//benchmarkWithSequence(sequenceRandom1, getPointBranch);
		benchmarkWithSequence(sequence, getPointBranchFree);
	}
	std::cout << "===============================================================\n";
	std::cout << "\n";


	std::cout << "PREDICTABLE SEQUENCE BENCHMARK TO SIMPLIFY A CPU BRANCH PREDICTION\n";
	std::cout << "==================================================================\n";
	sequence = generatePredictableSequence_1(numberOfIterations);
	printNElements(sequence, 15);
	std::cout << "Predictable sequence with pattern. Branch version:\n";
	for (int i = 0; i < numberOfRepetition; ++i)
	{
		benchmarkWithSequence(sequence, getPointBranch);
	}

	std::cout << "-------------------------------------\n";

	std::cout << "Predictable sequence with pattern. Branch free version:\n";
	for (int i = 0; i < numberOfRepetition; ++i)
	{
		benchmarkWithSequence(sequence, getPointBranchFree);
	}

	std::cout << "-------------------------------------\n";
	sequence = generatePredictableSequence_2(numberOfIterations);
	printNElements(sequence, 15);
	std::cout << "Predictable sequence fill with same number. Branch version:\n";
	for (int i = 0; i < numberOfRepetition; ++i)
	{
		benchmarkWithSequence(sequence, getPointBranch);
	}

	std::cout << "-------------------------------------\n";

	std::cout << "Predictable sequence fill with same number. Branch free version:\n";
	for (int i = 0; i < numberOfRepetition; ++i)
	{
		benchmarkWithSequence(sequence, getPointBranchFree);
	}
	std::cout << "==================================================================\n";

	return 0;
}