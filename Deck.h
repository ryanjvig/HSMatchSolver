#pragma once
#ifndef DECK_H
#define DECK_H
#include <vector>
#include <string>
struct Deck {
	Deck() {}
	Deck(std::string d, double w, char c) :
		name(d), weight(w), type(c) {}
	bool operator()(Deck& lhs, Deck& rhs) {
		return lhs.type == rhs.type ?
			lhs.name < rhs.name : lhs.type < rhs.type;
	}
	bool operator<(Deck& other) {
		return ewr > other.ewr;
	}

	std::vector<double> matchups;
	std::string name;
	double ewr;
	double weight;
	char type;
};
#endif
