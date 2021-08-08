PROGNAME=statcalc

make:
	g++ -O3 statcalc.cpp -o $(PROGNAME)
	chmod +x $(PROGNAME)
install:
	sudo cp $(PROGNAME) /usr/local/bin/$(PROGNAME)
.PHONY: make
