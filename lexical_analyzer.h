#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "syntactical_analyzer.h"
#include "dfaa.h"
#include "Hash_table.h"
#include "Tree_parse.h"
#include "syntactical_analyzer.h"
#include "semantical_analizator.h"

using namespace std; 
#define _CRT_SECURE_NO_WARNINGS

class lexical_analyzer
{
private:
	//Нужно для прочтения текста в файле;

public:

	lexical_analyzer() {}
	//дает следующее слово из файла;
	string f(ifstream& file);
	//	Определяем к какому типу относится лексема
	int get_last_state_from_path(string& path) {
		int i = path.size() - 1;
		while (path[i] == ' ') {
			i--;
		}

		string str_for_state = "";
		while (path[i] != ' ') {
			str_for_state += path[i];

			i--;
		}

		reverse(str_for_state.begin(), str_for_state.end());

		return stoi(str_for_state);
	}
	void my_insert(int &index_states, syntactical_analyzer &syn_analyzer, 
		Hash_table& table, int state, int st,
		string type_lexeme, string lexeme) {
		if (state == st) {
			//добавляем в хэш таблицу;
			syn_analyzer.symbol_analyze(type_lexeme, lexeme, index_states);
			table.ht_insert(type_lexeme, lexeme);

		}
	}
	
	int analyze(string path_input_file) {
		ifstream file;
		file.open(path_input_file);
		//ofstream fout("Error.txt");

		dfa my;
		my.initialize("dka_final.txt");

		Hash_table table;
		table.create_table(991);

		syntactical_analyzer syn_analyzer;
		vector<string> Digits;
		vector<string> ident;
		syn_analyzer.Start_Initialization();
		int index_states = 1;
		string word = f(file);
		//token buf;
		string path;
		string all6; 
		all6 += word; 
		while (!file.eof() || word.size() != 0)
		{
			if (my.isAccept(word, path)) {

				int state = get_last_state_from_path(path);
				//в нашей грамматике 7 ключевых слов типа integer var и тд; ключевые слова 

				my_insert(index_states, syn_analyzer, table, state, 9, "key_word", word);
				my_insert(index_states, syn_analyzer, table, state, 14, "key_word", word);
				my_insert(index_states, syn_analyzer, table, state, 17, "key_word", word);
				my_insert(index_states, syn_analyzer, table, state, 24, "key_word", word);
				my_insert(index_states, syn_analyzer, table, state, 28, "key_word", word);
				my_insert(index_states, syn_analyzer, table, state, 30, "key_word", word);
				my_insert(index_states, syn_analyzer, table, state, 33, "key_word", word); //ключевое слово;

				my_insert(index_states, syn_analyzer, table, state, 34, "operator", word);
				my_insert(index_states, syn_analyzer, table, state, 35, "operator", word);
				my_insert(index_states, syn_analyzer, table, state, 39, "operator", word);
				my_insert(index_states, syn_analyzer, table, state, 40, "operator", word);

				my_insert(index_states, syn_analyzer, table, state, 36, "separator", word);
				my_insert(index_states, syn_analyzer, table, state, 37, "separator", word);
				my_insert(index_states, syn_analyzer, table, state, 38, "separator", word);
				my_insert(index_states, syn_analyzer, table, state, 43, "separator", word);//разделтители;

				if (((state >= 1) && (state <= 8)) || ((state >= 10) && (state <= 13)) ||
					((state >= 15) && (state <= 16)) || ((state >= 18) && (state <= 23))
					|| ((state >= 25) && (state <= 27)) || (state == 29) || ((state >= 31) && (state <= 32))) {
					state = 41;
				}
				my_insert(index_states, syn_analyzer, table, state, 41, "identifier", word); //идентификатор;
				if (state == 41) {
					ident.push_back(word);
				}
				my_insert(index_states, syn_analyzer, table, state, 42, "constant", word);
				if (state == 42) {
					Digits.push_back(word);
				}
			}
			else {
				//выдаем сообщение об ошибке с файлом с ошибками;
				my_insert(index_states, syn_analyzer, table, -1, -1, "Error", word);
				//fout << word << "\n";
			}
			path.shrink_to_fit();
			path.clear();
			word.clear();
			word = f(file);
			all6 += " " + word ;
		
		}
		file.close();
		//fout.close();
		table.output();
		if (syn_analyzer.check(Digits)) {
			cout << "Accept!";
			
			vector<vector<string> > t;  
			syn_analyzer.rules_for_tree(t); 
			syn_analyzer.print_parse();
			Tree_parse tree;
			tree.root->info = t[0][0];
			/*for (int i = 1; i < t[0].size(); i++) {
				tree.root->childrens.push_back(tree.create_new_node(t[0][i]));
			}*/
			tree.set_rules(t);
			tree.build_tree(tree.root); 
			ofstream fout;
			fout.open("Tree_parse.txt");
			tree.print_Tree(tree.root,  0);
			fout.close();
			//создаем объект семантического анализатора, в который передаем дерево 
			semantical_analizator s; 
			cout << "\n\n";
			string result = s.Procedure(tree.root);
			cout << result;
		}
		else {
			cout << "Error!";
		}

		exit(0);
	}

	~lexical_analyzer() {}
};

