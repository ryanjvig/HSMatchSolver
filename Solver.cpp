#include "Solver.h"

// combinations helper (credit to geeksforgeeks)
int fact(int n)
{
	int res = 1;
	for (int i = 2; i <= n; i++)
		res = res * i;
	return res;
}
int nCr(int n, int r)
{
	return fact(n) / (fact(r) * fact(n - r));
}

Solver::Solver() {
	std::cin >> num_decks;
	decks.resize(num_decks);
	std::set<char> classes;
	for (int i = 0; i < num_decks; ++i) {
		// ignore '\n'
		std::cin.ignore();
		std::string name;
		char type, subtype;
		double weight, winrate;
		std::getline(std::cin, name, ',');
		std::cin >> weight >> type;
		decks[i] = Deck(name, weight, type);
		decks[i].matchups.reserve(num_decks);
		for (int j = 0; j < num_decks; ++j) {
			std::cin >> winrate;
			decks[i].matchups.push_back(winrate);
		}
		classes.insert(type);
	}
	double deck_weight = 0;
	// calculate deck ewr
	for (int i = 0; i < num_decks; ++i) {
		deck_weight += decks[i].weight;
		double wr_sum = 0;
		double temp = 0;
		for (int j = 0; j < num_decks; ++j) {
			wr_sum += decks[i].matchups[j] * decks[j].weight;
			temp += decks[i].matchups[j];
		}
		decks[i].ewr = wr_sum;
	}

	// # of unique lineups is nCr
	// where n is # of classes and r is decks in lineup
	lineups.reserve(nCr((int)classes.size(), 4));
	for (int i = 0; i < num_decks; ++i) {
		for (int j = i + 1; j < num_decks; ++j) {
			if (decks[j].type == decks[i].type) continue;
			for (int k = j + 1; k < num_decks; ++k) {
				if (decks[k].type == decks[j].type) continue;
				for (int l = k + 1; l < num_decks; ++l) {
					if (decks[l].type == decks[k].type) continue;
					lineups.push_back(Lineup(i, j, k, l));
				}
			}
		}
	}
	for (auto& i : lineups) {
		i.ban_order.reserve(num_decks);
		for (int j = 0; j < num_decks; ++j) {
			// average winrate against all 4 decks
			double winrate = (decks[i.deck1].matchups[j] + decks[i.deck2].matchups[j]
				+ decks[i.deck3].matchups[j] + decks[i.deck4].matchups[j]) / 4;
			i.ban_order.push_back({ winrate, j });
		}
		// will ban ascending by average winrate against deck
		std::sort(i.ban_order.begin(), i.ban_order.end());
	}
	// total_wr = 0;
	for (auto& i : lineups) {
		for (auto& j : lineups) {
			i.bo5_winrate += calculate_winrate_bo5(i, j);
		}
		i.bo5_winrate /= lineups.size();

	}
	
	std::sort(lineups.begin(), lineups.end(), CompBO5{});
	// output results
	std::cout << "Best of 5 Lineup Rankings (Unweighted):\n";
	for (size_t i = 0; i < lineups.size(); ++i) {
		std::cout << i + 1 << ". " << decks[lineups[i].deck1].name << '\\' << decks[lineups[i].deck2].name
			<< '\\' << decks[lineups[i].deck3].name << '\\' << decks[lineups[i].deck4].name << '\n'
			<< "Winrate: " << lineups[i].bo5_winrate << '\n';
	}

	// repeat for best of three format
	lineups.clear();
	lineups.reserve(nCr((int)classes.size(), 3));
	for (int i = 0; i < num_decks; ++i) {
		for (int j = i + 1; j < num_decks; ++j) {
			if (decks[j].type == decks[i].type) continue;
			for (int k = j; k < num_decks; ++k) {
				if (decks[k].type == decks[j].type) continue;
				lineups.push_back(Lineup(i, j, k));
			}
		}
	}
	for (auto& i : lineups) {
		i.ban_order.reserve(num_decks);
		for (int j = 0; j < num_decks; ++j) {
			// average winrate against all 3 decks
			double winrate = (decks[i.deck1].matchups[j] + decks[i.deck2].matchups[j]
				+ decks[i.deck3].matchups[j]) / 3;
			i.ban_order.push_back({ winrate, j });
		}
		// will ban ascending by average winrate against deck
		std::sort(i.ban_order.begin(), i.ban_order.end());
	}
	// double total_wr = 0.0;
	for (auto& i : lineups) {
		for (auto& j : lineups) {
			i.bo3_winrate += calculate_winrate_bo3(i, j);
		}
		i.bo3_winrate /= lineups.size();
		// total_wr += i.bo3_winrate;
	}

	
	std::sort(lineups.begin(), lineups.end(), CompBO3{});
	// output best of three results
	std::cout << "\nBest of 3 Lineup Rankings (Unweighted):\n";
	for (size_t i = 0; i < lineups.size(); ++i) {
		std::cout << i + 1 << ". " << decks[lineups[i].deck1].name << '\\' << decks[lineups[i].deck2].name
			<< '\\' << decks[lineups[i].deck3].name << '\n'
			<< "Winrate: " << lineups[i].bo3_winrate << '\n';
	}


	// don't sort until here to maintain indices for lineups
	std::sort(decks.begin(), decks.end());
	// output weighted deck results
	std::cout << "\nIndividual Deck Rankings (Weighted):\n";
	for (int i = 0; i < num_decks; ++i) {
		std::cout << i + 1 << ". " << decks[i].name << "\nWinrate: " << decks[i].ewr << '\n';
	}
}

