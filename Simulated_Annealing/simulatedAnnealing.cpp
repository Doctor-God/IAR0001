#include <bits/stdc++.h>
#include "funcoesResfriamento.hpp"
//Solução: vetor de valores booleanos para as variáveis
//Avaliação da solução: nro de cláusulas satisfeitas
//Perturbação do problema: flippar o valor de uma ou mais variáveis, dependendo da temperatura (não flippar muitas ~ max 5%)

using namespace std;

random_device rd{};
mt19937 generator(rd());


class Clausula
{
private:
    int v1, v2, v3; //Variáveis associadas à clausula (se ela for negativa está negada)
    bool satisfied; //Talvez não seja útil

public:
    Clausula(int v1, int v2, int v3)
    {
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
    }

    bool isSatisfied(vector<bool> &solucao)
    {
        return (v1 < 0 ? not solucao[abs(v1)-1] : solucao[v1-1]) or
                (v2 < 0 ? not solucao[abs(v2)-1] : solucao[v2-1]) or
                (v3 < 0 ? not solucao[abs(v3)-1] : solucao[v3-1]);
    }
};

double getRandDouble(double a, double b)
{
    uniform_real_distribution<double> dis(a, b);
    return dis(generator);
}

int getRandInt(int a, int b)
{
    uniform_int_distribution<int> dis(a, b);
    return dis(generator);
}


vector<bool> initialSolution(int nro_var)
{
    vector<bool> variaveis(nro_var);
    for(int i = 0; i < nro_var; i++)
    {
        variaveis[i] = getRandInt(0,2); //Gera random entre  0 e 1
    }

    return variaveis;
}

vector<bool> perturba(vector<bool> atual) //Não queremos que ele mude a solução atual, mas crie uma nova
{
    //Uma variável aleatória é flippada
    // int deve_mudar = getRandInt(0, atual.size());
    // atual[deve_mudar] = not atual[deve_mudar];


    // Cada variável tem 5% de chance de ser flippada
    for(int i = 0; i < atual.size(); i++)
    {
        double dice_roll = getRandDouble(0.0, 1.0);
        if(dice_roll < 0.05)
        {
            atual[i] = not atual[i];
        }
    }

    return atual;

}

int satisfiedClauses(vector<bool> &solucao, vector<Clausula> claus)
{
    int num = 0;
    for(int i = 0; i < claus.size(); i++)
    {
        if(claus[i].isSatisfied(solucao))
            num++;

    }
    return num;
}

int annealing(int nro_var, vector<Clausula> claus, double temp_inicial, double temp_final, int func_resf)
{
    ofstream dados;
    dados.open("saida_" + to_string(nro_var) + "var.txt");
    Temperatura temp(temp_inicial, temp_final, func_resf);
    int iteracao_atual = 0;

    vector<bool> solucao = initialSolution(nro_var);
    int nro_satisfeitas = satisfiedClauses(solucao, claus);

    while(temp.getTemp() > 0 and iteracao_atual < 250000)
    {
        dados << nro_satisfeitas << endl;

        vector<bool> novaSolucao = perturba(solucao);
        int nova_nro_satisfeitas = satisfiedClauses(novaSolucao, claus);

        if(nova_nro_satisfeitas > nro_satisfeitas)
        {
            solucao = novaSolucao;
            nro_satisfeitas = nova_nro_satisfeitas;
        }
        else if(nova_nro_satisfeitas < nro_satisfeitas)
        {
            double dice_roll = getRandDouble(0.0, 1.0);
            if(dice_roll < exp(-(nro_satisfeitas - nova_nro_satisfeitas)/temp.getTemp())) //Aceita a solução "ruim"
            {
                solucao = novaSolucao;
                nro_satisfeitas = nova_nro_satisfeitas;
            }
        }

        iteracao_atual++;
        temp.resfria(iteracao_atual);
    }
    return  satisfiedClauses(solucao, claus);

}

//Dicas: execuções são independentes das outras
//            feita a execução, já colocar a rotina para o gráfico de convergência

int main(int argc, char const *argv[])
{
    //argv[1] = arq entrada
    //argv[2] = temperatura inicial
    //argv[3] = temperatura final
    //argv[4] = função de resfriamento a utilizar (implementar algumas alternativas)

    if(argc != 5) //Adicionar os outros depois
    {
        cout << "Entrada: ./simulatedAnnealing.cpp arq_entr temp_inicial temp_final func_resfr\n";
        exit(1);
    }

    ifstream file;
    file.open(argv[1]);

    int nro_var, nro_claus;

    file >> nro_var >> nro_claus;

    vector<bool> variaveis(nro_var);
    vector<Clausula> clausulas;

    int var1, var2, var3, zero;
    while(file >> var1 >> var2 >> var3 >> zero)
    {
        clausulas.push_back(Clausula(var1, var2, var3));
    }


    int res = annealing(nro_var, clausulas, stod(argv[2]), stod(argv[3]), atoi(argv[4])); //Colocar função resfriamento também
    cout << res << " de " << nro_claus << " satisfeitas" << endl;

    return 0;
}
