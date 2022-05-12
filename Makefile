args = -lGL -lGLEW -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-system -lsfml-window

CXX = gcc # Flag for implicit rules
CXXFLAGS = -g -I ./ $(args) # Flag for implicit rules. Turn on debug info

objects = main.o

build: copy $(objects)
	g++ $(objects) $(args) -o ./Build/main

rebuild: clean build

# These files compile via implicit rules
# Syntax - targets ...: target-pattern: prereq-patterns ...
# In the case of the first target, foo.o, the target-pattern matches foo.o and sets the "stem" to be "foo".
# It then replaces the '%' in prereq-patterns with that stem
$(objects): %.o: %.cpp

copy:
	rsync --archive --delete ./res ./Build

clean:
	rm -f *.o all
