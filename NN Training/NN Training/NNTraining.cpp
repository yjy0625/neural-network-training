//
//  NNTraining.cpp
//  Neural Network Training
//
//  Created by Day Day Up on 2/1/16.
//  Copyright © 2016 Jingyun Yang. All rights reserved.
//

#include "NNTraining.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <GLUT/GLUT.h>

using namespace std;

//PRIVATE FUNCTIONS

double NNTraining::sigmoid(double z){
    return 1.0/(1+exp(-z));
}

double NNTraining::siggrad(double z){
    return z*(1.0-z);
}

void NNTraining::forward(int trainingSetIndex){
    for(int i=1;i<n_layer;i++)node[i][0]=1.0;
        //Preset bais nodes
    for(int i=1;i<=n_layer;i++){
        for(int j=1;j<=n_units[i];j++){
            node[i][j]=0.0;
        }
    }
        //Preset non-bais nodes
    for(int i=1;i<=n_units[1];i++)node[1][i]=X[trainingSetIndex][i];
        //Set 1st-layer nodes
    for(int layer=1;layer<n_layer;layer++){
        for(int end=1;end<=n_units[layer+1];end++){
            for(int start=0;start<=n_units[layer];start++){
                node[layer+1][end] += node[layer][start]*theta[layer][start][end];
            }
            node[layer+1][end] = sigmoid(node[layer+1][end]);
        }
    }
    
    if(n_units[n_layer]==1){
        if(fabs(node[n_layer][1]-Y[trainingSetIndex][1])<0.5){
            trainPrecision[trainingSetIndex]=true;
        }
        else trainPrecision[trainingSetIndex]=false;
    }
    else {
        double maxValue=0;
        int maxIndex=0;
        for(int unit=1;unit<=n_units[n_layer];unit++){
            if(node[n_layer][unit]>maxValue){
                maxValue = node[n_layer][unit];
                maxIndex = unit;
            }
        }
        trainPrecision[trainingSetIndex]=(Y[trainingSetIndex][maxIndex]==1);
    }
}

void NNTraining::accumulateJ(int trainingSetIndex){
    double increment=0.0;
    for(int i=1;i<=n_units[n_layer];i++){
        increment += Y[trainingSetIndex][i]*log(node[n_layer][i]);
        increment += (1-Y[trainingSetIndex][i])*log(1-node[n_layer][i]);
    }
    increment *= (-1.0)/n_train;
    J += increment;
}

void NNTraining::backward(int trainingSetIndex){
    double (*error)[MAX_N_UNITS+1]=new double[MAX_N_LAYER+1][MAX_N_UNITS+1]{0.0};
        //Preset errors
    for(int i=1;i<=n_units[n_layer];i++){
        error[n_layer][i] = node[n_layer][i] - Y[trainingSetIndex][i];
    }
        //Calculate error[n_layer]
    for(int layer=n_layer-1;layer>=2;layer--){
        for(int to=1;to<=n_units[layer];to++){
            for(int from=1;from<=n_units[layer+1];from++){
                error[layer][to] += theta[layer][to][from]*error[layer+1][from];
            }
            error[layer][to] *= siggrad(node[layer][to]);
        }
    }
        //Calculate deltas (layer 2) ~ (layer n_layer-1), adding regularization
    for(int x=1;x<=n_layer-1;x++){
        for(int y=0;y<=n_units[x];y++){
            for(int z=1;z<=n_units[x+1];z++){
                delta[x][y][z] += node[x][y]*error[x+1][z];
            }
        }
    }
    delete[] error;
}

void NNTraining::updateTheta(double a){
    for(int x=1;x<=n_layer-1;x++){
        for(int y=0;y<=n_units[x];y++){
            for(int z=1;z<=n_units[x+1];z++){
                delta[x][y][z]/=n_train;
                if(y)delta[x][y][z]+=lambda*theta[x][y][z];
                theta[x][y][z] -= a*delta[x][y][z];
            }
        }
    }
}

//PUBLIC FUNCTIONS

NNTraining::~NNTraining(){
    delete[] theta;
    delete[] delta;
    delete[] X;
    delete[] Y;
    delete[] node;
}

void NNTraining::setLayerNumber(int number){
    n_layer = number;
}

void NNTraining::setUnitsNumber(int numbers[]){
    for(int i=1;i<=n_layer;i++){
        n_units[i] = numbers[i-1];
    }
}

void NNTraining::setTrainNumber(int number){
    n_train = number;
}

void NNTraining::setTestsNumber(int number){
    n_tests = number;
}

void NNTraining::setLearningRate(double number){
    a = number;
}

