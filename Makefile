#OBJS specifies which files to compile as part of the project
OBJS = src/main.cpp src/Shader.h src/Shader.cpp src/Texture.h src/Texture.cpp

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = GLES3-engine

#This is the target that compiles our executable
all : $(OBJS)
	g++ $(OBJS) -w -lGLESv2 -lSDL2 -lSDL2_image -o $(OBJ_NAME)

run :
	./GLES3-engine
