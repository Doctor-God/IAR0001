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


vector<bool> randomSolution(int nro_var)
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
        if(dice_roll < 0.01)
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

int annealing(int exec, int nro_var, vector<Clausula> claus, double temp_inicial, double temp_final, int func_resf)
{
    ofstream dados;
    dados.open("annealing_" + to_string(nro_var) + "var_" + to_string(exec) + ".txt");
    Temperatura temp(temp_inicial, temp_final, func_resf);
    int iteracao_atual = 0;

    int nro_claus = claus.size();

    vector<bool> solucao = randomSolution(nro_var);
    int nro_satisfeitas = satisfiedClauses(solucao, claus);
    double satisfeitas_norm = (double) 1.0 - nro_satisfeitas/nro_claus; //normalização de satisfeitas entre 0 e 1 (mais clausulas resolvidas, mais próximo de 0)

    while(temp.getTemp() > temp_final and iteracao_atual < 250000)
    {
        dados << satisfeitas_norm << endl;

        vector<bool> novaSolucao = perturba(solucao);
        int nova_nro_satisfeitas = satisfiedClauses(novaSolucao, claus);
        double nova_satisfeitas_norm = (double) 1.0 - nova_nro_satisfeitas/nro_claus; //normalização de satisfeitas da nova solução entre 0 e 1 (mais clausulas resolvidas, mais próximo de 0)

        if(nova_satisfeitas_norm > satisfeitas_norm)
        {
            solucao = novaSolucao;
            // nro_satisfeitas = nova_nro_satisfeitas;
            satisfeitas_norm = nova_satisfeitas_norm;
        }
        else if(nova_satisfeitas_norm < satisfeitas_norm)
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
    dados.close();
    return  satisfiedClauses(solucao, claus);

}

int randomSearch(int exec, int nro_var, vector<Clausula> claus)
{
    ofstream dados;
    dados.open("random_" + to_string(nro_var) + "var_" + to_string(exec) + ".txt");

    vector<bool> melhor_solucao = randomSolution(nro_var);
    int most_solved = satisfiedClauses(melhor_solucao, claus);

    dados << most_solved << endl;
    for(int i = 0; i < 250000; i++)
    {

        vector<bool> solucao = randomSolution(nro_var);
        int nro_satisfeitas = satisfiedClauses(solucao, claus);

        if(nro_satisfeitas > most_solved)
        {
            melhor_solucao = solucao;
            most_solved = nro_satisfeitas;
        }
        dados << nro_satisfeitas << endl;
    }
    dados.close();
    return most_solved;

}


//Dicas: execuções são independentes das outras
//            feita a execução, já colocar a rotina para o gráfico de convergência

int main(int argc, char const *argv[])
{
    //argv[1] = arq entrada
    //argv[2] = temperatura inicial
    //argv[3] = temperatura final
    //argv[4] = função de resfriamento a utilizar (implementar algumas alternativas)

    if(argc != 5)
    {
        cout << "Entrada: ./a.out arq_entr temp_inicial temp_final func_resfr\n";
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


    for(int i = 0; i < 10; i++)
    {
        int res = annealing(i, nro_var, clausulas, stod(argv[2]), stod(argv[3]), atoi(argv[4]));
        cout << "Annealing Exec " << i << ": " << res << " de " << nro_claus << " satisfeitas" << endl;
    }

    cout << endl;

    for(int i = 0; i < 10; i++)
    {
        int res = randomSearch(i, nro_var, clausulas);
        cout << "Random Exec " << i << ": " << res << " de " << nro_claus << " satisfeitas" << endl;
    }

    return 0;
}
