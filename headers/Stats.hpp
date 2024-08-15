#ifndef STATS_HPP
#define STATS_HPP


#include <array>
#include <string>
#include <time.h>
#include <chrono>

#include "../config/config.hpp"

class Stats
{   
    private:
    
        int m_nbr_model_evaluation;
        int m_nbr_model_evaluation_aborted;
        double m_obj_fct_value;

        int m_mcmc_accept;
        int m_mcmc_not_accept;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_time_start;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_time_end;
        double m_time_taken;

        struct timespec m_CPU_time_start;
        struct timespec m_CPU_time_end;
        double m_CPU_time_taken;
        std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> m_p;
        
    public:

        Stats();
        void start_timing();

        void end_timing();

        //int COND_INIT_NBR;

        std::string m_header;

        void print();
        void write_in_file(std::string filename);

        void set_file_stats();
        int get_model_evaluation_nbr();

        void add_model_evaluation();
        void add_model_evaluation_aborted();
        void set_fct_obj(double value);
        void set_param_opti(std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> param_opti);

        void close_stats(double value, std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> param_opti);

        std::string make_stats_filename();

        void add_mcmc_accept();
        void add_mcmc_not_accept();


};

extern Stats STATS;

#endif