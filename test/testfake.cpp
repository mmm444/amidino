#include <amidino.h>
#include <iostream>

using namespace std;

#define t(cond) if (!(cond)) { cout << __FILE__ << ':' << __LINE__ << ' ' << #cond << '\n'; return 1; }
#define on(n, v) p.noteOn((n), (v))
#define off(n) p.noteOn((n), 0)

uint8_t out = 0;

void handleOutput(uint8_t note, uint8_t vel) {
	out = (vel > 0) ? note : 0;
}

int main(int argc, char** argv) {
	FakePoly p;
	p.setHandler(handleOutput);
	
	// simple note on & off
	t(out == 0);
	on(1, 1);
	t(out == 1);
	off(1);
	t(out == 0);

	// 2 interleaved
	on(1, 1);
	t(out == 1);
	on(2, 1);
	t(out == 2);
	off(1);
	t(out == 2);
	off(2);
	t(out == 0);

	// 2 stacked
	on(1, 1);
	t(out == 1);
	on(2, 1);
	t(out == 2);
	off(2);
	t(out == 1);
	off(1);
	t(out == 0);

	// remove from empty
	off(1);
	t(out == 0);

	// fill
	on(1, 1);
	on(2, 1);
	on(3, 1);
	on(4, 1);
	on(5, 1);
	on(6, 1);
	on(7, 1);
	on(8, 1);
	on(9, 1);
	on(10, 1);
	on(11, 1);
	on(12, 1);
	on(13, 1);
	on(14, 1);
	on(15, 1);
	on(16, 1);
	t(out == 16);
	on(17, 1);
	t(out == 16);
	off(17);
	t(out == 16);

	// remove some 
	off(14);
	t(out == 16);
	on(17, 1);
	t(out == 17);
	off(17);
	t(out == 16);
	off(15);
	t(out == 16);
	off(16);
	t(out == 13);

	p.reset();
	t(out == 0);

	return 0;
}
