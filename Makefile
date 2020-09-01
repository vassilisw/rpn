ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

LIB_FILES = $(shell find . -name "*.cpp")
CPPFLAGS = -std=c++11

all: rpn

rpn: $(LIB_FILES)
	$(CXX) -o $@ $(CPPFLAGS) $(LIB_FILES)
	# *** for the reviewer: Please check README ***

install: rpn
	install -d $(DESTDIR)$(PREFIX)/bin/
	install rpn $(DESTDIR)$(PREFIX)/bin/

clean:
	rm rpn
