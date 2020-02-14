void KP_Rs(){
    ifstream in("F2dis_os1tm1ht1mec1_Dav18_He3Salme");

    double x[101] = {0};
    double R3[101] = {0};
    double R2[101] = {0};

    for(int i=0;i<101;i++){
        double f2p,f2n,f2d,f2he3;
        in >> x[i];
        in >> f2p >> f2p >> f2n >> f2d >> f2he3 >> f2he3;

        R3[i] = f2he3/(f2n + 2.*f2p);
        R2[i] = f2d/(f2n+f2p);
    }

    ofstream out("KP_Rs.csv",ofstream::trunc);
    out << "x,R" << endl;
    for(int i=0;i<101;i++){
        if(x[i]!=0){
            out << x[i] << "," << R3[i] << "," << R2[i] << endl;
        }
    }

    in.close();
    out.close();
}
