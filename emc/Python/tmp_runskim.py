#This file is temporary until I recreate the raw files
#A few columns are mislabeled
#Clock is labeled as dnew
#dnew is labeled as dnew_rate
#dnew_rate is labeled as clock

import numpy as np
import pandas as pd

#kins = ['0','1','2','3','4','5','7_1st','7_2nd','9_1st','9_2nd','11_1st','11_2nd','13_1st','13_2nd','15_1st','15_2nd','15_3rd','16_1st','16_2nd']
kins = ['0']
targets = ['He3','H3','D2','H1']

#Cut definitions
z_cuts_upstream   = ['-.08', '-.08', '-.08', '-.08', '-.08', '-.09', '-.09', '-.095','-.095','-.1',  '-.1',  '-.105']
z_cuts_downstream = ['.1',  '.1',  '.1',  '.1',  '.1',  '.1',  '.1',  '.1',  '.1',  '.105', '.105', '.11']
acc_loose = "dp>-0.04 & dp<0.04 & Theta>-0.06 & Theta<0.06 & Phi>-0.03 & Phi<0.03"

tph = "Phi"
tth = "Theta"
accL = '(' + tph + ' + (1.1*' + tth + '))>-0.077'
accL = accL + ' & (' + tph + ' + (3.3*' + tth + '))>-0.198'
accL = accL + ' & (' + tph + ' - (3.3*' + tth + '))<0.198'
accL = accL + ' & (' + tph + ' - (1.1*' + tth + '))<0.077'
accL = accL + ' & (' + tph + ' + (1.1*' + tth + '))<0.088'
accL = accL + ' & (' + tph + ' + (3.3*' + tth + '))<0.231'
accL = accL + ' & (' + tph + ' - (3.3*' + tth + '))>-0.231'
accL = accL + ' & (' + tph + ' - (1.1*' + tth + '))>-0.088'
accL = accL + ' & ' + tph + ' < 0.033'
accL = accL + ' & ' + tph + ' > -0.033'
accL = accL + ' & dp>-0.035'
accL = accL + ' & dp<0.045'

accR = '((0.015*' + tph + ')-(0.037*' + tth + '))<0.00222'
accR = accR + ' & ' + tph + '<0.037'
accR = accR + ' & (' + tph + '-(14*' + tth + '))<0.877'
accR = accR + ' & ' + tph + '>-0.033'
accR = accR + ' & (' + tph + '+(6.6*' + tth + '))>-0.396'
accR = accR + ' & dp>-0.03'
accR = accR + ' & dp<-0.045'
accR = accR + ' & ((0.16*FPx)-(0.95*FPth))<0.0325'
accR = accR + ' & ((0.155*FPx)-(0.95*FPth))>-0.027'
accR = accR + ' & FPx<0.5'
accR = accR + ' & FPx>-0.45'

acc = accL

raw_location = "/mnt/d/data/pass2_raw/"
out_folder = "p2_ACCfinal"

bcm_gain   = 0.00033518
bcm_offset = 0.0217
clock_rate = 103700.0

meta_data = pd.DataFrame(columns=['T2','T2s','lt','avgI','Q'])
for t in targets:
    for i,kin in enumerate(kins):
        if kin.startswith('16'):
            acc = accR
        else:
            acc = accL
        with open("/mnt/d/GitHub/HallA-Online-Tritium/batch_farm/"+t+"_kin"+kin+".dat","r") as f:
            data = f.read().splitlines()
            
            #Livetime Dataframe - Contains total number of recorded events with physics trigger and the total number of physics triggers counts in the scalar. The ratio of these two values is the livetime of the run. This is indexed by run number.
            #Load in run list
            for run in data:
                run_events = pd.read_csv(raw_location + run + ".csv")
                run_events = run_events.replace([np.inf, -np.inf, np.nan, -np.nan], 0)
            
                #Build the cut
                cut = acc + ' & nTracks==1 & W2>3.0 & (((Preshower + Shower)/Momentum)/1000.)>0.7' #Things that won't change
                cut = cut + ' & Z>' + z_cuts_upstream[i] + ' & Z<' + z_cuts_downstream[i] + ' & '
                if i<11:
                    cut = cut + 'Cherenkov>1500'
                else:
                    cut = cut + 'Cherenkov>2000'
                cut = cut + ' & clock>0.0'
                
                cleaned = run_events.query(cut)
                cleaned.to_csv("/mnt/d/data/" + out_folder + "/" + run + ".csv")
                
                #Calculate Livetime
                meta_data.at[run,'T2']   = run_events.T2.sum()
                meta_data.at[run,'T2s']  = run_events.T2scalar.iloc[-1]
                meta_data.at[run,'lt']   = meta_data.at[run,'T2']/meta_data.at[run,'T2s']
                meta_data.at[run,'avgI'] = run_events.query('clock>0.0').clock.sum()*bcm_gain/run_events.query('clock>0.0').shape[0]
                meta_data.at[run,'Q']    = (run_events.dnew_rate.iloc[-1]*bcm_gain) + (run_events.dnew.iloc[-1]*bcm_offset/clock_rate)
            
meta_data.to_csv("/mnt/d/data/" + out_folder + "/meta.csv")
