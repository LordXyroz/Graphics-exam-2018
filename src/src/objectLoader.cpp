#include "../headers/objectLoader.hpp"
#include <fstream>
#include <string>
#include "../headers/logger.h"

std::vector <Mesh> loadObject(std::string fileName)
{
    std::vector <Mesh> obj;
    std::ifstream in;
    char mtllib[100];
    FileExt fileExt;

    if (fileName.substr(fileName.find_last_of(".") + 1) == "obj")
        fileExt = OBJ;

    switch (fileExt)
    {
    case OBJ:
        in.open(fileName);
        if (!in.is_open())
        {
            //std::cout << "Failed to open object file\n\n";
            LOG_DEBUG(("Failed to open object: " + fileName).c_str());
        }
        else
        {
            std::vector<glm::vec3> vertices;
            std::vector<glm::vec2> texCoord;
            std::vector<glm::vec3> normals;
            char dummy[100];
            in.ignore(100, '\n');
            in.ignore(100, '\n');
            in.ignore(10, ' ');
            in.getline(mtllib, 100);

            while (!in.eof())
            {
                in.getline(dummy, 100, ' ');
                if (dummy[0] == 'o' || dummy[0] == 'g')
                {
                    obj.resize(obj.size() + 1);
                    obj[obj.size() - 1].mtllib = mtllib;
                }
                else if (dummy[0] == 'v' && dummy[1] == 'n')
                {
                    //std::cout << "vn\n";
                    //obj[obj.size() - 1].normals.resize(obj[obj.size() - 1].normals.size() + 1);
                    //normals.resize(normals.size() + 1);
                    GLfloat x, y, z;
                    in >> x;
                    in >> y;
                    in >> z;
                    normals.push_back(glm::vec3(x, y, z));
                    //obj[obj.size() - 1].normals[obj[obj.size() - 1].normals.size() - 1] = glm::vec3(x, y, z);
                    //in >> obj[obj.size() - 1].normals[obj[obj.size() - 1].normals.size() - 3];
                    //in >> obj[obj.size() - 1].normals[obj[obj.size() - 1].normals.size() - 2];
                    //in >> obj[obj.size() - 1].normals[obj[obj.size() - 1].normals.size() - 1];
                    //std::cout << obj.normals[obj.normals.size() - 3] << ' ' << obj.normals[obj.normals.size() - 2] << ' ' << obj.normals[obj.normals.size() - 1] << std::endl;

                }
                else if (dummy[0] == 'v' && dummy[1] == 't')
                {
                    //obj[obj.size() - 1].texCoord.resize(obj[obj.size() - 1].texCoord.size() + 1);
                    //texCoord.resize(texCoord.size() + 1);
                    GLfloat x, y;
                    in >> x;
                    in >> y;
                    texCoord.push_back(glm::vec2(x, y));
                    //obj[obj.size() - 1].texCoord[obj[obj.size() - 1].texCoord.size() - 1] = glm::vec2(x, y);
                    //in >> obj[obj.size() - 1].texCoord[obj[obj.size() - 1].texCoord.size() - 2];
                    //in >> obj[obj.size() - 1].texCoord[obj[obj.size() - 1].texCoord.size() - 1];
                }
                else if (dummy[0] == 'v')
                {
                    //std::cout << "v\n";
                    //obj[obj.size() - 1].vertices.resize(obj[obj.size() - 1].vertices.size() + 1);
                    //vertices.resize(vertices.size() + 1);
                    GLfloat x, y, z;
                    in >> x;
                    in >> y;
                    in >> z;
                    vertices.push_back(glm::vec3(x, y, z));
                    //obj[obj.size() - 1].vertices[obj[obj.size() - 1].vertices.size() - 1] = glm::vec3(x, y, z);
                    //in >> obj[obj.size() - 1].vertices[obj[obj.size() - 1].vertices.size() - 3];
                    //in >> obj[obj.size() - 1].vertices[obj[obj.size() - 1].vertices.size() - 2];
                    //in >> obj[obj.size() - 1].vertices[obj[obj.size() - 1].vertices.size() - 1];
                    //std::cout << obj.vertices[obj.vertices.size() - 3] << ' ' << obj.vertices[obj.vertices.size() - 2] << ' ' << obj.vertices[obj.vertices.size() - 1] << std::endl;
                }
                else if (dummy[0] == 'f')
                {
                    //float v = 0, n = 0, t = 0;
                    std::vector<unsigned int> v, n, t;
                    v.resize(3);
                    n.resize(3);
                    t.resize(3);

                    for (int i = 0; i < 3; i++)
                    {
                        in >> v[i];
                        in.ignore(10, '/');
                        if(texCoord.size() != 0)
                            in >> t[i];
                        in.ignore(10, '/');
                        in >> n[i];
                        v[i]--;
                        t[i]--;
                        n[i]--;
                    }

                    /*obj[obj.size() - 1].verticesIndices.resize(obj[obj.size() - 1].verticesIndices.size() + 1);
                    obj[obj.size() - 1].verticesIndices[obj[obj.size() - 1].verticesIndices.size() - 1] = glm::uvec3(v[0], v[1], v[2]);*/
                    //obj[obj.size() - 1].vertices.resize(obj[obj.size() - 1].vertices.size() + 3);
                    obj[obj.size() - 1].vertices.push_back(vertices[v[0]]);
                    obj[obj.size() - 1].vertices.push_back(vertices[v[1]]);
                    obj[obj.size() - 1].vertices.push_back(vertices[v[2]]);

                    if (texCoord.size() != 0)
                    {
                        /*obj[obj.size() - 1].textCoordIndices.resize(obj[obj.size() - 1].textCoordIndices.size() + 1);
                        obj[obj.size() - 1].textCoordIndices[obj[obj.size() - 1].textCoordIndices.size() - 1] = glm::uvec2(t[0], t[1]);*/
                        //obj[obj.size() - 1].texCoord.resize(obj[obj.size() - 1].texCoord.size() + 3);
                        obj[obj.size() - 1].texCoord.push_back(texCoord[t[0]]);
                        obj[obj.size() - 1].texCoord.push_back(texCoord[t[1]]);
                        obj[obj.size() - 1].texCoord.push_back(texCoord[t[2]]);
                    }

                    /*obj[obj.size() - 1].normalsIndices.resize(obj[obj.size() - 1].normalsIndices.size() + 1);
                    obj[obj.size() - 1].normalsIndices[obj[obj.size() - 1].normalsIndices.size() - 1] = glm::uvec3(n[0], n[1], n[2]);*/
                    //obj[obj.size() - 1].normals.resize(obj[obj.size() - 1].normals.size() + 3);
                    obj[obj.size() - 1].normals.push_back(normals[n[0]]);
                    obj[obj.size() - 1].normals.push_back(normals[n[1]]);
                    obj[obj.size() - 1].normals.push_back(normals[n[2]]);
                }
                in.ignore(100, '\n');
            }
        }

        break;
    default:
        //std::cout << "File format not supported\nEmpty Object returned\n\n";
        LOG_DEBUG("File format not supported. Empty Object returned");
        break;
    }
    return obj;
}
