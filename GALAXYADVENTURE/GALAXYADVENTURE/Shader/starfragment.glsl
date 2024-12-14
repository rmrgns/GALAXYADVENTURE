#version 330 core

in vec3 FragPos;      // Vertex Shader에서 전달받은 월드 좌표
in vec3 Normal;       // Vertex Shader에서 전달받은 노멀

out vec4 FragColor;   // 최종 출력 색상

uniform vec3 lightPos;     // 빛의 위치
uniform vec3 lightColor;   // 빛의 색상
uniform vec3 objectColor;  // 물체의 색상
uniform vec3 viewPos;      // 카메라의 위치
uniform float emissionPower = 1.0f; // 방출 강도

void main() {
    // Ambient (주변광)
    vec3 ambientLight = vec3(0.3); // 고정된 주변광 비율
    vec3 ambient = ambientLight * lightColor;

    // Diffuse (확산광)
    vec3 norm = normalize(Normal);       // 노멀 벡터 정규화
    vec3 lightDir = normalize(lightPos - FragPos); // 빛의 방향
    float diffuseStrength = max(dot(norm, lightDir), 0.0); // Lambert 법칙
    vec3 diffuse = diffuseStrength * lightColor;

    // Specular (반사광)
    vec3 viewDir = normalize(viewPos - FragPos);       // 카메라 방향
    vec3 reflectDir = reflect(-lightDir, norm);        // 빛의 반사 방향
    float shininess = 32.0;                            // 광택 강도
    float specularStrength = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * lightColor;

    // Emission (방출광)
    vec3 emission = lightColor * emissionPower;
    
    // 최종 조명 계산
    vec3 result = (ambient + diffuse + specular + emission) * objectColor;
    FragColor = vec4(result, 1.0);
}
