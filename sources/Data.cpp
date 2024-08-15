#include <iostream>
#include <fstream>
#include <array>
#include <iomanip> 
#include <gsl/gsl_rng.h>

#include "../config/setup.hpp"
#include "../headers/Data.hpp"

Data::Data()
{
    read_data_csv<NB_DATA_DAY_DEATH, NB_DAY_CALIBRATION>(DAY_DEATH_DATA_filename, day_death);
    read_data_csv<NB_DATA_DAY_HOSP, NB_DAY_CALIBRATION>(DAY_HOSP_DATA_filename, day_hosp);
}

Data DATA;

template <std::size_t X, std::size_t Y>
void Data::read_data_csv(std::string filename, std::array<std::array<double, Y>, X>& data_matrix)
{
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier. (data.cpp [read_data_csv)" << std::endl;
        return;
    }

    std::string line;
    std::getline(file,line);

    int row = 0;
    while (std::getline(file, line) && row < Y) {
        std::istringstream iss(line);
        std::string value;
        int col = 0;

        while (std::getline(iss, value, ',') && col < X ) {
            if (std::stod(value) != -1) {
                data_matrix[col][row] = std::stod(value);
            } else {
                data_matrix[col][row] = -1;
            }
            col++;
        }
        row++;
    }

    file.close();
}






void write_data_csv(const ODE& f,const std::string& filename1) {
    std::ofstream file(filename1 + ".csv");
    if (file.is_open()) {

        //put headers in the top of the csv file
        for (int i=0;i<COMPARTIMENT_TOT*NB_CLASSE_AGE; i++) {
            file << NAMES_COMPARTIMENT_CSV[i];
            if(i != COMPARTIMENT_TOT*NB_CLASSE_AGE-1)
            {
                file << ',';
            }
        }
        file << "\n";

        for (size_t day = 0; day < NB_DAY; day++)
        {
            for (int classe=0; classe < NB_CLASSE_AGE; classe++) 
            {
                for (int comp=0;comp<COMPARTIMENT_TOT; comp++) 
                {
                    file << f.m_result_simulation[classe][comp][day];

                    if(comp != COMPARTIMENT_TOT-1)
                    {
                        file << ',';
                    }
                    if(classe != NB_CLASSE_AGE-1 && comp == COMPARTIMENT_TOT-1)
                    {
                        file << ',';
                    }
                }
            }
            file << '\n';
        }
        file.close();
        
    }else{
        std::cerr << "Impossible d'ouvrir le fichier [data.cpp write_data_csv] " << filename1 << std::endl;
    }
}


void write_save_parameters_MCMC(const std::vector<std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE>> p_storage) {
    
    std::ofstream file(std::string(SAVE_MCMC_WRITE) + ".csv");
    file << std::fixed << std::setprecision(15);
    
    if (file.is_open()){
        //put headers in the top of the csv file
        for (int i=0;i<NB_PARAM_TOT*NB_CLASSE_AGE; i++) {
            file << NAMES_PARAM[i];
            if(i != NB_PARAM_TOT*NB_CLASSE_AGE-1)
            {
                file << ',';
            }
        }
        file << "\n";

        //data storage
        for (size_t vec = 0; vec < p_storage.size(); vec++)
        {
            for (int i=0;i<NB_PARAM_TOT*NB_CLASSE_AGE; i++){
                file << p_storage[vec][i];
                if(i != NB_PARAM_TOT*NB_CLASSE_AGE-1)
                {
                    file << ',';
                }
            }
            file << "\n";
        }
        
        file.close();
        
    } else {
        std::cerr << "Impossible d'ouvrir le fichier [data.cpp write_save_parameters_MCMC]" << std::endl;
    }
}



