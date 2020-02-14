void KP_super(){
    ifstream in("F2dis_os1tm1ht1mec1_Dav18_He3Salme");

    double x[101] = {0};
    double y[101] = {0};
    double z[101] = {0};
    double h3[101] = {0};


    for(int i=0;i<101;i++){
        double f2p,f2n,f2d,f2he3,f2h3;
        in >> x[i];
        in >> f2p >> f2p >> f2n >> f2d >> f2h3 >> f2he3;

        y[i] = f2he3*(f2n + f2p);
        y[i] /= (f2n + 2*f2p) * f2d;
        z[i] = f2d/(f2n+f2p);
        h3[i] = f2h3*(f2n+f2p);
        h3[i] /= f2d * (f2p + 2*f2n);
    }

    ofstream out("KP_super.csv",ofstream::trunc);
    out << "x,R,Rdp,RH3" << endl;
    for(int i=0;i<101;i++){
        if(x[i]!=0){
            out << x[i] << "," << y[i] << "," << z[i] << "," << h3[i] << endl;
        }
    }

    in.close();
    out.close();
}