double Solver::calculate_winrate_bo3(Lineup& lhs, Lineup& rhs) {
	// find bans
	int lhs_ban = -1;
	for (auto& i : lhs.ban_order) {
		if (rhs.deck1 == i.second || rhs.deck2 == i.second || rhs.deck3 == i.second) {
			lhs_ban = i.second;
			break;
		}
	}
	int rhs_ban = -1;
	for (auto& i : rhs.ban_order) {
		if (lhs.deck1 == i.second || lhs.deck2 == i.second || lhs.deck3 == i.second) {
			rhs_ban = i.second;
			break;
		}
	}
	assert(lhs_ban != -1 || rhs_ban != -1);
	int lhs1, lhs2, rhs1, rhs2;
	if (lhs_ban == rhs.deck1) {
		rhs1 = rhs.deck2;
		rhs2 = rhs.deck3;
	}
	else if (lhs_ban == rhs.deck2) {
		rhs1 = rhs.deck3;
		rhs2 = rhs.deck1;
	}
	else {
		rhs1 = rhs.deck1;
		rhs2 = rhs.deck2;
	}

	if (rhs_ban == lhs.deck1) {
		lhs1 = lhs.deck2;
		lhs2 = lhs.deck3;
	}
	else if (rhs_ban == lhs.deck2) {
		lhs1 = lhs.deck3;
		lhs2 = lhs.deck1;
	}
	else {
		lhs1 = lhs.deck1;
		lhs2 = lhs.deck2;
	}
	double winrate = 0.0;
	// run markov chain to find winrate
	markov_solve_bo3(lhs1, lhs2, rhs1, rhs2, false, false, false, false, 1.0, winrate);
	return winrate;
}

