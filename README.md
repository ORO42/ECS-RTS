# ECS-RTS
Template for creating RTS games on top of a custom ECS and Raylib API

This is an early prototype and example of how an RTS could be built on top of a fairly simple ECS system.

Current features are:
- moveable agents
- drag-to-select
- ray collision detection
- a game grid
- Composable ECS system making it easy to quickly prototype game concepts

To compile, run this in the project root:
`clang++ *.cpp -std=c++17 -L lib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a -o compiled-raylib-game`
