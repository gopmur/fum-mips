#include <main.h>

int main() {

	FumMips computer("code.txt");
	while (true) {
		computer.show_memory(20);
		computer.show_register_file();
		computer.next_clock();
		cin.ignore();
	}
	return 0;
}