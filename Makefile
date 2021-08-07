make:
	g++ stat.cpp -o stat
	chmod +x stat
install:
	sudo cp stat /usr/local/bin/stat
.PHONY: make
