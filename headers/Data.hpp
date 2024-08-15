#ifndef DATA_HPP
#define DATA_HPP

#include <array>
#include <string>
#include <vector>

#include "../config/config.hpp"
#include "fonction_discret.hpp"


class Data
{   
    public:

    Data();

    //std::array<std::array<double, NB_CLASSE_SOCIAL_CONTACT_MATRIX>, NB_CLASSE_SOCIAL_CONTACT_MATRIX> social_contact_matrix;
    //std::array<std::array<double, NB_CLASSE_SOCIAL_CONTACT_MATRIX>, NB_CLASSE_SOCIAL_CONTACT_MATRIX> social_contact_matrix_home;
    //std::array<std::array<double, NB_CLASSE_SOCIAL_CONTACT_MATRIX>, NB_CLASSE_SOCIAL_CONTACT_MATRIX> social_contact_matrix_school;
    //std::array<std::array<double, NB_CLASSE_SOCIAL_CONTACT_MATRIX>, NB_CLASSE_SOCIAL_CONTACT_MATRIX> social_contact_matrix_work;
    //std::array<std::array<double, NB_CLASSE_SOCIAL_CONTACT_MATRIX>, NB_CLASSE_SOCIAL_CONTACT_MATRIX> social_contact_matrix_transport;
    //std::array<std::array<double, NB_CLASSE_SOCIAL_CONTACT_MATRIX>, NB_CLASSE_SOCIAL_CONTACT_MATRIX> social_contact_matrix_leisure;
    //std::array<std::array<double, NB_CLASSE_SOCIAL_CONTACT_MATRIX>, NB_CLASSE_SOCIAL_CONTACT_MATRIX> social_contact_matrix_other;

    std::array<std::array<double, NB_DAY_CALIBRATION>, NB_DATA_DAY_DEATH> day_death;
    std::array<std::array<double, NB_DAY_CALIBRATION>, NB_DATA_DAY_DEATH> day_hosp;

    template <std::size_t X, std::size_t Y>
    void read_data_csv(std::string filename, std::array<std::array<double, Y>, X>& data_matrix);

};


extern Data DATA;

void write_data_csv(const ODE& f,const std::string& filename);
void write_save_parameters_MCMC(const std::vector<std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE>> p_storage);
void write_save_parameters(const std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE>& p, const std::string& save_nbr);
std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> read_save_parameters(const std::string& save_nbr);
std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> read_nomad_best_feasible_solution(double & obj_fct_value);
std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> set_cond_init_mcmc(int CI_nbr, std::string filename);








#endif