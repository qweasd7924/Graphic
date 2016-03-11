import de.dislin.Dislin;

public class Paint {
    public static void main (String args []) throws InterruptedException {
        Calc calc = new Calc();

        Dislin.scrmod ("revers");
        Dislin.metafl ("cons");
        Dislin.disini ();
        Dislin.pagera ();
        Dislin.complx ();

        Dislin.axspos(450,1500);
        Dislin.axslen(2300,1200);

        Dislin.labdig (-1, "x");        //кол - во знаков после , подпись у осей
        Dislin.ticks  (10,  "x");       //кол во черточек
        Dislin.ticks  (10, "y");

        Dislin.graf   (0, 40, 0, 10,
                0, 1000, 0, 100);             //задача осей от - до и значения от - до

        for (int j = 0; j <100 ; j++) {
            Dislin.color("black");
            Dislin.curve(calc.xray,calc.yray,calc.length);

            Dislin.color  ("red");
            Dislin.curve  (calc.xray, calc.calcArr(j), calc.length);
            Dislin.setrgb(0,0,0);
            Dislin.grid (1, 1);

            Dislin.sendbf();
            Thread.sleep(30);
        }
        Dislin.disfin ();
    }
}
