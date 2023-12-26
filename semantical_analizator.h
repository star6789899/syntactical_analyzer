#pragma once
#include <string>
#include <vector>
#include <set>
#include "Tree_parse.h"
using namespace std;

class semantical_analizator
{
private:
	set <int> unique_tags; //локальные метки;
	set<string> variables;
	int num_clb = 0;
	vector<string> labels;//работаю, как со стеком;

	int get_label() {

	}

	set<string> errors;

public:

	//а потом этот root_parse_tree передадим в функцию Procedure, в которой будет реализован перевод в постфиксную запись; вызов осуществляется в основной функции,
	//после построения дерева;
	semantical_analizator() {}

	//Procedure → Begin Descriptions Operators End
	string Procedure(Node_tree* root) {
		string begin = Begin(root->childrens[3]);
		string descriptions = Descriptions(root->childrens[2]);
		string operators = Operators(root->childrens[1]);
		this->output();
		return begin + descriptions + operators;
	}

	void output() {
		ofstream fout("semantic_errors.txt");
		if (errors.size() == 0) {
			fout << "Программа успешно завершилась."; 
		}
		else {
			for (string error : errors) {
				fout << error << "\n";  
			} 
		}
		fout.close();
		return;
	}

	//Begin → procedure ProcedureName ; begin
	string Begin(Node_tree* root) {
		string procedure_name = ProcedureName(root->childrens[2]);
		return procedure_name + " DECL\n";
	} 

	//ProcedureName → Id
	string ProcedureName(Node_tree* root) {
		string id = Id(root->childrens[0]);
		return id;
	}

	//Descriptions → var DescrList
	string Descriptions(Node_tree* root) {
		string descrlist = DescrList(root->childrens[0]);
		return descrlist;
	}

	//DescrList → Descr | Descr DescrList
	string DescrList(Node_tree* root) {
		if (root->childrens.size() == 1) {
			string descr = Descr(root->childrens[0]);
			return descr;
		}
		else {
			string descr = Descr(root->childrens[1]);
			string descrlist = DescrList(root->childrens[0]);
			return descr + descrlist;
		}
	}
	
	int Count(string varlist) {  
		int result = 0;
		for (int i = 0; i < varlist.size(); i++) {
			if (varlist[i] == ' ') {
				result++; 
			}
		}
		//количество пробелов равно количеству слов в списке;
		return result;
	} 

	//Descr → VarList : Type ;
	string Descr(Node_tree* root) {
		string type = Type(root->childrens[1]);
		string varlist = VarList(root->childrens[3]);
		int count_variables = Count(varlist) + 1;
		return type + " " + varlist + " " + to_string(count_variables) + " DECL\n";
	}

	//VarList → Id | Id , VarList
	string VarList(Node_tree* root) {
		if (root->childrens.size() == 1) {
			string id = Id(root->childrens[0]);
			if (!variables.empty()) {
				if (variables.find(id) == variables.end()) {
					//Переменная ещё не была объявлена;
					variables.emplace(id);
				}
				else {
					//где-то раньше переменная была объявлена;
					string error;
					string message = "Ошибка: Переобъявление переменной: ";
					error = message + id;
					errors.emplace(error);
				}
			}
			else {
				variables.emplace(id);
			}
			return id + " ";
		}
		else {
			string id = Id(root->childrens[2]);
			if (!variables.empty()) {
				if (variables.find(id) == variables.end()) {
					//Переменная ещё не была объявлена;
					variables.emplace(id);
				}
				else {
					//где-то раньше переменная была объявлена;
					string error;
					string message = "Ошибка: Переобъявление переменной: ";
					error = message + id;
					errors.emplace(error); 
				}
			}
			else {
				variables.emplace(id);
			}
			string varlist = VarList(root->childrens[0]);
			return id + " " + varlist; 
		}
	}

	//Type → integer
	string Type(Node_tree* root) {
		return root->childrens[0]->info; 
	}

	//Operators → Op | Op Operators
	string Operators(Node_tree* root) {
		if (root->childrens.size() == 1) {
			string op = Op(root->childrens[0]);
			return op;
		}
		else {
			string op = Op(root->childrens[1]);
			string operators = Operators(root->childrens[0]);
			return op + operators; 
		}
	}

