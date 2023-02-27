obj += cputest
obj += memtest

all:$(obj)

clean:
	rm -rf $(obj)


cputest: cputest.cpp common.cpp RobotDebug.cpp
	g++ -o $@ $^ -std=c++11  -lglog -lpthread -fPIC -I./ -lyaml-cpp

memtest: memtest.cpp common.cpp RobotDebug.cpp
		g++ -o $@ $^ -std=c++11  -lglog -lpthread -fPIC -I./ -lyaml-cpp

