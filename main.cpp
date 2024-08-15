#include <array>
#include <gsl/gsl_sf_log.h>
#include <gsl/gsl_rng.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

#include "config/config.hpp"

#include "config/setup.hpp"
#include "fonction_discret.hpp"
#include "Data.hpp"

#include "Nomad_file.hpp"
#include "optimisation_algo.hpp"
#include "Stats.hpp"

int main (int argc, char* argv[])
{   
    COND_INIT_NBR = stoi(argv[1]);
    (void) argc;
 

    
    int test = CHOICE_ALGO_TYPE;
    config_table_extern();
;
    std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> param_opti;
    std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> cond_init;

    if(OPTIMISATION == true)
    {
        STATS.set_file_stats();
        STATS.start_timing();

        if(test == 0){
            
            gsl_rng* random_ptr = gsl_rng_alloc(gsl_rng_mt19937);// Initialiser le générateur de nombres aléatoires
            gsl_rng_set(random_ptr,time(NULL));

            ODE f(0);
            
        
            if (READ_SAVE_PARAM)
            {
                cond_init = read_save_parameters(SAVE_TO_READ);
            }else
            {
                cond_init = set_cond_init_mcmc(COND_INIT_NBR,COND_INIT_filename);
                //cond_init = set_parametres_random(random_ptr);
            }
            
            param_opti = optimisation_algo_choice(random_ptr, f, cond_init);

            //model(f,param_opti);
            
            //if (WRITE_SAVE_PARAM == true)
            //{
            //    write_save_parameters(param_opti,SAVE_TO_WRITE); 
            //}
            
            //write_data_csv(f,DATA_TO_WRITE); 
            
            //print_parameter(param_opti);

            gsl_rng_free(random_ptr);


        }else{
            
            main2();
            double obj_fct_value;
            param_opti = read_nomad_best_feasible_solution(obj_fct_value);
            STATS.close_stats(obj_fct_value, param_opti);
            
            

            //param_opti = optimisation_algo_choice(random_ptr, f, param_opti);

            //model(f,param_opti);
            
            //write_save_parameters(param_opti,SAVE_TO_WRITE2);
            //write_data_csv(f,DATA_TO_WRITE); 
        
        }


    }else {
        if(SCRIPT_PYTHON == false)
        {
            ODE f(0);
            cond_init = read_save_parameters(SAVE_TO_READ);
            model(f,cond_init);
            write_data_csv(f,DATA_TO_WRITE); 
        }else {
            ODE f(0);
            //cond_init = set_cond_init_mcmc(COND_INIT_NBR,"../CI_MMMI.csv"); 
            cond_init = read_save_parameters(SAVE_TO_READ_for_python);
            model(f,cond_init);

            //write_data_csv(f,"../temp/output_" + std::to_string(COND_INIT_NBR));
            write_data_csv(f,DATA_TO_WRITE_for_python); 
        }
    }   

    

/*
    std::cout << "Time taken by program is : " << std::fixed << STATS.m_time_taken << std::setprecision(9);
    std::cout << " sec" << std::endl;

    // Calculating total time taken by the program.
    cout << "Time taken (CPU) by program is : " << fixed 
         << STATS.m_CPU_time_taken << setprecision(9);
    cout << " sec " << endl;
    
    STATS.write_in_file();
*/
    return 0;

}

