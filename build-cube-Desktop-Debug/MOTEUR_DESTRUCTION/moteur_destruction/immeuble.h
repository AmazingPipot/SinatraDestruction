#ifndef IMMEUBLE_H
#define IMMEUBLE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QImage>
#include <QOpenGLTexture>

class Immeuble
{
public:
    explicit Immeuble();

    /*float X;
    float Y;
    float Z;

    float W;
    float H;
    float L;*/

    float Scale;

    void initDebris();
    void drawDebris(QOpenGLShaderProgram *program);
    void chute();
    void testCollision(float);

};

#endif // IMMEUBLE_H
