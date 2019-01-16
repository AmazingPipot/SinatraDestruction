#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#ifndef BOOLET_H
#define BOOLET_H

class Boolet : protected QOpenGLFunctions
{
public:
    Boolet(float x, float y, float w,float size);
    ~Boolet();

    float X;
    float Y;
    float Z;
    float Ray;
    float dirX,dirY,dirZ;

    void DrawStructure(QOpenGLShaderProgram *program);
    void applyForce();
    void giveForce(float fX,float fY,float fZ);
    bool collide(float x,float y,float z,float sizeX,float sizeY,float sizeZ);
private:

};

#endif
