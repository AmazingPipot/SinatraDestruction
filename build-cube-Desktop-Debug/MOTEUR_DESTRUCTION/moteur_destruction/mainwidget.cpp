/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mainwidget.h"
#include "destructmesh.h"
#include <QMouseEvent>

#include <math.h>
#include <sys/time.h>
#include <unistd.h>


static float s = 1.0;
static float SCALE = 0.1;
clock_t TimeDebut = 0.0, TimeFin = 0.0;
float temps;

int load = 0;

MainWidget::MainWidget(int fps, int saison, QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    texture(0),
    angularSpeed(0),
    FPS(60),
    Saison(saison)

{
    objTab = QVector<Mesh*>();
    BasiqueSaison();
}

/*MainWidget::MainWidget(int fps, int saison) :
    geometries(0),
    texture(0),
    angularSpeed(0),
    FPS(fps),
    Saison(saison)
    BasiqueSaison();
{
}*/


MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete geometries;
    doneCurrent();
}
void MainWidget::wheelEvent(QWheelEvent *e){
    if (e->orientation()==Qt::Vertical)
        {
            if (e->delta()>0){
              static int calendrier = 0;  Z+=1.0;
            }
            else{
               Z-=1.0;
            }
        }

    e->accept();
}
void MainWidget::keyPressEvent(QKeyEvent *e)
{   switch(e->key()){
        case Qt::Key_D:
            destroyEverything();
            break;
        default : break;
    }
    if (e->key()==Qt::Key_Left){
        X-=1.0;
    }
    else if (e->key()==Qt::Key_Right){
        X+=1.0;
    }
    if (e->key()==Qt::Key_Down){
        Y-=1.0;
    }
    else if (e->key()==Qt::Key_Up){
        Y+=1.0;
    }
    if (e->key()==Qt::Key_Space){
        aff += 1;
        aff = aff % 2;
    }
    if (e->key()==Qt::Key_Backspace){
        printf("ON REINITIALISE LA MAP");
        geometries->reinitialiserMapDebris();
    }

    if (e->key()==Qt::Key_Plus){
        s+=0.01;
    }
    else {
        if (e->key()==Qt::Key_Minus){
            s-=0.01;
        }
    }
}

//! [0]
void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}
//! [0]

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    //pivoter();
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update

    }
    update();

}

void MainWidget::pivoter()
{
    // Decrease angular speed (friction)
    angularSpeed = 0.1+s;
    QVector2D diff = QVector2D{0.0, 1.0};
    qreal acc = 1;
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();
    rotationAxis = (rotationAxis * angularSpeed + n*acc ).normalized();
    rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;
    //update();

}
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.5, 0.5, 0.5, 1);



    initShaders();
    initTextures();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    //glEnable(GL_CULL_FACE);
//! [2]
//!
    geometries = new GeometryEngine(10.0 *  qrand() / (float) RAND_MAX,10.0 * ((float) qrand()) / (float) RAND_MAX, SCALE);
    for(int i=0;i<100;i++){
        objTab.append(fromCubeToTrueMesh(CubeCreator(geometries->tailleMap * qrand()/RAND_MAX, geometries->tailleMap * qrand()/RAND_MAX, (1.0 + 10.0 * qrand()/RAND_MAX)+10,0.5,0.5,0.5),10));
    }
    for (int i = 0; i < 1000; i++){
        //debris = new Debris(10.0 * qrand()/RAND_MAX, 10.0 * qrand()/RAND_MAX, 1.0 + 2.0 * qrand()/RAND_MAX, 1.0 + 5.0 * qrand()/RAND_MAX, 1.0 + 5.0 * qrand()/RAND_MAX, 1.0 + 5.0 * qrand()/RAND_MAX, SCALE);
        //listDebris.push_back(new Debris(9.0 * qrand()/RAND_MAX, 9.0 * qrand()/RAND_MAX, (1.0 + 5.0 * qrand()/RAND_MAX), 1.0 + 5.0 * qrand()/RAND_MAX, 1.0 + 5.0 * qrand()/RAND_MAX, 1.0 + 5.0 * qrand()/RAND_MAX, SCALE));
        //listDebrisChute.push_back(1);
    }

    // Use QBasicTimer because its faster than QTimer
    if (FPS == 0){
        FPS = 1;
    }
    timer.start(1000/FPS, this);
}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();

}
//! [3]

