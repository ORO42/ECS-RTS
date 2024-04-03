# ECS-RTS
Template for creating RTS games on top of a custom ECS and Raylib API

To compile run this in the project root:
`clang++ *.cpp -std=c++17 -L lib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a -o compiled-raylib-game`
