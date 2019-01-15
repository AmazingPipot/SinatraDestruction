#ifndef SUPERSTRUCTURE_H
#define SUPERSTRUCTURE_H
//#include "destructmesh.h"
#include <QList>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

class SuperStructure : protected QOpenGLFunctions
{
public:
    SuperStructure(float x, float y, float w, float h, float d, int type);
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
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // SUPERSTRUCTURE_H
