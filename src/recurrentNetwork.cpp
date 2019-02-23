#include "headers/recurrentNetwork.hpp"
#define     e      (2.71828)

Connection::Connection(double weightValue){
    this->weightValue = weightValue;
}
double Connection::updateWeightValue(double learningRatio, double resultActivation,double error){
    this->weightValue += (learningRatio * resultActivation * error);
    return this->weightValue;
}

ActivationFunction::ActivationFunction(double MU,int typeFunction){
    this->MU = MU;
    this->typeFunction = typeFunction;
}
double ActivationFunction::startFunction(){
    switch(typeFunction)
    {
        case 1: //función Sigmoide f(x) = 1 / (1 + e ^ -MU)
          return (1.0 / (1.0 + pow(e,-MU)));
          break;
        case 2: //función Hiperbolica f(x) = (1 - e ^ -MU) / (1 + e ^ -MU)
          return (1.0 - pow(e,-MU)) / (1.0 + pow(e,-MU));
          break;
    }
}
double ActivationFunction::derivateFunction(){
    switch(typeFunction)
    {
        case 1: //derivada de la función sigmoide f'(x) = f(x) * (1 - f(x))
          return ((1.0 / (1.0 + pow(e,-MU))) * (1 - (1.0 / (1.0 + pow(e,-MU)))));
          break;
        case 2: //derivada de la función hiperbolica f'(x) = 2 * fsigmoide(x) * (1 - fsigmoide(x))
          return (2.0 * ((1.0 / (1 + pow(e,-MU))) * (1.0 - (1.0 / (1 + pow(e,-MU))))));
          break;
    }
}

Neuron::Neuron(double MU,int layer,int typeFunction){
    this->MU = MU;
    this->layer = layer;
    this->typeFunction = typeFunction;
}
double Neuron::updateBias(double learningRatio,double error){
    this->bias += (learningRatio * error);
    return this->bias;
}
double Neuron::startActivationFunction(){
    ActivationFunction af(this->MU,this->typeFunction);
    this->resultActivationFunction = af.startFunction();
    return this->resultActivationFunction;
}
double Neuron::derivateActivationFunction(){
    ActivationFunction af(this->MU,this->typeFunction);
    this->resultDerivateActivationFunction = af.derivateFunction();
    return this->resultDerivateActivationFunction;
}
