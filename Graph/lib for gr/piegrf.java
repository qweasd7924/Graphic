import de.dislin.Dislin;

public class piegrf {
  public static void main (String args []) {
     float xray  []  = {1.f, 2.5f, 2.f, 2.7f, 1.8f};
     String ctit = "Pie Charts (PIEGRF)";
     String cbuf = " ";
       
     Dislin.metafl ("cons");
     Dislin.setpag ("da4p");
     Dislin.disini ();
     Dislin.pagera ();
     Dislin.hwfont ();

     Dislin.chnpie ("both");

     Dislin.axslen (1600, 1000);
     Dislin.titlin (ctit, 2);

     Dislin.legini (cbuf, 5, 8);
     Dislin.leglin (cbuf, "FIRST",  1);
     Dislin.leglin (cbuf, "SECOND", 2);
     Dislin.leglin (cbuf, "THIRD",  3);
     Dislin.leglin (cbuf, "FOURTH", 4);
     Dislin.leglin (cbuf, "FIFTH",   5);

     // Selecting shading patterns
     Dislin.patcyc (1, 7);
     Dislin.patcyc (2, 4);
     Dislin.patcyc (3, 13);
     Dislin.patcyc (4, 3);
     Dislin.patcyc (5, 5);

     Dislin.axspos (250, 2800);
     Dislin.piegrf (cbuf, 1, xray, 5);
     Dislin.endgrf ();
 
     Dislin.axspos (250, 1600);
     Dislin.labels ("DATA", "PIE");
     Dislin.labpos ("EXTERNAL", "PIE");
     Dislin.piegrf (cbuf, 1, xray, 5);

     Dislin.height (50);
     Dislin.title  ();

     Dislin.disfin ();
  }
}
