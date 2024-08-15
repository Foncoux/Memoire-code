#ifndef FONCTION_HPP
#define FONCTION_HPP

#include <array>
#include <gsl/gsl_rng.h>

#include "../config/config.hpp"
#include "Nomad/nomad.hpp"


std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> set_parametres_random(gsl_rng* r);
std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> set_parametres_random_normal(gsl_rng* r,double sigma,std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> p_opt);
std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> set_parametres_random_normal_all(gsl_rng* r,double sigma,std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> p_opt);
std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> set_parametres_random_normal_gibbs(gsl_rng* r,double sigma,std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> p_opt);


bool validation_parametres(const std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> p);


#endif