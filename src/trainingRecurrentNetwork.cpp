#include "headers/recurrentNetwork.hpp"

/*
* Contador
*/
int count=0;

/*
* Programa Principal
*/
void trainingRecurrent(std::vector<std::vector<float>> trainData,std::vector<std::vector<float>> validationData)
{

    /*
     * error cometido para el alumno n
     * y acumulador
     * */
    double ErrorSum=0;

    /*
     * Error total en el entrenamiento
     * */
    double totalerror = 0;


    /*
     * Contadores
     */
    int n = 0;

    /*
     * Numero de capas
     */
    int NumberNeurons = 0;

    /*
     * Razon de aprendizaje para la etapa de validación
     * */
    double LearningRatio = 0.001;

    /*
     * Varible real, para almacenar valores aleatorios
     * */
    double random;

    /*
     * Indicador del tipo de función. Las Funciones validas para backpropagation son:
     * 1. Función Sigmoidal
     * 2. Función Hiperbolica
     * */
    int TypeFunction;
    std::string function;
    /*
     * Puntero de vector tridimensional de objetos de tipo connection (Connection.cpp)
     */
    Connection** connection;

    /*
     * Puntero de vector bidimensional de objetos de tipo sneuron (Neuron.cpp)
     */
    Neuron* neuron;

    /*
     * Declaramos la variable tiempo
     */
    int Time;
    int CountTime = 1;
    /*
     * Puntero de vector bidimensional de los valores de errores de cada neurna
     **/
    double** NeuronError;

    double MUTime;

    int numberOutputNeuron = 6;
    int numberInputNeuron = 35;

    /*
     * Leemos los parametros de la red de tipo Perceptron Multicapa y los parametros necesarios para
     * el aprendizaje de tipo de retropropagacion para atras (backpropagation)
     */
    std::cout<<"Ingrese el tiempo \n";
    std::cin>>Time;
    std::cout<<"\nTIPOS DE FUNCIÓN DE ACTIVACIÓN PARA BACKPROPAGATION:\n\n"
    		"Opción 1. Función Sigmoidal"
    		"\nOpción 2. Función Hiperbolica\n"
    		"\nSeleccione un numero de la función que desea utilizar:\n";
    std::cin>>TypeFunction;
    if(TypeFunction == 1)function="sigmoide";
    else function = "hyperbolic";
    std::cout<<"Ingrese el numero de neuronas de contexto para la red recurrente: \n";
    std::cin>>NumberNeurons;
    NumberNeurons = NumberNeurons + numberOutputNeuron + numberInputNeuron;

    //consturimos la arquitectura
    neuron = new Neuron[NumberNeurons];
    double cache[NumberNeurons];
    connection = new Connection *[NumberNeurons];
    for(int i = 0;i<NumberNeurons;i++)
    {
      	neuron[i].typeFunction = TypeFunction;
      	//las entrdas no son recurrentes
      	connection[i] = new Connection[NumberNeurons];
      	for(int j = numberInputNeuron;j<NumberNeurons;j++)
      	{
      		random = ((double)rand()/(RAND_MAX));
      		connection[i][j].weightValue = random;
          std::cout << connection[i][j].weightValue << std::endl;
      	}
    }

    double RealTimeDerived[NumberNeurons];

    /*
     * Salida del sistema dinamico
     */
    double OutDinamicSystem[NumberNeurons][NumberNeurons];

    /*
     * Acumulador
     */
    double SumOutDinamicSystem[NumberNeurons][NumberNeurons];

    for(int i = 0;i<NumberNeurons;i++)
    {
      	for(int j = numberInputNeuron;j<NumberNeurons;j++)
      	{
        		random = ((double)rand()/(RAND_MAX));
        		connection[i][j].weightValue = random;
      	}
    }

    /*
    	* inicializamos el valor de las salidas de las neuronas
    	*/
    for(int i = numberInputNeuron;i<NumberNeurons;i++)
    {
        neuron[i].resultActivationFunction = 0;
    }

    /*
    	* Inicializamos la salida dinamica del sistema
    	*/
    for(int i =0;i<NumberNeurons;i++)
    {
      	for(int j =numberInputNeuron;j<NumberNeurons;j++){
      		  OutDinamicSystem[i][j] = 0;
      			SumOutDinamicSystem[i][j] = 0;
    		}

    }
    totalerror = 0;
    n=0;
    CountTime=1;

    /*
    	* Recorremos los tiempos establecidos
    	*/
    while(CountTime <= Time)
    {
      	double MSE_out[6];
        double errorOut[6];
      	/*
      		* Conseguimos el patron de entrada n
      		*/
      	for(int i = 0;i < numberInputNeuron;i++){
      		  neuron[i].resultActivationFunction = trainData[n][i];
      	}

      	for(int i = numberInputNeuron;i<NumberNeurons;i++){
        		MUTime =0;
        		for(int j = 0;j<NumberNeurons;j++){
        		    MUTime = MUTime + (connection[j][i].weightValue * neuron[j].resultActivationFunction);
        		}

        		neuron[i].MU = MUTime;
        		neuron[i].startActivationFunction();

        		if(i>=NumberNeurons - 6)
        		{
        		    errorOut[i] = trainData[n][13] - neuron[i].resultActivationFunction;
        		}
      	}

      	/*
      		* hallar sumatorias de salidas dinamica del sistema
      		*/
      	for(int i = numberInputNeuron;i < NumberNeurons;i++)
      	{
      	   for(int j = 0;j < NumberNeurons;j++)
           {
      		     for(int k=numberInputNeuron;k<NumberNeurons;k++)
      			   {
      			        SumOutDinamicSystem[j][i] = SumOutDinamicSystem[j][i] + ((connection[k][i].weightValue * OutDinamicSystem[j][i]) + neuron[j].resultActivationFunction);
      			   }
      		 }
      	}

      	/*
      		* hallar salidas dinamicas del sistema para el tiempo actual
      		*/
      	for(int i = numberInputNeuron;i < NumberNeurons;i++)
      	{
        		for(int j = 0;j < NumberNeurons;j++)
        		{
          			/*
          				* Las salidas dinamicas influyen en la actualizacion de los pesos de las conexiones
          				*/
          			OutDinamicSystem[j][i] = neuron[i].resultDerivateActivationFunction * SumOutDinamicSystem[j][i];
          			connection[j][i].updateWeightValue(LearningRatio,errorOut[i],OutDinamicSystem[j][i]);
        		}
      	}
      	for(int k = 0;k < 6;k++){
        		errorOut[k] = 0.5 * pow(errorOut[k] ,2);
            ErrorSum += errorOut[k];
      	}

      	totalerror = ErrorSum/(n+1);

      	std::cout << " tiempo " << CountTime << " error " << totalerror << std::endl;

      	for(int i = 0;i < validationData.size();i++)
      	{
        		//inicializamos entradas
        		for(int j = 0;j < numberInputNeuron;j++){
        			  neuron[i].resultActivationFunction = validationData[i][j];
        		}

        		for(int i = numberInputNeuron;i<NumberNeurons;i++){
          			MUTime =0;
          			for(int j = 0;j<NumberNeurons;j++){
          				MUTime = MUTime + (connection[j][i].weightValue * neuron[j].resultActivationFunction);
          			}
          			neuron[i].MU = MUTime;
          			cache[i] = neuron[i].resultActivationFunction;
          			neuron[i].startActivationFunction();
        		}
            double sumTotal = 0;
            for(int j = 0;j < 6;j++){
                sumTotal += neuron[NumberNeurons - j].resultActivationFunction;
            }

            for(int j = 0;j < 6;j++){
                std::cout<< "output " << i << " class " << j  << " " << neuron[NumberNeurons - j].resultActivationFunction / sumTotal << std::endl;
                MSE_out[j] += pow((validationData[i][13] - neuron[NumberNeurons - j].resultActivationFunction),2);
            }
        		for(int i = numberInputNeuron;i<NumberNeurons;i++){
        			   neuron[i].resultActivationFunction = cache[i];
        		}
      	}
        double validationError=0;

        for(int i = 0;i < 6;i++){
            MSE_out[i] +=  (0.5*MSE_out[i]);
            std::cout << " mseout " << MSE_out[i] << std::endl;
            std::cout << " error validation " << validationError << std::endl;
        }
        for(int k = 0;k < 6;k++ ){
            validationError /=  (1/validationData.size());
        }
      	/*
      		* Invocamos a la función InsertWeightValues
      		*/
      	/*if((CountCross == 5 && n == 99) || (CountCross != 5 && n == 119) )
      	{
      		resultDB = database.InsertResult(sql);
      		if(resultDB == true)cout<<"Records created successfully"<<endl;
      		else cout<<"Error while registering the date"<<endl;
      	}
      	cout<<"ciclo cross " << CountCross + 1 << " error " << ErrorCross << endl;
      	cout<<"Error total " << totalerror << endl;*/
      	if (n == trainData.size() - 1)
      		  n = 0;
      	else
      		  n++;
      	CountTime++;
    }
}