void MainWidget::applyGravity(){
    destroyOutOfMap();
    Mesh** daObj = objTab.data();
    for(int i=0;i<objTab.size();i++){
        giveStrenght(daObj[i],0,0,-0.02f);
        Cube cube = daObj[i]->cube;
        if (geometries->positionAltitude(daObj[i]->cube.x,daObj[i]->cube.y) < daObj[i]->cube.z)
        {
            applyForce(daObj[i]);
        }
        else daObj[i]->cube.z = geometries->positionAltitude(daObj[i]->cube.x,daObj[i]->cube.y);
        //if(Collision(cube,CubeCreator(cube.x,cube.y,qGray(heightMap.pixel((cube.x*heightMap.width()),(cube.y*heightMap.height())))/128.f,0,0,cube.sizeZ))) applyForce(daObj[i]);
    }
}

void MainWidget::destroyOutOfMap(){
    float x = geometries->tailleMap;
    float y = geometries->tailleMap;
    //fprintf(stderr,"%f %f\n",x,y);
    QVector<Mesh*> newList = QVector<Mesh*>();
    for(;objTab.size()!=0;){
        Mesh* mesh = objTab.takeAt(0);
        if(mesh->cube.x<0 || mesh->cube.y<0 || mesh->cube.x>x || mesh->cube.y>y) freeMesh(mesh);
        else if(mesh->cube.sizeX<0.005 || mesh->cube.sizeY<0.005) freeMesh(mesh); //Transformation particule
        else newList.append(mesh);
    }
    for(;newList.size()!=0;){
        objTab.append(newList.takeAt(0));
    }
}

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
    QVector4D normal;

};

void MainWidget::drawObj(Mesh* mush,QOpenGLShaderProgram *program)
{
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf(QOpenGLBuffer::IndexBuffer);
    arrayBuf.create();
    indexBuf.create();
    Cube cube = mush->cube;
    VertexData* VD = (VertexData*) malloc(sizeof(VertexData)*24);
    //////Face 1 Devant
    VD[0]={QVector3D(cube.x,cube.y,cube.z), QVector2D(0.0f, 0.0f)};
    VD[1]={QVector3D(cube.x+cube.sizeX,cube.y,cube.z), QVector2D(0.0f, 0.0f)};
    VD[2]={QVector3D(cube.x+cube.sizeX,cube.y+cube.sizeY,cube.z), QVector2D(0.0f, 0.0f)};
    VD[3]={QVector3D(cube.x,cube.y+cube.sizeY,cube.z), QVector2D(0.0f, 0.0f)};
    //////Face Dessus
    VD[4]={QVector3D(cube.x,cube.y,cube.z), QVector2D(0.0f, 0.0f)};
    VD[5]={QVector3D(cube.x,cube.y,cube.z+cube.sizeZ), QVector2D(0.0f, 0.0f)};
    VD[6]={QVector3D(cube.x+cube.sizeX,cube.y,cube.z+cube.sizeZ), QVector2D(0.0f, 0.0f)};
    VD[7]={QVector3D(cube.x+cube.sizeX,cube.y,cube.z), QVector2D(0.0f, 0.0f)};
    //////Face Derrière
    VD[8]={QVector3D(cube.x,cube.y,cube.z+cube.sizeZ), QVector2D(0.0f, 0.0f)};
    VD[9]={QVector3D(cube.x,cube.y+cube.sizeY,cube.z+cube.sizeZ), QVector2D(0.0f, 0.0f)};
    VD[10]={QVector3D(cube.x+cube.sizeX,cube.y+cube.sizeY,cube.z+cube.sizeZ), QVector2D(0.0f, 0.0f)};
    VD[11]={QVector3D(cube.x+cube.sizeX,cube.y,cube.z+cube.sizeZ), QVector2D(0.0f, 0.0f)};
    //////Face Dessous
    VD[12]={QVector3D(cube.x,cube.y+cube.sizeY,cube.z), QVector2D(0.0f, 0.0f)};
    VD[13]={QVector3D(cube.x+cube.sizeX,cube.y+cube.sizeY,cube.z), QVector2D(0.0f, 0.0f)};
    VD[14]={QVector3D(cube.x+cube.sizeX,cube.y+cube.sizeY,cube.z+cube.sizeZ), QVector2D(0.0f, 0.0f)};
    VD[15]={QVector3D(cube.x,cube.y+cube.sizeY,cube.z+cube.sizeZ), QVector2D(0.0f, 0.0f)};
    //////Face Gauche
    VD[16]={QVector3D(cube.x,cube.y,cube.z), QVector2D(0.0f, 0.0f)};
    VD[17]={QVector3D(cube.x,cube.y+cube.sizeY,cube.z), QVector2D(0.0f, 0.0f)};
    VD[18]={QVector3D(cube.x,cube.y+cube.sizeY,cube.z+cube.sizeZ), QVector2D(0.0f, 0.0f)};
    VD[19]={QVector3D(cube.x,cube.y,cube.z+cube.sizeZ), QVector2D(0.0f, 0.0f)};
    //////Face Droite
    VD[20]={QVector3D(cube.x+cube.sizeX,cube.y,cube.z), QVector2D(0.0f, 0.0f)};
    VD[21]={QVector3D(cube.x+cube.sizeX,cube.y,cube.z+cube.sizeZ), QVector2D(0.0f, 0.0f)};
    VD[22]={QVector3D(cube.x+cube.sizeX,cube.y+cube.sizeY,cube.z+cube.sizeZ), QVector2D(0.0f, 0.0f)};
    VD[23]={QVector3D(cube.x+cube.sizeX,cube.y+cube.sizeY,cube.z), QVector2D(0.0f, 0.0f)};

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
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}

