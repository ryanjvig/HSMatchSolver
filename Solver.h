#pragma once
#ifndef SOLVER_H
#define SOLVER_H
#include "Deck.h"
#include "Lineup.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <set>
#include <map>
#include <algorithm>
#include <utility>
#include <vector>
#include <string>
#include <unordered_map>
class Solver {
public:
	Solver();
	double calculate_winrate_bo3(Lineup& lhs, Lineup& rhs);
	double calculate_winrate_bo5(Lineup& lhs, Lineup& rhs);
private:
	int num_decks = -1;
	std::vector<Deck> decks;
	std::vector<Lineup> lineups;
};
#endif