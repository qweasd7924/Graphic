/**
 * Created by Павел on 10.02.2016.
 */
public  class Calc {
    int length =  1000;

    double[] xray = new double[length];
    double[] yray = new double[length];

    Calc (){
        for (int i = 0; i <length ; i++) {
            xray[i] = i;
        }
    }

    double[] calcArr(int t){
        for (int i = 0; i <length ; i++) {
            yray[i] = xray[i]*xray[i]  + t ;
        }
        return yray;
    }
}
