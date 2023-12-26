#include "Hash_table.h"

//token create_token(string type_new_lexeme, string new_lexeme, my_type new_index);
void Hash_table::create_table(my_type new_size) {
	// Создаем хеш-таблицу;
	this->info.resize(new_size);
	this->size = new_size;
	this->count_elems = 0;
}

token create_token(string type_new_lexeme, string new_lexeme, my_type new_index) {
	//Создаем новый токенl;
	token new_token(type_new_lexeme, new_lexeme, new_index, new_index);
	return new_token;
}

void free_token(token old) {
	old.index = -1;
	//очищаем строку;
	old.lexeme.clear();
	//освобождаем память, занимаемой строкой;
	old.lexeme.shrink_to_fit();
	old.type_lexeme.clear();
	old.type_lexeme.shrink_to_fit();
}




//void free_token(token old);
//Освобождаем всю таблицу
void Hash_table::free_table(Hash_table& table) {
	for (int i = 0; i < table.size; i++) {
		table.info[i].~List();
	}
	table.info.clear();
	table.info.shrink_to_fit();

	table.count_elems = 0;
	table.size = 0;
}

token Hash_table::find_in_table(my_type hash) {
	my_type index = hash % this->size;
	if (this->info[index].get_size() == 1) {
		return this->info[index].get_elem(0)->info_node;
	}
	else {
		for (int i = 0; i < this->info[index].get_size(); i++) {
			if (hash == this->info[index].get_elem(i)->info_node.hash) {
				return this->info[index].get_elem(i)->info_node;
			}
		}
	}
}


bool Hash_table::find_in_list(my_type index, my_type new_hash) {

	for (int i = 0; i < this->info[index].get_size(); i++) {
		if (new_hash == this->info[index].get_elem(i)->info_node.hash) {
			return true;
		}
	}
	return false;
}
unsigned long Hash_table::hash_function(string lexeme) {
	vector<my_type> pow(lexeme.size());
	vector<my_type> hash(lexeme.size(), 1);

	int simple_num = 911;
	pow[0] = 1;
	for (int i = 1; i < pow.size(); i++) {
		pow[i] = (pow[i - 1] * simple_num);
	}
	hash[0] = (int)(lexeme[0]);
	for (int i = 1; i < hash.size(); i++) {
		hash[i] = (hash[i] * (int)(lexeme[i]) + hash[i - 1]);
	}

	return hash[hash.size() - 1];
}

void Hash_table::handle_collision(my_type new_index, token new_token) {
	this->info[new_index].add_to_end(new_token);
}

void Hash_table::ht_insert(string& type_new_lexeme, string& new_lexeme) {
	// Create the item
	// Compute the index
	unsigned long new_hash = hash_function(new_lexeme);
	unsigned long new_index = new_hash % this->size;
	token new_token(type_new_lexeme, new_lexeme, new_index, new_hash);

	//если в списке ноль элементов;
	if (this->info[new_index].get_size() == 0) {
		// Элемента еще нет в таблице;
		if (this->count_elems == this->size) {
			// Hash Table Full
			printf("Insert Error: Hash Table is full\n");
			// Remove the create item
			free_token(new_token);
			return;
		}
		// Добавляем новый элемент
		this->info[new_index].add_to_end(new_token);
		this->count_elems++;
	}
	else {
		if (find_in_list(new_index, new_hash) == false) {
			//Обрабатываем коллизию
			handle_collision(new_index, new_token);
		}
	}


}

void Hash_table::output() {
	ofstream fout("table_of_names.txt");
	for (int i = 0; i < size; i++) {
		if (this->info[i].get_size() > 0) {
			if (this->info[i].get_size() == 1) {
				fout <<
					this->info[i].get_elem(0)->info_node.type_lexeme << ' ' <<
					this->info[i].get_elem(0)->info_node.lexeme << ' ' <<
					this->info[i].get_elem(0)->info_node.index << '\n';
			}
			else {
				for (int j = 0; j < this->info[i].get_size(); j++) {
					fout <<
						this->info[i].get_elem(j)->info_node.type_lexeme << ' ' <<
						this->info[i].get_elem(j)->info_node.lexeme << ' ' <<
						this->info[i].get_elem(j)->info_node.index << '\n';
				}
				fout << "\n ";
			}
		}
	}
	fout.close();
	return;
}

Hash_table::~Hash_table() {
	for (int i = 0; i < info.size(); i++) {
		info[i].~List();
	}
	//очищаем от значений;
	info.clear();
	//освобождаем память;
	info.shrink_to_fit();
}
