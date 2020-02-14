#Copied here because I can't figure out how to include this from a level up in scripts

import math

def f2np(x):
    f2r = 0.90975462
    f2r -= x*1.07242453
    f2r += x*x*0.58567877
    f2r += x*x*x*0.01183161
    return f2r

f2np_err_arr = [0.016995877
,0.016537276
,0.01680723
,0.016682204
,0.017890173
,0.018118659
,0.019910167
,0.019825702
,0.021032978
,0.020370104
,0.019512336
,0.020113808
,0.019823018
,0.020445354
,0.021116439
,0.020550864
,0.021913246
,0.020319837
,0.02020853
,0.01999537
,0.021465156
,0.023272319]

def f2np_err(x):
    if x<0.18 or x>0.84:
        return 0
    
    x -= 0.18
    x /= 0.03
    x = math.floor(x)
    
    return f2np_err_arr[x]

def iso_cor(x, A, Z):
    iso = 0.5*(1 + f2np(x))
    iso /= (1/A)*(Z + ((A-Z)*f2np(x)))
    return iso

def iso_err(x, A, Z):
    err = (A/2) * (2*Z - A)
    err /= ((Z + ((A-Z)*f2np(x)))**2)
    err *= f2np_err(x)
    return err