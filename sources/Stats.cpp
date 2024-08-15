
#include <fstream>
#include <iomanip>
#include <string>
#include <iostream>

#include <sstream>

#include "Stats.hpp"

Stats::Stats()
{
    //int COND_INIT_NBR;

    m_nbr_model_evaluation = 0;
    m_nbr_model_evaluation_aborted=0;
    m_obj_fct_value=0;

    m_mcmc_accept = 0;
    m_mcmc_not_accept=0;

    std::stringstream ss;
    std::vector<std::string> names_param;
    std::vector<std::string> NAMES_PARAM_temp = {"delta", "gamma", "eps", "r", "x0_infect", "beta0", "beta1", "beta2", "beta3", "beta4", "beta5", "beta6"};
    for (size_t classe = 0; classe < NB_CLASSE_AGE; classe++)
    {
        for (size_t param_id = 0; param_id < NB_PARAM_TOT; param_id++)
        {
            names_param.push_back(NAMES_PARAM_temp[param_id] + "_" + std::to_string(classe));
        }
        
    }
    for(size_t i = 0; i < names_param.size(); ++i)
    {
        
        ss << ",";
        ss << names_param[i];
    }    
    std::string chaine = ss.str();
    std::string temp = "NbCI,NB_Classe,Method,Parametre_method_1,Parametre_method_2,accept_mcmc,reject_mcmc,fct_obj,nbr_evaluation,nbr_evaluation_aborted,temps,temps_CPU";

    m_header = temp + chaine;
 
}


Stats STATS;

void Stats::add_mcmc_accept()
{
    m_mcmc_accept++;
}

void Stats::add_mcmc_not_accept()
{
    m_mcmc_not_accept++;
}


int Stats::get_model_evaluation_nbr()
{
    return m_nbr_model_evaluation;
}

void Stats::set_file_stats()
{
    std::string filename = make_stats_filename();
    std::ofstream file(filename);
    file << std::fixed << std::setprecision(15);
    
    if (file.is_open()) {

        file << m_header;
        file << "\n";
        file.close();
        
    } else {
        std::cerr << "Impossible d'ouvrir le fichier [Stats.cpp  set_file_stats]" << std::endl;
    }

    
}


void Stats::close_stats(double value, std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> param_opti)
{
    end_timing();
    set_fct_obj(value);
    set_param_opti(param_opti);
    std::string filename = make_stats_filename();
    write_in_file(filename);
}

void Stats::add_model_evaluation()
{
    m_nbr_model_evaluation ++;
    return;
}

void Stats::add_model_evaluation_aborted()
{
    m_nbr_model_evaluation_aborted ++;
    return;
}

void Stats::set_fct_obj(double value)
{   
    if(CHOICE_ALGO_TYPE == ALGO_MCMC)
    {
        m_obj_fct_value = -value;
    }else {
        m_obj_fct_value = value;
    }
    
    return;
}

void Stats::set_param_opti(std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> param_opti)
{
    m_p = param_opti;
    return;
}

void Stats::start_timing()
{
    m_time_start = std::chrono::high_resolution_clock::now();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &m_CPU_time_start);
    return;
}


void Stats::end_timing()
{
    m_time_end = std::chrono::high_resolution_clock::now();
    // Calculating total time taken by the program.
    m_time_taken = std::chrono::duration_cast<std::chrono::nanoseconds>(m_time_end - m_time_start).count();
    m_time_taken *= 1e-9;
    // Stop measuring time and calculate the elapsed time
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &m_CPU_time_end);
    long seconds = m_CPU_time_end.tv_sec - m_CPU_time_start.tv_sec;
    long nanoseconds = m_CPU_time_end.tv_nsec - m_CPU_time_start.tv_nsec;
    m_CPU_time_taken = seconds + nanoseconds*1e-9;

    return;

}

std::string Stats::make_stats_filename()
{
    std::ostringstream formattedStream;
    formattedStream << std::setw(3) << std::setfill('0') << COND_INIT_NBR;

    return STATS_FILENAME + formattedStream.str() + ".csv";

    
}




void Stats::write_in_file(std::string filename)
{

    std::ofstream file(filename, std::ios_base::app);
    file << std::fixed << std::setprecision(15);
    
    if (file.is_open()) {

        file << COND_INIT_NBR << "," << NB_CLASSE_AGE << "," <<  ALGO_NAME << "," << PARAMETRES_STRING_1 << "," << PARAMETRES_STRING_2 << "," << m_mcmc_accept << "," << m_mcmc_not_accept << "," << m_obj_fct_value << "," << m_nbr_model_evaluation << "," << m_nbr_model_evaluation_aborted << "," << m_time_taken << "," << m_CPU_time_taken << "," ;

        for (int i=0;i<NB_PARAM_TOT*NB_CLASSE_AGE; i++){
            file << m_p[i];
            if(i != NB_PARAM_TOT*NB_CLASSE_AGE-1)
            {
                file << ',';
            }
        }
        file << "\n";
        file.close();
        
    } else {
        std::cerr << "Impossible d'ouvrir le fichier [Stats.cpp  write_in_file]" << std::endl;
    }
}

