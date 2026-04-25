#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

void main()
{
    vec4 totalPosition = vec4(0.0f);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if (boneIDs[i] == -1)
            continue;
        if (boneIDs[i] >= MAX_BONES)
        {
            totalPosition = vec4(aPos, 1.0);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIDs[i]] * vec4(aPos, 1.0);
        totalPosition += localPosition * weights[i];
    }

    mat4 viewModel = view * model;
    gl_Position = projection * viewModel * totalPosition;
}