void NNTraining::presetTheta(){
    srand((unsigned)time(NULL));
    for(int x=1;x<=n_layer-1;x++){
        for(int y=0;y<=n_units[x];y++){
            for(int z=1;z<=n_units[x+1];z++){
                theta[x][y][z] = double(rand()%10000)/5000-1;
            }
        }
    }
}

void NNTraining::resetDelta(){
    for(int x=1;x<=n_layer-1;x++){
        for(int y=0;y<=n_units[x];y++){
            for(int z=1;z<=n_units[x+1];z++){
                delta[x][y][z] = 0.0;
            }
        }
    }
}

void NNTraining::setLambda(double number){
    lambda = number;
}

void NNTraining::resetJ(){
    J = 0.0;
}

void NNTraining::regularizeJ(){
    double increment=0.0;
    for(int x=1;x<=n_layer-1;x++){
        for(int y=1;y<=n_units[x];y++){
            for(int z=1;z<=n_units[x+1];z++){
                increment += pow(theta[x][y][z],2.0);
            }
        }
    }
    increment *= lambda/(2*n_train);
    J += increment;
}

void NNTraining::inputX(double *data){
    for(int trainIndex=1;trainIndex<=n_train+n_tests;trainIndex++){
        for(int i=1;i<=n_units[1];i++){
            X[trainIndex][i]=data[(trainIndex-1)*n_units[1]+i];
//            printf("X[%d][%d]=%f ",trainIndex,i,X[trainIndex][i]);
        }
    }
}

void NNTraining::inputXFromImg(char* dir, int count,int width,int hight){
    typedef struct{
        GLubyte b;
        GLubyte g;
        GLubyte r;
    }RGB;
    RGB ele;
    for(int i=1;i<=count;i++){
        char buffer[100];
        int tempInt;
        tempInt = sprintf(buffer,"%s%d.bmp",dir,i);
        FILE *pfin=fopen(buffer,"rb");
        GLubyte notWhatYouWant[54];
        fread(&notWhatYouWant,sizeof(notWhatYouWant),1,pfin);
        for(int k=1;k<=hight;k++){
            for(int j=1;j<=width;j++){
                fread(&ele,sizeof(RGB),1,pfin);
                X[i][(k-1)*width+j]=(double)(ele.b+ele.g+ele.r)/3/256;
            }
        }
        fclose(pfin);
    }
}

void NNTraining::inputY(double *data){
    for(int trainIndex=1;trainIndex<=n_train+n_tests;trainIndex++){
        for(int i=1;i<=n_units[n_layer];i++){
            Y[trainIndex][i]=data[(trainIndex-1)*n_units[n_layer]+i];
        }
    }
}

double NNTraining::performIteration(){
    resetDelta();
    resetJ();
    for(int j=0;j<=n_train;j++)trainPrecision[j]=false;
    for(int i=1;i<=n_train;i++){
        forward(i);
        accumulateJ(i);
        backward(i);
    }
    regularizeJ();
    updateTheta(a);
    return J;
}

double NNTraining::outputTrainSetPrecision(){
    double precision=0.0;
    for(int i=1;i<=n_train;i++){
        precision+=trainPrecision[i];
    }
    precision/=n_train;
    return precision;
}

double NNTraining::outputTestsSetPrecision(){
    double precision=0.0;
    for(int i=n_train+1;i<=n_train+n_tests;i++){
        resetDelta();
        resetJ();
        forward(i);
        if(n_units[n_layer]==1){
            if(fabs(node[n_layer][1]-Y[i][1])<0.5){
                precision++;
                testPrecision[i-n_train]=true;
            }
            else testPrecision[i-n_train]=false;
        }
        else {
            double maxValue=0;
            int maxIndex=0;
            for(int unit=1;unit<=n_units[n_layer];unit++){
                if(node[n_layer][unit]>maxValue){
                    maxValue = node[n_layer][unit];
                    maxIndex = unit;
                }
            }
            testPrecision[i-n_train]=(Y[i][maxIndex]==1);
            precision+=(Y[i][maxIndex]==1);
//            if(Y[i][maxIndex]==1){
//                cout<<"Test set "<<i<<" is correct with max value "<<maxValue<<".\n";
//            }
        }
    }
    precision/=n_tests;
    return precision;
}

void NNTraining::outputThetas(){
    for(int x=1;x<=n_layer-1;x++){
        for(int y=0;y<=n_units[x];y++){
            for(int z=1;z<=n_units[x+1];z++){
                cout<<theta[x][y][z]<<" ";
            }
            cout<<endl;
        }
        cout<<endl<<endl;
    }
}

bool* NNTraining::outputPrecision(){
    return testPrecision;
}
