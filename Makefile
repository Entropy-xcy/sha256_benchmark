default:
	gcc -o sha256_benchmark main.c -static -L/usr/local/lib/ -lssl -lcrypto -lpthread
