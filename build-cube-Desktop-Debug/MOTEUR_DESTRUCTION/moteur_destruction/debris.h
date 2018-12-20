#ifndef DEBRIS_H
#define DEBRIS_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QImage>
#include <QOpenGLTexture>

class Debris : protected QOpenGLFunctions
{
public:
    explicit Debris(float, float, float, float, float, float, float);
    ~Debris();
    float X;
    float Y;
    float Z;

    float W;
    float H;
    float L;

    float Scale;

    void initDebris();
    void drawDebris(QOpenGLShaderProgram *program);
    void chute();
    void testCollision(float);


    //int collision;

private :
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
};

#endif // DEBRIS_H
