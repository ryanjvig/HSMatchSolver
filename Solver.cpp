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
		for (int j = 0; j < num_decks; ++j) {
			wr_sum += decks[i].matchups[j] * decks[j].weight;
		}
		decks[i].ewr = wr_sum;
	}
	// std::cout << "Weighted winrate sum: " << total_wr << '\n';
	// std::cout << "Total deck weight: " << deck_weight << '\n';
	// number of unique lineups is nCr
	// where n is # of classes and r is decks in lineup
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
	double total_weight = 0;
	for (auto& i : lineups) {
		// calculate lineup weight (multiply by number of permutations
		// i.weight = (decks[i.deck1].weight * decks[i.deck2].weight * decks[i.deck3].weight);
		// total_weight += i.weight;
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
	// std::cout << "Total lineup weight: " << total_weight << '\n';
	for (auto& i : lineups) {
		for (auto& j : lineups) {
			i.winrate += calculate_winrate_bo3(i, j);
		}
	}
	std::sort(lineups.begin(), lineups.end());
	std::cout << "Lineup Rankings (Unweighted):\n";
	for (size_t i = 0; i < lineups.size(); ++i) {
		std::cout << i + 1 << ". " << decks[lineups[i].deck1].name << '\\' << decks[lineups[i].deck2].name
			<< '\\' << decks[lineups[i].deck3].name << '\n'
			<< "Winrate: " << lineups[i].winrate / lineups.size() << '\n';
	}
	// don't sort until the end to maintain indices for lineups
	std::sort(decks.begin(), decks.end());
	std::cout << "\nIndividual Rankings (Weighted):\n";
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

	// winrate is odds of 2-0 + odds of 2-1
	// 1-1 picks
	// 1v1 wr * 2v1 wr
	// +
	// ((1v1 wr * 2v1 lr) + (1v1 lr * 1v2 wr)) * 2v2 wr
	double winrate_11 = (decks[lhs1].matchups[rhs1] * decks[lhs2].matchups[rhs1])
		+ (((decks[lhs1].matchups[rhs1] * decks[rhs1].matchups[lhs2])+(decks[rhs1].matchups[lhs1] * decks[lhs1].matchups[rhs2]))
			* (decks[lhs2].matchups[rhs2]));
	// 1-2 picks
	// 1v2 wr * 2v2 wr
	// +
	// ((1v2 wr * 2v2 lr) + (1v2 lr * 1v1 wr)) * 2v1 wr
	double winrate_12 = (decks[lhs1].matchups[rhs2] * decks[lhs2].matchups[rhs2])
		+ (((decks[lhs1].matchups[rhs2] * decks[rhs2].matchups[lhs2]) + (decks[rhs2].matchups[lhs1] * decks[lhs1].matchups[rhs1]))
			* (decks[lhs2].matchups[rhs1]));
	// 2-2 picks
	// 2v2 wr * 2v1 wr
	// +
	// ((2v2 wr * 1v2 lr) + (2v2 lr * 2v1 wr)) * 1v1 wr
	double winrate_22 = (decks[lhs2].matchups[rhs2] * decks[lhs2].matchups[rhs1])
		+ (((decks[lhs2].matchups[rhs2] * decks[rhs2].matchups[lhs1]) + (decks[rhs2].matchups[lhs2] * decks[lhs2].matchups[rhs1]))
			* (decks[lhs1].matchups[rhs1]));


	return (winrate_11 + (2 * winrate_12) + winrate_22) / 4;
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
	// match win states:
	// 3-0, 3-1, 3-2
	// 3-0: p1 v p1, p2 vs p1/p2/p3, p3 vs p1/p2/p3, 
	// winrate is odds of 2-0 + odds of 2-1
	// 1-1 picks
	// 1v1 wr * 2v1 wr
	// +
	// ((1v1 wr * 2v1 lr) + (1v1 lr * 1v2 wr)) * 2v2 wr
	double winrate_11 = (decks[lhs1].matchups[rhs1] * decks[lhs2].matchups[rhs1])
		+ (((decks[lhs1].matchups[rhs1] * decks[rhs1].matchups[lhs2]) + (decks[rhs1].matchups[lhs1] * decks[lhs1].matchups[rhs2]))
			* (decks[lhs2].matchups[rhs2]));
	// 1-2 picks
	// 1v2 wr * 2v2 wr
	// +
	// ((1v2 wr * 2v2 lr) + (1v2 lr * 1v1 wr)) * 2v1 wr
	double winrate_12 = (decks[lhs1].matchups[rhs2] * decks[lhs2].matchups[rhs2])
		+ (((decks[lhs1].matchups[rhs2] * decks[rhs2].matchups[lhs2]) + (decks[rhs2].matchups[lhs1] * decks[lhs1].matchups[rhs1]))
			* (decks[lhs2].matchups[rhs1]));
	/*
	// 2-1 picks (always same wr as 1-2)
	// 2v1 wr * 1v1 wr
	// +
	// ((2v1 wr * 1v1 lr) + (2v1 lr * 2v2 wr)) * 1v2 wr
	double winrate_21 = (decks[lhs2].matchups[rhs1] * decks[lhs1].matchups[rhs1])
		+ (((decks[lhs2].matchups[rhs1] * decks[rhs1].matchups[lhs1]) + (decks[rhs1].matchups[lhs2] * decks[lhs2].matchups[rhs2]))
			* (decks[lhs1].matchups[rhs2]));
	*/
	// 2-2 picks
	// 2v2 wr * 2v1 wr
	// +
	// ((2v2 wr * 1v2 lr) + (2v2 lr * 2v1 wr)) * 1v1 wr
	double winrate_22 = (decks[lhs2].matchups[rhs2] * decks[lhs2].matchups[rhs1])
		+ (((decks[lhs2].matchups[rhs2] * decks[rhs2].matchups[lhs1]) + (decks[rhs2].matchups[lhs2] * decks[lhs2].matchups[rhs1]))
			* (decks[lhs1].matchups[rhs1]));


	return (winrate_11 + (2 * winrate_12) + winrate_22) / 4;
}