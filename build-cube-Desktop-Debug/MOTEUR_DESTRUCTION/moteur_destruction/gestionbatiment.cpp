#include "gestionbatiment.h"

GestionBatiment::GestionBatiment(int t, float x, float y, float z) :
    typeBat(t),
    X(x),
    Y(y),
    Z(z)
{
    constructionBat();
}

GestionBatiment::~GestionBatiment()
{

}
void GestionBatiment::constructionBat(){
    if (typeBat == 1){
        CarteBatiment.load(":/petiteMaison1.pgm");
    }
}
