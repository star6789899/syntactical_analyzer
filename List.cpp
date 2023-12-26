#include "List.h"

int List::get_size() {
	return amount_elems;
}

//�������� ������� � ������;
void List::add_to_begin(token num) {
	// ������� ����� �����
	Node* new_node = new Node(num);
	// ����� ����� ��������� �����
	new_node->next = head;
	if (head != NULL)
		head->prev = new_node;
	// ��������, ���� �� ��������� ����� ������ ������� � ������
	if (tail == NULL)
		tail = new_node;
	head = new_node;
	++amount_elems;
}

//�������� ������� � �����;
void  List::add_to_end(token num) {
	// ������� ����� ������
	Node* new_node = new Node(num);
	// ����� ����� ��������� �����
	new_node->prev = tail;
	if (tail != NULL)
		tail->next = new_node;
	// ��������, ���� �� ��������� ����� ������ ������� � ������
	if (head == NULL)
		head = new_node;
	tail = new_node;
	++amount_elems;
}

//������� ������ �������;
void  List::remove_first_element() {
	// ���� ��������� �� ������ ����� NULL, �� ������� ������
	if (head == NULL)
	{
		cout << "������ ����\n";
		return;
	}
	// ��������� �� ��������� ����� �������
	Node* new_node = head->next;
	//������� ������;
	new_node->~Node();
	// ���� �� ����������, ��, ����� �������� ��������, � ����
	// �� ����� ���������� �� ���������� �������
	if (new_node != NULL)
		new_node->prev = NULL;
	// ���� �� ������� ������������ ������, �� �� ����
	// ����� ����� ��� ���������, ������� "�������  �������" �� ���������� ������� � ������ ������� �������� ������;
	else
		//���� ������� ������������ � ������
		tail = NULL;
	delete head;
	--amount_elems;
	head = new_node;
}

//������� ��������� �������;
void  List::remove_last_element() {
	// ���� ��������� �� ����� ����� NULL, �� ������� ������
	if (tail == NULL)
	{
		cout << "������ ����\n";
		return;
	}
	// ��������� �� ������������� �������
	Node* new_node = tail->prev;
	
	new_node->~Node();
	// ���� �� ����������, ��, ����� �������� ��������, � ����
	// �� ����� ���������� �� ��������� �������
	if (new_node != NULL)
		new_node->next = NULL;
	// ���� �� ������� ������������ ������, �� �� ����
	// ����� ����� ��� ���������
	else
		head = NULL;
	delete tail;
	--amount_elems;
	tail = new_node;

}

Node* List::get_elem(int index)
{
	// ��������� �� ������ �������, ������ 0
	Node* new_node = head;
	for (int i = 0; i < index; i++)
	{
		new_node = new_node->next;
		//���� ����� �� ������� ������;
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
		cout << "������ ����.";
	}
	else {
		for (int i = 0; i < answer.get_size(); i++)
			cout << answer[i]->info_node.index << " " << answer[i]->info_node.type_lexeme << " " << answer[i]->info_node.lexeme << " \n";
	}
	cout << "\n";

}
 token::token() {}
