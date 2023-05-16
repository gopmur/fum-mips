#include <main.h>

int main() {

	FumMips computer("code.txt");

	computer.show_register_file();
	computer.next_clock();
	computer.show_register_file();
	
	return 0;
}