#include <vector>
#include <iostream>
#include <gsl/gsl_rng.h>

#include "../config/setup.hpp"
#include "../headers/fonction_discret.hpp"
#include "Stats.hpp"


ODE::ODE(int function)
{   
    switch (function)
    {
    case 0:
        m_function_discret_new = SIRQD_discret_new;
    
    case 1:
        m_function_discret_nomad = SIRQD_discret_new;
    default:
        break;
    }

}

void set_condition_initiale(ODE &f,const NOMAD::EvalPoint &p)
{
    for (size_t classe = 0; classe < NB_CLASSE_AGE; classe++)
    {
        f.m_result_simulation[classe][I_COMP][0] = p[NB_PARAM_TOT*classe + PARAM_ID_X0_infect].todouble();
        f.m_result_simulation[classe][S_COMP][0] = PROP_PAR_CLASSE[classe] - p[NB_PARAM_TOT*classe + PARAM_ID_X0_infect].todouble();
        f.m_result_simulation[classe][R_COMP][0] = 0;
        f.m_result_simulation[classe][Q_COMP][0] = 0;
        f.m_result_simulation[classe][D_COMP][0] = 0;
        f.m_result_simulation[classe][Q_ENTRY_COMP][0] = 0;
        f.m_result_simulation[classe][D_ENTRY_COMP][0] = 0;
    }
       
}

void set_condition_initiale(ODE &f,std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> &p)
{
    for (size_t classe = 0; classe < NB_CLASSE_AGE; classe++)
    {
        f.m_result_simulation[classe][I_COMP][0] = p[NB_PARAM_TOT*classe + PARAM_ID_X0_infect];
        f.m_result_simulation[classe][S_COMP][0] = PROP_PAR_CLASSE[classe] - p[NB_PARAM_TOT*classe + PARAM_ID_X0_infect];
        f.m_result_simulation[classe][R_COMP][0] = 0;
        f.m_result_simulation[classe][Q_COMP][0] = 0;
        f.m_result_simulation[classe][D_COMP][0] = 0;
        f.m_result_simulation[classe][Q_ENTRY_COMP][0] = 0;
        f.m_result_simulation[classe][D_ENTRY_COMP][0] = 0;
    }
       
}










int model(ODE& f,std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> &param_opti)
{
    STATS.add_model_evaluation();
    set_condition_initiale(f,param_opti);
    
    int value = bb_discret_new(f,param_opti);
    if(value!=0)
    {
        STATS.add_model_evaluation_aborted();
    }

    return value;
}



/**
 * @brief calcule la force d'infection pour un classe d'age donnée et un jour particulier
 * 
 * @param jour 
 * @param classe_age 
 * @param f 
 * @param data 
 * @return double 
 */
double force_infection_classe(int jour,int classe_age,ODE& f) 
{
    
    double result=0;
    for (size_t j = 0; j < NB_CLASSE_AGE; j++){
        result = result + SOCIAL_CONTACT_MATRIX[classe_age][j]*(f.m_result_simulation[classe_age][I_COMP][jour]) /*+ 0.51*data.social_contact_matrix[classe_age][j]*f[j].m_result_integration[A_COMP][jour]*/ ;
    }

    result = result*PROP_FACTOR_BETA[classe_age];
    
    return result;

}


/**
 * @brief permet d'appliquer la blackbox avec le modèle en temps discret pour plusieurs classe d'age
 * 
 * @param f tableau de fonction discrete. Modifiée en sortie de fonction.
 * @param p tableau de structure parametres pour la fonction f.
 * @param data données réelles pour la calibration.
 */

int bb_discret_new(ODE& f,std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> p)
{   
    int output_model;
    double confinement = 0;
    
    double lambda;
    for (int jour = 0; jour < NB_DAY-1; jour++)
    {   
        if (confinement < TAB_DATE_CONFINEMENT.size()) {
            if (jour == TAB_DATE_CONFINEMENT[confinement]) {
                confinement++;
            }
        }

        for (size_t classe = 0; classe < NB_CLASSE_AGE; classe++)
        {
            lambda = force_infection_classe(jour,classe,f);
            output_model = f.m_function_discret_new(f.m_result_simulation[classe],p,jour,lambda,classe,confinement);
            if(output_model != 0){
                return output_model;
            }    
        }

    }
    return 0;
}



/**
 * @brief caclule l'itération n+1 du modèle SIRQD
 * 
 * @param y tableau 2D contenant toutes les itérations du modèle. Modifié en sortie de fonction
 * @param p parametres pour le modèle
 * @param n entier n pour connaitre l'itération
 * @param lambda force d'infection
 */

