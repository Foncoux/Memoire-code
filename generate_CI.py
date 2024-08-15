from scipy.stats import qmc
import pandas as pd
import numpy as np

nb_sample = 100
# Vos limites supérieures et inférieures


# Noms de base pour les colonnes
base_names = ["delta","gamma","eps","r","x0_infect","beta0","beta1","beta2","beta3","beta4","beta5","beta6"]








# Nombre de classes
num_classes = 1  
eigenvalue = 14.09858
POP_TOT = 17407585.0

u_bounds_original = [1,1,1,1,5000/POP_TOT,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue]
l_bounds_original = [0.000000000000001]*12


u_bounds = []
l_bounds = []
for i in range(num_classes):
    u_bounds = u_bounds + u_bounds_original
    l_bounds = l_bounds + l_bounds_original


sampler = qmc.LatinHypercube(d=12*num_classes)
sample = sampler.random(n=nb_sample)
sample_scaled = qmc.scale(sample, l_bounds, u_bounds)

# Mettre à l'échelle chaque deuxième variable en fonction de la première pour chaque échantillon
for i in range(nb_sample):
    for classe in range(num_classes):
        sample_scaled[i, classe*12 + 1] = sample_scaled[i, classe*12 + 1]*(1-sample_scaled[i, classe*12 + 0])
        sample_scaled[i, classe*12 + 3] = sample_scaled[i, classe*12 + 3]*(1-sample_scaled[i, classe*12 + 2])

# Créer un DataFrame vide avec les noms de colonnes appropriés
column_names = [f"{name}_{i}" for i in range(num_classes) for name in base_names]
df = pd.DataFrame(sample_scaled,columns=column_names)

df.to_csv("./data/Conditions_initiales/Classe_1_CI.csv",index = False)



###########################
###########################
###########################

# Nombre de classes
num_classes = 2  
eigenvalue = 14.4923
u_bounds_original = [1,1,1,1,5000/POP_TOT,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue]


u_bounds = []
l_bounds = []
for i in range(num_classes):
    u_bounds = u_bounds + u_bounds_original
    l_bounds = l_bounds + l_bounds_original


sampler = qmc.LatinHypercube(d=12*num_classes)
sample = sampler.random(n=nb_sample)
sample_scaled = qmc.scale(sample, l_bounds, u_bounds)

# Mettre à l'échelle chaque deuxième variable en fonction de la première pour chaque échantillon
for i in range(nb_sample):
    for classe in range(num_classes):
        sample_scaled[i, classe*12 + 1] = sample_scaled[i, classe*12 + 1]*(1-sample_scaled[i, classe*12 + 0])
        sample_scaled[i, classe*12 + 3] = sample_scaled[i, classe*12 + 3]*(1-sample_scaled[i, classe*12 + 2])

# Créer un DataFrame vide avec les noms de colonnes appropriés
column_names = [f"{name}_{i}" for i in range(num_classes) for name in base_names]
df = pd.DataFrame(sample_scaled,columns=column_names)

df.to_csv("./data/Conditions_initiales/Classe_2_CI.csv",index = False)




###########################
###########################
###########################

# Nombre de classes
num_classes = 4  

eigenvalue = 14.9728
u_bounds_original = [1,1,1,1,5000/POP_TOT,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue]


u_bounds = []
l_bounds = []
for i in range(num_classes):
    u_bounds = u_bounds + u_bounds_original
    l_bounds = l_bounds + l_bounds_original


sampler = qmc.LatinHypercube(d=12*num_classes)
sample = sampler.random(n=nb_sample)
sample_scaled = qmc.scale(sample, l_bounds, u_bounds)

# Mettre à l'échelle chaque deuxième variable en fonction de la première pour chaque échantillon
for i in range(nb_sample):
    for classe in range(num_classes):
        sample_scaled[i, classe*12 + 1] = sample_scaled[i, classe*12 + 1]*(1-sample_scaled[i, classe*12 + 0])
        sample_scaled[i, classe*12 + 3] = sample_scaled[i, classe*12 + 3]*(1-sample_scaled[i, classe*12 + 2])

# Créer un DataFrame vide avec les noms de colonnes appropriés
column_names = [f"{name}_{i}" for i in range(num_classes) for name in base_names]
df = pd.DataFrame(sample_scaled,columns=column_names)

df.to_csv("./data/Conditions_initiales/Classe_4_CI.csv",index = False)

###########################
###########################
###########################

# Nombre de classes
num_classes = 8

eigenvalue = 15.054
u_bounds_original = [1,1,1,1,5000/POP_TOT,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue,1/eigenvalue]


u_bounds = []
l_bounds = []
for i in range(num_classes):
    u_bounds = u_bounds + u_bounds_original
    l_bounds = l_bounds + l_bounds_original


sampler = qmc.LatinHypercube(d=12*num_classes)
sample = sampler.random(n=nb_sample)
sample_scaled = qmc.scale(sample, l_bounds, u_bounds)

# Mettre à l'échelle chaque deuxième variable en fonction de la première pour chaque échantillon
for i in range(nb_sample):
    for classe in range(num_classes):
        sample_scaled[i, classe*12 + 1] = sample_scaled[i, classe*12 + 1]*(1-sample_scaled[i, classe*12 + 0])
        sample_scaled[i, classe*12 + 3] = sample_scaled[i, classe*12 + 3]*(1-sample_scaled[i, classe*12 + 2])

# Créer un DataFrame vide avec les noms de colonnes appropriés
column_names = [f"{name}_{i}" for i in range(num_classes) for name in base_names]
df = pd.DataFrame(sample_scaled,columns=column_names)

df.to_csv("./data/Conditions_initiales/Classe_8_CI.csv",index = False)



#sampler = qmc.LatinHypercube(d=12, optimization="random-cd")

#sample = sampler.random(n=100)
#sample_scaled = qmc.scale(sample, l_bounds, u_bounds)

#print(qmc.discrepancy(sample_scaled))


