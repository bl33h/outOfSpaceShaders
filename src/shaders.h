#pragma once

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;

    out vec3 Normal;
    out vec3 FragPos;
    out vec3 LightPos;
    out vec3 ObjectPos;  // This is the new output

    uniform mat4 projection;
    uniform mat4 view;
    uniform mat4 model;

    uniform vec3 lightPosition; // Position of the light source in world coordinates

    void main()
    {
        vec4 worldSpacePosition = model * vec4(aPos, 1.0);
        gl_Position = projection * view * worldSpacePosition;
        FragPos = vec3(worldSpacePosition);

        // Transform the normals using the inverse transpose of the model matrix
        Normal = mat3(transpose(inverse(model))) * aPos;

        LightPos = lightPosition;
        
        ObjectPos = aPos; // Pass the object space position to the fragment shader
    }
)";


const char* fragmentShaderSource = R"(
#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;
in vec3 ObjectPos;

out vec4 FragColor;

// This function creates a "ground" on the globe based on provided center, size, and colors
vec3 createGround(vec2 center, float size, vec3 backgroundColor, vec3 foregroundColor) {
    // Convert the object position from Cartesian to spherical coordinates
    float longitude = atan(ObjectPos.z, ObjectPos.x);
    float latitude = acos(ObjectPos.y);

    float longitudeDelta = abs(longitude - center.x);
    float latitudeDelta = abs(latitude - center.y);

    // If the point is within the "ground", use the foreground color
    if (sqrt(longitudeDelta * longitudeDelta + latitudeDelta * latitudeDelta) < size) {
        return foregroundColor;
    }

    // Else, return the base color (background)
    return backgroundColor;
}

void main() {
    vec3 lightColor = vec3(1.0); // White light color

    // Simple representation of the Earth colors
    vec3 oceanColor = vec3(0.0, 0.0, 1.0);      // Blue for oceans
    vec3 continentColor = vec3(0.0, 1.0, 0.0);  // Green for continents
    vec3 poleColor = vec3(1.0);  // White for the south pole

    // Ambient light properties
    vec3 ambientColor = vec3(0.2);  // Ambient light color
    float ambientStrength = 0.5;    // Ambient light strength

    // Apply lighting calculations
    vec3 lightDirection = normalize(LightPos - FragPos);
    float diffuseStrength = max(dot(Normal, lightDirection), 0.0);

    // Calculate base color with ambient and diffuse lighting
    vec3 baseColor = lightColor * (ambientStrength * ambientColor + diffuseStrength * oceanColor);
    
    // Calculate shaded color with ambient and diffuse lighting
    vec3 shadedColor = lightColor * (ambientStrength * ambientColor + diffuseStrength * continentColor);

    vec3 result;
    // Create a "ground" in the form of a continent
    result = createGround(vec2(0.0, 1.0), 0.3, baseColor, shadedColor);
    result = createGround(vec2(0.425, 1.0), 0.1, result, shadedColor);

    // Create a "ground" in the form of the south pole
    vec3 poleShadedColor = lightColor * (ambientStrength * ambientColor + diffuseStrength * poleColor);
    // Creating the South Pole
    result = createGround(vec2(2.0, 4.0), 2.0, result, vec3(1.0));

    FragColor = vec4(result, 1.0);
}

)";
