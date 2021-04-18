default:
	gcc -o benchmark main.c -static -L/usr/local/lib/ -lssl -lcrypto -lpthread
