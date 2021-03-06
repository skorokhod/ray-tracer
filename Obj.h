#include <vector>
#include <string>

#pragma warning(disable : 4996)

struct PolyVector
{
    float x, y, z;
    friend std::ostream& operator<< (std::ostream& stream, const PolyVector& point) { stream << "(" << point.x << "," << point.y << "," << point.z << ")"; }
};

struct GeoVertex : public PolyVector
{
    GeoVertex(const std::string& line);
};

struct VertexNormal : public PolyVector
{
    VertexNormal(const std::string& line);
};

struct TextureVertex
{
    TextureVertex(const std::string& line);
    float u, v;
};

class Object;
class FaceElement
{
private:
    int v, vt, vn;
public:
    FaceElement(const std::string& line, const Object& obj);
    const TextureVertex& texture_vertex(const Object& obj) const;
    const GeoVertex& geo_vertex(const Object& obj) const;
    const VertexNormal& vertex_normal(const Object& obj) const;
};

struct Triangle
{
    Triangle(const GeoVertex& a, const GeoVertex& b, const GeoVertex& c) : is_shadowed(false), _a(a), _b(b), _c(c){}
    bool is_shadowed;
    GeoVertex _a, _b, _c;
};

struct Face
{
    Face(const std::string& line, const Object& obj);
    void triangulate();

    const Object& _obj;
    std::vector<Triangle> _triangles;
    std::vector<FaceElement> _fs;
};

class Object
{
private:
    bool _is_dot_float;

private:
    std::vector<GeoVertex>      _geo_vertices;
    std::vector<TextureVertex>  _tex_vertices;
    std::vector<VertexNormal>   _normals;

public:
    std::vector<Face>           _faces;

    const TextureVertex& getVt(uint16_t i)const;
    const GeoVertex& getV(uint16_t i)const;
    const VertexNormal& getVn(uint16_t i)const;

    Object();
    void load(const std::string& file_name);
};
