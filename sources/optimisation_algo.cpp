#include <iostream>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_machine.h>
#include <gsl/gsl_sf_log.h>

#include "../config/setup.hpp"

#include "../headers/MCMC.hpp"
#include "../headers/optimisation_algo.hpp"


std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> optimisation_algo_choice(gsl_rng* random_ptr,ODE& f,std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> cond_init)
{   
    std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> param_opti;
    switch (ALGO)
    {
    case 1:
        break;

    case 2:
        
        break;  
    case 3:
        param_opti = MCMC(cond_init,random_ptr,f);
        break; 
    case 4:
        break;
    default:
        std::cout << "mauvais choix d'algo" <<   std::endl;
        exit(0);
        break;
    }
    return param_opti;
}
