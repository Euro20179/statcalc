make:
	g++ statcalc.cpp -o statcalc
	chmod +x statcalc
install:
	sudo cp statcalc /usr/local/bin/statcalc
.PHONY: make
