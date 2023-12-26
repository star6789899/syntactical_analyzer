#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "List.h"

using namespace std;

class Hash_table
{
private:
	vector<List> info;
	my_type size;
	my_type count_elems;
	
public:
	Hash_table(){}
	void create_table(my_type new_size);

	//void free_token(token old);

	//Освобождаем всю таблицу
	void free_table(Hash_table& table);

	token find_in_table(my_type hash);
	bool find_in_list(my_type index, my_type new_hash);
	unsigned long hash_function(string lexeme);

	void handle_collision(my_type new_index, token new_token);
	string find_str(); 
	void ht_insert(string& type_new_lexeme, string& new_lexeme);
	void output();
	~Hash_table();
};

