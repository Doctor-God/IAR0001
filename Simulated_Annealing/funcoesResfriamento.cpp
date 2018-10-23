#include "funcoesResfriamento.hpp"
#include <bits/stdc++.h>

Temperatura::Temperatura(double temp_inicial, double temp_final, int funcao)
{
    this->funcao = funcao;
    this->temp_inicial = temp_inicial;
    this->temp_final = temp_final;
    atual = temp_inicial;
    max_iter = 250000;
}

void Temperatura::resfria(int iter)
{
        switch(funcao)
        {
            case 0:
                Temperatura::cooling0(iter);
                 break;
            case 1:
                Temperatura::cooling1(iter);
                break;
            case 2:
                Temperatura::cooling2(iter);
                break;
            case 3:
                Temperatura::cooling3(iter);
                break;
            case 4:
                Temperatura::cooling4(iter);
                break;
            case 6:
                Temperatura::cooling6(iter);
                break;
            case 8:
                Temperatura::cooling8(iter);
                break;
            case 9:
                Temperatura::cooling9(iter);
                break;
        }
}


double Temperatura::getTemp()
{
    return atual;
}


void Temperatura::cooling0(int iter)
{
    double temp = temp_inicial - iter*((temp_inicial - temp_final))/max_iter;
    atual = temp;
}

void Temperatura::cooling1(int iter)
{
    double temp = temp_inicial*pow((temp_final/temp_inicial), iter/max_iter);
    atual = temp;
}

void Temperatura::cooling2(int iter)
{
    double a = ((temp_inicial - temp_final)*(max_iter + 1))/max_iter;
    double b = temp_inicial - a;
    double temp = a/(iter + 1) + b;
    atual = temp;
}


void Temperatura::cooling3(int iter)
{
    double a = log(temp_inicial - temp_final)/log(max_iter);
    double temp = temp_inicial - pow(iter, a);
    atual = temp;
}


void Temperatura::cooling4(int iter)
{
    double temp = (temp_inicial - temp_final)/(1 + exp(3*(iter - (max_iter/2)))) + temp_final;
    atual = temp;
}


void Temperatura::cooling6(int iter)
{
    double temp = 0.5 * (temp_inicial - temp_final)*(1 - tanh((10*iter/max_iter)-5)) + temp_final;
    atual = temp;
}

void Temperatura::cooling8(int iter)
{
    double temp = 0.5*(temp_inicial - temp_final)*(1 + cos((iter*M_PI)/max_iter)) + temp_final;
    atual = temp;
}

void Temperatura::cooling9(int iter)
{
    double a = (1/pow(max_iter, 2))*log(temp_inicial/temp_final);
    atual = temp_inicial*exp(-a*pow(iter, 2));
}
