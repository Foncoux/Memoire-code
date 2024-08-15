/**
 * @file MCMC.cpp
 * @author Esteban Foncoux
 * @brief gere la partie MCMC
 * @version 0.1
 * @date 2023-06-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <cmath>
#include <vector>

#include <iostream>
#include <iomanip>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_machine.h>
#include <gsl/gsl_sf_log.h>
#include <gsl/gsl_sf_exp.h>


#include "../config/setup.hpp"

#include "../headers/Parametres.hpp"
#include "../headers/fonction_discret.hpp"

#include "../headers/Data.hpp"
#include "../headers/fonction_obj.hpp"

#include "../headers/MCMC.hpp"

#include "Stats.hpp"

std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> MCMC(std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> cond_init,gsl_rng* random_ptr,ODE& f)
{

    std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> p = cond_init;
    
    double sigma = SIGMA;
    if(BURNIN_PHASE == true)
    {
        p = burning_phase(p,f,random_ptr,sigma);
    }
    if(MCMC_PHASE == true)
    {
        p = metropolis(p,f,random_ptr,sigma);
    }
    
    return p;

}

std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> burning_phase(std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> cond_init,ODE& f,gsl_rng* r,double &sigma)
{
    double LL_old;
    double LL_new;
    double alpha;
    std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> p_old;
    std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> p_new;
    double nombre_acceptation = 0;
    double taux_acceptation = 0;
    
    double gamma = 0.3;

    /*
    while (model(f,cond_init) !=0)
    {
        cond_init = set_parametres_random(r);
    }
    */
    model(f,cond_init);

    p_old = cond_init;
    LL_old = fonction_obj(f);

    STATS.close_stats(LL_old, p_old);
    
    
    int j;
    int iter_total=0;
    int iter_maj_sigma=0;
    bool stop = false; 

    if (ON_CLUSTER == false) {
    
    
    std::cout << std::left  << std::setw(10) << "ite_tot"
                            << std::setw(4) << "|"
                            << std::setw(15) << "Nb_post_dist"
                            << std::setw(4) << "|"
                            << std::setw(15) << "taux_accept"
                            << std::setw(4) << "|"
                            << std::setw(15) << "LL_old"
                            << std::setw(4) << "|"
                            << std::setw(15) << "sigma"
                            << std::setw(4) << "|"
                            << std::setw(15) << "sigma"
                            << "\r" << std::endl;
    }
    while(stop == false)
    {
    
        p_new = set_parametres_random_normal(r,sigma,p_old);

        if (model(f,p_new) == 0)
        {
            LL_new = fonction_obj(f);
            alpha = gsl_rng_uniform(r);

            if (gsl_sf_log(alpha) < LL_new - LL_old)
            {   
                p_old = p_new;
                LL_old = LL_new;
                
                nombre_acceptation++;
                
                STATS.add_mcmc_accept();
                
            }else {
                STATS.add_mcmc_not_accept();
            }
            



            taux_acceptation = nombre_acceptation/(iter_maj_sigma+1);

            

            if (iter_maj_sigma > 5000)
            {
                iter_maj_sigma = 0;
                sigma = sigma*gsl_sf_exp(gamma*((taux_acceptation-TAUX_ACCEPT_OBJ)/(TAUX_ACCEPT_OBJ)));
                
                sigma = sigma > 0.001 ? 0.001 : sigma;
                sigma = sigma < 0.0000001 ? 0.0000001 : sigma;

                nombre_acceptation=0;

            }
            iter_maj_sigma++;

        if (ON_CLUSTER == false) {
    
            std::cout << std::left  << std::setw(10) << iter_total
                                    << std::setw(4) << "|"
                                    << std::setw(15) << j
                                    << std::setw(4) << "|"
                                    << std::setw(15) << std::setprecision(10)<< taux_acceptation
                                    << std::setw(4) << "|"
                                    << std::setw(15) << std::fixed << std::setprecision(2) << LL_old
                                    << std::setw(4) << "|"
                                    << std::setw(15) << std::setprecision(10) << p_old[PARAM_ID_DELTA]
                                    << std::setw(4) << "|"
                                    << std::setw(15) << p_old[NB_PARAM_TOT + PARAM_ID_DELTA]
                                    << std::setw(4) << "|"
                                    << std::setw(15)<< std::setprecision(10) << sigma

                                    << "\r" << std::flush;
        }



             
        }else
        {
            iter_total--;
        }     
        
        if(iter_total>BURNIN_STEP)
        {
            stop=true;
        }
        
        iter_total++; 


        if (STATS.get_model_evaluation_nbr() % NB_RECORD_IN_STATS_FILE == 0)
        {
            STATS.close_stats(LL_old, p_old);
        }


        if (LL_old > -FCT_OBJ_LIMIT && STOP_FCT_OBJ) {
            STATS.close_stats(LL_old,p_old);
            gsl_rng_free(r);
            exit(0);
        }
      

    }

    
    
    std::cout << std::setprecision(16) << std::endl;
    return p_old;

}







