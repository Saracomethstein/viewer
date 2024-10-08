CPP=gcc
CPPFLAGS=-Wall -Wextra -Werror
DIR?=../build
TESTS_DIR=tests
DIST_DIR=archive

MAIN_PROJECT=$(shell find . -type f -name '*.c' -o -name '*.h') 
SOURCES=$(filter-out ./qtgifimage/% , $(MAIN_PROJECT))
TEST_SOURCES=$(filter-out ./view/% %.h, $(MAIN_PROJECT))
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)
TEST=$(TESTS_DIR)/test

.PHONY: all clean install uninstall tests gcov_report clang dvi dist run git

OS := $(shell uname)
ifeq ($(OS),Linux)
	OPEN_COMMAND= xdg-open
	OPEN= $(OPEN_COMMAND) report/index.html
	RESULT=3d_viewer_cpp
	CHECK_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURSE
	CHECK_LIBCPP = -lgtest -lsubunit -lm -lrt -lpthread -D_GNU_SOURSE
endif

ifeq ($(OS),Darwin)
	OPEN_COMMAND= open
	OPEN= $(OPEN_COMMAND) report/index.html
	RESULT= 3d_viewer_cpp.app
	CHECK_LIB = -lcheck
	CHECK_LIBCPP = -lgtest
endif

all: install run

install:
	@cd view && qmake && make
	@mv view/$(RESULT) $(DIR)
	@echo "Program Installed Successfully"

run:
	$(OPEN_COMMAND) ../build  

uninstall:
	@rm -rf $(DIR)/$(RESULT)
	@echo "Program Is Unistalled Successfully"

tests: $(TEST_OBJECTS) 
	@$(CPP) $(CPPFLAGS) $^ -o $(TEST) $(CHECK_LIBCPP)
	@./$(TEST)

dvi:
	$(OPEN_COMMAND) ../dvi/3d_viewer_c_about.html	

dist: clean_dist
	cd ../ && mkdir -p ${DIST_DIR}
	cd ../ && cp -rf src/* $(DIST_DIR)
	cd ../ && tar -czvf archive.tar.gz ${DIST_DIR}
	cd ../ && rm -rf ${DIST_DIR}

dist_unpack:
	cd ../ && tar -xzvf archive.tar.gz

.cc.o:
	$(CPP) -c $(CPPFLAGS) $< -o $@

gcov_report: $(TEST_SOURCES) 
	$(CPP) $(CPPFLAGS) --coverage $^ $(CHECK_LIBCPP) -o $@
	chmod +x *
	./$@ 
	lcov -t  "$@" --ignore-errors inconsistent -o $@.info --no-external -c -d .
	genhtml -o report/ $@.info
	$(OPEN)

clang:
	clang-format -style=Google -n $(SOURCES)

valgrind: tests
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --read-var-info=yes $(TEST)

clean: clean_dist
	@rm -rf $(TEST) $(TEST_OBJECTS) gcov_report* *.gc* report/ *.o
	@make clean_desktop
	@echo "Everything is cleaned"

clean_desktop:
	@cd view && make distclean && rm .qmake.stash && rm -rf build

clean_dist:
	@cd ../ && rm -rf archive
	@cd ../ && rm -rf archive.tar.gz