	//Op → Id := Expr ; | case Expr of Options end ;
	string Op(Node_tree* root) {
		if (root->childrens.size() == 4) {
			//Id := Expr ;
			string id = Id(root->childrens[3]);
			if (variables.find(id) == variables.end()) {
				// переменная не была объявлена;
				string error;
				string f_message = "Ошибка: Переменная    ";
				string s_message = "    не объявлена;";
				error = f_message + id + s_message; 
				errors.emplace(error); 
			}
			string expr = Expr(root->childrens[1]);
			return id + " " + expr + " " + ":=  ";
		}
		else {
			//case Expr of Options end;
			num_clb++;  

			string case_label = "clb" + to_string(num_clb); 
			labels.push_back(case_label);

			string expr = Expr(root->childrens[4]);
			string options = Options(root->childrens[2]);

			
			//  /n - для удобства чтения;
			string result = "\n" + expr + " " + options + " \n" + labels[labels.size() - 1] + " DEFL\n";
			labels.pop_back();
			//удаляем метку из вектора меток, вектор - это стек;
			return result;
		}
	}

	//case Expr of Options end;
	//Options → Const : Operators | Const : Operators Options
	string Options(Node_tree* root) {
		if (root->childrens.size() == 3) {
			//Const : Operators
			string constant = Const(root->childrens[2]); 
			string local_label = "lb" + constant;

			string operators = Operators(root->childrens[0]);
			string result = "\n" + local_label + " BF " + operators + " " + labels[labels.size() - 1] + " BRL  " + local_label + " DEFL\n";
			return result; 
		}
		else {
			if (root->childrens.size() == 4) {
				//Const : Operators Options 
				string constant = Const(root->childrens[3]); 
				string local_label = "lb" + constant; 

				string operators = Operators(root->childrens[1]);
				string options = Options(root->childrens[0]);
				string result = "\n" + local_label + " BF " + operators + " " + labels[labels.size() - 1] + " BRL  " + local_label + " DEFL\n" + options;
				return result;
			}
		}
	}

	//Expr → SimpleExpr | SimpleExpr + Expr | SimpleExpr - Expr
	string Expr(Node_tree* root) {
		if (root->childrens.size() == 1) {
			string simpleexpr = SimpleExpr(root->childrens[0]);
			return simpleexpr;
		}
		else if ((root->childrens.size() == 3) && (root->childrens[1]->info == "+")) {
			string simpleexpr = SimpleExpr(root->childrens[2]);
			string expr = Expr(root->childrens[0]);
			return simpleexpr + " " + expr + " " + "+";
		}
		else if ((root->childrens.size() == 3) && (root->childrens[1]->info == "-")) {
			string simpleexpr = SimpleExpr(root->childrens[2]);
			string expr = Expr(root->childrens[0]);
			return simpleexpr + " " + expr + " " + "-";
		}
	}

	//SimpleExpr → Id | Const | ( Expr )
	string SimpleExpr(Node_tree* root) {
		if (root->childrens[0]->info == "Id") {
			string id = Id(root->childrens[0]);
			if (variables.find(id) == variables.end()) { 
				// переменная не была объявлена;
				string error; 
				string f_message = "Ошибка: Переменная    "; 
				string s_message = "    не объявлена;"; 
				error = f_message + id + s_message; 
				errors.emplace(error);  
			} 
			return id;
		}
		else if (root->childrens[0]->info == "Const") {
			string constant = Const(root->childrens[0]);
			return constant;
		}
		else if (root->childrens.size() == 3) {
			string expr = Expr(root->childrens[1]);
			return expr; 
		}
	}

	//Id → Letter | Letter Id
	string Id(Node_tree* root) {
		if (root->childrens.size() == 1) {
			string letter = Letter(root->childrens[0]);
			return letter;
		}
		else {
			string letter = Letter(root->childrens[1]);
			string id = Id(root->childrens[0]);
			return letter + id; 
		}
	}

	string Letter(Node_tree* root) {
		return root->childrens[0]->info;
	}

	//Const → Digit | Digit Const
	string Const(Node_tree* root) { 
		if (root->childrens.size() == 1) {
			string digit = Digit(root->childrens[0]);
			return digit;
		}
		else {
			string digit = Digit(root->childrens[1]); 
			string constant = Const(root->childrens[0]);
			return digit + constant; 
		}
	}

	string Digit(Node_tree* root) {
		return root->childrens[0]->info; 
	}


};