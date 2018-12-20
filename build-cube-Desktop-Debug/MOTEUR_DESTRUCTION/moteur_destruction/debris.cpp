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

    /*VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D((X), (Y+Scale*L),  (Z)), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( (X), (Y+Scale*L),  (Z)), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D((X+Scale*W), (Y),  Scale*Z), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( (X), (Y),  Scale*Z), QVector2D(0.33f, 0.5f)}, // v3

        // VerteX data for face 1
        {QVector3D( (X), (Y+Scale*L),  Scale*Z), QVector2D( 0.0f, 0.5f)}, // v4
        {QVector3D( (X), (Y+Scale*L), (Z-Scale*H)),QVector2D(0.33f, 0.5f)}, // v5
        {QVector3D( (X), (Y),  (Z)), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( (X), (Y), (Z-Scale*H)), QVector2D(0.33f, 1.0f)}, // v7

        // VerteX data for face 2
        {QVector3D((X), (Y+Scale*L), (Z-Scale*H)), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D((X+Scale*W), (Y+Scale*L), (Z-Scale*H)), QVector2D(Scale*1.0f, 0.5f)},  // v9
        {QVector3D((X),  (Y), (Z-Scale*H)), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D((X+Scale*W),  (Y), (Z-Scale*H)), QVector2D(Scale*1.0f, 1.0f)},  // v11

        // VerteX data for face 3
        {QVector3D((X+Scale*W), (Y+Scale*L), (Z-Scale*H)), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D((X+Scale*W), (Y+Scale*L),  (Z)), QVector2D(Scale*1.0f, 0.0f)},  // v13
        {QVector3D((X+Scale*W), (Y), (Z-Scale*H)), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D((X+Scale*W), (Y),  Scale*1.0f), QVector2D(Scale*1.0f, 0.5f)},  // v15

        // VerteX data for face 4
        {QVector3D((X+Scale*W), (Y+Scale*L), (Z-Scale*H)), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( (X), (Y+Scale*L), (Z-Scale*H)), QVector2D(0.66f, 0.0f)}, // v17
        {QVector3D((X+Scale*W), (Y+Scale*L),  (Z)), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D((X), (Y+Scale*L),  (Z)), QVector2D(0.66f, 0.5f)}, // v19

        // VerteX data for face 5
        {QVector3D((X+Scale*W),  (Y),  (Z)), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D((X),  (Y),  (Z)), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D((X+Scale*W),  (Y), (Z-Scale*H)), QVector2D(0.33f, 1.0f)}, // v22
        {QVector3D((X),  (Y), (Z-Scale*H)), QVector2D(0.66f, 1.0f)}  // v23
    };*/
    /*float a, b, c;

    for (int i = 0; i < 2; i++) {
        if (i==0){
            a = 1;
            b = 1;
            c = 1;
        }
        else {
            a = -1;
            b = -1;
            c = -1;
        }
        VertexData vertex;
        vertex.position = QVector3D((X-a*0.5),(Y-b*0.5),(Z-c*0.5));
        vertex.texCoord = QVector2D(0.0f, 0.0f);
        vertices[i*12] = vertex;

        vertex.position = QVector3D((X+a*0.5),(Y-b*0.5),(Z-c*0.5));
        vertex.texCoord = QVector2D(0.0f, 0.33f);
        vertices[i*12+1] = vertex;

        vertex.position = QVector3D((X-a*0.5),(Y+b*0.5),(Z-c*0.5));
        vertex.texCoord = QVector2D(0.5f, 0.0f);
        vertices[i*12+2] = vertex;

        vertex.position = QVector3D((X+a*0.5),(Y+b*0.5),(Z-c*0.5));
        vertex.texCoord = QVector2D(0.5f, 0.33f);
        vertices[i*12+3] = vertex;





        vertex.position = QVector3D((X-a*0.5),(Y-b*0.5),(Z-c*0.5));
        vertex.position = QVector3D((X+a*0.5),(Y-b*0.5),(Z-c*0.5));
        vertex.position = QVector3D((X-a*0.5),(Y-b*0.5),(Z+c*0.5));
        vertex.position = QVector3D((X+a*0.5),(Y-b*0.5),(Z+c*0.5));

        vertex.texCoord = QVector2D(0.0f, 0.0f);
        vertex.texCoord = QVector2D(0.0f, 0.33f);
        vertex.texCoord = QVector2D(0.5f, 0.0f);
        vertex.texCoord = QVector2D(0.5f, 0.33f);


        vertex.position = QVector3D((X-a*0.5),(Y-b*0.5),(Z-c*0.5));
        vertex.position = QVector3D((X-a*0.5),(Y+b*0.5),(Z-c*0.5));
        vertex.position = QVector3D((X-a*0.5),(Y-b*0.5),(Z+c*0.5));
        vertex.position = QVector3D((X-a*0.5),(Y+b*0.5),(Z+c*0.5));

        vertex.texCoord = QVector2D(0.0f, 0.0f);
        vertex.texCoord = QVector2D(0.0f, 0.33f);
        vertex.texCoord = QVector2D(0.5f, 0.0f);
        vertex.texCoord = QVector2D(0.5f, 0.33f);



    }*/

    /*VertexData vertices[] = {
        // Vertex data for face 0
        {QVector3D(Scale*1.0f, -Scale*1.0f,  Scale*1.0f), QVector2D(0.0f, 0.0f)},  // v0
        {QVector3D( Scale*1.0f, -Scale*1.0f,  Scale*1.0f), QVector2D(0.33f, 0.0f)}, // v1
        {QVector3D(-Scale*1.0f,  Scale*1.0f,  Scale*1.0f), QVector2D(0.0f, 0.5f)},  // v2
        {QVector3D( Scale*1.0f,  Scale*1.0f,  Scale*1.0f), QVector2D(0.33f, 0.5f)}, // v3

        // Vertex data for face 1
        {QVector3D( Scale*1.0f, -Scale*1.0f,  Scale*1.0f), QVector2D( 0.0f, 0.5f)}, // v4
        {QVector3D( Scale*1.0f, -Scale*1.0f, -Scale*1.0f),QVector2D(0.33f, 0.5f)}, // v5
        {QVector3D( Scale*1.0f,  Scale*1.0f,  Scale*1.0f), QVector2D(0.0f, 1.0f)},  // v6
        {QVector3D( Scale*1.0f,  Scale*1.0f, -Scale*1.0f), QVector2D(0.33f, 1.0f)}, // v7

        // Vertex data for face 2
        {QVector3D( Scale*1.0f, -Scale*1.0f, -Scale*1.0f), QVector2D(0.66f, 0.5f)}, // v8
        {QVector3D(-Scale*1.0f, -Scale*1.0f, -Scale*1.0f), QVector2D(Scale*1.0f, 0.5f)},  // v9
        {QVector3D( Scale*1.0f,  Scale*1.0f, -Scale*1.0f), QVector2D(0.66f, 1.0f)}, // v10
        {QVector3D(-Scale*1.0f,  Scale*1.0f, -Scale*1.0f), QVector2D(Scale*1.0f, 1.0f)},  // v11

        // Vertex data for face 3
        {QVector3D(-Scale*1.0f, -Scale*1.0f, -Scale*1.0f), QVector2D(0.66f, 0.0f)}, // v12
        {QVector3D(-Scale*1.0f, -Scale*1.0f,  Scale*1.0f), QVector2D(Scale*1.0f, 0.0f)},  // v13
        {QVector3D(-Scale*1.0f,  Scale*1.0f, -Scale*1.0f), QVector2D(0.66f, 0.5f)}, // v14
        {QVector3D(-Scale*1.0f,  Scale*1.0f,  Scale*1.0f), QVector2D(Scale*1.0f, 0.5f)},  // v15

        // Vertex data for face 4
        {QVector3D(-Scale*1.0f, -Scale*1.0f, -Scale*1.0f), QVector2D(0.33f, 0.0f)}, // v16
        {QVector3D( Scale*1.0f, -Scale*1.0f, -Scale*1.0f), QVector2D(0.66f, 0.0f)}, // v17
        {QVector3D(-Scale*1.0f, -Scale*1.0f,  Scale*1.0f), QVector2D(0.33f, 0.5f)}, // v18
        {QVector3D( Scale*1.0f, -Scale*1.0f,  Scale*1.0f), QVector2D(0.66f, 0.5f)}, // v19

        // Vertex data for face 5
        {QVector3D(-Scale*1.0f,  Scale*1.0f,  Scale*1.0f), QVector2D(0.33f, 0.5f)}, // v20
        {QVector3D( Scale*1.0f,  Scale*1.0f,  Scale*1.0f), QVector2D(0.66f, 0.5f)}, // v21
        {QVector3D(-Scale*1.0f,  Scale*1.0f, -Scale*1.0f), QVector2D(0.33f, 1.0f)}, // v22
        {QVector3D( Scale*1.0f,  Scale*1.0f, -Scale*1.0f), QVector2D(0.66f, 1.0f)}  // v23
    };*/

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
    /*GLushort indices[] = {
         0,  1,  2,  1,  3, 2,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
         4,  5,  6,  6,  7,  4, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
         8,  9,  10, 10, 11, 8, // Face 2 - triangle strip ( v8,  v9, v10, v11)
        12, 13, 14, 14, 15, 12, // Face 3 - triangle strip (v12, v13, v14, v15)
        16, 17, 18, 18, 19, 16, // Face 4 - triangle strip (v16, v17, v18, v19)
        20, 21, 22, 22, 23, 20     // Face 5 - triangle strip (v20, v21, v22, v23)
    };*/
    //printf("%d %d %d",indices[0],indices[1],indices[2]);

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
