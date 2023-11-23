/*---------------------------------------------------------------------------
Copyright (C), 2022-2023, Sara Echeverria (bl33h)
@author Sara Echeverria
FileName: main.cpp
@version: I
Creation: 29/09/2023
Last modification: 23/11/2023
*Some parts were made using the AIs Bard and ChatGPT
------------------------------------------------------------------------------*/
#include "print.h"
#include "noise.h"
#include "camera.h"
#include "colors.h"
#include "shaders.h"
#include "fragment.h"
#include "triangles.h"
#include "framebuffer.h"
#include "triangleFill.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <vector>
#include <cassert>

Color currentColor;
bool sunPresent = false;
bool moonPresent = false;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
shaderType currentshaderType = shaderType::Sun;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error: SDL_Init failed." << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Out Of Space Shaders by bl33h", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    setupNoise();

    return true;
}

void setColor(const Color& color) {
    currentColor = color;
}

std::vector<Vertex> vertexShaderStep(const std::vector<glm::vec3>& VBO, const Uniforms& uniforms) {
    std::vector<Vertex> transformedVertices(VBO.size() / 3);
    for (size_t i = 0; i < VBO.size() / 3; ++i) {
        Vertex vertex = { VBO[i * 3], VBO[i * 3 + 1], VBO[i * 3 + 2] };;
        transformedVertices[i] = vertexShader(vertex, uniforms);
    }
    return transformedVertices;
}

std::vector<std::vector<Vertex>> primitiveAssemblyStep(const std::vector<Vertex>& transformedVertices) {
    std::vector<std::vector<Vertex>> assembledVertices(transformedVertices.size() / 3);
    for (size_t i = 0; i < transformedVertices.size() / 3; ++i) {
        Vertex edge1 = transformedVertices[3 * i];
        Vertex edge2 = transformedVertices[3 * i + 1];
        Vertex edge3 = transformedVertices[3 * i + 2];
        assembledVertices[i] = { edge1, edge2, edge3 };
    }
    return assembledVertices;
}

std::vector<Fragment> rasterizationStep(const std::vector<std::vector<Vertex>>& assembledVertices) {
    std::vector<Fragment> concurrentFragments;
    for (size_t i = 0; i < assembledVertices.size(); ++i) {
        std::vector<Fragment> rasterizedTriangle = triangle(
            assembledVertices[i][0],
            assembledVertices[i][1],
            assembledVertices[i][2]
        );
        std::lock_guard<std::mutex> lock(std::mutex);
        for (const auto& fragment : rasterizedTriangle) {
            concurrentFragments.push_back(fragment);
        }
    }
    return concurrentFragments;
}

void fragmentShaderStep( std::vector<Fragment>& concurrentFragments, shaderType shaderType) {
for (size_t i = 0; i < concurrentFragments.size(); ++i) {
        const Fragment& fragment = fragmentShader(concurrentFragments[i], shaderType);
        point(fragment);
    }
}

void render(const std::vector<glm::vec3>& VBO, const Uniforms& uniforms) {
    std::vector<Vertex> transformedVertices = vertexShaderStep(VBO, uniforms);
    std::vector<std::vector<Vertex>> assembledVertices = primitiveAssemblyStep(transformedVertices);
    std::vector<Fragment> concurrentFragments = rasterizationStep(assembledVertices);
    fragmentShaderStep(concurrentFragments, currentshaderType);
    if (moonPresent) {
            fragmentShaderStep(concurrentFragments, currentshaderType);
    }
}

void toggleFragmentShader() {
    switch (currentshaderType) {
        case shaderType::Earth:
            currentshaderType = shaderType::Neptune;
            break;
        case shaderType::Neptune:
            currentshaderType = shaderType::Venus;
            break;
        case shaderType::Venus:
            currentshaderType = shaderType::Random;
            break;
        case shaderType::Random:
            currentshaderType = shaderType::Pluton;
            break;
        case shaderType::Pluton:
            currentshaderType = shaderType::Sun;
            break;
        case shaderType::Sun:
            currentshaderType = shaderType::Earth;
            break;
    }
}

glm::mat4 createViewportMatrix(size_t screenWidth, size_t screenHeight) {
    glm::mat4 viewport = glm::mat4(1.0f);

    viewport = glm::scale(viewport, glm::vec3(screenWidth / 2.0f, screenHeight / 2.0f, 0.5f));

    viewport = glm::translate(viewport, glm::vec3(1.0f, 1.0f, 0.5f));

    return viewport;
}

