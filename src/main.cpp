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