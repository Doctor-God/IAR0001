#ifndef FUNCOESRESFRIAMENTO_H_
#define FUNCOESRESFRIAMENTO_H_

class Temperatura
{
private:
    double temp_inicial;
    double temp_final;
    double atual;
    int funcao;
    int max_iter;
    void cooling0(int iter);
    void cooling1(int iter);
    void cooling2(int iter);
    void cooling3(int iter);
    void cooling4(int iter);
    void cooling6(int iter);
    void cooling9(int iter);

public:
    Temperatura(double temp_inicial, double temp_final, int funcao);
    void resfria(int iter);
    double getTemp();
};
#endif