int main(int argc, char* argv[]) {
    if (!init()) {
        return 1;
    }

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
        std::vector<glm::vec3> texCoords;
    std::vector<Face> faces;
    std::vector<glm::vec3> vertexBufferObject;

    std::string filePath = "src/objects/sphere.obj";

    if (!triangleFill(filePath, vertices, normals, texCoords,  faces)) {
            std::cout << "Error: Could not load OBJ file." << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    for (const auto& face : faces)
    {
        for (int i = 0; i < 3; ++i)
        {
            glm::vec3 vertexPosition = vertices[face.vertexIndices[i]];

            glm::vec3 vertexNormal = normals[face.normalIndices[i]];

            glm::vec3 vertexTexture = texCoords[face.texIndices[i]];

            vertexBufferObject.push_back(vertexPosition);
            vertexBufferObject.push_back(vertexNormal);
            vertexBufferObject.push_back(vertexTexture);
        }
    }

    Uniforms uniforms;

    glm::mat4 model = glm::mat4(1);
    glm::mat4 view = glm::mat4(1);
    glm::mat4 projection = glm::mat4(1);

    glm::vec3 translationVector(0.0f, 0.0f, 0.0f);
    float a = 45.0f;
    glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f); // Rotate around the Y-axis
    glm::vec3 scaleFactor(1.0f, 1.0f, 1.0f);

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), translationVector);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), scaleFactor);

    Camera camera;
    camera.cameraPosition = glm::vec3(0.0f, 0.0f, 2.5f);
    camera.targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    float fovRadians = glm::radians(45.0f);
    float aspectRatio = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT); // Assuming a screen resolution of 800x600
    float nearClip = 0.1f;
    float farClip = 100.0f;
    uniforms.projection = glm::perspective(fovRadians, aspectRatio, nearClip, farClip);

    uniforms.viewport = createViewportMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);
    int speed = 10;

    bool running = true;
    while (running) {

            SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_SPACE:
                    toggleFragmentShader();
                    break;
                }
            }
        }

        a += 1.0;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(a), rotationAxis);

        uniforms.model = translation * rotation * scale;

        uniforms.view = glm::lookAt(
            camera.cameraPosition,
            camera.targetPosition,
            camera.upVector
        );

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        clearFramebuffer();

        render(vertexBufferObject, uniforms);

    if (currentshaderType == shaderType::Earth) {
        moonPresent = true;
        
        float x_translation = 3.0f;
        float y_translation = 0.0f;
        float z_translation = 0.0f;
        float moonOrbitRadius = 0.85f;
        float earthRotationAngle = glm::radians(a);

        float moonRotationAngle = glm::radians(a * 1.5f);
        float moonXPosition = moonOrbitRadius * glm::cos(moonRotationAngle);
        float moonZPosition = moonOrbitRadius * glm::sin(moonRotationAngle);

        glm::mat4 moonModel = glm::mat4(1.0f);
        moonModel = glm::translate(moonModel, glm::vec3(moonXPosition, 0.0f, moonZPosition));

        glm::vec3 earthRotationAxis(0.0f, 1.0f, 0.0f);
        moonModel = glm::rotate(moonModel, earthRotationAngle, earthRotationAxis);
        float moonScaleFactor = 0.4f;
        moonModel = glm::scale(moonModel, glm::vec3(moonScaleFactor, moonScaleFactor, moonScaleFactor));

        Uniforms moonUniforms = uniforms;
        moonUniforms.model = moonModel;

        shaderType originalEarthshaderType = currentshaderType;
        currentshaderType = shaderType::Moon;
        render(vertexBufferObject, moonUniforms);
        currentshaderType = originalEarthshaderType;
    }

    if (currentshaderType == shaderType::Earth) {
        sunPresent = true;
        float x_translation = 3.0f;
        float y_translation = 0.0f;
        float z_translation = 0.0f;
        float sunOrbitRadius = 0.85f;
        float earthRotationAngle = glm::radians(a);

        float sunRotationAngle = glm::radians(a * 0.5f);

        float sunXPosition = sunOrbitRadius * glm::cos(sunRotationAngle);
        float sunZPosition = sunOrbitRadius * glm::sin(sunRotationAngle);

        glm::mat4 sunModel = glm::mat4(1.0f);
        sunModel = glm::translate(sunModel, glm::vec3(sunXPosition, 0.0f, sunZPosition));
        glm::vec3 earthRotationAxis(0.0f, 1.0f, 0.0f);
        sunModel = glm::rotate(sunModel, earthRotationAngle, earthRotationAxis);

        float sunScaleFactor = 0.4f;
        sunModel = glm::scale(sunModel, glm::vec3(sunScaleFactor, sunScaleFactor, sunScaleFactor));
        Uniforms sunUniforms = uniforms;
        sunUniforms.model = sunModel;

        shaderType originalEarthshaderType = currentshaderType;
        currentshaderType = shaderType::Sun;
        render(vertexBufferObject, sunUniforms);
        currentshaderType = originalEarthshaderType;
    }
        renderBuffer(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}