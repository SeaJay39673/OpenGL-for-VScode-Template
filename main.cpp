/*
    Author: Christopher Edmunds
    Last Updated: 10/28/2024
*/

//====| Includes |====//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shader.h"
//====| Namespaces |====//
using namespace std;

//====| Global Variables |====//
int _width = 800, _height = 600;
//====| Function Declarations |====//
GLFWwindow *initWindow();                                                  // Create and initialize window to default variables
bool initGlad();                                                           // Initialize glad to expose OpenGL function pointers
void framebuffer_size_callback(GLFWwindow *window, int width, int height); // function that sets GLFWwindow size when user changes it
void processInput(GLFWwindow *window);                                     // Process user input

//====| Main |====//
int main(int, char **)
{
    GLFWwindow *window = initWindow();
    if (window == NULL) // If failed, exit
    {
        return 1;
    }

    // Draw a triangle

    // Triangle Vertices
    float TriVerts[] = {
        -0.5f, -0.5f, 0.0f, // lower-left corner
        0.5f, -0.5f, 0.0f,  // lower-right corner
        0.0f, 0.5f, 0.0f    // top-center corner
    };

    unsigned int VAO[2], VBO[2], EBO; // Create VAOs, VBOs, and EBO

    // Generate the ID's for each object
    /*
        If using only one VAO, it would look like this:
            glGenVertexArrays(1, &VAO);
    */
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    // Bind the VAO[0] which will become the instructions for how to draw the triangle.
    glBindVertexArray(VAO[0]);

    // Bind the VBO[0] which will hold the vertex information of the triangle.
    /*
        GLenum target: the type of buffer to bind to (GL_ARRAY_BUFFER, GL_ELEMENT_ARRAY_BUFFER).
        GLuint buffer: The VBO to bind to.
    */
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);

    // Pass in vertex information to VBO[0]
    /*
        GLenum target: specifies the type of buffer to pass data to.
        Glsizeiptr size: specifies the size of the array passed in.
        const void* data: the data array to be passed to the buffer
        GLenum usage: How this data is going to be used (static or dynamic)
    */
    glBufferData(GL_ARRAY_BUFFER, sizeof(TriVerts), TriVerts, GL_STATIC_DRAW);

    // Specify how data in the vertices should be read
    /*
        GLuint index: Which layout in the vertex shader this data will be passed into.
        GLint size: How many elements per vertex there is.
        GLenum type: What type of data it is.
        GLboolean normalized: Wether or not to normalize the data. Use false.
        GLsizei stride: How many elements in the array to skip before you start reading in the next vertex element.
            If one point consists of 3 floats, the stride would be 3 (skip 3 floats to get to the next vertex).
        const void* pointer: Index of where to start reading the data.
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    // Enable the layout the vertex shader needs to use
    glEnableVertexAttribArray(0);

    // Unbind the VAO and VBO
    glBindVertexArray(0);
    glBindBuffer(VBO[0], 0);

    // Draw a square

    // Vertices of square (2 triangles)
    float SquareVerts[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };
    // Indices of overlapping vertices
    unsigned int SquareInds[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SquareVerts), SquareVerts, GL_STATIC_DRAW);
    // Bind the EBO and set index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SquareInds), SquareInds, GL_STATIC_DRAW);

    /*
        3 elements (3 floats make up a point)
        3 elements for each vertex (span is 3)
        start at 0
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    // Unbind VAO and buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Shader shader("../Resources/Shaders/Simple.vs", "../Resources/Shaders/Simple.fs"); // Load in shader files, compile, and create shader object

    while (!glfwWindowShouldClose(window)) // Where the window stuff happens.
    {
        // input
        processInput(window);

        // rendering commands

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glBindVertexArray(VAO[0]); // Bind the triangle VAO
        // Draw the triangle VAO
        /*
            GLenum mode: How to draw the data (use GL_TRIANGLES)
            GLint first: what element to start with
            GLsizei count: how many elements to draw
        */
        // glDrawArrays(GL_TRIANGLES, 0, 3); // Uncomment to draw triangle
        glBindVertexArray(0);

        glBindVertexArray(VAO[1]); // Bind the square VAO
        // Draw the square VAO
        /*
            GLenum mode: How to draw the data (use GL_TRIANGLES)
            GLsizei count: How many indices are there
            GLenum type: Type of data to draw (type of data the indices are)
            const void * indices: what index to start with
        */
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // Uncomment to draw square
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate(); // Properly exit the application
    return 0;
}

//====| Function Definitions |====//

/*
    Creates and initializes window with default variables
    Parameters: None
    returns: GLWwindow*
*/
GLFWwindow *initWindow()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Specify which type of OpenGL to use
    glfwWindowHint(GLFW_MAXIMIZED, GL_TRUE);                       // Start in maximized mode
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Needed for Mac

    // Create the window and display it
    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) // Error creating window
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);

    glfwGetWindowSize(window, &_width, &_height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); // Call back so user can resize the window

    if (!initGlad()) // If failed, exit
    {
        if (window != NULL) // If window was successful, terminate it first
        {
            glfwTerminate();
        }
        cout << "Failed to initialize glad" << endl;
        return NULL;
    }
    glViewport(0, 0, _width, _height); // Tell OpenGL size of rendering window

    return window;
}
/*
    Initializes GLAD and exposes OpenGL function pointers
    Parameters: None
    Returns: Boolean
*/
bool initGlad()
{
    // Initialize glad to find and expose OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}

/*
    Callback function to reset the window size global variables and set viewport accordingly
    Parameters: GLFWwindow* window, int width, int height
    Returns: None
*/
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    _width = width;
    _height = height;
    glViewport(0, 0, _width, _height);
}

/*
    Gets user input and responds accordingly
    Parameters: GLFWwindow* window
    Returns: None
*/
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // Sets drawing to wire frame mode
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) // Sets drawing to fill mode
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}