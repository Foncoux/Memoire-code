#ifndef MCMC_HPP
#define MCMC_HPP

#include <array>
#include <gsl/gsl_rng.h>
#include "../config/config.hpp"
#include "fonction_discret.hpp"


std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> MCMC(std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> cond_init,gsl_rng* random_ptr,ODE& f);
std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> metropolis(std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> cond_init,ODE& f,gsl_rng* r,double &sigma);
std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> burning_phase(std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> cond_init,ODE& f,gsl_rng* r,double &sigma);
#endif