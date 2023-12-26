#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <set>
#include <fstream>

class dfa
{
private:
	int countStates;
	std::vector<char> alphabet;
	std::vector<int> finalStates;
	std::vector<std::vector<int>> transitFunction;
	int getState(int state, char c);
public:
	dfa();

	dfa(int countStates, std::vector<char> alphabet, std::vector<int> finalStates, std::vector<std::vector<int>> transitFunction);

	bool isAccept(std::string s, std::string& path);

	void initialize(std::string name_file);

};