int SIRQD_discret_new(std::array<std::array<double, NB_DAY>, COMPARTIMENT_TOT> &y,std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> p,int n,double lambda, int classe, int confinement)
{

    //std::cout << lambda << std::endl;
    double delta = p[NB_PARAM_TOT*classe + PARAM_ID_DELTA];
    double gamma = p[NB_PARAM_TOT*classe + PARAM_ID_GAMMA];
    double eps = p[NB_PARAM_TOT*classe + PARAM_ID_EPS];
    double r = p[NB_PARAM_TOT*classe + PARAM_ID_R];
    double beta = p[NB_PARAM_TOT*classe + NB_PARAM + confinement];

    double Beta_lambda_S = beta*lambda*y[S_COMP][n];
    double delta_I = delta*y[I_COMP][n];
    double r_Q = r*y[Q_COMP][n];

    y[S_COMP][n+1] = y[S_COMP][n] - Beta_lambda_S;
    y[I_COMP][n+1] = y[I_COMP][n] + Beta_lambda_S - (delta+gamma)*y[I_COMP][n];
    y[R_COMP][n+1] = y[R_COMP][n] + gamma*y[I_COMP][n] + eps*y[Q_COMP][n];
    y[Q_COMP][n+1] = y[Q_COMP][n] + delta_I - (eps + r)*y[Q_COMP][n];
    y[D_COMP][n+1] = y[D_COMP][n] + r_Q;
    y[Q_ENTRY_COMP][n+1] = delta_I;
    y[D_ENTRY_COMP][n+1] = r_Q;


    if (beta*lambda > 1)
    {
        std::cout <<  "!!! p.beta[i]*lambda > 1 !!!" << "  \n" << std::endl;
        return -1;
    }
    
    
    //std::cout << y[S_COMP][n+1] << " " << y[I_COMP][n+1] << " " << y[R_COMP][n+1] << " " << y[Q_COMP][n+1] << " " << y[D_COMP][n+1] << std::endl;
    

    if(y[S_COMP][n+1] < 0 || y[I_COMP][n+1] < 0 || y[R_COMP][n+1] < 0 || y[Q_COMP][n+1] < 0 || y[D_COMP][n+1] < 0)

    {
        std::cout << "\n output negatifs \n" << std::endl;
        //return -2;
        exit(0);
    }

    return 0;
}









int model(ODE& f,const NOMAD::EvalPoint &x)
{
    STATS.add_model_evaluation();
    set_condition_initiale(f,x);
    
    int value = bb_discret_new(f,x);
    if(value!=0)
    {
        STATS.add_model_evaluation_aborted();
    }

    return value;
}


/**
 * @brief permet d'appliquer la blackbox avec le modèle en temps discret pour plusieurs classe d'age
 * 
 * @param f tableau de fonction discrete. Modifiée en sortie de fonction.
 * @param p tableau de structure parametres pour la fonction f.
 * @param data données réelles pour la calibration.
 */

int bb_discret_new(ODE& f,const NOMAD::EvalPoint &p)
{   
    int output_model;
    double confinement = 0;
    
    double lambda;

    for (int jour = 0; jour < NB_DAY-1; jour++)
    {           
        if (confinement < TAB_DATE_CONFINEMENT.size()) {
            if (jour == TAB_DATE_CONFINEMENT[confinement]) {
                confinement++;
            }
        }

        for (size_t classe = 0; classe < NB_CLASSE_AGE; classe++)
        {
            lambda = force_infection_classe(jour,classe,f);
            output_model = f.m_function_discret_nomad(f.m_result_simulation[classe],p,jour,lambda,classe,confinement);
            if(output_model != 0){
                return output_model;
            }    
        }

    }

    return 0;
}

/**
 * @brief caclule l'itération n+1 du modèle SIRQD
 * 
 * @param y tableau 2D contenant toutes les itérations du modèle. Modifié en sortie de fonction
 * @param p parametres pour le modèle
 * @param n entier n pour connaitre l'itération
 * @param lambda force d'infection
 */

int SIRQD_discret_new(std::array<std::array<double, NB_DAY>, COMPARTIMENT_TOT> &y,const NOMAD::EvalPoint &p,int n,double lambda, int classe, int confinement)
{
    double delta = p[NB_PARAM_TOT*classe + PARAM_ID_DELTA].todouble();
    double gamma = p[NB_PARAM_TOT*classe + PARAM_ID_GAMMA].todouble();
    double eps = p[NB_PARAM_TOT*classe + PARAM_ID_EPS].todouble();
    double r = p[NB_PARAM_TOT*classe + PARAM_ID_R].todouble();
    double beta = p[NB_PARAM_TOT*classe + NB_PARAM + confinement].todouble();


    double Beta_lambda_S = beta*lambda*y[S_COMP][n];
    double delta_I = delta*y[I_COMP][n];
    double r_Q = r*y[Q_COMP][n];

    y[S_COMP][n+1] = y[S_COMP][n] - Beta_lambda_S;
    y[I_COMP][n+1] = y[I_COMP][n] + Beta_lambda_S - (delta+gamma)*y[I_COMP][n];
    y[R_COMP][n+1] = y[R_COMP][n] + gamma*y[I_COMP][n] + eps*y[Q_COMP][n];
    y[Q_COMP][n+1] = y[Q_COMP][n] + delta_I - (eps + r)*y[Q_COMP][n];
    y[D_COMP][n+1] = y[D_COMP][n] + r_Q;
    y[Q_ENTRY_COMP][n+1] = delta_I;
    y[D_ENTRY_COMP][n+1] = r_Q;

    

    if (beta*lambda > 1)
    {
        //std::cout <<  "!!! p.beta[i]*lambda > 1 !!!" << "  \n" << std::endl;
        //throw std::invalid_argument("beta*lambda > 1");
        return -1;
    }
    
    
    //std::cout << y[S_COMP][n+1] << " " << y[I_COMP][n+1] << " " << y[R_COMP][n+1] << " " << y[Q_COMP][n+1] << " " << y[D_COMP][n+1] << std::endl;
    

    if(y[S_COMP][n+1] < 0 || y[I_COMP][n+1] < 0 || y[R_COMP][n+1] < 0 || y[Q_COMP][n+1] < 0 || y[D_COMP][n+1] < 0)
    {
        //std::cout << "\n output negatifs \n" << std::endl;

        //std::cout << delta + gamma << "  " << eps + r << "  " << std::endl;

        return -2;
        //throw std::invalid_argument("outputs négatifs");
        exit(0);
    }

    return 0;
}


