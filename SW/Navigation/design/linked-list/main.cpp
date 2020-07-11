#include "main.hpp"
#include "MEM_LinkedList.hpp"

using namespace MEM;

int main() {

	char buff2[8] = "0123456";
	char c = '\0';
	LinkedList<char> char_list;

	char_list.push(buff2[0], Position::BACK);
	char_list.push(buff2[1], Position::BACK);
	char_list.push(buff2[2], Position::FRONT);
	char_list.push(buff2[3], Position::FRONT);

	char_list.pop(buff2[2]);
	char_list.pop(buff2[3]);
	char_list.pop(buff2[0]);

	while (true);
	return 0;
}