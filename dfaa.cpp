#include "dfaa.h"
#include <string>
#include<fstream>
#include <iostream>
using namespace std;
dfa::dfa() {}

dfa::dfa(int countStates, std::vector<char> alphabet, std::vector<int> finalStates, std::vector<std::vector<int>> transitFunction)
{
	this->countStates = countStates;
	this->alphabet = alphabet;
	this->finalStates = finalStates;
	this->transitFunction.resize(countStates);
	for (int i = 0; i < countStates; i++)
		this->transitFunction[i] = transitFunction[i];
}
//Инициализация ДКА 
void dfa::initialize(string name_file) {
	ifstream fin(name_file);

	this->countStates;
	this->alphabet;
	this->finalStates;
	this->transitFunction;
	if (fin.is_open()) {
		string line;

		getline(fin, line);
		this->countStates = stol(line);

		getline(fin, line);
		for (char c : line) {
			if (c != ' ')
				this->alphabet.push_back(c);
		}

		getline(fin, line);
		string buff = "";
		for (char c : line) {
			if (c != ' ')
				buff += c;
			else {
				this->finalStates.push_back(stoi(buff));
				buff = "";
			}
		}

		this->finalStates.push_back(stoi(buff));
		this->transitFunction.resize(this->countStates, vector<int>(this->alphabet.size()));
		for (int i = 0; i < this->countStates; ++i) {
			for (int j = 0; j < this->alphabet.size(); ++j) {
				fin >> this->transitFunction[i][j];
			}
		}
	}
	else {
		cout << "File not found! don't initilized DKA!\n";
		exit(0);
	}
	fin.close();
}

// Находит состояние автомата по текущему состоянию state и входному символу c
int dfa::getState(int state, char c)
{
	int i = 0;
	while (alphabet[i] != c)
	{
		i++;
		if (i == alphabet.size()) {
			return -2;
			//throw std::exception("Wrong symbol");
		}
	}
	return transitFunction[state][i];
}

// Проверяет, допускается ли строка s автоматом
bool dfa::isAccept(std::string s, std::string& path)
{
	int state = 0;

	path = "0 ";
	for (char c : s)
	{
		state = getState(state, c);
		if (state == -1) {
			return false;
		}
		if (state == -2) {
			return false;
		}
		path += std::to_string(state) + " ";
	}
	if (std::find(finalStates.begin(), finalStates.end(), state) != finalStates.end())
		return true;
	return false;
}