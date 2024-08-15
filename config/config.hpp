#ifndef CONFIG_HPP1
#define CONFIG_HPP1

#include <vector>
#include <string>

#include "setup.hpp"


void config_table_extern();
void set_social_contact_matrix(std::vector<std::vector<double>>& matrix, std::string filename);

#define POP_TOT 17407585.0

#define S_COMP 0
#define I_COMP 1
#define R_COMP 2
#define Q_COMP 3
#define D_COMP 4
#define Q_ENTRY_COMP 5
#define D_ENTRY_COMP 6
/*295*/
#define NB_DAY 350
#define NB_DAY_CALIBRATION 282

extern int COND_INIT_NBR;

#define DELAY_DAY_SEROLOGY 7
#define DAY_SEROLOGY 39


#define NB_PARAM 5
#define PARAM_ID_DELTA 0
#define PARAM_ID_GAMMA 1
#define PARAM_ID_EPS 2
#define PARAM_ID_R 3

#define PARAM_ID_X0_infect 4
#define PARAM_ID_BETA0 5

#define NB_PARAM_TOT (NB_PARAM + NB_CONFINEMENT + 1)



#define PHANTOM_COMPART 2
#define COMPARTIMENT 5
#define COMPARTIMENT_TOT (PHANTOM_COMPART+COMPARTIMENT)

extern std::vector<int> TAB_DATE_CONFINEMENT;
extern std::vector<double> PROP_FACTOR_BETA;

extern std::vector<double> PROP_PAR_CLASSE;
extern std::vector<std::string> NAMES_COMPARTIMENT;
extern std::vector<std::string> NAMES_COMPARTIMENT_CSV;

extern std::vector<std::string> NAMES_PARAM;
extern std::vector<std::vector<double>> SOCIAL_CONTACT_MATRIX;
extern std::vector<std::string> SELECT_CLASSE_AGE;



#if NB_CLASSE_AGE == 1
    #define FCT_OBJ_LIMIT -5000.0

    #define NB_DATA_DAY_DEATH 1
    #define NB_DATA_DAY_HOSP 1
    #define NB_CLASSE_SOCIAL_CONTACT_MATRIX 1

    #define CLASSE_FILENAME_PART "Classe_1"

#endif

#if NB_CLASSE_AGE == 2

    #define FCT_OBJ_LIMIT -5000.0

    #define NB_DATA_DAY_DEATH 3
    #define NB_DATA_DAY_HOSP 3
    #define NB_CLASSE_SOCIAL_CONTACT_MATRIX 2

    #define CLASSE_FILENAME_PART "Classe_2"
#endif

#if NB_CLASSE_AGE == 4

    #define FCT_OBJ_LIMIT -5000.0

    #define NB_DATA_DAY_DEATH 5
    #define NB_DATA_DAY_HOSP 5
    #define NB_CLASSE_SOCIAL_CONTACT_MATRIX 4

    #define CLASSE_FILENAME_PART "Classe_4"
#endif


#if NB_CLASSE_AGE == 8

    #define FCT_OBJ_LIMIT -5000.0

    #define NB_DATA_DAY_DEATH 9
    #define NB_DATA_DAY_HOSP 9
    #define NB_CLASSE_SOCIAL_CONTACT_MATRIX 8

    #define CLASSE_FILENAME_PART "Classe_8"
#endif

#define DAY_DEATH_DATA_filename "../data/DATA_calibration/" CLASSE_FILENAME_PART "/death.csv"
#define DAY_HOSP_DATA_filename "../data/DATA_calibration/" CLASSE_FILENAME_PART "/hosp.csv"
#define SOCIAL_CONTACT_MATRIX_filename "../data/DATA_calibration/" CLASSE_FILENAME_PART "/Social_contact_matrix.csv"

#define SAVE_TO_READ "../data/save_dir/save_SIRQD_" CLASSE_FILENAME_PART ".csv"
#define SAVE_TO_WRITE "../data/save_dir/save_SIRQD_Classe_1" CLASSE_FILENAME_PART
#define SAVE_TO_WRITE2 "../data/save_dir/save_SIRQD_" CLASSE_FILENAME_PART "_mads" 
#define COND_INIT_filename "../data/Conditions_initiales/" CLASSE_FILENAME_PART "_CI.csv"



