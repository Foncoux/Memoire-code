#include <array>
#include <gsl/gsl_sf_log.h>
#include <gsl/gsl_rng.h>
#include <iostream>
#include <math.h>
#include <vector>

#include "../config/setup.hpp"
#include "../config/config.hpp"

#include "../headers/Data.hpp"
#include "../headers/fonction_obj.hpp"



/**
 * @brief Calcule -log likelyhood relative aux morts par âges
 * 
 * @param data données rélles pour la calibration
 * @param output_data tableau de itérations du modèle
 * @return double -log likelyhood relative au mort totales
 */
double log_likelyhood_death(ODE& output_data)
{
    double somme = 0;
    
    double total_output;
    for (size_t classe = 0; classe < NB_CLASSE_AGE; classe++)
    {
        for (size_t day = 0; day < NB_DAY_CALIBRATION; day++)
        {
            total_output = output_data.m_result_simulation[classe][D_ENTRY_COMP][day]*POP_TOT;
            
            if( total_output > 0){
                somme = somme + (((DATA.day_death[classe][day]))*gsl_sf_log(total_output) - total_output);
            }

        }
    }

    return somme;
    
}



/**
 * @brief Calcule -log likelyhood relative aux morts par âges
 * 
 * @param data données rélles pour la calibration
 * @param output_data tableau de itérations du modèle
 * @return double -log likelyhood relative au mort totales
 */

double log_likelyhood_hosp(ODE& output_data)
{
    double somme = 0;
    
    double total_output;
    for (size_t classe = 0; classe < NB_CLASSE_AGE; classe++)
    {
        
        for (size_t day = 0; day < NB_DAY_CALIBRATION; day++)
        {
            total_output = output_data.m_result_simulation[classe][Q_ENTRY_COMP][day]*POP_TOT;
            
            if( total_output > 0){
                somme = somme + (((DATA.day_hosp[classe][day]))*gsl_sf_log(total_output) - total_output);
            }


        }
    }

    return somme;
}



/**
 * @brief Calcule -log likelyhood total
 *
 * @param data données rélles pour la calibration
 * @param output_data tableau de itérations du modèle
 * @param loglikelyhood 1 si utilisation du log likelyhood, 0 sinon
 * @return double -log likelyhood total
 */

double fonction_obj(ODE& output_data)
{
    double result,result1,result2,result3;
    
    result1 = log_likelyhood_death(output_data);
    result3 = log_likelyhood_hosp(output_data);
    if (ON_SEROPREVALENCE_CALIBRATION == true) {    
        switch (NB_CLASSE_AGE)
        {
        case 1:
            result2 = - least_square_recovered_1(output_data);
            break;
        
        case 2:
            result2 = - least_square_recovered_2(output_data);
            break;

        case 4:
            result2 = - least_square_recovered_4(output_data);
            break;

        case 8:
            result2 = - least_square_recovered_8(output_data);
            break;

        default:
            break;
        }
    }
    else {
        result2 = 0;
    }

    result = result1 + result2 + result3;
    return result;

}




