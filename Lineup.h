#pragma once
#ifndef LINEUP_H
#define LINEUP_H
#include <vector>
#include <utility>
#include <cassert>
#include "Deck.h"
struct Lineup {
	Lineup(int one, int two, int three) :
		deck1(one), deck2(two), deck3(three) {}
	bool operator<(Lineup& other) {
		return winrate > other.winrate;
	}
	std::vector<std::pair<double, int>> ban_order;
	double weight = 0;
	double winrate = 0;
	int deck1;
	int deck2;
	int deck3;
	int deck4;
};
#endif