void MainWidget::destroyEverything(){
    for(int i=0;i<objTab.size();i++){
        fprintf(stderr,"|%d : %f %f %f|",objTab.at(i)->nbp,objTab.at(i)->cube.dirX,objTab.at(i)->cube.dirY,objTab.at(i)->cube.dirY);
    }
    fprintf(stderr,"]  --->  [");
    QVector<Mesh*> newList = QVector<Mesh*>();
    for(;objTab.size()!=0;){
        float x = 1.0*((float)qrand()/RAND_MAX);
        float y = 1.0*((float)qrand()/RAND_MAX);
        float z = 1.0*((float)qrand()/RAND_MAX);
        int nbmesh=0;
        Mesh** meshTab = breakMesh(objTab.takeAt(0),x,y,z,&nbmesh);
        for(int i=0;i<nbmesh;i++){
            newList.append(meshTab[i]);
        }
        free(meshTab);
    }
    for(;newList.size()!=0;){
        objTab.append(newList.takeAt(0));
    }
    for(int i=0;i<objTab.size();i++){
        fprintf(stderr,"|%d : %f %f %f|",objTab.at(i)->nbp,objTab.at(i)->cube.dirX,objTab.at(i)->cube.dirY,objTab.at(i)->cube.dirY);
    }
    fprintf(stderr,"] size : %d",objTab.size());
}

//! [4]
void MainWidget::BasiqueSaison()
{
    if (Saison == 0){
        setWindowTitle("Eté");
        CouleurSaison = QVector4D(0.8f,0.8f,0.1f,1.0f);
    }
    else if (Saison == 1){
        setWindowTitle("Automne");
        CouleurSaison = QVector4D(1.0f,0.7f,0.1f,1.0f);
    }
    else if (Saison == 2){
        setWindowTitle("Hiver");
        CouleurSaison = QVector4D(0.5f,0.5f,0.9f,1.0f);
    }
    else if (Saison == 3){
        setWindowTitle("Printemps");
        CouleurSaison = QVector4D(0.8f,1.0f,0.1f,1.0f);
    }
}
void MainWidget::SaisonSuivante()
{
    Saison += 1;
    Saison = Saison % 4;

    BasiqueSaison();
}