double least_square_recovered_1(ODE& output_data)
{
    double total_output;
    double somme2=0;
    double total_SIR = 0;
    double somme_SIR = 0;    
    std::vector<double> somme_SIR_vector;

    for (int i=0; i<NB_CLASSE_AGE ; i++) {
        somme_SIR = output_data.m_result_simulation[i][S_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY] + output_data.m_result_simulation[i][I_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY] + output_data.m_result_simulation[i][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];

        total_SIR = total_SIR + somme_SIR;

        
    }
    
    total_output = 0;
    for (size_t classe = 0; classe < NB_CLASSE_AGE; classe++)
    {                           
        total_output = total_output + output_data.m_result_simulation[classe][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    }   
    

    if (total_output < (2.1/100)*total_SIR || total_output > (3.7/100)*total_SIR)
    {   
        somme2 = somme2 + sqrt(((2.8/100)*total_SIR - total_output)*((2.8/100)*total_SIR - total_output))*1000000000000;
        
    }


        
    return somme2;
}



double least_square_recovered_2(ODE& output_data)
{
    double total_output=0;
    double somme2=0;
    double total_SIR = 0;
    double somme_SIR = 0;    
    std::vector<double> somme_SIR_vector;

    for (int i=0; i<NB_CLASSE_AGE ; i++) {
        somme_SIR = output_data.m_result_simulation[i][S_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY] + output_data.m_result_simulation[i][I_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY] + output_data.m_result_simulation[i][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];

        somme_SIR_vector.push_back(somme_SIR);

        total_SIR = total_SIR + somme_SIR;
        
    }

    total_output = output_data.m_result_simulation[0][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    if(total_output < (0.6/100)*somme_SIR_vector[0] || total_output > (7.5/100)*somme_SIR_vector[0])
    {   
        somme2 = somme2 + sqrt(((4.05/100)*somme_SIR_vector[0] - total_output)*((4.05/100)*somme_SIR_vector[0] - total_output))*10000000;
    }
    
    total_output = output_data.m_result_simulation[1][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    if(total_output < (1.2/100)*somme_SIR_vector[1] || total_output > (5.1/100)*somme_SIR_vector[1])
    {   
        somme2 = somme2 + sqrt(((2.5/100)*somme_SIR_vector[1] - total_output)*((2.5/100)*somme_SIR_vector[1] - total_output))*10000000;
    }    

    total_output = output_data.m_result_simulation[0][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY] + output_data.m_result_simulation[1][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    if(total_output < (2.1/100)*total_SIR || total_output > (3.7/100)*total_SIR)
    {   
        somme2 = somme2 + sqrt(((2.8/100)*total_SIR - total_output)*((2.8/100)*total_SIR - total_output))*10000000;
    }
    

        
    return somme2;
}



double least_square_recovered_4(ODE& output_data)
{
    
    double total_output=0, somme2=0;
    double total_SIR = 0;
    double somme_SIR = 0;    
    std::vector<double> somme_SIR_vector;

    for (int i=0; i<NB_CLASSE_AGE ; i++) {
        somme_SIR = output_data.m_result_simulation[i][S_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY] + output_data.m_result_simulation[i][I_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY] + output_data.m_result_simulation[i][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
        somme_SIR_vector.push_back(somme_SIR);

        total_SIR = total_SIR + somme_SIR;
        
    }
    
    total_output = output_data.m_result_simulation[0][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    if(total_output < (0.6/100)*somme_SIR_vector[0] || total_output > (4.9/100)*somme_SIR_vector[0])
    {   
        somme2 = somme2 + sqrt(((1.7/100)*somme_SIR_vector[0] - total_output)*((1.7/100)*somme_SIR_vector[0] - total_output))*10000000;
    }

    
    total_output = output_data.m_result_simulation[1][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    if(total_output < (3.2/100)*somme_SIR_vector[1] || total_output > (7.5/100)*somme_SIR_vector[1])
    {   
        somme2 = somme2 + sqrt(((4.9/100)*somme_SIR_vector[1] - total_output)*((4.9/100)*somme_SIR_vector[1] - total_output))*10000000;
    }



    total_output = output_data.m_result_simulation[2][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    if(total_output < (1.2/100)*somme_SIR_vector[2] || total_output > (3.2/100)*somme_SIR_vector[2])
    {   
        somme2 = somme2 + sqrt(((1.9/100)*somme_SIR_vector[2] - total_output)*((1.9/100)*somme_SIR_vector[2] - total_output))*10000000;
    }


    
    total_output = output_data.m_result_simulation[3][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    if(total_output < (1.2/100)*somme_SIR_vector[3] || total_output > (5.1/100)*somme_SIR_vector[3])
    {   
        somme2 = somme2 + sqrt(((2.5/100)*somme_SIR_vector[3] - total_output)*((2.5/100)*somme_SIR_vector[3] - total_output))*10000000;
    }


    total_output = 0;
    for (size_t i = 0; i < 4; i++)
    {
        total_output = total_output + output_data.m_result_simulation[i][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    }
    if(total_output < (2.1/100)*total_SIR || total_output > (3.7/100)*total_SIR)
    {   
        somme2 = somme2 + sqrt(((2.8/100)*total_SIR - total_output)*((2.8/100)*total_SIR - total_output))*10000000;
    }
    
    
    return somme2;
}




double least_square_recovered_8(ODE& output_data)
{
    double total_output=0, somme2=0;
    double total_SIR = 0;
    double somme_SIR = 0;    
    std::vector<double> somme_SIR_vector;
    double SIR_vector_sum_classe;

    for (int i=0; i<NB_CLASSE_AGE ; i++) {
        somme_SIR = output_data.m_result_simulation[i][S_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY] + output_data.m_result_simulation[i][I_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY] + output_data.m_result_simulation[i][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
        somme_SIR_vector.push_back(somme_SIR);

        total_SIR = total_SIR + somme_SIR;
        
    }
    
    total_output = output_data.m_result_simulation[0][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    if(total_output < (0.6/100)*somme_SIR_vector[0]  || total_output > (4.9/100)*somme_SIR_vector[0])
    {   
        somme2 = somme2 + sqrt(((1.7/100)*somme_SIR_vector[0] - total_output)*((1.7/100)*somme_SIR_vector[0] - total_output))*10000000;
    }

    
    total_output = output_data.m_result_simulation[1][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    if(total_output < (3.2/100)*somme_SIR_vector[1] || total_output > (7.5/100)*somme_SIR_vector[1])
    {   
        somme2 = somme2 + sqrt(((4.9/100)*somme_SIR_vector[1] - total_output)*((4.9/100)*somme_SIR_vector[1] - total_output))*10000000;
    }


    SIR_vector_sum_classe = somme_SIR_vector[2] + somme_SIR_vector[3] + somme_SIR_vector[4];
    total_output = output_data.m_result_simulation[2][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY] + output_data.m_result_simulation[3][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY] + output_data.m_result_simulation[4][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    if(total_output < (1.2/100)*SIR_vector_sum_classe || total_output > (3.2/100)*SIR_vector_sum_classe)
    {   
        somme2 = somme2 + sqrt(((1.9/100)*SIR_vector_sum_classe - total_output)*((1.9/100)*SIR_vector_sum_classe - total_output))*10000000;
    }


    SIR_vector_sum_classe = somme_SIR_vector[5] + somme_SIR_vector[6] + somme_SIR_vector[7];
    total_output = output_data.m_result_simulation[5][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY] + output_data.m_result_simulation[6][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY] + output_data.m_result_simulation[7][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    if(total_output < (1.2/100)*SIR_vector_sum_classe || total_output > (5.1/100)*SIR_vector_sum_classe)
    {   
        somme2 = somme2 + sqrt(((2.5/100)*SIR_vector_sum_classe - total_output)*((2.5/100)*SIR_vector_sum_classe - total_output))*10000000;
    }


    total_output = 0;
    for (size_t i = 0; i < 8; i++)
    {
        total_output = total_output + output_data.m_result_simulation[i][R_COMP][DAY_SEROLOGY - DELAY_DAY_SEROLOGY];
    }
    if(total_output < (2.1/100)*total_SIR || total_output > (3.7/100)*total_SIR)
    {   
        somme2 = somme2 + sqrt(((2.8/100)*total_SIR - total_output)*((2.8/100)*total_SIR - total_output))*10000000;
    }
    
    
    return somme2;
}