#define SAVE_TO_READ_for_python "../../visualisation/Model_cpp_call/input_model/param_to_read.csv"
#define DATA_TO_WRITE_for_python "../../visualisation/Model_cpp_call/output_model/output_compartiments"



#define DATA_TO_WRITE "../data/integration"

#define SAVE_MCMC_WRITE "../data/MCMC_param/save_MCMC"
#define DATA_MCMC_WRITE "../data/MCMC_result/data_MCMC"

#define STOP_ALGO 0.00001

#define ITE_RECUP_MCMC 10000
#define NB_POST_DIST 100

#define RADIUS_INIT 0.05



#if MCMC_PARAM_TYPE_SELECTION == 1
    #define TAUX_ACCEPT_OBJ 0.5
    #define MCMC_PARAM_TYPE_SELECTION_string "Gibbs"
#endif

#if MCMC_PARAM_TYPE_SELECTION == 0
    #define TAUX_ACCEPT_OBJ 0.234
    #define MCMC_PARAM_TYPE_SELECTION_string "Metropolis"
#endif

#if NOMAD_STRATEGY == OPPORTUNISTIC_STRATEGY
    #define NOMAD_STRATEGY_string "OPPORTUNISTIC_STRATEGY"
#elif NOMAD_STRATEGY == NOT_OPPORTUNISTIC_STRATEGY
    #define NOMAD_STRATEGY_string "NOT_OPPORTUNISTIC_STRATEGY"
#endif

#if NOMAD_DIRECTION_TYPE == 1
    #define NOMAD_DIRECTION_TYPE_string "/ORTHO_NP1_QUAD"
#elif NOMAD_DIRECTION_TYPE == 2
    #define NOMAD_DIRECTION_TYPE_string "/ORTHO_NP1_NEG"
#elif NOMAD_DIRECTION_TYPE == 3
    #define NOMAD_DIRECTION_TYPE_string "/ORTHO_2N"
#endif

#if CHOICE_ALGO_TYPE == ALGO_NOMAD
    #define ALGO_NAME "NOMAD"
    #define PARAMETRES_STRING_1 NOMAD_STRATEGY_string 
    #define PARAMETRES_STRING_2 NOMAD_DIRECTION_TYPE_string
    

#endif
#if CHOICE_ALGO_TYPE == ALGO_MCMC
    #define ALGO_NAME "MCMC"
    #define PARAMETRES_STRING_1 MCMC_PARAM_TYPE_SELECTION_string 
    #define PARAMETRES_STRING_2 ""
#endif







#if ON_CLUSTER
    #define STATS_FILENAME "/workdir/efoncoux/STATS_files/" SERO_FILENAME_PART CLASSE_FILENAME_PART "/" ALGO_NAME "/" PARAMETRES_STRING_1 PARAMETRES_STRING_2 "/stat_file_"
    #define SOLUTION_FILE_MADS_filename "/workdir/efoncoux/MADS_temp/" SERO_FILENAME_PART CLASSE_FILENAME_PART "/" ALGO_NAME "/" PARAMETRES_STRING_1 PARAMETRES_STRING_2 "/best_feasible_point_"
    #define DISPLAY_DEGREE_value 0
    

#endif

#if ON_CLUSTER == false
    #define STATS_FILENAME "../data/STATS_files/" SERO_FILENAME_PART CLASSE_FILENAME_PART "/" ALGO_NAME "/" PARAMETRES_STRING_1 PARAMETRES_STRING_2 "/stat_file_"
    #define SOLUTION_FILE_MADS_filename "../data/MADS_temp/" SERO_FILENAME_PART CLASSE_FILENAME_PART "/" ALGO_NAME "/" PARAMETRES_STRING_1 PARAMETRES_STRING_2 "/best_feasible_point_"
    #define DISPLAY_DEGREE_value 2

#endif





#endif