double Solver::calculate_winrate_bo5(Lineup& lhs, Lineup& rhs) {
	// find bans
	int lhs_ban = -1;
	for (auto& i : lhs.ban_order) {
		if (rhs.deck1 == i.second || rhs.deck2 == i.second || rhs.deck3 == i.second || rhs.deck4 == i.second) {
			lhs_ban = i.second;
			break;
		}
	}
	int rhs_ban = -1;
	for (auto& i : rhs.ban_order) {
		if (lhs.deck1 == i.second || lhs.deck2 == i.second || lhs.deck3 == i.second || rhs.deck4 == i.second) {
			rhs_ban = i.second;
			break;
		}
	}
	assert(lhs_ban != -1 || rhs_ban != -1);
	int lhs1, lhs2, lhs3, rhs1, rhs2, rhs3;
	if (lhs_ban == rhs.deck1) {
		rhs1 = rhs.deck2;
		rhs2 = rhs.deck3;
		rhs3 = rhs.deck4;
	}
	else if (lhs_ban == rhs.deck2) {
		rhs1 = rhs.deck3;
		rhs2 = rhs.deck4;
		rhs3 = rhs.deck1;
	}
	else if (lhs_ban == rhs.deck3) {
		rhs1 = rhs.deck4;
		rhs2 = rhs.deck1;
		rhs3 = rhs.deck2;
	}
	else {
		rhs1 = rhs.deck1;
		rhs2 = rhs.deck2;
		rhs3 = rhs.deck3;
	}

	if (rhs_ban == lhs.deck1) {
		lhs1 = lhs.deck2;
		lhs2 = lhs.deck3;
		lhs3 = lhs.deck4;
	}
	else if (rhs_ban == lhs.deck2) {
		lhs1 = lhs.deck3;
		lhs2 = lhs.deck4;
		lhs3 = lhs.deck1;
	}
	else if (rhs_ban == lhs.deck3) {
		lhs1 = lhs.deck4;
		lhs2 = lhs.deck1;
		lhs3 = lhs.deck2;
	}
	else {
		lhs1 = lhs.deck1;
		lhs2 = lhs.deck2;
		lhs3 = lhs.deck3;
	}
	double winrate = 0.0;
	// run markov chain to find winrate
	markov_solve_bo5(lhs1, lhs2, lhs3, rhs1, rhs2, rhs3, false, false, false, false, false, false, 1.0, winrate);
	return winrate;
}
// absorbing markov chain for best of three match
void Solver::markov_solve_bo3(int LD1, int LD2, int RD1, int RD2, bool lhs1W, bool rhs1W,
	bool lhs2W, bool rhs2W, double cur_prob, double& winrate) {
	if (rhs1W && rhs2W) return;
	if (lhs1W && lhs2W) {
		winrate += cur_prob;
		return;
	}
	double choice_factor = (2 - (lhs1W + lhs2W)) * (2 - (rhs1W + rhs2W));
	if (!lhs1W) {
		if (!rhs1W) {
			markov_solve_bo3(LD1, LD2, RD1, RD2, true, rhs1W, lhs2W, rhs2W, (cur_prob * decks[LD1].matchups[RD1]) / choice_factor, winrate);
			markov_solve_bo3(LD1, LD2, RD1, RD2, lhs1W, true, lhs2W, rhs2W, (cur_prob * decks[RD1].matchups[LD1]) / choice_factor, winrate);
		}
		if (!rhs2W) {
			markov_solve_bo3(LD1, LD2, RD1, RD2, true, rhs1W, lhs2W, rhs2W, (cur_prob * decks[LD1].matchups[RD2]) / choice_factor, winrate);
			markov_solve_bo3(LD1, LD2, RD1, RD2, lhs1W, rhs1W, lhs2W, true, (cur_prob * decks[RD2].matchups[LD1]) / choice_factor, winrate);
		}
	}
	if (!lhs2W) {
		if (!rhs1W) {
			markov_solve_bo3(LD1, LD2, RD1, RD2, lhs1W, rhs1W, true, rhs2W, (cur_prob * decks[LD2].matchups[RD1]) / choice_factor, winrate);
			markov_solve_bo3(LD1, LD2, RD1, RD2, lhs1W, true, lhs2W, rhs2W, (cur_prob * decks[RD1].matchups[LD2]) / choice_factor, winrate);
		}
		if (!rhs2W) {
			markov_solve_bo3(LD1, LD2, RD1, RD2, lhs1W, rhs1W, true, rhs2W, (cur_prob * decks[LD2].matchups[RD2]) / choice_factor, winrate);
			markov_solve_bo3(LD1, LD2, RD1, RD2, lhs1W, rhs1W, lhs2W, true, (cur_prob * decks[RD2].matchups[LD2]) / choice_factor, winrate);
		}
	}
}
// absorbing markov chain for best of five match
void Solver::markov_solve_bo5(int LD1, int LD2, int LD3, int RD1, int RD2, int RD3,
	bool lhs1W, bool lhs2W, bool lhs3W, bool rhs1W, bool rhs2W, bool rhs3W,
	double cur_prob, double& winrate) {
	if (rhs1W && rhs2W && rhs3W) return;
	if (lhs1W && lhs2W && lhs3W) {
		winrate += cur_prob;
		return;
	}
	double choice_factor = (3 - (lhs1W + lhs2W + lhs3W)) * (3 - (rhs1W + rhs2W + rhs3W));
	if (!lhs1W) {
		if (!rhs1W) {
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, true, lhs2W, lhs3W, rhs1W, rhs2W, rhs3W,
				(cur_prob * decks[LD1].matchups[RD1]) / choice_factor, winrate);
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, lhs2W, lhs3W, true, rhs2W, rhs3W,
				(cur_prob * decks[RD1].matchups[LD1]) / choice_factor, winrate);
		}
		if (!rhs2W) {
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, true, lhs2W, lhs3W, rhs1W, rhs2W, rhs3W,
				(cur_prob * decks[LD1].matchups[RD2]) / choice_factor, winrate);
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, lhs2W, lhs3W, rhs1W, true, rhs3W,
				(cur_prob * decks[RD2].matchups[LD1]) / choice_factor, winrate);
		}
		if (!rhs3W) {
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, true, lhs2W, lhs3W, rhs1W, rhs2W, rhs3W,
				(cur_prob * decks[LD1].matchups[RD3]) / choice_factor, winrate);
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, lhs2W, lhs3W, rhs1W, rhs2W, true,
				(cur_prob * decks[RD3].matchups[LD1]) / choice_factor, winrate);
		}
	}
	if (!lhs2W) {
		if (!rhs1W) {
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, true, lhs3W, rhs1W, rhs2W, rhs3W,
				(cur_prob * decks[LD2].matchups[RD1]) / choice_factor, winrate);
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, lhs2W, lhs3W, true, rhs2W, rhs3W,
				(cur_prob * decks[RD1].matchups[LD2]) / choice_factor, winrate);
		}
		if (!rhs2W) {
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, true, lhs3W, rhs1W, rhs2W, rhs3W,
				(cur_prob * decks[LD2].matchups[RD2]) / choice_factor, winrate);
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, lhs2W, lhs3W, rhs1W, true, rhs3W,
				(cur_prob * decks[RD2].matchups[LD2]) / choice_factor, winrate);
		}
		if (!rhs3W) {
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, true, lhs3W, rhs1W, rhs2W, rhs3W,
				(cur_prob * decks[LD2].matchups[RD3]) / choice_factor, winrate);
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, lhs2W, lhs3W, rhs1W, rhs2W, true,
				(cur_prob * decks[RD3].matchups[LD2]) / choice_factor, winrate);
		}
	}
	if (!lhs3W) {
		if (!rhs1W) {
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, lhs2W, true, rhs1W, rhs2W, rhs3W,
				(cur_prob * decks[LD3].matchups[RD1]) / choice_factor, winrate);
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, lhs2W, lhs3W, true, rhs2W, rhs3W,
				(cur_prob * decks[RD1].matchups[LD3]) / choice_factor, winrate);
		}
		if (!rhs2W) {
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, lhs2W, true, rhs1W, rhs2W, rhs3W,
				(cur_prob * decks[LD3].matchups[RD2]) / choice_factor, winrate);
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, lhs2W, lhs3W, rhs1W, true, rhs3W,
				(cur_prob * decks[RD2].matchups[LD3]) / choice_factor, winrate);
		}
		if (!rhs3W) {
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, lhs2W, true, rhs1W, rhs2W, rhs3W,
				(cur_prob * decks[LD3].matchups[RD3]) / choice_factor, winrate);
			markov_solve_bo5(LD1, LD2, LD3, RD1, RD2, RD3, lhs1W, lhs2W, lhs3W, rhs1W, rhs2W, true,
				(cur_prob * decks[RD3].matchups[LD3]) / choice_factor, winrate);
		}
	}
}
