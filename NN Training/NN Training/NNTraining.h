//
//  NNTraining.h
//  Neural Network Training
//
//  Created by Day Day Up on 2/1/16.
//  Copyright © 2016 Jingyun Yang. All rights reserved.
//

#ifndef NNTraining_h
#define NNTraining_h

#define MAX_N_LAYER 5
#define MAX_N_UNITS 10000
#define MAX_N_TRAIN 100000 //TRAIN+TESTS

class NNTraining {
    int n_layer,n_units[MAX_N_LAYER+1],n_train,n_tests=0;
    //n_units[] 层数不含bais_unit
    double lambda;
    //Regularization constant
    double a=1.0;
    //Learning rate
    double (*theta)[MAX_N_UNITS+1][MAX_N_UNITS+1]
        =new double[MAX_N_LAYER][MAX_N_UNITS+1][MAX_N_UNITS+1];
    //ϴ[出发层数][出发编号][到达编号],三维数组,各维度大小看后面
    double (*delta)[MAX_N_UNITS+1][MAX_N_UNITS+1]
        =new double[MAX_N_LAYER][MAX_N_UNITS+1][MAX_N_UNITS+1];
    //预存ϴ的调整数值
    double (*X)[MAX_N_UNITS+1]=new double[MAX_N_TRAIN+1][MAX_N_UNITS+1];
    //X[训练数据组编号][数据编号]
    double (*Y)[MAX_N_UNITS+1]=new double[MAX_N_TRAIN+1][MAX_N_UNITS+1];
    //Y[训练数据组编号][数据编号]
    double J;
    //Cost funcation J
    double (*node)[MAX_N_UNITS+1]=new double[MAX_N_LAYER+1][MAX_N_UNITS+1];
    //计算过程中每个节点的数值
    bool testPrecision[MAX_N_TRAIN];
    //Test set中的每个元素是否检验正确
    bool trainPrecision[MAX_N_TRAIN]={false};
    
    double sigmoid(double);
    double siggrad(double);
    void forward(int);
    void accumulateJ(int);
    void backward(int);
    void updateTheta(double);
    
public:
    ~NNTraining();
    void setLayerNumber(int);
    void setUnitsNumber(int*);
    void setTrainNumber(int);
    void setTestsNumber(int);
    void setLearningRate(double);
    void presetTheta(void);
    void resetDelta(void);
    void setLambda(double);
    void resetJ(void);
    void regularizeJ(void);
    void inputX(double*);
    void inputXFromImg(char*,int,int,int);
    void inputY(double*);
    double performIteration(void);
    double outputTrainSetPrecision(void);
    double outputTestsSetPrecision(void);
    void outputThetas(void);
    bool* outputPrecision(void);
};

#endif /* NNTraining_h */
