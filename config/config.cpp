#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iomanip> 


#include "config.hpp"


std::vector<int> TAB_DATE_CONFINEMENT;
std::vector<double> PROP_FACTOR_BETA;

std::vector<double> PROP_PAR_CLASSE;
std::vector<std::string> NAMES_COMPARTIMENT;
std::vector<std::string> NAMES_COMPARTIMENT_CSV;
std::vector<std::string> NAMES_PARAM;
std::vector<std::vector<double>> SOCIAL_CONTACT_MATRIX;
std::vector<std::string> SELECT_CLASSE_AGE;

int COND_INIT_NBR;


void config_table_extern(){

    switch (NB_CONFINEMENT)
    {
    case 4:
        //std::array<int, NB_CONFINEMENT> TAB_DATE_CONFINEMENT = {19,76,233,269};
        TAB_DATE_CONFINEMENT = {21,77,98,233};
        break;
        
    case 6:
        TAB_DATE_CONFINEMENT = {21,77,98,133,189,233};
        break;

    default:
        break;
    }
    





    
    //std::array<double, NB_CLASSE_AGE> PROP_PAR_CLASSE = {0.2434811,0.2719328,0.2918983,0.1926878};
    //std::array<double, NB_CLASSE_AGE> PROP_PAR_CLASSE = {0.23,0.265,0.332,0.173};
    switch (NB_CLASSE_AGE)
    {
    case 1:
        PROP_PAR_CLASSE = {1};
        PROP_FACTOR_BETA = {1};
        break;

    case 2:
        PROP_PAR_CLASSE = {0.80511053, 0.19488947};
        //PROP_FACTOR_BETA = {0.771,0.9353333333};
        PROP_FACTOR_BETA = {1,1};
        break;

    case 4:
        PROP_PAR_CLASSE = {0.21687425,0.25169953,0.33653675,0.19488947};
        //PROP_FACTOR_BETA = {0.3903333333,1.0805,0.9453333333,0.9353333333};
        PROP_FACTOR_BETA = {1,1,1,1};

        break;
    case 8:
        PROP_PAR_CLASSE = {0.21687425,0.25169953,0.12684563,0.14547785,0.06421327,0.05721919,0.05406948,0.08360080};
        //PROP_FACTOR_BETA = {0.3903333333,1.0805,1.003,0.897,0.936,0.936,0.953,0.935};
        PROP_FACTOR_BETA = {1,1,1,1,1,1,1,1};


        break;



    default:
        break;
    }




    
    switch (COMPARTIMENT_TOT)
    {
    case 7:
        NAMES_COMPARTIMENT = {"S","I","R","Q","D","Q_entry","D_entry"};
        break;
    
    case 8:
        NAMES_COMPARTIMENT = {"S","I","R","Q","D","E"};
        break;
    
    default:
        break;
    }

    for (size_t classe = 0; classe < NB_CLASSE_AGE; classe++)
    {
        for (size_t comp = 0; comp < COMPARTIMENT_TOT; comp++)
        {
            NAMES_COMPARTIMENT_CSV.push_back(NAMES_COMPARTIMENT[comp] + "_" + std::to_string(classe));
        }
        
    }


    std::string filename = SOCIAL_CONTACT_MATRIX_filename;
    set_social_contact_matrix(SOCIAL_CONTACT_MATRIX,filename);
    
    std::vector<std::string> NAMES_PARAM_temp = {"delta", "gamma", "eps", "r", "x0_infect", "beta0", "beta1", "beta2", "beta3", "beta4", "beta5", "beta6"};
    
    for (size_t classe = 0; classe < NB_CLASSE_AGE; classe++)
    {
        for (size_t param_id = 0; param_id < NB_PARAM_TOT; param_id++)
        {
            NAMES_PARAM.push_back(NAMES_PARAM_temp[param_id] + "_" + std::to_string(classe));
        }
        
    }
    





}


void set_social_contact_matrix(std::vector<std::vector<double>>& matrix, std::string filename) {   
    
    std::ifstream file(filename);
    if(!file.is_open()){
        std::cout << "Impossible d'ouvrir le fichier. (set social contact matrix config.cpp)" << std::endl;
        return;
    }
    
    std::string line;
    
    while(std::getline(file, line)){
        std::vector<double> row;
        std::istringstream iss(line);
        std::string value;
        while(std::getline(iss, value, ',')){
            row.push_back(std::stod(value));
        }
        matrix.push_back(row);
    }

    file.close();    
}


//https://dub.uu.nl/en/depth/timeline-looking-back-whole-year-pandemic
//https://grenzinfo.eu/nl/strengere-maatregelen-in-nederland-vanaf-14-oktober-2020/
//https://www.rijksoverheid.nl/onderwerpen/coronavirus-tijdlijn/november-2020-verzwaring-en-verlenging-van-de-gedeeltelijke-lockdown
//https://www.rijksoverheid.nl/onderwerpen/coronavirus-tijdlijn/maart-2020-maatregelen-tegen-verspreiding-coronavirus
//https://www.containmentnu.nl/articles/timeline?lang=en
//https://www.researchgate.net/figure/Timeline-of-the-COVID-19-pandemic-in-the-Netherlands-and-Lifelines-data-collection-A_fig1_350142568
//file:///home/foncoux/T%C3%A9l%C3%A9chargements/jech-2020-215678.full1.pdf
//https://dc-covid.site.ined.fr/en/data/netherlands/