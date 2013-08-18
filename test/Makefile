run: test testfake
	./test midi.txt
	./test midi2.txt
	./testfake

test: test.cpp ../*
	$(CXX) -g -o test -Wall -I.. test.cpp ../amidino.cpp

testfake: testfake.cpp ../*
	$(CXX) -g -o testfake -Wall -I.. testfake.cpp ../amidino.cpp

clean:
	rm -f test testfake
	rm -rf test.dSYM testfake.dSYM

.PHONY: run clean