void write_save_parameters(const std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE>& p, const std::string& save_nbr) {
    std::ofstream file(save_nbr + ".csv");
    file << std::fixed << std::setprecision(15);
    if (file.is_open()) {
        for (int i=0;i<NB_PARAM_TOT*NB_CLASSE_AGE; i++) {
            file << NAMES_PARAM[i];
            if(i != NB_PARAM_TOT*NB_CLASSE_AGE-1)
            {
                file << ',';
            }
        }
        file << "\n";

        for (int i=0;i<NB_PARAM_TOT*NB_CLASSE_AGE; i++){
            file << p[i];
            if(i != NB_PARAM_TOT*NB_CLASSE_AGE-1)
            {
                file << ',';
            }
        }
        
        file.close();
        
    } else {
        std::cerr << "Impossible d'ouvrir le fichier [data.cpp write_save_parameters]" << save_nbr << std::endl;
    }
}


std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> read_save_parameters(const std::string& filename)
{
    std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> p;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier. (read_save_parameters)" << std::endl;
        exit(0);
    }

    std::string line;
    std::getline(file,line);

    std::getline(file, line);
    std::istringstream iss(line);
    std::string value;
    int i = 0;

    while (std::getline(iss, value, ',') && i < NB_PARAM_TOT*NB_CLASSE_AGE ) {
        p[i] = std::stod(value);
        
        i++;
    }
        
    file.close();
    return p;
}


std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> read_nomad_best_feasible_solution(double & obj_fct_value)
{
    std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> p;
    std::ifstream file;
    if(ON_CLUSTER)
    {
        std::ostringstream formattedStream;
        formattedStream << std::setw(3) << std::setfill('0') << COND_INIT_NBR;

        file.open(SOLUTION_FILE_MADS_filename + formattedStream.str() + ".txt");
    }else {
        std::ostringstream formattedStream;
        formattedStream << std::setw(3) << std::setfill('0') << COND_INIT_NBR;

        file.open(SOLUTION_FILE_MADS_filename + formattedStream.str() + ".txt");
    }
    
    if (!file.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier. (read_nomad_best_feasible)" << std::endl;
        exit(0);
    }


    double value;
    int i=0;
    while (file >> value && i < NB_PARAM_TOT*NB_CLASSE_AGE) {
        p[i] = value;
        i++;
    }
    obj_fct_value = value;

    // Fermer le fichier
    file.close();
    
    return p;
}




std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> set_cond_init_mcmc(int CI_nbr, std::string filename)
{

    std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> X0;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier. (set_cond_init)" << std::endl;
        exit(0);
    }

    std::string line;
    std::getline(file,line);

    for (size_t i = 0; i < CI_nbr+1; i++)
    {
        std::getline(file,line);
    }

    std::istringstream iss(line);
    std::string value;
    int i = 0;

    while (std::getline(iss, value, ',') && i < NB_PARAM_TOT*NB_CLASSE_AGE ) {
        X0[i] = std::stod(value);
        
        i++;
    }
    
    file.close();
    return X0;
}


/*
void write_output_data_in_file(const std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE>& p)
{
    std::ofstream file("../output.csv");
    file << std::fixed << std::setprecision(15);
    
    if (file.is_open()) {

        file << "Method,Parametre_method_1,Parametre_method_2,fct_obj,nbr_evaluation,nbr_evaluation_aborted,temps,temps_CPU," ;

        for (int i=0;i<NB_PARAM_TOT*NB_CLASSE_AGE; i++) {
            file << NAMES_PARAM[i];
            if(i != NB_PARAM_TOT*NB_CLASSE_AGE-1)
            {
                file << ',';
            }
        }
        file << "\n";


        file << "MCMC,Gibbs,/" << "," << STAT_obj_fct_value << "," << STAT_nbr_model_evaluation << "," << STAT_nbr_model_evaluation_aborted << "," << STAT_time_taken << "," << STAT_CPU_time_taken << "," ;

        for (int i=0;i<NB_PARAM_TOT*NB_CLASSE_AGE; i++){
            file << p[i];
            if(i != NB_PARAM_TOT*NB_CLASSE_AGE-1)
            {
                file << ',';
            }
        }
        
        file.close();
        
    } else {
        std::cerr << "Impossible d'ouvrir le fichier [data.cpp write_save_parameters]" << std::endl;
    }
}
*/

