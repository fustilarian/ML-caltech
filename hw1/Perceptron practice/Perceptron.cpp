//
//  main.cpp
//  Perceptron practice
//
//  Created by Henry on 12/06/2016.
//  Copyright © 2016 Henry. All rights reserved.
//

#include "vect.h"
#include <iomanip>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_math.h>
#include <ctime>


using namespace std;

void updateweights(vect&, vect, int);
int TargetFunction(double, double, vect);
vect GetSample();
int HypothesisFunction(vect , vect);
template <typename T> int sgn(T);



gsl_rng * r;
const gsl_rng_type * T;


typedef struct TrainingDatum {
    vect domain= vect(3);
    int TFunction;
    int HFunction;
} TrainingDatum;


int main(int argc, const char * argv[]) {
    gsl_rng_env_setup();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    cout<<"RNG :"<<'\t'<<gsl_rng_name(r)<<endl; 
    gsl_rng_set(r,(unsigned long) time(NULL));

    
    vect p1=GetSample();
    vect p2=GetSample();
    double ycoeff= (p2.geti(0)-p1.geti(0))/(p1.geti(1)-p2.geti(1));
    double zerothc= -p1.geti(0)-ycoeff*p1.geti(1);
    
    const unsigned int N=100000;
    vector<TrainingDatum> trainingdata(N);
    
    vect weight(3);
    for(int i=0; i<N ; i++){//generate training data
        trainingdata[i].domain = GetSample();
        trainingdata[i].TFunction = TargetFunction(ycoeff, zerothc, trainingdata[i].domain);
        trainingdata[i].HFunction = HypothesisFunction(weight,trainingdata[i].domain);
    }
    bool wrong=false;
    int wrongcount=0;
    int loops=1;
    for(int i=0; i<N ; i++){//run perceptron iterations on all mis-assigned data points
        bool there_exists_wrong=trainingdata[i].TFunction!=trainingdata[i].HFunction;
        wrong= wrong || there_exists_wrong;
        if(there_exists_wrong){
            updateweights(weight, trainingdata[i].domain,trainingdata[i].TFunction);
            trainingdata[i].HFunction = HypothesisFunction(weight,trainingdata[i].domain);
            wrongcount++;
        }
        if(i==N-1 && wrong){
            cout<<loops<<'\t'<<wrongcount<<endl;
            i=0;
            wrong=false;
            loops++;
            wrongcount=0;
        }
    }
    cout<<loops<<'\t'<<wrongcount<<endl;
    cout<<weight.geti(1)/weight.geti(0)<<'\t'<<weight.geti(2)/weight.geti(0)<<endl;
    cout<<ycoeff<<'\t'<<zerothc<<endl;
    /*
    for(int i=0 ; i<N ; i++){
        cout<<trainingdata[i].TFunction<<'\t'<<trainingdata[i].HFunction<<endl;
    }
    */
    
    gsl_rng_free (r); //frees all the memory associated with the generator r.
    return 0;
}


/** ------------------------------------------------------------
 ** Functions
 ** ----------------------------------------------------------*/

/** Target function
 ** ----------------------------------------------------------*/

int TargetFunction(double yco, double zerco, vect sample){
    vect targetw(3);
    targetw.seti(0,1.0);
    targetw.seti(1,yco);
    targetw.seti(2,zerco);
    return sgn(sample*targetw);
}

/** generate sample point
 ** ----------------------------------------------------------*/
vect GetSample(){
    vect pt(3);
    for(int j=0; j<2; j++){
        pt.seti(j, gsl_rng_uniform(r)*2.0-1.0);
    }
    pt.seti(2,1.0);
    return pt;
}

/** Perceptron
 ** ----------------------------------------------------------*/
int HypothesisFunction(vect weights, vect sample){
    return sgn(weights*sample);
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void updateweights(vect &weights, vect sample, int tfunct){
    weights=weights+ sample*tfunct;
}
