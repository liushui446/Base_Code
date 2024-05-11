.PHONY: all swl-demo



all: swl-demo



swl-demo:

	g++ -Wall swl-demo.cpp -g -lm -L. -Wl,-rpath=. -ldlpc-swl-api -o swl-demo

swl-arm：
	arm-linux-gnueabihf-g++ swl-demo.cpp -g -lm -L. -Wl,-rpath=. -ldlpc-swl-api -o swl-arm-demo