std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> metropolis(std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> cond_init,ODE& f,gsl_rng* r,double &sigma)
{
    double LL_old,LL_new;
    double alpha;
    std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> p_old;
    std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE> p_new;

    std::vector<std::array<double,NB_PARAM_TOT*NB_CLASSE_AGE>> p_storage;
    double nombre_acceptation = 0;
    double taux_acceptation = 0;
    std::string savename;

    while (model(f,cond_init) !=0)
    {
        cond_init = set_parametres_random(r);
    }
    
    p_old = cond_init;
    LL_old = fonction_obj(f);
    
    int j=0;
    int iter_select = 0;
    int iter_total=0;
    bool stop = false; 
    //std::cout << "ite_tot\t| " << "Nb_post_dist\t| " << "taux_acceptation\t| " << "LL_old\t\t| " << "\n";
    

    std::cout << std::left  << std::setw(10) << "ite_tot"
                            << std::setw(4) << "|"
                            << std::setw(15) << "Nb_post_dist"
                            << std::setw(4) << "|"
                            << std::setw(15) << "taux_accept"
                            << std::setw(4) << "|"
                            << std::setw(15) << "LL_old"
                            << std::setw(4) << "|"
                            << std::setw(15) << "LL_new - LL_old"
                            << std::setw(4) << "|"
                            << std::setw(15) << "sigma"
                            << "\r" << std::endl;



    while(stop == false)
    {
      
        p_new = set_parametres_random_normal(r,sigma,p_old);

        if (model(f,p_new) == 0)
        {
            LL_new = fonction_obj(f);
            alpha = gsl_rng_uniform(r);

            if (gsl_sf_log(alpha) < LL_new - LL_old)
            {   
                p_old = p_new;
                LL_old = LL_new;
                
                nombre_acceptation++;
                
            }

 
                  
            taux_acceptation = nombre_acceptation/(iter_total+1);
            if (iter_select > ITE_RECUP_MCMC)
            {
                iter_select=0;
                

                p_storage.push_back(p_old);

                model(f,p_old);
                savename = DATA_MCMC_WRITE + std::to_string(j);
                write_data_csv(f,savename);

                j++;
                if (j >= NB_POST_DIST)
                {
                    stop = true;
                    
                }
                
            }

            

            std::cout << std::left  << std::setw(10) << iter_total
                                    << std::setw(4) << "|"
                                    << std::setw(15) << j
                                    << std::setw(4) << "|"
                                    << std::setw(15) << std::setprecision(10)<< taux_acceptation
                                    << std::setw(4) << "|"
                                    << std::setw(15) << std::fixed << std::setprecision(2) << LL_old
                                    << std::setw(4) << "|"
                                    << std::setw(15) << LL_new - LL_old
                                    << std::setw(4) << "|"
                                    << std::setw(15)<< std::setprecision(10) << sigma
                                    << "\r" << std::flush;

            iter_select++;
        }else
        {
            iter_total--;
        }     
    
        
        iter_total++;
    }
    
    write_save_parameters_MCMC(p_storage);

    std::cout << std::setprecision(16) << std::endl;
    return p_old;

}