#ifndef SUPERSTRUCTURE_H
#define SUPERSTRUCTURE_H
//#include "destructmesh.h"
#include <QList>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class SuperStructure
{
public:
    SuperStructure(float x, float y, float w, float h, float d, int type);
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

private:
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // SUPERSTRUCTURE_H
