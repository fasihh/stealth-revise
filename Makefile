all: compile link

compile:
	g++ -Isfml/include -c main.cpp -DSFML_STATIC

link:
	g++ main.o -o main -Lsfml/lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32

clean:
	erase -f main *.o