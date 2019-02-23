#include "utils.hpp"

#ifndef CONNECTION_HPP
#define CONNECTION_HPP
class Connection{
public:
    Connection();
    Connection(int,double);
    double weightValue;
    double totalError;

    double updateWeightValue(double reasonlearning,double resultActivation,double error);
};
#endif

#ifndef ACTIVATIONFUNCTION_HPP
#define ACTIVATIONFUNCTION_HPP
class ActivationFunction{
public:
    ActivationFunction();
    ActivationFunction(double,int);
    int typeFunction; //tipo de funcion
    double MU; //variable de la funcion (Entrada efectiva (MU))

    double startFunction();
    double derivateFunction();
};
#endif

#ifndef NEURON_HPP
#define NEURON_HPP
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
    double startActivateFunction();
    double startDerivateActivationFunction();
};
#endif
