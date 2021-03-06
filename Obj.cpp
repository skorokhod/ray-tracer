#include "Obj.h"
#include <array>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <assert.h>
#include "Triangulate.h"

#pragma warning(disable : 4996)

GeoVertex::GeoVertex(const std::string& line)
{
    int count = std::sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
    assert(count == 3);
}

VertexNormal::VertexNormal(const std::string& line)
{
    int count = std::sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
    assert(count == 3);
}

TextureVertex::TextureVertex(const std::string& line)
{
    int count = std::sscanf(line.c_str(), "vt %f %f", &u, &v);
    assert(count == 2);
}

Face::Face(const std::string& line, const Object& obj) : _obj(obj)
{
    std::size_t start = 2;
    std::size_t end = std::string::npos;

    do
    {
        end = line.find(" ", start+1);
        if (end == std::string::npos)
            end = line.size();
        _fs.push_back(FaceElement(line.substr(start+1, end-start-1), obj));
        start = end;
    }
    while(end < line.size());
    assert(_fs.size() > 2);
	assert(_fs.size() <= 3);
}

const TextureVertex& Object::getVt(uint16_t i)const
{
    assert(i > 1 && i < _tex_vertices.size());
    return _tex_vertices[i - 1];
}
const GeoVertex& Object::getV(uint16_t i)const
{
    assert(i > 0 && i <= _geo_vertices.size());
    return _geo_vertices[i - 1];
}
const VertexNormal& Object::getVn(uint16_t i)const
{
    assert(i > 1 && i < _normals.size());
    return _normals[i - 1];
}

Object::Object()
{
    float x, y;
    if (std::sscanf("123.4 234.5", "%f %f", &x, &y) == 2)
        _is_dot_float = true;
    else if (std::sscanf("123,4 234,5", "%f %f", &x, &y) == 2)
        _is_dot_float = false;
    else
    {
        assert(false);
    }
}

void Object::load(const std::string& file_name)
{
    std::ifstream f(file_name);
    std::string line;


    while (std::getline(f, line))
    {
        if (line.empty())
            continue;

        if (!_is_dot_float)
            std::replace(line.begin(), line.end(), '.', ',');

        if (line.substr(0, 2) == "v ")
            _geo_vertices.push_back(GeoVertex(line));
        else if (line.substr(0, 3) == "vt ")
            _tex_vertices.push_back(TextureVertex(line));
        else if (line.substr(0, 3) == "vn ")
            _normals.push_back(VertexNormal(line));
        if (line.substr(0, 2) == "f ")
            _faces.push_back(Face(line, *this));
    }
}

FaceElement::FaceElement(const std::string& line, const Object& obj) : v(0), vt(0), vn(0)
{
    if (line.find("//") == std::string::npos)
    {
        int count = std::sscanf(line.c_str(), "%d/%d/%d", &v, &vt, &vn);
        assert(count == 3);
    }
    else
    {
        int count = std::sscanf(line.c_str(), "%d//%d", &v, &vn);
        vt = 0;
        assert(count == 2);
    }
}

const TextureVertex& FaceElement::texture_vertex(const Object& obj) const { return obj.getVt(vt); }
const GeoVertex& FaceElement::geo_vertex(const Object& obj) const { return obj.getV(v); }
const VertexNormal&  FaceElement::vertex_normal(const Object& obj) const { return obj.getVn(vn); }


void Face::triangulate()
{
    Vector2dVector source_flat; //поверхность в XY

    for(const FaceElement& i: _fs)
    {
        const GeoVertex& geo = i.geo_vertex(_obj);
        const TextureVertex& tex = i.texture_vertex(_obj);
        const VertexNormal& normal = i.vertex_normal(_obj);
        //////////////
        source_flat.push_back(); //заполняем массив вершин, лежащих в XY
    }

    Vector2dVector result_flat;
    bool isOk = Triangulate::Process(source_flat, result_flat);
    assert(isOk);
    assert(result_flat.size() % 3 == 0);

    for(int i = 0; i < result_flat.size()/3; ++i)
    {
        Vector2d a = result_flat[i];
        Vector2d b = result_flat[i + 1];
        Vector2d c = result_flat[i + 2]

        //повернуть a b c из XY в XYZ -> GeoVertex _1, _2, _3
        ///////////////
        _triangles.emplace_back(Triangle(_1, _2, _3));
    }
}
