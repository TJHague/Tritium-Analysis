import numpy as np

#Avogadro's Number
Na = 6.0221409e23

#Positron Corrections
#Return a value that is multiplied by the Yield to correct for positron contamination
def He3Positrons(x):
    c = np.exp((x*-8.424)-2.635)
    c = (1-c)
    return c

def H3Positrons(x):
    c = np.exp((x*-8.429)-2.611)
    c = (1-c)
    return c

def H2Positrons(x):
    c = np.exp((x*-9.157)-2.465)
    c = (1-c)
    return c

def H1Positrons(x):
    c = np.exp((x*-10.146)-2.389)
    c = (1-c)
    return c

#Multiplicative factor by which the targets density is affected by the beam current
def He3Boiling(I):
    return (1 + (-0.004759*I) + (8.69e-5*I*I))

def H3Boiling(I):
    return (1 + (-0.007399*I) + (0.0001293*I*I))

def H2Boiling(I):
    return (1 + (-0.006651*I) + (0.0001147*I*I))

def H1Boiling(I):
    return (1 + (-0.008529*I) + (0.0001527*I*I))

#Target thickness as affected by the beam current
def He3Thickness(I):
    return 0.0534*He3Boiling(I)

def H3Thickness(I):
    return 0.077*H3Boiling(I)

def H2Thickness(I):
    return 0.1422*H2Boiling(I)

def H1Thickness(I):
    return 0.0708*H1Boiling(I)

#Nuclei in the given target thickness seen by the beam
def He3Nuclei(I):
    return Na*He3Thickness(I)/3.0160293

def H3Nuclei(I):
    return Na*H3Thickness(I)/3.0160492

def H2Nuclei(I):
    return Na*H2Thickness(I)/2.01410178

def H1Nuclei(I):
    return Na*H1Thickness(I)/1.00794

#Nuclei in the given target thickness seen by the beam
def He3Nucleons():
    return 3

def H3Nucleons():
    return 3

def H2Nucleons():
    return 2

def H1Nucleons():
    return 1

#Endcap Contamination correction

def He3H2ECC(x):
    ecc = 1
    ecc -= np.exp((-2.54412287*x) - 3.69437788)
    return ecc

def H3He3ECC(x):
    ecc = 1
    ecc += np.exp((-5.03250363*x) - 3.97644112)
    return ecc

def H2H1ECC(x):
    ecc = 1
    ecc += np.exp((-3.90944653*x) - 3.50802943)
    return ecc

def H3H2ECC(x):
    ecc = H3He3ECC(x) * He3H2ECC(x)
    return ecc

#Charge values for coulomb corrections
def He3Z():
    return 2

def H3Z():
    return 1

def H2Z():
    return 1

def H1Z():
    return 1

#Hard sphere radius for coulomb corrections
def He3HSR():
    He3_rmsR = 1.88
    return (5.*He3_rmsR/3.)**(1./2.)

def H2HSR():
    H2_rmsR = 2.127
    return (5.*H2_rmsR/3.)**(1./2.)
