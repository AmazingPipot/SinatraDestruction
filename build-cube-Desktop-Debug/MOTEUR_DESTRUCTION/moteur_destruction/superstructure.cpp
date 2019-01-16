#include "superstructure.h"
//#include "destructmesh.h"

SuperStructure::SuperStructure(float x, float y, float z, float w, float h, float d, int type, GeometryEngine* geo)
    :indexBuf(QOpenGLBuffer::IndexBuffer),
      X(x),
      Y(y),
      Z(z),
      Width(w),
      Height(h),
      Depth(d),
      typeStruct(type),
      terrain(geo)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();
    if (terrain->positionAltitude(X,Y) != Z && typeStruct == 0)
    {
        Z = terrain->positionAltitude(X,Y);
    }
    if (X > terrain->tailleMap - Width)
    {
        X = terrain->tailleMap - Width;
    }
    if (Y > terrain->tailleMap - Height)
    {
        Y = terrain->tailleMap - Height;
    }
    for (int i = 0; i < Depth; i++)
    {
        //SuperStructure* A = new SuperStructure(X,Y,Z+i * 1.0f,Width,Height,1.0f, typeStruct+1,terrain);
        //listStructure.append(A);
    }
    initStruct();
}

SuperStructure::~SuperStructure()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
    QVector4D normal;

};

void SuperStructure::initStruct()
{
    VertexData vertices[24];

    float tex1 = 40.0f/512.0f;
    float tex2 = 60.0f/512.0f;
    float tex3 = 20.0f/512.0f;

    float toit1 = 60.0f/512.0f;
    float toit2 = 80.0f/512.0f;
    float toit3 = 20.0f/512.0f;
    //VertexData* VD = (VertexData*) malloc(sizeof(VertexData)*24);
    VertexData VD[24];
    /*for (int i = 0; i < 24; i++){
        VD[i].normal = QVector4D(0.0f, 0.0f, 0.0f,1.0f);
    }*/
    //////Face 1 Devant
    VD[0]={QVector3D(X,Y,Z), QVector2D(tex1, 0.0f),QVector4D(0.0f, 0.0f, .0f,1.0f)};
    VD[1]={QVector3D(X+Width,Y,Z), QVector2D(tex2, 0.0f),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    VD[2]={QVector3D(X+Width,Y+Height,Z), QVector2D(tex2, tex3),QVector4D(0.0f, 1.0f, 0.0f,1.0f)};
    VD[3]={QVector3D(X,Y+Height,Z), QVector2D(tex1, tex3),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    //////Face Dessus
    VD[4]={QVector3D(X,Y,Z), QVector2D(tex1, 0.0f),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    VD[5]={QVector3D(X,Y,Z+Depth), QVector2D(tex2, 0.0f),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    VD[6]={QVector3D(X+Width,Y,Z+Depth), QVector2D(tex2, tex3),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    VD[7]={QVector3D(X+Width,Y,Z), QVector2D(tex1, tex3),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    //////Face Derrière
    VD[8]={QVector3D(X,Y,Z+Depth), QVector2D(toit1, 0.0f),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    VD[9]={QVector3D(X,Y+Height,Z+Depth), QVector2D(toit2, 0.0f),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    VD[10]={QVector3D(X+Width,Y+Height,Z+Depth), QVector2D(toit2, toit3),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    VD[11]={QVector3D(X+Width,Y,Z+Depth), QVector2D(toit1, toit3),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    //////Face Dessous
    VD[12]={QVector3D(X,Y+Height,Z), QVector2D(tex1, 0.0f),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    VD[13]={QVector3D(X+Width,Y+Height,Z), QVector2D(tex2, 0.0f),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    VD[14]={QVector3D(X+Width,Y+Height,Z+Depth), QVector2D(tex2, tex3),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    VD[15]={QVector3D(X,Y+Height,Z+Depth), QVector2D(tex1, tex3),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    //////Face Gauche
    VD[16]={QVector3D(X,Y,Z), QVector2D(tex1, 0.0f),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    VD[17]={QVector3D(X,Y+Height,Z), QVector2D(tex2, 0.0f),QVector4D(0.0f, 0.0f, 1.0f,1.0f)};
    VD[18]={QVector3D(X,Y+Height,Z+Depth), QVector2D(tex2, tex3),QVector4D(0.0f, 0.0f, 0.0f,1.0f)};
    VD[19]={QVector3D(X,Y,Z+Depth), QVector2D(tex1, tex3),QVector4D(0.0f, 0.0f, 0.0f,1.0f)};
    //////Face Droite
    VD[20]={QVector3D(X+Width,Y,Z), QVector2D(tex1, 0.0f),QVector4D(0.0f, 0.0f, 0.0f,1.0f)};
    VD[21]={QVector3D(X+Width,Y,Z+Depth), QVector2D(tex2, 0.0f),QVector4D(0.0f, 0.0f, 0.0f,1.0f)};
    VD[22]={QVector3D(X+Width,Y+Height,Z+Depth), QVector2D(tex2, tex3),QVector4D(0.0f, 0.0f, 0.0f,1.0f)};
    VD[23]={QVector3D(X+Width,Y+Height,Z), QVector2D(tex1, tex3),QVector4D(0.0f, 0.0f, 0.0f,1.0f)};

    GLushort indices[36];
    for(int i=0,j=0;i<36;i+=6,j+=4){
        indices[i] = j;
        indices[i+1] = j+1;
        indices[i+2] = j+2;
        indices[i+3] = j+2;
        indices[i+4] = j+3;
        indices[i+5] = j;
    }
    arrayBuf.bind();
    arrayBuf.allocate(VD,24*sizeof(VertexData));
    indexBuf.bind();
    indexBuf.allocate(indices,36*sizeof(GLushort));
}

void SuperStructure::DrawStructure(QOpenGLShaderProgram *program)
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

    // Offset for texure coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texure coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}
