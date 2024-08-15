#ifndef SETUP_HPP
#define SETUP_HPP

//setup (don't touch until next comment)
#define GIBBS 1
#define METROPOLIS 0

#define ALGO_NOMAD 1
#define ALGO_MCMC 0

#define OPPORTUNISTIC_STRATEGY true
#define NOT_OPPORTUNISTIC_STRATEGY false

#define NB_CONFINEMENT 6
#define DISCRET 1
////////////////////////////////////////

#define ON_CLUSTER true
#define STOP_FCT_OBJ false

//select wich algorithm is used : 
// ALGO_MCMC or ALGO_NOMAD
#define CHOICE_ALGO_TYPE ALGO_NOMAD

 
//select the method used for MCMC :
// GIBBS or METROPOLIS
#define MCMC_PARAM_TYPE_SELECTION GIBBS

//select the strategy used for NOMAD :
// OPPORTUNISTIC_STRATEGY or NOT_OPPORTUNISTIC_STRATEGY
#define NOMAD_STRATEGY NOT_OPPORTUNISTIC_STRATEGY


//select the method used for MCMC :
//1 : ORTHO_NP1_QUAD;
//2 : ORTHO_NP1_NEG;
//3 : ORTHO_2N;
#define NOMAD_DIRECTION_TYPE 3

#define NB_CLASSE_AGE NB_CLASSE_AGE_CMAKE




#define SIGMA 0.000003
#define SIGMA_INIT_BURNING 0.0005
#define ALGO 3
#define READ_SAVE_PARAM false
#define WRITE_SAVE_PARAM false

#if NB_CLASSE_AGE_CMAKE == 1
    //#define NB_ITER_TOT 16000000
    #define NB_ITER_TOT 200000000
    #define NBR_ITERATION_MADS 32000000
#elif NB_CLASSE_AGE_CMAKE == 2
    //define NB_ITER_TOT 70000000
    #define NB_ITER_TOT 200000000
    #define NBR_ITERATION_MADS 80000000

#elif NB_CLASSE_AGE_CMAKE == 4
    //#define NB_ITER_TOT 60000000
    #define NB_ITER_TOT 200000000
    #define NBR_ITERATION_MADS 120000000
#elif NB_CLASSE_AGE_CMAKE == 8
    //#define NB_ITER_TOT 100000000
    #define NB_ITER_TOT 200000000
    #define NBR_ITERATION_MADS 200000000
#endif



#define NB_RECORD_IN_STATS_FILE 20000
#define NB_RECORD_IN_STATS_FILE_MADS 200

#define OPTIMISATION true
#define SCRIPT_PYTHON false

#define BURNIN_PHASE true
#define MCMC_PHASE false



#define ON_SEROPREVALENCE_CALIBRATION false



/////////////////////////////////////////////
//don't touch

#if ON_SEROPREVALENCE_CALIBRATION == true
    #define SERO_FILENAME_PART ""
#elif ON_SEROPREVALENCE_CALIBRATION == false
    #define SERO_FILENAME_PART "WITHOUT_SERO/"
#endif



#if MCMC_PHASE == true
    #define BURNIN_STEP 700000
#elif MCMC_PHASE == false
    #define BURNIN_STEP NB_ITER_TOT
#endif

#endif 