/*
Copyright 2013 Kathryn Atwell
	
This file is part of DTC-OSTI-InfectiousDisease-F.

DTC-OSTI-InfectiousDisease-F is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DTC-OSTI-InfectiousDisease-F is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DTC-OSTI-InfectiousDisease-F. If not, see <http://www.gnu.org/licenses/>.
*/

#include "mex.h"
#include "matrix.h"

 /*declare all model parameters*/
  double s; 
  double d;
  double beta; 
  double alpha; 
  double d; 
  double k0;
  double p;
  double c;
  double a_E;
  double theta;
  double d_E;
  
/*give the names of the functions that: 1) read in parameter struct, */ 
/*2) specify the model equations, 3) ODE solver*/
#define odeInit initDerivativesEM
#define odeFunction derivativesEM
#define odeStepper rk4Step


void initDerivativesEM(const mxArray *params)
{
    s = mxGetScalar(mxGetField(params,0,"s"));
    d = mxGetScalar(mxGetField(params,0,"d"));
    beta = mxGetScalar(mxGetField(params,0,"beta"));
    alpha = mxGetScalar(mxGetField(params,0,"alpha"));
    d = mxGetScalar(mxGetField(params,0,"d"));
    k0 = mxGetScalar(mxGetField(params,0,"k0"));
    p = mxGetScalar(mxGetField(params,0,"p"));
    c = mxGetScalar(mxGetField(params,0,"c"));
    a_E = mxGetScalar(mxGetField(params,0,"a_E"));
    theta = mxGetScalar(mxGetField(params,0,"theta"));
    d_E = mxGetScalar(mxGetField(params,0,"d_E"));
}

void derivativesEM(double t, const double* currenty, double* nexty)
{
    nexty[0]=s-d*currenty[0]-beta*currenty[0]*currenty[2];
    nexty[1]=beta*currenty[0]*currenty[2]-(alpha*d+k0*currenty[3])*currenty[1];
    nexty[2]=p*currenty[1]-c*currenty[2];
    nexty[3]=a_E*currenty[1]/(theta+currenty[1])-d_E*currenty[3];
} 


/*DO NOT NEED TO EDIT ANYTHING BELOW THIS LINE TO CHANGE MODEL*/
/*____________________________________________________________*/

void rk4Step(
        mwSize numberVariables, 
        void (*odeFunction)(double, const double *, double *), 
        double h, 
        double t, 
        const double *currentStep, 
        double *nextStep)
{
    int i;
    double *const k1 = (double*)malloc(sizeof(double)*numberVariables);
    double *const k2 = (double*)malloc(sizeof(double)*numberVariables);
    double *const k3 = (double*)malloc(sizeof(double)*numberVariables);
    double *const k4 = (double*)malloc(sizeof(double)*numberVariables);
    
    double *const derivInput = (double*)malloc(sizeof(double)*numberVariables);
    double *const derivOutput = (double*)malloc(sizeof(double)*numberVariables);
    
    
    odeFunction(t,currentStep,derivOutput);
    for(i=0; i<numberVariables; i++){
        k1[i] = h*derivOutput[i];
        derivInput[i] = currentStep[i]+0.5*k1[i];
    }
    
    odeFunction(t+0.5*h,derivInput,derivOutput);
    for(i=0; i<numberVariables; i++){
        k2[i] = h*derivOutput[i];
        derivInput[i] = currentStep[i]+0.5*k2[i];
    }
    
    odeFunction(t+0.5*h,derivInput,derivOutput);
    for(i=0; i<numberVariables; i++){
        k3[i] = h*derivOutput[i];
        derivInput[i] = currentStep[i]+k3[i];
    }
    
    odeFunction(t+h,derivInput,derivOutput);
    for(i=0; i<numberVariables; i++){
        k4[i] = h*derivOutput[i];
        nextStep[i] = currentStep[i] + (1/6.0)*(k1[i]+2*k2[i]+2*k3[i]+k4[i]);
    }
    
    free(k1);
    free(k2);
    free(k3);
    free(k4);
    free(derivInput);
    free(derivOutput);
}



void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    
    /*N = Number of steps to take*/
    int N;
    int i; 
    double h;
    mxArray* t;
    mwSize numberVariables;
    double* times;
    mxArray* y;
    double* solution;
    mxArray* yTranspose;
    
    /*Read inputs*/
    double* timeRange = mxGetPr(prhs[0]);
    double* initialConditions = mxGetPr(prhs[1]);
    
    odeInit(prhs[2]);
    
    N=5000;
    h=(timeRange[1]-timeRange[0])/(double)N;
    numberVariables = mxGetDimensions(prhs[1])[1];
    
    
    /*Enter timepoints*/
    t = mxCreateDoubleMatrix(N,1,mxREAL); 
    times = mxGetPr(t);
    for(i=0; i<N; i++){
        times[i]=timeRange[0]+i*h;
    }

    /*Make y vector and initialise*/    
    y = mxCreateDoubleMatrix(numberVariables,N,mxREAL);
    solution = mxGetPr(y);
    for(i=0; i<numberVariables; i++){
        solution[i] = initialConditions[i]; 
    }
    
    
    /*Solve ODE using Runge-Kutta*/
    for(i=0;i<N-1;i++){
        odeStepper(numberVariables, 
                &odeFunction, 
                h, 
                times[i], 
                solution+i*numberVariables,
                solution+(i+1)*numberVariables);
    }
    
    yTranspose = mxCreateDoubleMatrix(N,numberVariables,mxREAL);
    mexCallMATLAB(1,&yTranspose,1,&y,"transpose");
    mxDestroyArray(y);
    
    plhs[0]=t;
    plhs[1]=yTranspose;
    
}


