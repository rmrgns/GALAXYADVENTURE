#pragma once

#include <sstream>
#include <stdexcept>
#include "common.h"

class Model
{
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord; // 텍스처 좌표 추가
    };

    struct Face {
        unsigned int v1, v2, v3; // 정점 인덱스
    };

    std::vector<Model::Vertex> vertices;
    std::vector<glm::vec3> tempNormals;
    std::vector<Model::Face> faces;
    std::vector<glm::vec2> tempTexCoords; // 임시로 저장할 텍스처 좌표

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;

            if (prefix == "v") {
                glm::vec3 position;
                if (iss >> position.x >> position.y >> position.z) {
                    vertices.push_back({ position, glm::vec3(0.0f) });
                }
            }
            else if (prefix == "vn") {
                glm::vec3 normal;
                if (iss >> normal.x >> normal.y >> normal.z) {
                    tempNormals.push_back(normal);
                }
            }
            else if (prefix == "vt") {
                glm::vec2 texCoord;
                if (iss >> texCoord.x >> texCoord.y) {
                    tempTexCoords.push_back(texCoord);
                }
            }
            else if (prefix == "f") {
                std::vector<unsigned int> vIndices, tIndices, nIndices;
                std::string vertex;
                while (iss >> vertex) {
                    size_t pos1 = vertex.find('/');
                    size_t pos2 = vertex.find_last_of('/');

                    if (pos1 != std::string::npos && pos2 != std::string::npos && pos1 != pos2) {
                        unsigned int vIndex = std::stoi(vertex.substr(0, pos1)) - 1;
                        unsigned int tIndex = std::stoi(vertex.substr(pos1 + 1, pos2 - pos1 - 1)) - 1;
                        unsigned int nIndex = std::stoi(vertex.substr(pos2 + 1)) - 1;

                        vIndices.push_back(vIndex);
                        tIndices.push_back(tIndex);
                        nIndices.push_back(nIndex);
                    }
                }
                if (vIndices.size() == 3 && tIndices.size() == 3 && nIndices.size() == 3) {
                    faces.push_back({ vIndices[0], vIndices[1], vIndices[2] });

                    // 정상적인 인덱스라면, 텍스처 좌표와 노멀을 추가
                    if (vIndices[0] < vertices.size() && tIndices[0] < tempTexCoords.size() && nIndices[0] < tempNormals.size()) {
                        vertices[vIndices[0]].texCoord = tempTexCoords[tIndices[0]];
                        vertices[vIndices[1]].texCoord = tempTexCoords[tIndices[1]];
                        vertices[vIndices[2]].texCoord = tempTexCoords[tIndices[2]];
                        vertices[vIndices[0]].normal += tempNormals[nIndices[0]];
                        vertices[vIndices[1]].normal += tempNormals[nIndices[1]];
                        vertices[vIndices[2]].normal += tempNormals[nIndices[2]];
                    }
                }
            }
        }

        file.close();
    }



    void calculateNormals(std::vector<Model::Vertex>& vertices, const std::vector<Model::Face>& faces) {
        // 모든 정점의 법선을 초기화

        for (auto& vertex : vertices) {
            vertex.normal = glm::vec3(0.0f);
        }

        // 각 삼각형의 법선을 계산하고 정점에 추가
        for (const auto& face : faces) {
            glm::vec3 v0 = vertices[face.v1].position;
            glm::vec3 v1 = vertices[face.v2].position;
            glm::vec3 v2 = vertices[face.v3].position;

            glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
            vertices[face.v1].normal += normal;
            vertices[face.v2].normal += normal;
            vertices[face.v3].normal += normal;
        }

        // 모든 법선을 정규화
        for (auto& vertex : vertices) {
            vertex.normal = glm::normalize(vertex.normal);
        }
    }

};
