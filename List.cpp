#include "List.h"

int List::get_size() {
	return amount_elems;
}

//Добавить элемент в начало;
void List::add_to_begin(token num) {
	// Создали новый объек
	Node* new_node = new Node(num);
	// Далее нужно настроить связи
	new_node->next = head;
	if (head != NULL)
		head->prev = new_node;
	// Проверка, если мы добавляем самый первый элемент в список
	if (tail == NULL)
		tail = new_node;
	head = new_node;
	++amount_elems;
}

//Добавить элемент в конец;
void  List::add_to_end(token num) {
	// Создали новый объект
	Node* new_node = new Node(num);
	// Далее нужно настроить связи
	new_node->prev = tail;
	if (tail != NULL)
		tail->next = new_node;
	// Проверка, если мы добавляем самый первый элемент в список
	if (head == NULL)
		head = new_node;
	tail = new_node;
	++amount_elems;
}

//Извлечь первый элемент;
void  List::remove_first_element() {
	// Если указатель на начало равен NULL, то удалять нечего
	if (head == NULL)
	{
		cout << "Список пуст\n";
		return;
	}
	// Указатель на следующий после первого
	Node* new_node = head->next;
	//очищаем память;
	new_node->~Node();
	// Если он существует, то, после удаления элемента, у него
	// не будет указателся на предыдущий элемент
	if (new_node != NULL)
		new_node->prev = NULL;
	// Если мы удаляем единственный объект, то на него
	// будут вести оба указателя, поэтому "убираем  стрелку" на предыдущий элемент у нового первого элемента списка;
	else
		//если элемент единственный в списке
		tail = NULL;
	delete head;
	--amount_elems;
	head = new_node;
}

//Извлечь последний элемент;
void  List::remove_last_element() {
	// Если указатель на конец равен NULL, то удалять нечего
	if (tail == NULL)
	{
		cout << "Список пуст\n";
		return;
	}
	// Указатель на предпоследний элемент
	Node* new_node = tail->prev;
	
	new_node->~Node();
	// Если он существует, то, после удаления элемента, у него
	// не будет указателся на следующий элемент
	if (new_node != NULL)
		new_node->next = NULL;
	// Если мы удаляем единственный объект, то на него
	// будут вести оба указателя
	else
		head = NULL;
	delete tail;
	--amount_elems;
	tail = new_node;

}

Node* List::get_elem(int index)
{
	// Ссылается на первый элемент, индекс 0
	Node* new_node = head;
	for (int i = 0; i < index; i++)
	{
		new_node = new_node->next;
		//если вышли за границу списка;
		if (new_node == NULL) {
			return new_node;
		}
	}
	return new_node;
}

Node* List::operator[] (int index)
{
	return get_elem(index);
}
List::~List() {
	while (head != NULL) {
		remove_first_element();
	}
}

void delete_end(List& list)
{
	list.remove_last_element();
}

void delete_start(List& list)
{
	list.remove_first_element();
}

void print_list(List& answer) {
	if (answer.get_size() == 0) {
		cout << "Список пуст.";
	}
	else {
		for (int i = 0; i < answer.get_size(); i++)
			cout << answer[i]->info_node.index << " " << answer[i]->info_node.type_lexeme << " " << answer[i]->info_node.lexeme << " \n";
	}
	cout << "\n";

}
 token::token() {}
