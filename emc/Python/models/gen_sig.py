import numpy as np
import pandas as pd

def th(x, q2):
    t = 12 * q2 * (.125**2)
    t /= ((q2 + 1) * ((.125**2) + (x**2)))
    t += 1
    return t

def R_Whitlow(x, q2):
    b1 = 0.0635
    b2 = 0.5747
    b3 = -0.3534
    L = 0.2

    R = b1 * th(x,q2) / np.log(q2/(L**2))
    R += b2/q2
    R += b3/ (q2 + (0.3**2))

    return R

xs_mod = pd.read_csv("KP_sig.csv")

targs = ['H3', 'He3', 'H2', 'H1']

Ep = 3.1
E = 10.6
M = 0.938272

xs_mod['Theta'] = xs_mod.Q2.apply(lambda x: 2 * np.arcsin(np.sqrt(x/(4*E*Ep))))
xs_mod['xs_base'] = xs_mod.apply(lambda x: (((Ep*np.cos(x.Theta/2)/x.Q2)**2) * ((1/(E-Ep)) + ((1 + (x.Q2/((E-Ep)**2))) * (np.tan(x.Theta/2)**2) / (x.x * M * (1 + R_Whitlow(x.x,x.Q2)))))), axis=1)

for targ in targs:
    if targ=="H1":
        t="p"
    else:
        t=targ

    xs_mod = xs_mod.drop([targ + "sig", targ + "logsig"], errors='ignore')

    xs_mod[targ + "sig"] = xs_mod.xs_base * xs_mod["F2" + t]
    xs_mod[targ + "logsig"] = xs_mod[targ + "sig"].apply(np.log)

xs_mod.to_csv("KP_sig.csv")
