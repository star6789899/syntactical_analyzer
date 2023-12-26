#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct rule {
	string first;
	vector<string> second;
};

struct Node_tree {
	string info = "";
	Node_tree* parent;
	vector<Node_tree*> childrens;
	bool flag;

	Node_tree() {};
	Node_tree(string rule) {
		info = rule;
		parent = NULL;
		flag = false;
	};
};

class Tree_parse
{


	vector<rule> rules;
public: 
	Node_tree* root = new Node_tree;  
	void set_rules(vector<vector<string>> new_rules) {
		rules.resize(new_rules.size());
		for (int i = 0; i < new_rules.size(); i++) {
			rules[i].first = new_rules[i][0]; 
			for (int j = 1; j < new_rules[i].size(); j++) { 
				rules[i].second.push_back(new_rules[i][j]);
			} 
		}
	}
	Tree_parse() {};  
	Tree_parse(vector<rule> r) {
		rules = r;
	};
	//‘ункци€ дл€ создани€ нового узла;
	Node_tree* create_new_node(string rule) {
		Node_tree* new_node = new Node_tree; 
		new_node->parent = NULL;  
		new_node->info = rule;
		new_node->flag = false;
		return new_node;
	}
	void clear_node(Node_tree* n) {
		for (int i = 0; i < n->childrens.size(); i++) {
			n->childrens[i]->flag = false;
			n->childrens[i]->info = "";
			n->childrens[i]->parent = NULL;
		}
		n->parent = NULL; 
		n->info = "";
		n->childrens.clear();
		n->childrens.shrink_to_fit();
	}
	//ќпредел€ем, что перед нами терминал или нетерминал; 
	//≈сли нетерминал выдаст 0, если терминал, то выдаст 1;
	int is_terminal(string s) {
		string alph = "abcdefghijklmnopqrstuvwxyz";
		string Alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		string Digit = "0123456789";

		for (int i = 0; i < alph.size(); i++) {
			if (s[0] == alph[i]) {
				return 1;
			}
		}

		//если заглавна€, строчна€ буквы, разделители;
		if (s.size() == 1) {
			for (int i = 0; i < Alph.size(); i++) {
				if (s[0] == Alph[i]) {
					return 1;
				}
			}
			for (int i = 0; i < Digit.size(); i++) {
				if (s[0] == Digit[i]) {
					return 1;
				}
			}
			if ((s[0] == ';') || (s[0] == ':') || (s[0] == ',') || (s[0] == '+') || (s[0] == '-')
				|| (s[0] == '(') || (s[0] == ')')) {
				return 1;
			}
		}

		if (s == ":=") {
			return 1;
		}

		return 0;
	}
	bool check_all_terminals(vector<Node_tree*> &n) {
		for (int i = 0; i < n.size(); i++) {
			if (n[i]->flag == false) {
				return false;
			}
		}
		return true;
	}
	int ind_in_rules = 0;
	//проверить а корень подходить дл€ ребенка;
	Node_tree* build_tree(Node_tree* root) {
	//проверить а корень подходить дл€ ребенка;
		//тут проверим, а €вл€етс€ ли info  терминалом, если так, то flag мен€ем на true; возвращаем ничего
		if (is_terminal(root->info)) {
			root->flag = true;
			return root;
		}
	
		for (int i = rules[ind_in_rules].second.size() - 1; i >= 0; i--) {
			Node_tree* new_child = new Node_tree(rules[ind_in_rules].second[i]);
			root->childrens.push_back(new_child);
		} 
		 
		for (int i = 0; i < root->childrens.size(); i++) {
			if (is_terminal(root->childrens[i]->info)) {
				root->childrens[i]->flag = true;
			} 
		} 
		 
		if (check_all_terminals(root->childrens)) {
			root->flag = true;
			return root; 
		} 
		 
		//проверить а корень подходит дл€ ребенка;
		for (int i = 0; i < root->childrens.size(); i++) {
			//проверить а корень подходит дл€ ребенка;
			if (root->childrens[i]->flag == false) {
				ind_in_rules++;
				build_tree(root->childrens[i]);
			}
		} 
		 
	} 
	void print_Tree(Node_tree* tree_node, int depth = 0) {
		for (int i = 0; i < depth; i++) { 
			cout << "  "; 
		} 
		cout << tree_node->info << "\n";  
		for (int i = tree_node->childrens.size() - 1; i >= 0; i--) {  
			print_Tree(tree_node->childrens[i], depth + 1);  
		} 
	} 
	 
}; 
