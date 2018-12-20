#ifndef GESTIONBATIMENT_H
#define GESTIONBATIMENT_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QImage>
#include <QOpenGLTexture>

#include "quadtree.h"


class GestionBatiment : protected QOpenGLFunctions
{
public:
    GestionBatiment();
    explicit GestionBatiment(int, float, float, float);
    int typeBat;
    float X;
    float Y;
    float Z;
    virtual ~GestionBatiment();
    void constructionBat();

    QImage CarteBatiment;
};

#endif // GESTIONBATIMENT_H
