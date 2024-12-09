#version 330 core
layout(location = 0) in vec3 vPos;
//layout (location = 1) in vec3 in_Color;   //--- attribute로 설정된 위치 속성: 인덱스 1
layout(location = 1) in vec3 vNormal;

out vec3 FragPos;
out vec3 Normal;
//out vec3 out_Color;
uniform mat4 modelTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

void main() {
    gl_Position = projectionTransform * viewTransform * modelTransform * vec4(vPos, 1.0);
    FragPos = vec3(modelTransform * vec4(vPos, 1.0));
    Normal = mat3(transpose(inverse(modelTransform))) * vNormal; // 모델 행렬 변환 반영

    //out_Color = in_Color;
}
