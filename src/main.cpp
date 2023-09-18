#include <map>
#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <SDL2/SDL_opengl.h>
#include "shaders.h"

bool init(SDL_Window*& window, SDL_GLContext& context) {

    // Initialize SDL's video subsystem.
    // SDL_Init will return -1 if it fails to initialize.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "OpenGL",               // window title
        SDL_WINDOWPOS_CENTERED, // initial x position
        SDL_WINDOWPOS_CENTERED, // initial y position
        800,                    // width, in pixels
        600,                    // height, in pixels
        SDL_WINDOW_OPENGL       // flags - set to use OpenGL
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
        return false;
    }

    // We want to request that SDL creates a OpenGL context for our window.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3); // Request OpenGL 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3); // Request OpenGL 3.3

    // Create an OpenGL context associated with the window.
    context = SDL_GL_CreateContext(window);

    // Initialize GLEW. GLEW manages function pointers for OpenGL so we want to initialize it before
    // calling any OpenGL functions. We are using the experimental flag to get access to more features.
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    return true;
}

GLuint setupShaders(const char* vertexSource, const char* fragmentSource) {
    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Check the vertex shader
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Check the fragment shader
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check the shader program
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Delete shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// This function sets up the vertex data and creates a VAO, VBO and EBO
GLuint vertexSetup(const std::vector<glm::vec3>& vertices, const std::vector<GLuint>& indices) {
    // Generate the Vertex Array Object (VAO), the Vertex Buffer Object (VBO) and the Element Buffer Object (EBO)
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object
    glBindVertexArray(VAO);

    // Bind the Vertex Buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    // Send the vertex data to the Vertex Buffer
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // Bind the Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    // Send the index data to the Element Buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    
    // Set the vertex attributes (only position here)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Unbind the VAO (optional)
    glBindVertexArray(0);

    // Unbind the VBO, EBO and delete them
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Return the VAO
    return VAO;
}

void createEarthApproximation(std::vector<glm::vec3>& vertices, std::vector<GLuint>& indices) {
    // Define the number of segments for latitude and longitude
    int numLatSegments = 8;  // We'll have 8 latitudes
    int numLongSegments = 6;  // We'll have 6 longitudes

    // Calculate the angles for each segment
    float latStep = M_PI / numLatSegments;
    float longStep = 2.0f * M_PI / numLongSegments;

    // Add the vertices
    for(int i = 0; i <= numLatSegments; ++i){
        for(int j = 0; j <= numLongSegments; ++j){
            float theta = i * latStep;
            float phi = j * longStep;

            glm::vec3 vertex;
            vertex.x = sinf(theta) * cosf(phi);
            vertex.y = sinf(theta) * sinf(phi);
            vertex.z = cosf(theta);

            vertices.push_back(vertex);
        }
    }

    // Add the indices
    for(int i = 0; i < numLatSegments; ++i){
        for(int j = 0; j < numLongSegments; ++j){
            int first = i * (numLongSegments + 1) + j;
            int second = first + numLongSegments + 1;

            // First triangle
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            // Second triangle - Ensure we are within the vertices array bounds.
            if((second + 1) < vertices.size() && (first + 1) < vertices.size()){
                indices.push_back(second);
                indices.push_back(second + 1);
                indices.push_back(first + 1);
            }
        }
    }
}

int main() {

    // The window we'll be rendering to
    SDL_Window* window = nullptr;

    // OpenGL context
    SDL_GLContext context;

    // Initialize SDL and OpenGL
    if (!init(window, context)) {
        std::cerr << "Failed to initialize!" << std::endl;
        return EXIT_FAILURE;
    }

    // Setup shaders
    GLuint shaderProgram = setupShaders(vertexShaderSource, fragmentShaderSource);
    if (shaderProgram == 0) {
        std::cerr << "Failed to set up the shader program." << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    
    // Setup vertex data
    /*
    vertices = {
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3(0.5f, -0.5f, 0.0f),
        glm::vec3(0.0f, 0.5f, 0.0f)
    };
    // Indices in counterclockwise order
    indices = {0, 1, 2};
    */
    createEarthApproximation(vertices, indices);
    GLuint VAO = vertexSetup(vertices, indices);

    // Setup time related variables
    Uint32 currentTime = SDL_GetTicks();
    Uint32 lastTime = 0;
    float deltaTime = 0.0f;

    float rotateAngle = 0.0f;

    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);

    // Main loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        cameraPos.z -= 1.0f; // move closer
                        break;
                    case SDLK_DOWN:
                        cameraPos.z += 1.0f; // move away
                        break;
                }
            }
        }
        // Calculate delta time
        lastTime = currentTime;
        currentTime = SDL_GetTicks();
        deltaTime = (currentTime - lastTime) / 1000.0f;

        // Clear the screen to black
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(
            cameraPos, // Camera position is updated based on keyboard input
            glm::vec3(0.0f, 0.0f, 0.0f), // and looks at the origin
            glm::vec3(0.0f, 1.0f, 0.0f)  // Head is up (set to 0,-1,0 to look upside-down)
        );

        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), // The field of view
            800.0f / 600.0f, // Aspect Ratio
            0.1f, // Near clipping plane
            100.0f // Far clipping plane
        );

        rotateAngle += glm::radians(120.0f) * deltaTime;  // 120 deg per second
        glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), rotateAngle, glm::vec3(0.0f, 1.0f, 0.0f));
        // glm::mat4 rotateMat = glm::mat4(1.0f);

        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotateMat));

        GLuint lightPositionLoc = glGetUniformLocation(shaderProgram, "lightPosition");
        glm::vec3 lightPosition = glm::vec3(2.0f, 2.0f, 2.0f); // Position of the light source
        glUniform3fv(lightPositionLoc, 1, glm::value_ptr(lightPosition));

        // Draw your object
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap buffers
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}