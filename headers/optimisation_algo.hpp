#ifndef BLACKBOX_HPP
#define BLACKBOX_HPP

#include <array>
#include <gsl/gsl_rng.h>

#include "../config/config.hpp"
#include "fonction_discret.hpp"

std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> optimisation_algo_choice(gsl_rng* random_ptr,ODE& f,std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> cond_init);



#endif