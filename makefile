linkfile := ./lib/stropt.cpp ./lib/configfile.cpp ./lib/configitem_base.cpp  ./lib/mysqlopt.cpp ./lib/slide_window.cpp
main: main.cpp
	g++ -g -o main main.cpp -lmysqlclient -I./lib -I./libunp ./lib/*.cpp ./binunp/libunp.a  -std=gnu++0x
exec:
	./main config/config.conf
client: tcpcli.cpp
	g++ -g -o client tcpcli.cpp -I./lib -I./libunp ./lib/slide_window.cpp ./binunp/libunp.a
clean:
	rm  main client
