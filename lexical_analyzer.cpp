#include "lexical_analyzer.h"

//дает следующее слово из файла;
char buffer;
char buffer_alph;
string lexical_analyzer::f(ifstream& file) {
	string word;
	char x = '/0';
	char x_op = '/0';
	if ((buffer == ':') || (buffer == '=') || (buffer == '+') || (buffer == '-')
		|| (buffer == ';') || (buffer == '(') || (buffer == ')') || (buffer == ',')) {
		x_op = buffer;
	}
	else {
		if (((buffer_alph >= 'a') && (buffer_alph <= 'z')) || ((buffer_alph >= 'A') && (buffer_alph <= 'Z'))) {
			x = buffer_alph;
			buffer_alph = '-';
		}
		else {
			x = file.get();
		}
	}
	bool flag = true;
	//считывание пробелов в файле до первой буквы в строке;
	while ((!file.eof() || (x == ' ') || (x == '\n') || (x == '\t')) &&
		((!(((x >= 'a') && (x <= 'z')) || ((x >= 'A') && (x <= 'Z')))) && (!((x >= '0') && (x <= '9'))) &&
			(x != ':') && (x != '=') && (x != '+') && (x != '-') && (x != ';') && (x != '(') && (x != ')')))
	{
		if ((x != ' ') && (x != '\n') && (x != '\t')) {
			word += x;
			break;
		}
		x = file.get();
	}
	if (((x == ':') || (x_op == ':')) && flag) {

		flag = false;
		if (x == ':')
			word = word + x;
		else {
			word = word + x_op;
		}
		buffer = '/0';
		x = file.get();
		if (x == '=') {
			word = word + x;
		}
		else {
			if (((x >= 'a') && (x <= 'z')) || ((x >= 'A') && (x <= 'Z'))) {
				buffer_alph = x;
			}
		}
	}
	if (((x == ',') || (x_op == ',')) && flag) {
		flag = false;
		buffer = '/0';
		if (x == ',') {
			if(word.size() == 0)
				word = word + x;
		}
		else {
			if (word.size() == 0)
				word = word + x_op;
		}
	}
	if (((x == '=') || (x_op == '=')) && flag) {
		flag = false;
		buffer = '/0';
		if (x == '=')
			word = word + x;
		else {
			word = word + x_op;
		}
	}

	if (((x == '-') || (x_op == '-')) && flag) {
		flag = false;
		buffer = '/0';
		if (x == '-')
			word = word + x;
		else {
			word = word + x_op;
		}
	}
	if (((x == '+') || (x_op == '+')) && flag) {
		flag = false;
		buffer = '/0';
		if (x == '+')
			word = word + x;
		else {
			word = word + x_op;
		}
	}
	if (((x == ';') || (x_op == ';')) && flag) {
		flag = false;
		buffer = '/0';
		if (x == ';')
			word = word + x;
		else {
			word = word + x_op;
		}
	}
	if (((x == '(') || (x_op == '(')) && flag) {
		flag = false;
		buffer = '/0';
		if (x == '(')
			word = word + x;
		else {
			word = word + x_op;
		}
	}
	if (((x == ')') || (x_op == ')')) && flag) {
		flag = false;
		buffer = '/0';
		if (x == ')')
			word = word + x;
		else {
			word = word + x_op;
		}
	}
	if ((flag) && (((x >= 'a') && (x <= 'z')) || ((x >= 'A') && (x <= 'Z')))) {
		flag = false;
		word = word + x;
		while (!file.eof() && (x != ' ') && (x != '\n') && (x != '\t') && (x != ':') &&
			(x != '=') && (x != '+') && (x != '-') && (x != ';') && (x != '(') && (x != ':') && (x != ')') && (x != ','))
		{

			x = file.get();
			if (!file.eof() && ((((x >= '0') && (x <= '9')) || ((x >= 'a') && (x <= 'z')) || ((x >= 'A') && (x <= 'Z')) ||
				((x != ' ') && (x != '\n') && (x != '\t'))) && (x != ':') &&
				(x != '=') && (x != '+') && (x != '-') && (x != ';') && (x != '(') && (x != ')') && (x != ','))) {
				word = word + x;

			}
			if ((x == ':') || (x == '=') || (x == '+') || (x == '-') || (x == ';') || (x == '(') || (x == ')') || (x == ',')) {
				buffer = x;
			}
		}
		
		buffer_alph = '/0';
	}
	if ((x >= '0') && (x <= '9') && flag) {
		flag = false;
		word = word + x;
		while (!file.eof() && (x != ' ') && (x != '\n') && (x != '\t') && (x != ':') &&
			(x != '=') && (x != '+') && (x != '-') && (x != ';') && (x != '(') && (x != ':') && (x != ')') && (x != ','))
		{
			x = file.get();

			if (!file.eof() && ((((x >= '0') && (x <= '9')) || ((x >= 'a') && (x <= 'z')) || ((x >= 'A') && (x <= 'Z')) ||
				((x != ' ') && (x != '\n') && (x != '\t')))
				&&
				(x != '=') && (x != '+') && (x != '-') && (x != ';') && (x != '(') && (x != ':') && (x != ')') && (x != ','))) {
				word = word + x;
			}
			if ((x == ':') || (x == '=') || (x == '+') || (x == '-') || (x == ';') || (x == '(') || (x == ')') || (x == ',')) {
				buffer = x;
			}
		}
		
		buffer_alph = '/0';
	}


	return word;
}

