#ifndef SUPERSTRUCTURE_H
#define SUPERSTRUCTURE_H
//#include "destructmesh.h"
#include "geometryengine.h"
#include <QList>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#ifndef MESHSTRUCT
#define MESHSTRUCT

typedef struct point{
    float x;
    float y;
    float z;
}Point;

typedef struct cube{
    float x,y,z;
    float sizeX,sizeY,sizeZ;
    float dirX,dirY,dirZ;
    Point center;
}Cube;

typedef struct mesh{
    Point* tab;
    int** links;
    int nbp;
    int nbl;
    Cube cube;
    int** neighbours;
    int* nbNeighbours;
}Mesh;

#endif

#define BrickSize 1

class SuperStructure : protected QOpenGLFunctions
{
public:
    SuperStructure(float x, float y, float w, float z, float h, float d, int type, GeometryEngine* geo);
    ~SuperStructure();
    QList<SuperStructure*> listStructure;
    //QList<Mesh> listElementDestructible;

    float X;
    float Y;
    float Z;

    float Width;
    float Height;
    float Depth;

    int typeStruct;

    void initStruct();
    QVector<Mesh*> destroyStruct();
    void DrawStructure(QOpenGLShaderProgram *program);

private:
    GeometryEngine* terrain;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // SUPERSTRUCTURE_H
