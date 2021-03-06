#include "Obj.h"
#include <iostream>

int main()
{
    Object obj;
    obj.load("C:/Users/f/source/repos/Poly/Topex-Posidon-composite.obj");
    //obj.add_poly();

    for (int face_index = 0; face_index < obj._faces.size(); ++face_index)
    {
        std::cout << "face " << face_index << ": ";
        Face& face = obj._faces[face_index];
        face.triangulate();

        //печать исходных вершин
        for(const FaceElement& e: face._fs)
        {
            const GeoVertex vertex = e.geo_vertex(obj);
            std::cout << "(" << vertex.x << " " << vertex.y << " " << vertex.z << ")";
            std::cout << " ";
        }
        std::cout << std::endl;

        //печать треугольников
        for(const Triangle& triangle: face._triangles)
        {
            std::cout << triangle._a << " "<< triangle._b << " " << triangle._c;
        }
    }

    return 0;
}

