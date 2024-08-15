#include <array>
#include <cstddef>
#include <iomanip>
#include <vector>
#include <gsl/gsl_rng.h>
#include <string>
#include <time.h>


#include "Nomad_file.hpp"

#include "../config/config.hpp"

#include "Data.hpp"
#include "Type/DirectionType.hpp"
#include "fonction_discret.hpp"
#include "fonction_obj.hpp"
#include "Stats.hpp"


std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> nomad_point_to_array(NOMAD::EvalPoint &x)
{
    std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> param;

    for (int i=0; i < NB_PARAM_TOT*NB_CLASSE_AGE; i++) {
        param[i] = x[i].todouble();
    }

    return param;
}


NOMAD::Point set_cond_init(NOMAD::Point X0, int CI_nbr, std::string filename)
{
    std::ifstream file(filename);
    if (!file.is_open()){
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



void initAllParams(std::shared_ptr<NOMAD::AllParameters> allParams)
{
    
    // Number of variables
    size_t n = NB_CLASSE_AGE*NB_PARAM_TOT;
    allParams->setAttributeValue( "DIMENSION", n);

    // max number of evaluation of bb
    allParams->setAttributeValue( "MAX_ITERATIONS", NBR_ITERATION_MADS);

    // Starting point
    NOMAD::Point X0(n);

    X0 = set_cond_init(X0, COND_INIT_NBR,COND_INIT_filename);
    allParams->setAttributeValue( "X0", X0 );

    // Control Granularity, the number of decimal
    allParams->getPbParams()->setAttributeValue("GRANULARITY", NOMAD::ArrayOfDouble(n, 0));

    // Constraints and objective
    NOMAD::BBOutputTypeList bbOutputTypes;
    bbOutputTypes.push_back(NOMAD::BBOutputType::OBJ); // define objective placement
    for (size_t classe = 0; classe < NB_CLASSE_AGE; classe++) // define constraint placement
    {
        bbOutputTypes.push_back(NOMAD::BBOutputType::EB);
        bbOutputTypes.push_back(NOMAD::BBOutputType::EB);
    }

    //Set information to output of MADS
    allParams->setAttributeValue("BB_OUTPUT_TYPE", bbOutputTypes );

    // 2 for normal, 3 is too much, 0 or 1 is nothing
    allParams->setAttributeValue("DISPLAY_DEGREE", DISPLAY_DEGREE_value);

    //Opportunistic strategy: Terminate evaluations as soon as a success is found

    allParams->setAttributeValue("EVAL_OPPORTUNISTIC", NOMAD_STRATEGY);

    NOMAD::DirectionType nomad_direction_type;
    switch (NOMAD_DIRECTION_TYPE) {
        case 1:
            nomad_direction_type = NOMAD::DirectionType::ORTHO_NP1_QUAD;
            break;
        case 2:
            nomad_direction_type = NOMAD::DirectionType::ORTHO_NP1_NEG;
            break;
        case 3:
            nomad_direction_type = NOMAD::DirectionType::ORTHO_2N;
            break;
        case 4:

            break;
        
    }
    
    // Direction types for poll step
    allParams->setAttributeValue("DIRECTION_TYPE", nomad_direction_type);

    // output of the best feasible solution in a file

    std::string best_feasible_sol_filename;
    if(ON_CLUSTER)
    {
        std::ostringstream formattedStream;
        formattedStream << std::setw(3) << std::setfill('0') << COND_INIT_NBR;

        best_feasible_sol_filename = SOLUTION_FILE_MADS_filename + formattedStream.str() + ".txt";
    }else {
        std::ostringstream formattedStream;
        formattedStream << std::setw(3) << std::setfill('0') << COND_INIT_NBR;
        best_feasible_sol_filename = SOLUTION_FILE_MADS_filename + formattedStream.str() + ".txt";
    }

    allParams->setAttributeValue("SOLUTION_FILE", best_feasible_sol_filename);

    // set the lower bound of variables
    allParams->setAttributeValue("LOWER_BOUND", NOMAD::ArrayOfDouble(n, 0.0000000000001)); // all var. >= 0 

    // set the upper bound of variables
    NOMAD::ArrayOfDouble ub(n,1);     // x_i < 1
    /*
    for (size_t i = 0; i < NB_CLASSE_AGE; i++)
    {
        ub[i*NB_PARAM_TOT + PARAM_ID_BETA0] = 1;
        ub[i*NB_PARAM_TOT + PARAM_ID_BETA0 +1] = 1;
        ub[i*NB_PARAM_TOT + PARAM_ID_BETA0 +2] = 1;
        ub[i*NB_PARAM_TOT + PARAM_ID_BETA0 +3] = 1;
        ub[i*NB_PARAM_TOT + PARAM_ID_BETA0 +4] = 1;
        ub[i*NB_PARAM_TOT + PARAM_ID_BETA0 +5] = 1;
        ub[i*NB_PARAM_TOT + PARAM_ID_BETA0 +6] = 1;
    }
    */

    for (size_t classe=0; classe < NB_CLASSE_AGE; classe++)
    {
        ub[classe*NB_PARAM_TOT + PARAM_ID_X0_infect] = 5000/POP_TOT; 
    }

        // x_x_0 <= 5000/POP_TOT 
    allParams->setAttributeValue("UPPER_BOUND", ub);

    //MADS uses anisotropic mesh for generating directions
//    allParams->setAttributeValue("ANISOTROPIC_MESH", true);
    //MADS anisotropy factor for mesh size change
//    allParams->setAttributeValue("ANISOTROPIC_FACTOR", 0.1);

    //histoire de cache

    //Coordinate Search optimization
    //allParams->setAttributeValue("CS_OPTIMIZATION", false);
    // Direction types for Mads secondary poll
    //allParams->setAttributeValue("DIRECTION_TYPE_SECONDARY_POLL", NOMAD::DirectionType::DOUBLE);
    //Flag to display all evaluations
    //allParams->setAttributeValue("DISPLAY_ALL_EVAL", false);
    //Flag to display failed evaluation
    //allParams->setAttributeValue("DISPLAY_FAILED", false);
    //Frequency at which the stats header is displayed
    //allParams->setAttributeValue("DISPLAY_HEADER", 40);
    //Format for displaying the evaluation points
    //allParams->setAttributeValue("DISPLAY_STATS", 0.1);
    //Flag to display unsuccessful
    //allParams->setAttributeValue("DISPLAY_UNSUCCESSFUL", false);
    
    //Opportunistic strategy: Flag to clear EvaluatorControl queue between each run
    //allParams->setAttributeValue("EVAL_QUEUE_CLEAR", 0.1);
    //How to sort points before evaluation
    //allParams->setAttributeValue("EVAL_QUEUE_SORT", 0.1);
    //The name of the file for stats about evaluations and successes
    //allParams->setAttributeValue("EVAL_STATS_FILE", 0.1);
    //Initial value of hMax
    //allParams->setAttributeValue("H_MAX_0", 0.1);

    //The initial frame size of MADS
    //allParams->setAttributeValue("INITIAL_FRAME_SIZE", 0.1);
    //The initial mesh size of MADS
    //allParams->setAttributeValue("INITIAL_MESH_SIZE", 0.1);
    
    //std::string stat_filename = "../stat_nomad.txt";
    //allParams->setAttributeValue("STATS_FILE", stat_filename);
    /*
    std::string eval_stat_filename = "../eval_stat_nomad.txt";
    allParams->setAttributeValue("EVAL_STATS_FILE", eval_stat_filename);
    */
    allParams->setAttributeValue("INITIAL_FRAME_SIZE",NOMAD::ArrayOfDouble(n,0.1));

    allParams->setAttributeValue("MIN_FRAME_SIZE",NOMAD::ArrayOfDouble(n,0));
    allParams->setAttributeValue("MIN_MESH_SIZE",NOMAD::ArrayOfDouble(n,0));

    // Parameters validation
    allParams->checkAndComply();




}


/*----------------------------------------*/
/*               The problem              */
/*----------------------------------------*/

My_Evaluator::My_Evaluator(const std::shared_ptr<NOMAD::EvalParameters>& evalParams)
: NOMAD::Evaluator(evalParams, NOMAD::EvalType::BB) // Evaluator for true blackbox evaluations only
{}

My_Evaluator::~My_Evaluator() {}

bool My_Evaluator::eval_x(NOMAD::EvalPoint &x, const NOMAD::Double &hMax, bool &countEval) const
{
    bool eval_ok = false;
    // Based on G2.
    NOMAD::Double f = 1e+20;
    std::vector<double> nbr;  

    try
    {
        ODE fct(1);
        
        int test = model(fct,x);
        if ( test == 0)
        {
            f = -fonction_obj(fct);
            countEval = true;
        }else{
            f = 1e+20;
            countEval = true;
            /*
            if(test == -2)
            {
                return false;
            }
            */
        }

        NOMAD::Double EB;
        std::string bbo = f.tostring();

        for (size_t classe = 0; classe < NB_CLASSE_AGE; classe++)
        {
            EB = x[NB_PARAM_TOT*classe + PARAM_ID_EPS].todouble() + x[NB_PARAM_TOT*classe + PARAM_ID_R].todouble() -1;
            bbo += " " + EB.tostring();
            EB = x[NB_PARAM_TOT*classe + PARAM_ID_DELTA].todouble() + x[NB_PARAM_TOT*classe + PARAM_ID_GAMMA].todouble() -1;
            bbo += " " + EB.tostring();
        }


        x.setBBO(bbo);

        if (STATS.get_model_evaluation_nbr() % NB_RECORD_IN_STATS_FILE_MADS == 0 && STATS.get_model_evaluation_nbr() != 0)
        {
            double obj_fct_value;
            std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> param_opti = read_nomad_best_feasible_solution(obj_fct_value);
            STATS.close_stats(obj_fct_value, param_opti);
        }
        

        if (f < FCT_OBJ_LIMIT && STOP_FCT_OBJ) {

            double obj_fct_value;
            std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> param_opti = read_nomad_best_feasible_solution(obj_fct_value);
            STATS.close_stats(obj_fct_value, param_opti);
            exit(0);
        }

        eval_ok = true;
    }
    catch (std::exception &e)
    {
        std::string err("Exception: ");
        err += e.what();
        throw std::logic_error(err);
    }

    
    return eval_ok;
}


void main2()
{
    NOMAD::MainStep TheMainStep;

    auto params = std::make_shared<NOMAD::AllParameters>();
    initAllParams(params);
    TheMainStep.setAllParameters(params);

    auto ev = std::make_unique<My_Evaluator>(params->getEvalParams());
    TheMainStep.setEvaluator(std::move(ev));

    try
    {
        TheMainStep.start();
        TheMainStep.run();
        TheMainStep.end();
    }
    catch(std::exception &e)
    {
        std::cerr << "\nNOMAD has been interrupted (" << e.what() << ")\n\n";
    }
}