#ifndef SUPERSTRUCTURE_H
#define SUPERSTRUCTURE_H
//#include "destructmesh.h"
#include "geometryengine.h"
#include <QList>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

class SuperStructure : protected QOpenGLFunctions
{
public:
    SuperStructure(float x, float y, float w, float z, float h, float d, int type, GeometryEngine* geo);
    ~SuperStructure();
    QList<SuperStructure> listStructure;
    //QList<Mesh> listElementDestructible;

    float X;
    float Y;
    float Z;

    float Width;
    float Height;
    float Depth;

    int typeStruct;

    void initStruct();
    void DrawStructure(QOpenGLShaderProgram *program);

private:
    GeometryEngine* terrain;

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // SUPERSTRUCTURE_H
