#include "utils.hpp"

class Connection{
public:
    Connection();
    Connection(int,double);
    double weightValue;
    double totalError;
};

class Neuron{
public:
    Neuron();
    Neuron(double,int,int);
    double MU;
    int layer;
    int typeFunction;
    double resultActivationFuncion;
    double resultDerivateActivationFunction;
    double bias;
    bool isInput;

    double updateBias(double,double);
    double StartActivateFunction();
    double StartDerivateActivationFunction();
};

class ActivateFunction(){
public:
    ActivationFunction();
    ActivationFunction(double,int);
    int TypeFunction; //tipo de funcion
    double MU; //variable de la funcion (Entrada efectiva (MU))
    double startFunction();
    double derivateFunction();
};
