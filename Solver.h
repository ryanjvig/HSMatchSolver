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
	// custom comparators for sorting
	struct CompBO3 {
		bool operator()(Lineup& lhs, Lineup& rhs) {
			return lhs.bo3_winrate > rhs.bo3_winrate;
		}
	};
	struct CompBO5 {
		bool operator()(Lineup& lhs, Lineup& rhs) {
			return lhs.bo5_winrate > rhs.bo5_winrate;
		}
	};
	// find winrate for best of three match
	double calculate_winrate_bo3(Lineup& lhs, Lineup& rhs);
	// find winrate for best of five match
	double calculate_winrate_bo5(Lineup& lhs, Lineup& rhs);
	// markov chain solver for main winrate calculation
	void markov_solve_bo3(int LD1, int LD2, int RD1, int RD2, bool lhs1W, bool rhs1W,
		bool lhs2W, bool rhs2W, double cur_prob, double& winrate);
	void markov_solve_bo5(int LD1, int LD2, int LD3, int RD1, int RD2, int RD3, 
		bool lhs1W, bool lhs2W, bool lhs3W, bool rhs1W, bool rhs2W, bool rhs3W,
		double cur_prob, double& winrate);
private:
	int num_decks = -1;
	std::vector<Deck> decks;
	std::vector<Lineup> lineups;
};
#endif
