main: main.cpp
	g++ -g -o main main.cpp -lmysqlclient -I./lib -I./libunp ./lib/*.cpp ./binunp/libunp.a  -std=gnu++0x
exec:
	./main config/config.conf
clean:
	rm  main