void MainWidget::Gravity(){
    //debris->~Debris();
    //debris = new Debris(2.5, 2.5, debris->Z, debris->W, debris->L, debris->H, SCALE);
    /*if (listDebris.size() != 0){
        for (int i = 0; i < listDebris.size(); i++){
            if (geometries->positionAltitude(listDebris[i]->X,listDebris[i]->Y) < listDebris[i]->Z){
                listDebris[i]->chute();
                listDebris[i] = new Debris(listDebris[i]->X, listDebris[i]->Y, listDebris[i]->Z, listDebris[i]->W, listDebris[i]->L, listDebris[i]->H, SCALE);
            }
            else {
                listDebrisChute.push_back(listDebris[i]);
                //printf("Taille : "+listDebris.size());
                listSuppressionDebris.push_back(i);
                //listDebris.removeAt(i);
                //geometries->fusionDebrisMap(listDebris[i]->X,listDebris[i]->Y, listDebris[i]->W, listDebris[i]->L, listDebris[i]->H);
                //listDebrisChute.removeAt(i);
            }
        }

    }*/
    if (listDebris.size() != 0){
        int Index = listDebris.size();
        for (int i = Index-1; i >= 0; i--){
            if (geometries->positionAltitude(listDebris[i]->X,listDebris[i]->Y) < listDebris[i]->Z){
                listDebris[i]->chute();
                listDebris[i] = new Debris(listDebris[i]->X, listDebris[i]->Y, listDebris[i]->Z, listDebris[i]->W, listDebris[i]->L, listDebris[i]->H, SCALE);
            }
            else{
                listDebrisChute.push_back(listDebris[i]);
                delete listDebrisChute[i];
                //listDebris[i]->~Debris();
                listDebris.removeAt(i);
            }
        }
    }
    if (listDebrisChute.size() != 0){
        if (listDebrisChute.size() > 400){
            for (int i = 0; i < 200; i++){
                geometries->fusionDebrisMap(listDebrisChute[0]->X,listDebrisChute[0]->Y, listDebrisChute[0]->W, listDebrisChute[0]->L, listDebrisChute[0]->H);
                delete listDebrisChute[0];
                listDebrisChute.removeAt(0);
            }
        geometries->SauvegardeMapDebris();
        }
    //listDebris.push_back(new Debris(9.0 * qrand()/RAND_MAX, 9.0 * qrand()/RAND_MAX, (1.0 + 5.0 * qrand()/RAND_MAX), 1.0 + 5.0 * qrand()/RAND_MAX, 1.0 + 5.0 * qrand()/RAND_MAX, 1.0 + 5.0 * qrand()/RAND_MAX, SCALE));
    }
    /*if (listDebris.size() != 0 && listDebrisChute.size() == 0) {
        //load++;
        //load= load%2;
        geometries->SauvegardeMapDebris();
        listDebris.clear();
        //geometries = new GeometryEngine(10*((float) rand()) / (float) RAND_MAX,10*((float) rand()) / (float) RAND_MAX, SCALE);

        for (int i = 0; i < 10+load; i++){
            debris = new Debris(10.0 * qrand()/RAND_MAX, 10.0 * qrand()/RAND_MAX, 1.0 + 2.0 * qrand()/RAND_MAX, 1.0 + 5.0 * qrand()/RAND_MAX, 1.0 + 5.0 * qrand()/RAND_MAX, 1.0 + 5.0 * qrand()/RAND_MAX, SCALE);
            listDebris.push_back(new Debris(9.0 * qrand()/RAND_MAX, 9.0 * qrand()/RAND_MAX, (1.0 + 5.0 * qrand()/RAND_MAX), 1.0 + 5.0 * qrand()/RAND_MAX, 1.0 + 5.0 * qrand()/RAND_MAX, 1.0 + 5.0 * qrand()/RAND_MAX, SCALE));
            listDebrisChute.push_back(1);
            //printf("DEBRIS : %d %f %f \n",i, listDebris[i]->X, listDebris[i]->Y);
        }
    }*/
}


float d1 = 0.1, d2 = 0.1;

void MainWidget::DeplacementPoint()
{
    if (posX < 0){
        d1 = ((float) rand()) / (float) RAND_MAX+0.05;//random().range(0,)
    }
    if (posX > 10){
        d1 = -((float) rand()) / (float) RAND_MAX-0.05;
    }
    if (posY < 0){
        d2 = ((float) rand()) / (float) RAND_MAX+0.05;
    }
    if (posY > 10){
        d2 = -((float) rand()) / (float) RAND_MAX-0.05;
    }
    posX += d1;
    posY += d2;
    geometries = new  GeometryEngine(posX,posY, SCALE);
}

void MainWidget::initTextures()
{
    // Load cube.png image
    //texture = new QOpenGLTexture(QImage(":/paletteAltitue.png").mirrored());
    texture = new QOpenGLTexture(QImage(":/paletteAltitue.png")/*.mirrored()*/);
    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    //texture->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Nearest);


    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]pivoter();
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 0.1, zFar = 80.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

int i = 0;
void MainWidget::paintGL()
{

    TimeDebut = clock();
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (aff == 1){
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    texture->bind();

//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(X, Y, Z);
    matrix.rotate(-45, QVector3D(1,0,0));
    //matrix.rotate(s * (i++),QVector3D(0,0,1));

    //matrix.pivoter();
    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);
    program.setUniformValue("ambiant_color", CouleurSaison);
    program.setUniformValue("light_position", QVector4D(8.0, 8.0, 8.0, 1.0));
//! [6]

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    //geometries->drawCubeGeometry(&program);
    //geometries->drawPlaneGeometry(&program);
    geometries->drawQuadPlaneGeometry(&program);

    /*if (listDebris.size() != 0){
        for (int i = 0; i < listDebris.size(); i++){
            //->drawDebris(&program);
            listDebris[i]->drawDebris(&program);
        }
    }
    if (listDebrisChute.size() != 0){
        for (int i = 0; i < listDebrisChute.size(); i++){
            //->drawDebris(&program);
            listDebrisChute[i]->drawDebris(&program);
        }
    }*/
    applyGravity();
    Mesh** daObj = objTab.data();
        for(int i=0;i<objTab.size();i++){
            drawObj(daObj[i],&program);
        }

    //debris->drawDebris(&program);
    TimeFin = clock();

    temps = (float)(TimeFin-TimeDebut)/CLOCKS_PER_SEC;
    fprintf(stderr,"temps : %f", 1.0f/temps);
}
