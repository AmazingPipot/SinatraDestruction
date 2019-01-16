#include "boolet.h"

Boolet::Boolet(float x,float y,float z,float size):
    X(x),
    Y(y),
    Z(z),
    Ray(size),
    dirX(0),
    dirY(0),
    dirZ(0)
{

}

Boolet::~Boolet()
{
    ;
}

void Boolet::giveForce(float fX,float fY,float fZ){
    dirX+=fX;
    dirY+=fY;
    dirZ+=fZ;
}

void Boolet::applyForce(){
    X+=dirX;
    Y+=dirY;
    Z+=dirZ;
    dirX*=0.99;
    dirY*=0.99;
    dirZ*=0.9;
    if(dirX < 0.001) dirX=0;
    if(dirY < 0.001) dirY=0;
    if(dirZ < 0.001) dirZ=0;
}

bool Boolet::collide(float x,float y,float z,float sizeX,float sizeY,float sizeZ){
    if(!(X<x ^ X<x+sizeX)) return false;
    if(!(Y<y ^ Y<y+sizeY)) return false;
    if(!(Z<z ^ Z<z+sizeZ)) return false;
    return true;
}
