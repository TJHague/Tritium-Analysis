kins = ['0','1','2','3','4','5','7_1st','7_2nd','9_1st','9_2nd','11_1st','11_2nd','13_1st','13_2nd','15_1st','15_2nd','15_3rd','16_1st','16_2nd']
targets = ['He3','H3','D2','H1']

#Cut definitions
z_cuts_upstream   = ['-.08', '-.08', '-.08', '-.08', '-.08', '-.09', '-.09', '-.095','-.095','-.1',  '-.1',  '-.15']
z_cuts_downstream = ['-.1',  '-.1',  '-.1',  '-.1',  '-.1',  '-.1',  '-.1',  '-.1',  '-.1',  '-.15', '-.15', '-.11']
acc_loose = "dp>-0.04 & dp<0.04 & th>-0.06 & th<0.06 & ph>-0.03 & ph<0.03"
acc_elliptical = "dp>-0.04 & dp<0.04 & (((ph/0.033)**2) + (((th-0.0025)/0.0675)**2))<1.0"
acc_hex
acc_smallhex

acc = acc2



raw_location = "/mnt/e/raw/"

bcm_gain   = 0.00033518
bcm_offset = 0.0217
clock_rate = 103700.0

meta_data = pd.DataFrame(index=data,columns=['T2','T2s','lt','avgI','Q'])
for t in targets:
    for i,kin in enumerate(kins):
        with open("Runlists/"+t+"_kin"+kin".dat","r") as f:
        data = f.readlines()
        
        #Livetime Dataframe - Contains total number of recorded events with physics trigger and the total number of physics triggers counts in the scalar. The ratio of these two values is the livetime of the run. This is indexed by run number.
        #Load in run list
        for run in data:
            run_events = pd.read_csv(raw_location + run + ".csv")
            
            #Build the cut
            cut = 'n==1 & W2_EL>3.0 ((Preshower + Shower/Momentum)>0.7 & ' #Things that won't change
            cut = cut + acc + ' & z>' + z_cuts_upstream[i] + ' & z<' + z_cuts_downstream[i]
            if i<11:
                cut = cut + 'cer>1500'
            else:
                cut = cut + 'cer>2000'
            
            cleaned = run_events.query(cut)
            
            #Calculate Livetime
            meta_data.at[run,'T2']   = run_events.T2.sum()
            meta_data.at[run,'T2s']  = run_events.T2s.iloc[-1]
            meta_data.at[run,'lt']   = meta_data.at[run,'T2']/meta_data.at[run,'T2s']
            meta_data.at[run,'avgI'] = run_events.query('dnew_rate>0.0').sum()*bcm_gain/run_events.query('dnew_rate>0.0').shape[0]
            meta_data.at[run,'Q']    = (run_events.dnew.iloc[-1]*bcm_gain) + (run_events.clock.iloc[-1]*offset/clock_rate)
            
meta_data.to_csv("/mnt/e/p2_loose/meta.csv")
cleaned.to_csv("/mnt/e/p2_loose/"+run+".csv")