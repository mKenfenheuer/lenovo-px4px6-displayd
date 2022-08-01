CC = g++
ODIR = obj
SDIR = src
OUT = displayd


default:
	mkdir -p  $(ODIR)
	rm $(ODIR)/$(OUT) || true
	$(CC) $(SDIR)/*.cpp $(SDIR)/pages/*.cpp  -o  $(ODIR)/$(OUT)

debug:
	mkdir -p  $(ODIR)
	$(CC) $(SDIR)/*.cpp $(SDIR)/pages/*.cpp -g -o  $(ODIR)/$(OUT)

run: default
	$(ODIR)/$(OUT)

install:
	cp $(ODIR)/$(OUT) /usr/bin/displayd