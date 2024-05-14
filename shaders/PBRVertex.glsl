#version 410 core

// Declare MVP and normalMatrix uniforms
uniform mat4 MVP;
uniform mat3 normalMatrix;

// Vertex attributes
layout(location = 0) in vec3 inVert;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;

// Outputs
out vec3 worldPos;
out vec3 normal;

void main()
{
    // Transform vertex position to world space
    worldPos = vec3(MVP * vec4(inVert, 1.0));

    // Transform normal to world space
    normal = normalize(normalMatrix * inNormal);

    // Output position
    gl_Position = MVP * vec4(inVert, 1.0);
}
