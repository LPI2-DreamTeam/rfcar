#include "main.hpp"
#include "CircularList.hpp"
#include "Thread.hpp"

using namespace MEM;

CircularList<char, 7> char_list;

void threadMethod(OS::Thread* daddy) {

	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	char_list.push('q');
}

int main() {

	char buff[5] = "abcd";
	char buff2[8] = "1234567";
	char buff3[10] = "xxxxxxxxx";
	OS::Thread thread1("Thread1", threadMethod);

	thread1.run();

	char_list.pushBulk(buff3, 6);
	char_list.pushBulk(buff, 3);
	char_list.pushBulk(buff, 3);
	//char_list.push('k');
	//char_list.push('s');
	char_list.pushBulk(buff, 2);

	char_list.empty();
	char_list.pushBulk(buff2, 7);

	char_list.popBulk(buff, 6);
	char_list.popBulk(buff, 2, BlockingMode::BLOCKING);

	thread1.join();

	while (true);
	return 0;
}