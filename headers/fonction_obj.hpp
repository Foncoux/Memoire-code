#ifndef FONCTION_OBJ_HPP
#define FONCTION_OBJ_HPP

#include "fonction_discret.hpp"

double log_likelyhood_death(ODE& output_data);
double fonction_obj(ODE& output_data);
double log_likelyhood_hosp(ODE& output_data);

double least_square_recovered_1(ODE& output_data);
double least_square_recovered_2(ODE& output_data);
double least_square_recovered_4(ODE& output_data);
double least_square_recovered_8(ODE& output_data);

#endif