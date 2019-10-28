#Uncertainty calculation functions

#Positron calculation uncertainties
def He3PositronsUnc(x):
  e2 = x*((x*0.18265823) - 0.04746606)
  e2 += (x*-0.04746606) + 0.01300976
  e2 *= He3Positrons(x)**2
  return e2

def H3PositronsUnc(x):
  e2 = x*((x*0.11225664) - 0.02803864)
  e2 += (x*-0.02803864) + 0.0073564
  e2 *= H3Positrons(x)**2
  return e2

def H2PositronsUnc(x):
  e2 = x*((x*0.08534386) - 0.0208084)
  e2 += (x*-0.0208084 ) + 0.00529717
  e2 *= H2Positrons(x)**2
  return e2 

def H1PositronsUnc(x):
  e2 = x*((x*0.4684634) - 0.10968713)
  e2 += (x*-0.10968713) + 0.02644275
  e2 *= H1Positrons(x)**2
  return e2

#Boiling Uncertainties
def He3BoilingUnc(I):
  e2 = 2.27472736e-5 + (I*-3.87742681e-6) + (I*I*1.33896582e-7)
  e2 += I*(-3.87742681e-6 + (I*8.29429505e-7) + (I*I*-3.12627997e-8))
  e2 += I*I*(1.33896582e-7 + (I*-3.12627997e-8) + (I*I*1.23486170e-9))
  return e2

def H3BoilingUnc(I):
  e2 = 1.86657952e-5 + (I*-3.19917239e-6) + (I*I*1.11529912e-7)
  e2 += I*(-3.19917239e-6 + (I*6.78338365e-7) + (I*I*-2.56740813e-8))
  e2 += I*I*(1.11529912e-7 + (I*-2.56740813e-8) + (I*I*1.01360349e-9))
  return e2

def H2BoilingUnc(I):
  e2 = 3.29666199e-5 + (I*-5.43509145e-6) + (I*I*1.85710186e-7)
  e2 += I*(-5.43509145e-6 + (I*1.02699833e-6) + (I*I*-3.73633538e-8))
  e2 += I*I*(1.85710186e-7 + (I*-3.73633538e-8) + (I*I*1.42075428e-9))
  return e2

def H1BoilingError(I):
  e2 = 3.51614436e-5 + (I*-5.65699024e-6) + (I*I*1.89975924e-7)
  e2 += I*(-5.65699024e-6 + (I*1.08065286e-6) + (I*I*-3.91159021e-8))
  e2 += I*I*(1.89975924e-7 + (I*-3.91159021e-8) + (I*I*1.48091481e-9))
  return e2

def He3D2ECCerror(x):
  A = -2.54412287
  B = -3.69437788
  err = 0
  ex = -1. * np.exp((A*x) + B)

  err = x*((x*0.16547304) - 0.0450909)
  err += ((x*(-0.0450909)) + 0.013428)
  err *= (ex*ex)

  return err

