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
    };

    struct Face {
        unsigned int v1, v2, v3; // ���� �ε���
    };

    std::vector<Model::Vertex> vertices;
    std::vector<glm::vec3> tempNormals;
    std::vector<Model::Face> faces;

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
                    vertices.push_back({ position, glm::vec3(0.0f) }); // �ʱ� ��� ����
                    //std::cout << "Vertex loaded: " << position.x << ", " << position.y << ", " << position.z << std::endl;
                }
                else {
                    std::cerr << "Error parsing vertex: " << line << std::endl;
                }
            }
            else if (prefix == "vn") {
                glm::vec3 normal;
                if (iss >> normal.x >> normal.y >> normal.z) {
                    tempNormals.push_back(normal);
                    //std::cout << "Normal loaded: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
                }
                else {
                    std::cerr << "Error parsing normal: " << line << std::endl;
                }
            }
            else if (prefix == "f") {
                std::vector<unsigned int> vIndices, nIndices;
                std::string vertex;
                while (iss >> vertex) {
                    size_t pos1 = vertex.find("//");
                    if (pos1 != std::string::npos) {
                        // v1//n1 ����
                        unsigned int vIndex = std::stoi(vertex.substr(0, pos1)) - 1;
                        unsigned int nIndex = std::stoi(vertex.substr(pos1 + 2)) - 1;
                        vIndices.push_back(vIndex);
                        nIndices.push_back(nIndex);
                    }
                    else {
                        // v1 ����
                        unsigned int vIndex = std::stoi(vertex) - 1;
                        vIndices.push_back(vIndex);
                    }
                }
                if (vIndices.size() == 3) {
                    faces.push_back({ vIndices[0], vIndices[1], vIndices[2] });
                    //std::cout << "Face loaded: " << vIndices[0] << ", " << vIndices[1] << ", " << vIndices[2] << std::endl;
                    vertices[vIndices[0]].normal += tempNormals[nIndices[0]];
                    vertices[vIndices[1]].normal += tempNormals[nIndices[1]];
                    vertices[vIndices[2]].normal += tempNormals[nIndices[2]];
                }
                else {
                    std::cerr << "Error parsing face: " << line << std::endl;
                }
            }
        }

        file.close();

        // ����� ���
        //std::cout << "Vertices loaded: " << vertices.size() << std::endl;
        //std::cout << "Faces loaded: " << faces.size() << std::endl;
    }


    void calculateNormals(std::vector<Model::Vertex>& vertices, const std::vector<Model::Face>& faces) {
        // ��� ������ ������ �ʱ�ȭ
        for (auto& vertex : vertices) {
            vertex.normal = glm::vec3(0.0f);
        }

        // �� �ﰢ���� ������ ����ϰ� ������ �߰�
        for (const auto& face : faces) {
            glm::vec3 v0 = vertices[face.v1].position;
            glm::vec3 v1 = vertices[face.v2].position;
            glm::vec3 v2 = vertices[face.v3].position;

            glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
            vertices[face.v1].normal += normal;
            vertices[face.v2].normal += normal;
            vertices[face.v3].normal += normal;
        }

        // ��� ������ ����ȭ
        for (auto& vertex : vertices) {
            vertex.normal = glm::normalize(vertex.normal);
        }
    }

};