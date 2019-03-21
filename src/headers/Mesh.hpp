#ifndef MESH
#define MESH

#include <vector>
#include <iostream>
#include <string>
#include "GL/glew.h"
#include "glm/glm/glm.hpp"

class Mesh 
{
public:
    std::vector <glm::vec3> vertices;	        //	Vertices
    std::vector <glm::vec3> normals;	        //	Normals
    std::vector <glm::vec2> texCoord;        //	Texture
    std::vector <glm::uvec3> verticesIndices;  //  
    std::vector <glm::uvec3> normalsIndices;   //  
    std::vector <glm::uvec2> textCoordIndices; //  
    std::string mtllib;				        //

	Mesh() {}

    Mesh(std::vector <glm::vec3> vert, std::vector <glm::vec3> norm, std::vector <glm::vec2> text, std::string mtll)
    {
		vertices = vert;	//	Vertices
		normals = norm;		//	Normals
		texCoord = text;	//	Texture
		mtllib = mtll;		//
    }
};
#endif // !OBJECT
