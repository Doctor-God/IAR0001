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
