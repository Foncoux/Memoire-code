#ifndef NOMAD_FILE_HPP
#define NOMAD_FILE_HPP

#include "Nomad/nomad.hpp"

// Déclaration de la classe d'évaluateur
class My_Evaluator : public NOMAD::Evaluator
{
public:
    My_Evaluator(const std::shared_ptr<NOMAD::EvalParameters>& evalParams);
    ~My_Evaluator();

    bool eval_x(NOMAD::EvalPoint &x, const NOMAD::Double &hMax, bool &countEval) const override;
};

// Déclaration de la fonction d'initialisation des paramètres
void initAllParams(std::shared_ptr<NOMAD::AllParameters> allParams);

// Déclaration de la fonction principale
void main2();


#endif // MAIN_HPP

