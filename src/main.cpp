/*
About: This is a simple OpenGL engine that is targeting destop gl 2.1,
however it will only use features that are available in OpenGL ES 2.0.
Hopefully this will allow it to easily run on desktops and a wide variety of 
single board arm devices that can usually run desktop gl 2.1.
*/

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>




void renderScene();
void initGame();
void updateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
    glfwInit();

    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 2 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 1 );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );

    GLFWwindow *window = glfwCreateWindow(800, 600, "Simple GLES Engine", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, updateKeyboard);
    glewInit();
    initGame();

    while (!glfwWindowShouldClose(window)) {

        renderScene();
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    glfwTerminate();
    return 0;
}

void renderScene(){
    glClearColor(0.0, 0.0, 0.0, 0.0); 
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void initGame() {
    glEnable(GL_DEPTH_TEST);
}


void updateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (true) {
        if ((key == GLFW_KEY_UP || key == GLFW_KEY_SPACE) && action == GLFW_PRESS) {
            printf("pressed up key \n");
        }
    }
}
