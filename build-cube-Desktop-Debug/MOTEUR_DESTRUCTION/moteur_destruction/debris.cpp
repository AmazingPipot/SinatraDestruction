#include "debris.h"

Debris::Debris(float x, float y, float z, float w, float l, float h, float s) :
    X(x),
    Y(y),
    Z(z),
    L(l),
    W(w),
    H(h),
    Scale(s),
    //collision(0),
    indexBuf(QOpenGLBuffer::IndexBuffer)

{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    initDebris();
}


Debris::~Debris()
{
    //makeCurrent();
    arrayBuf.destroy();
    indexBuf.destroy();
    //doneCurrent();
}
//! [0]

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
    QVector4D normal;

};
void Debris::chute(){
    Z -= 0.1;
}

void Debris::testCollision(float z){
    printf("dd");
    if (Z < z){
        Z = z;
    }
}


void Debris::initDebris()
{
    // For cube we would need only 8 vertices but we have to
    // duplicate vertex for each face because texture coordinate
    // is different.
    VertexData vertices[24];

    VertexData vertex;
    for (int i = 0; i < 24; i++){
        vertex.position = QVector3D(X+Scale*((rand() % 5)-2.5),Y+Scale*((rand() % 5)-2.5),Z+Scale*((rand() % 5)-2.5));
        vertex.texCoord = QVector2D((float)(i % 2)/4.0, 0.5f+(i)%2/2.0);
        vertex.normal = QVector4D(0.0f, 0.0f, 0.0f,1.0f);
        vertices[i] = vertex;
    }

    // Indices for drawing cube faces using triangle strips.
    // Triangle strips can be connected by duplicating indices
    // between the strips. If connecting strips have opposite
    // vertex order then last index of the first strip and first
    // index of the second strip needs to be duplicated. If
    // connecting strips have same vertex order then only last
    // index of the first strip needs to be duplicated.
    GLushort indices[36];
    for (int i = 0, j = 0; i < 36; i+=6, j+= 4){
        indices[i] = j;
        indices[i+1] = j+1;
        indices[i+2] = j+2;
        indices[i+3] = j+2;
        indices[i+4] = j+3;
        indices[i+5] = j;
    }

//! [1]
    // Transfer vertex d-8.0f+iata to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, 24 * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, 36 * sizeof(GLushort));

//! [1]
}

//! [2]
void Debris::drawDebris(QOpenGLShaderProgram *program)
{
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, 34, GL_UNSIGNED_SHORT, 0);
}
