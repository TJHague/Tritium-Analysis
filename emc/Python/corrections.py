#Avogadro's Number
Na = 6.0221409e23

#Positron Corrections
#Return a value that is multiplied by the Yield to correct for positron contamination
def He3Positrons(x):
    c = 0.1008*np.exp(-1*x*8.913)
    c = (1-c)
    return c

def H3Positrons(x):
    c = 0.0778*np.exp(-1*x*8.026)
    c = (1-c)
    return c

def H2Positrons(x):
    c = 0.087557*np.exp(-1*x*8.446762)
    c = (1-c)
    return c

def H1Positrons(x):
    c = 0.079031*np.exp(-1*x*8.587641)
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
    return 0.077*H3Thickness(I)

def H2Thickness(I):
    return 0.1422*H2Boiling(I)

def H1Thickness(I):
    return 0.0708*H1Boiling(I)

#Nuclei in the given target thickness seen by the beam
def He3Nuclei(I):
    return He3Thickness(I)/3.0160293

def H3Nuclei(I):
    return H3Thickness(I)/3.0160492

def H2Nuclei(I):
    return H2Thickness(I)/2.01410178

def H1Nuclei(I):
    return H1Thickness(I)/1.00794

#Nuclei in the given target thickness seen by the beam
def He3Nucleons(I):
    return He3Nuclei(I)*3

def H3Nucleons(I):
    return H3Nuclei(I)*3

def H2Nucleons(I):
    return H2Nuclei(I)*2

def H1Nucleons(I):
    return H1Nuclei(I)

#Endcap Contamination corretction as a function of kinematic
#TODO: Convert this to depend on x


