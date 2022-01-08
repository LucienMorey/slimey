#version 460 core

layout (local_size_x = 8, local_size_y = 8) in;
layout (rgba32f) uniform readonly image2D imgInput;
layout (rgba32f) uniform image2D imgOutput;
// layout(rgba32f, binding = 1) uniform image2D imgOutput;
uniform float deltaTime;

const float evaporateSpeed = 0.9;
const float diffuseSpeed = 5.0;

void main()
{
    ivec2 imgSize = imageSize(imgOutput);
    ivec2 pixelCoords = ivec2(gl_GlobalInvocationID.xy);
    vec4 originalValue = imageLoad(imgOutput, pixelCoords);

    vec4 sum = vec4(0.0);
    for (int x_off = -1; x_off <= 1; x_off++)
        for (int y_off = -1; y_off <= 1; y_off++)
        {
            ivec2 sampleCoords = ivec2(pixelCoords.x + x_off, pixelCoords.y + y_off);
            if (sampleCoords.x >= 0 && sampleCoords.x < imgSize.x && sampleCoords.y >= 0 && sampleCoords.y < imgSize.y)
                sum += imageLoad(imgOutput, sampleCoords);
        }

    vec4 blurred = sum / 9.0;
    float diffuseWeight = min(1.0, max(0.0, diffuseSpeed * deltaTime));
    vec4 blurredColour = originalValue * (1-diffuseWeight) + blurred * diffuseWeight;

    vec4 processedValue = max(blurredColour - evaporateSpeed*deltaTime, 0);

    imageStore(imgOutput, pixelCoords, processedValue);
}