//	Определяем к какому типу относится лексема
/*int get_last_state_from_path(string& path) {
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
void my_insert(Hash_table& table, int state, int st, string type_lexeme, string lexeme) {
	if (state == st) {
		//добавляем в хэш таблицу;

		table.ht_insert(type_lexeme, lexeme);

	}
}*/
/*
void lexical_analyzer::analyze(string path_input_file) {
	ifstream file;
	file.open(path_input_file);
	//ofstream fout("Error.txt");

	dfa my;

	my.initialize("dka_final.txt");
	Hash_table table;
	table.create_table(991);
	string word = f(file);
	//token buf;
	string path;
	while (!file.eof() || word.size() != 0)
	{
		if (my.isAccept(word, path)) {

			int state = get_last_state_from_path(path);
			//в нашей грамматике 7 ключевых слов типа integer var и тд; ключевые слова 

			my_insert(table, state, 9, "key_word", word);
			my_insert(table, state, 14, "key_word", word);
			my_insert(table, state, 17, "key_word", word);
			my_insert(table, state, 24, "key_word", word);
			my_insert(table, state, 28, "key_word", word);
			my_insert(table, state, 30, "key_word", word);
			my_insert(table, state, 33, "key_word", word); //ключевое слово;

			my_insert(table, state, 34, "operator", word);
			my_insert(table, state, 35, "operator", word);
			my_insert(table, state, 39, "operator", word);
			my_insert(table, state, 40, "operator", word);

			my_insert(table, state, 36, "separator", word);
			my_insert(table, state, 37, "separator", word);
			my_insert(table, state, 38, "separator", word);
			my_insert(table, state, 43, "separator", word);//разделтители;

			if (((state >= 1) && (state <= 8)) || ((state >= 10) && (state <= 13)) ||
				((state >= 15) && (state <= 16)) || ((state >= 18) && (state <= 23))
				|| ((state >= 25) && (state <= 27)) || (state == 29) || ((state >= 31) && (state <= 32))) {
				state = 41;
			}
			my_insert(table, state, 41, "identifier", word); //идентификатор;
			my_insert(table, state, 42, "constant", word);
		}
		else {
			//выдаем сообщение об ошибке с файлом с ошибками;
			my_insert(table, -1, -1, "Error", word);
			//fout << word << "\n";
		}
		path.shrink_to_fit();
		path.clear();
		word.clear();
		word = f(file);
	}
	file.close();
	//fout.close();
	table.output();
	exit(0);
}*/
