TEXI2DVI = makeinfo --html
SRC_DIR = Trading/
BUILD_DIR = build/
CC = g++
STD = -std=c++17
CFLAG = -Wall -Wextra -Wshadow -pedantic -g -fsanitize=address
SUP = --suppress=
suppress = $(SUP)unusedFunction\

SRC_FILES = Trading/main.cc \
	Trading/Controller/controller.cc \
	Trading/Model/model.cc \
	Trading/Model/Approximation/approximation.cc \
	Trading/Model/Gauss/gauss.cc \
	Trading/Model/InterpolationNewton/interpolation_newton.cc \
	Trading/Model/InterpolationSpline/interpolation_spline.cc \
	Trading/View/mainwindow.cc \
	Trading/View/plotwidget.cc


OS := $(shell uname -s)

ifeq ($(OS), Darwin)
	APP = Trading.app
else
	APP = Trading
endif


all: install

uninstall:
	@rm -rf $(BUILD_DIR)

install: uninstall
	@mkdir $(BUILD_DIR)
	@cmake $(SRC_DIR)CMakeLists.txt 
	@make -C $(SRC_DIR)
	@cp -r $(SRC_DIR)$(APP) $(BUILD_DIR)/$(APP)
	@make clean -C $(SRC_DIR)

dvi:
	$(TEXI2DVI) ./documentation.texi

dist: 
	@cmake $(SRC_DIR)CMakeLists.txt 
	@make -C $(SRC_DIR) 
	@tar -zcf Trading.tar $(SRC_DIR)$(APP) 
	@make clean -C $(SRC_DIR)

clean:
	@rm -rf documentation
	@rm -rf .clang-format
	@rm -rf $(BUILD_DIR)
	@rm -rf *.tar

clang-format:
	@cp ../materials/linters/.clang-format .
	@clang-format -i Trading/main.*
	@clang-format -i Trading/Model/*.*
	@clang-format -i Trading/Model/Approximation/*.*
	@clang-format -i Trading/Model/Gauss/*.*
	@clang-format -i Trading/Controller/*.*
	@clang-format -i Trading/Model/InterpolationNewton/*.*
	@clang-format -i Trading/Model/InterpolationSpline/*.*
	@clang-format -i Trading/View/mainwindow.cc Trading/View/mainwindow.h
	@clang-format -i Trading/View/plotwidget.cc Trading/View/plotwidget.h

clang:
	@cp ../materials/linters/.clang-format .
	@clang-format -n Trading/main.*
	@clang-format -n Trading/Model/*.*
	@clang-format -n Trading/Model/Approximation/*.*
	@clang-format -n Trading/Model/InterpolationNewton/*.*
	@clang-format -n Trading/Model/InterpolationSpline/*.*
	@clang-format -n Trading/Model/Gauss/*.*
	@clang-format -n Trading/Controller/*.*
	@clang-format -n Trading/View/mainwindow.cc Trading/View/mainwindow.h
	@clang-format -n Trading/View/plotwidget.cc Trading/View/plotwidget.h

cppcheck:
	cppcheck -$(STD) --enable=all $(suppress) $(SRC_FILES) 


