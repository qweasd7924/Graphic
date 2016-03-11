package de.dislin;

public class Dislin {
  public static native void abs3pt (float x, float y, float z, 
                             float [] xray);
  public static native void addlab (String cstr, float v, int itic, 
                                    String cax);
  public static native void angle  (int ngrad);
  public static native void arcell (int nx, int ny, int na, int nb, 
                             float a, float b, float t);
  public static native void areaf  (int [] nxray, int [] nyray, int n);
  public static native void autres (int ixdim, int iydim);
  public static native void ax2grf ();
  public static native void ax3len (int nxl, int nyl, int nzl);
  public static native void axclrs (int nclr, String copt, String cax);
  public static native void axends (String cstr, String cax);
  public static native void axgit  ();
  public static native void axis3d (float x3, float y3, float z3);
  public static native void axsbgd (int nclr);
  public static native void axsers ();
  public static native void axslen (int nxl, int nyl);
  public static native void axsorg (int nxa, int nya);
  public static native void axspos (int nxa, int nya);
  public static native void axsscl (String cscl, String cax);
  public static native void axstyp (String copt);
  public static native void barbor (int n);
  public static native void barclr (int i1, int i2, int i3);
  public static native void bargrp (int ngrp, float gap);
  public static native void barmod (String cmod, String copt);
  public static native void baropt (float x1, float x2);
  public static native void barpos (String copt);
  public static native void bars   (float [] xray, float [] y1ray, 
                                    float [] y2ray, int n);
  public static native void bars   (double [] xray, double [] y1ray, 
                                    double [] y2ray, int n);
  public static native void bars3d  (float [] xray, float [] yray, 
           float [] z1ray, float [] z2ray, float [] xwray, float [] ywray,
           int [] icray, int n);
  public static native void bars3d  (double [] xray, double [] yray, 
           double [] z1ray, double [] z2ray, double [] xwray, double [] ywray,
           int [] icray, int n);
  public static native void bartyp (String ctyp);
  public static native void barwth (float factor);
  public static native void basalf (String calph);
  public static native void basdat (int id, int im, int iy);
  public static native void bezier (float [] xray, float [] yray, int nray, 
                             float [] x, float [] y, int n);
  public static native void bezier (double [] xray, double [] yray, int nray, 
                             double [] x, double [] y, int n);
  public static native short bitsi2 (int nbits, short mher, int iher, 
                             short mhin, int ihin);
  public static native int bitsi4  (int nbits, int mher,int iher, int mhin, 
                             int ihin);
  public static native void bmpfnt (String copt);
  public static native void bmpmod (int n, String cval, String copt);
  public static native void box2d  ();
  public static native void box3d  ();
  public static native void bufmod (String c1, String c2);
  public static native void center ();
  public static native void cgmbgd (float xr, float xg, float xb);
  public static native void cgmpic (String cfil);
  public static native void cgmver (int n);
  public static native void chaang (float angle);
  public static native void chacod (String copt);
  public static native void chaspc (float xspc);
  public static native void chawth (float xwth);
  public static native void chnatt ();
  public static native void chncrv (String copt);
  public static native void chndot ();
  public static native void chndsh ();
  public static native void chnbar (String copt);
  public static native void chnpie (String copt);
  public static native void circ3p (float x1, float y1, float x2, float y2,
                  float x3, float y3, float [] xray);
  public static native void circle (int nx, int ny, int nr);
  public static native void circsp (int nspc);
  public static native void clip3d (String ctyp);
  public static native int  closfl (int nu); 
  public static native void clpbor (String copt);
  public static native void clpmod (String copt);
  public static native void clpwin (int nx, int ny, int nw, int nh);
  public static native void clrcyc (int index, int iclr);
  public static native void clrmod (String cmode);
  public static native void clswin (int id);
  public static native void color  (String col);
  public static native void colran (int nca, int nce);
  public static native void colray (float [] zray, int [] nray, int n);
  public static native void colray (double [] zray, int [] nray, int n);
  public static native void complx ();
  public static native void conclr (int [] nxray, int n);
  public static native void concrv (float [] xray, float [] yray, int n, float zlev);
  public static native void concrv (double [] xray, double [] yray, int n, float zlev);
  public static native void cone3d (float xm, float ym, float zm, float r, 
              float h1, float h2, int nsk1, int nsk2); 
  public static native void confll (float [] xray, float [] yray, 
				    float [] zray, int n, int [] i1, int []i2,
                                    int [] i3, int ntri, float [] zlvray,
                                    int nlev);
  public static native void confll (double [] xray, double [] yray, 
				    double [] zray, int n, int [] i1, int []i2,
                                    int [] i3, int ntri, double [] zlvray,
                                    int nlev);
  public static native void congap (float xfac);
  public static native void conlab (String clab);
  public static native void conmat (float [] zmat, int n, int m, float zlev);
  public static native void conmat (double [] zmat, int n, int m, float zlev);
  public static native void conmod (float xfac, float xquot);
  public static native void conn3d (float x, float y, float z);
  public static native void connpt (float x, float y);
  public static native int  conpts (float [] xray, int n, float [] yray, int m, 
                             float [] zmat, float zlev, float [] xpts, 
                             float [] ypts, int maxpts, 
                             int [] nray, int maxray);
  public static native int  conpts (double [] xray, int n, double [] yray, int m, 
                             double [] zmat, float zlev, double [] xpts, 
                             double [] ypts, int maxpts, 
                             int [] nray, int maxray);
  public static native void conshd (float [] xray, int n, float [] yray, int m,
                             float [] zmat, float [] zlev, int nlev);
  public static native void conshd (double [] xray, int n, double [] yray, int m,
                             double [] zmat, double [] zlev, int nlev);
  public static native void conshd3d (float [] xray, int n, float [] yray, int m,
                             float [] zmat, float [] zlev, int nlev);
  public static native void conshd3d (double [] xray, int n, double [] yray, int m,
                             double [] zmat, double [] zlev, int nlev);
  public static native void contri (float [] xray, float [] yray, 
				    float [] zray, int n, int [] i1, int []i2,
                                    int [] i3, int ntri, float zlev);
  public static native void contri (double [] xray, double [] yray, 
				    double [] zray, int n, int [] i1, int []i2,
                                    int [] i3, int ntri, float zlev);
  public static native void contur (float [] xray, int n, float [] yray, int m,
                             float [] zmat, float zlev);
  public static native void contur (double [] xray, int n, double [] yray, 
                             int m, double [] zmat, float zlev);
  public static native void cross  ();
  public static native void crvmat (float [] zmat, int n, int m, int ixpts, 
                             int iypts);
  public static native void crvmat (double [] zmat, int n, int m, int ixpts, 
                             int iypts);
  public static native void crvqdr (float [] xray, float [] yray, 
                             float [] zray, int n);
  public static native void crvqdr (double [] xray, double [] yray, 
                             double [] zray, int n);
  public static native void crvt3d (float [] xray, float [] yray, 
                             float [] zray, float [] rray, int [] icray, 
                             int n);
  public static native void crvt3d (double [] xray, double [] yray, 
                             double [] zray, double [] rray, int [] icray, 
                             int n);
  public static native void crvtri (float [] xray, float [] yray, 
				    float [] zray, int n, int [] i1, int []i2,
                                    int [] i3, int ntri);
  public static native void crvtri (double [] xray, double [] yray, 
				    double [] zray, int n, int [] i1, int []i2,
                                    int [] i3, int ntri);
  public static native void csrlin (int [] iray);
  public static native void csrmod (String c1, String c2);
  public static native int  csrmov (int [] ixray, int [] iyray, int nmax);
  public static native int  csrpt1 (int i);
  public static native int  csrpos (int [] iray);
  public static native int  csrkey ();
  public static native int  csrpts (int [] ixray, int [] iyray, int nmax);
  public static native void csrrec (int [] iray);
  public static native void csrtyp (String copt);
  public static native void csruni (String copt);
  public static native void curv3d (float [] xray, float [] yray, 
                             float [] zray, int n);
  public static native void curv3d (double [] xray, double [] yray, 
                             double [] zray, int n);
  public static native void curv4d (float [] xray, float [] yray, 
                             float [] zray, float [] wray, int n);
  public static native void curv4d (double [] xray, double [] yray, 
                             double [] zray, double [] wray, int n);
  public static native void curve  (float [] xray, float [] yray, int n);
  public static native void curve  (double [] xray, double [] yray, int n);
  public static native void curve3 (float [] xray, float [] yray, 
                                    float [] zray, int n);
  public static native void curve3 (double [] xray, double [] yray, 
                                    double [] zray, int n);
  public static native void curvmp (float [] xray, float [] yray, int n);
  public static native void curvmp (double [] xray, double [] yray, int n);
  public static native void curvx3 (float [] xray, float y, float [] zray,
                                    int n);
  public static native void curvx3 (double [] xray, double y, double [] zray,
                                    int n);
  public static native void curvy3 (float x, float [] yray, float [] zray, 
                                    int n);
  public static native void curvy3 (float x, double [] yray, double [] zray, 
                                    int n);
  public static native void cyli3d (float xm, float ym, float zm, float r, 
                                    float h, int nsk1, int nsk2); 
  public static native void dash   ();
  public static native void dashl  ();
  public static native void dashm  ();
  public static native void dattim (String cdate, String ctime);
  public static native void delglb ();
  public static native void dbffin ();
  public static native int  dbfini ();
  public static native void dbfmod (String cstr);
  public static native void digits (int ndig, String cax);
  public static native void disalf ();
  public static native void disenv (String cfil);
  public static native void disfin ();
  public static native void disini ();
  public static native void disk3d (float xm, float ym, float zm, float r1, 
                                    float r2, int nsk1, int nsk2); 
  public static native void doevnt ();
  public static native void dot    ();
  public static native void dotl   ();
  public static native void duplx  ();
  public static native int  dwgbut (String cstr, int ival);
  public static native int  dwgerr ();
  public static native String dwgfil (String clab, String cstr, String cmask);
  public static native int dwglis  (String clab, String clis, int ilis);
  public static native void dwgmsg (String cstr);
  public static native String dwgtxt (String clab, String cstr);
  public static native void ellips (int nx, int ny, int na, int nb);
  public static native void endgrf ();
  public static native void erase  ();
  public static native void errbar (float [] x, float [] y, float [] err1,
                             float [] err2, int n);
  public static native void errbar (double [] x, double [] y, double [] err1,
                             double [] err2, int n);
  public static native void errdev (String cdev);
  public static native void errfil (String cfil);
  public static native void errmod (String cstr, String copt);
  public static native void eushft (String cnat, String cshf);
  public static native void expimg (String cfil, String copt);
  public static native void expzlb (String cstr);
  public static native void fbars  (float [] xray, float [] y1ray,
				    float [] y2ray, float [] y3ray,
                                    float [] y4ray, int n); 
  public static native void fbars  (double [] xray, double [] y1ray,
				    double [] y2ray, double [] y3ray,
                                    double [] y4ray, int n); 
  public static native String fcha (float x, int ndig);
  public static native void field  (float [] xray, float [] yray,
                             float [] uray, float [] vray, int n, int ivec);
  public static native void field  (double [] xray, double [] yray,
                             double [] uray, double [] vray, int n, int ivec);
  public static native void field3d  (float [] x1ray, float [] y1ray,
                             float [] z1ray, float [] x2ray, float [] y2ray,
                             float [] z2ray, int n, int ivec);
  public static native void field3d  (double [] x1ray, double [] y1ray,
                             double [] z1ray, double [] x2ray, double [] y2ray,
                             double [] z2ray, int n, int ivec);
  public static native void filbox (int nx, int ny, int nw, int nh);
  public static native void filclr (String copt);
  public static native void filmod (String cmod);
  public static native void filopt (String copt, String ckey);
  public static native int  filsiz (String cfil, int i);
  public static native int  filtyp (String cfil);
  public static native void filwin (int nx, int ny, int nw, int nh);
  public static native void fixspc (float xfac);
  public static native void flab3d ();
  public static native int  flen   (float x, int ndig);
  public static native void frame  (int nfrm);
  public static native void frmclr (int nclr);
  public static native void frmbar (int n);
  public static native void frmess (int nfrm);
  public static native void gapcrv (float xgap);
  public static native void gapsiz (float x, String copt);
  public static native int  gaxpar (float v1, float v2, String copt, 
                             String cax, float [] xray);
  public static native String getalf ();
  public static native int  getang ();
  public static native int  getbpp ();
  public static native int  getclr ();
  public static native int  getclp (int i);
  public static native int getdig (int i);
  public static native String getdsp ();
  public static native String getfil ();
  public static native float getgrf (int i, String cax);
  public static native int  gethgt ();
  public static native int  gethnm ();
  public static native float getind (int index, int i);
  public static native String getlab (int i);
  public static native int  getlen (int i);
  public static native int  getlev ();
  public static native int  getlin ();
  public static native int  getlit (float xp, float yp, float zp,
                             float xn, float yn, float zn);
  public static native void getmat (float [] xray, float [] yray, float [] zray,
                             int n, float [] zmat, int nx, int ny, float zval,
                             int [] imat, float [] wmat);
  public static native void getmat (double [] xray, double [] yray, 
                             double [] zray, int n, double [] zmat, int nx, 
                             int ny, float zval, int [] imat, double [] wmat);
  public static native String getmfl ();
  public static native String getmix (String copt);
  public static native int  getor  (int i);
  public static native int  getpag (int i);
  public static native int  getpat ();
  public static native int  getplv ();
  public static native int  getpos (int i);
  public static native float getrgb (int i);
  public static native int  getran (int i);
  public static native int  getres (int i);
  public static native int  getscl (int i);
  public static native int  getscm (int i);
  public static native int  getscr (int i);
  public static native String getshf (String copt);
  public static native int  getsp1 (int i);
  public static native int  getsp2 (int i);
  public static native int  getsym (int i);
  public static native int  gettcl (int i);
  public static native int  gettic (int i);
  public static native int  gettyp ();
  public static native int  getuni ();
  public static native float getver ();
  public static native int  getvk  (int i);
  public static native String getvlt ();
  public static native int  getwid ();
  public static native int  getwin (int i);
  public static native int  getxid (String copt);
  public static native void gifmod (String c1, String c2);
  public static native void gothic ();
  public static native void grace  (int ngrace);
  public static native void graf   (float xa, float xe, float xor, float xstp,
                             float ya, float ye, float yor, float ystp);
  public static native void graf3  (float xa, float xe, float xor, float xstp,
                             float ya, float ye, float yor, float ystp,
                             float za, float ze, float zor, float zstp);
  public static native void graf3d (float xa, float xe, float xor, float xstp,
                             float ya, float ye, float yor, float ystp,
                             float za, float ze, float zor, float zstp);
  public static native void grafmp (float xa, float xe, float xor, float xstp,
                             float ya, float ye, float yor, float ystp);
  public static native void grafp  (float xe, float xor, float xstp,
                              float yor, float ystp);
  public static native void grffin ();
  public static native void grfini (float x1, float y1, float z1,
                             float x2, float y2, float z2, 
                             float x3, float y3, float z3);
  public static native void grdpol (int ixgrid, int iygrid);
  public static native void grid   (int ixgrid, int iygrid);
  public static native void grid3d (int ixgrid, int iygrid, String copt);
  public static native void gridmp (int ixgrid, int iygrid);
  public static native int  gwgatt (int id, String copt);
  public static native int  gwgbox (int id);
  public static native int  gwgbut (int id);
  public static native String gwgfil (int id);
  public static native float gwgflt (int id);
  public static native int  gwggui ();
  public static native int  gwgint (int id);
  public static native int  gwglis (int id);
  public static native float gwgscl (int id);
  public static native int  gwgsiz (int id, int i);
  public static native float gwgtbf (int id, int irow, int icol);
  public static native int  gwgtbi (int id, int irow, int icol);
  public static native void gwgtbl (int id, float [] xray, int n, int idx,
                                    String copt);
  public static native void gwgtbl (int id, double [] xray, int n, int idx,
                                    String copt);
  public static native String gwgtbs (int id, int irow, int icol);
  public static native String gwgtxt (int id);
  public static native int  gwgxid (int id);
  public static native void height (int nhchar);
  public static native void helve  ();
  public static native void helves ();
  public static native void helvet ();
  public static native void hidwin  (int id, String copt);
  public static native int  histog (float [] xray, int n, float [] x, float [] y);
  public static native int  histog (double [] xray, int n, double [] x, double [] y);
  public static native void hname  (int nhchar);
  public static native void hpgmod (String c1, String c2);
  public static native void hsvrgb (float  xh, float  xs, float  xv, float [] xray); 
  public static native void hsym3d (float x);
  public static native void hsymbl (int nhsym);
  public static native void htitle (int nhtit);
  public static native void hwfont ();
  public static native void hwmode (String c1, String c2);
  public static native void hworig (int nx, int ny);
  public static native void hwpage (int nw, int nh);
  public static native void hwscal (float xfac);
  public static native void imgbox (int nx, int ny, int nw, int nh);
  public static native void imgclp (int nx, int ny, int nw, int nh);
  public static native void imgfin ();
  public static native void imgfmt (String copt);
  public static native void imgini ();
  public static native void imgmod (String copt);
  public static native void imgsiz (int nw, int nh);
  public static native void imgtpr (int n);
  public static native int  incdat (int id, int im, int iy);
  public static native void inccrv (int ncrv);
  public static native void incfil (String cfil);
  public static native void incmrk (int nmrk);
  public static native int  indrgb (float xr, float xg, float xb);
  public static native int  intrgb (float xr, float xg, float xb);
  public static native void intax  ();
  public static native String intcha (int nx);
  public static native int  intlen (int nx);
  public static native int  intutf (int [] iray, int nray, String s, int nmax);
  public static native int  isopts (float [] xray, int nx, float [] yray, 
              int ny, float [] zray, int nz, float [] wmat, float wlev,
              float [] xtri, float [] ytri, float [] ztri, int nmax);
  public static native int  isopts (double [] xray, int nx, double [] yray, 
              int ny, double [] zray, int nz, double [] wmat, float wlev,
              double [] xtri, double [] ytri, double [] ztri, int nmax);
  public static native String itmcat (String clis, String cstr);
  public static native String itmncat (String clis, int nmax, String cstr);
  public static native int  itmcnt (String clis);
  public static native String itmstr (String clis, int nlis);
  public static native void jusbar (String copt);
  public static native void labclr (int iclr, String copt);
  public static native void labdig (int ndig, String cax);
  public static native void labdis (int ndis, String cax);
  public static native void labels (String clab, String cax);
  public static native void labjus (String copt, String cax);
  public static native void labl3d (String copt);
  public static native void labmod (String ckey, String copt, String cax);
  public static native void labpos (String cpos, String cax);
  public static native void labtyp (String ctyp, String cax);
  public static native int  ldimg  (String cfl, short [] iray, int nmax, 
                                    int nc);
  public static native void legbgd (int ncol);
  public static native void legclr ();
  public static native void legend (String cbuf, int ncor);
  public static native void legini (String cbuf, int nlin, int nmaxln);
  public static native void leglin (String cbuf, String cstr, int ilin);
  public static native void legopt (float x1, float x2, float x3);
  public static native void legpat (int ityp, int ithk, int isym, int ipat, 
                             int iclr, int ilin);
  public static native void legpos (int nx, int ny);
  public static native void legsel (int [] nray, int n);
  public static native void legtit (String cstr);
  public static native void legtyp (String cstr);
  public static native void legval (float x, String copt);
  public static native void lfttit ();
  public static native void licmod (String c1, String c2);
  public static native void licpts (float [] xv, float [] yv, int nx, int ny,
			     int [] itmat, int [] iwmat, float [] wmat);
  public static native void licpts (double [] xv, double [] yv, int nx, int ny,
			     int [] itmat, int [] iwmat, double [] wmat);
  public static native void light  (String cstr);
  public static native void linclr (int [] nray, int n);
  public static native void lincyc (int index, int ityp);
  public static native void line   (int nx, int ny, int nu, int nv);
  public static native void linesp (float xfac);
  public static native void linmod (String copt, String cax);
  public static native void lintyp (int ntyp);
  public static native void linwid (int i);
  public static native void litmod (int id, String cstr);
  public static native void litop3 (int id, float xr, float xg, float xb, String cstr);
  public static native void litopt (int id, float x, String cstr);
  public static native void litpos (int id, float x, float y, float z, String cstr);
  public static native void lncap  (String copt);
  public static native void lnjoin (String copt);
  public static native void lnmlt  (float x);
  public static native void logtic (String cmod);
  public static native void lsechk (String cmod);
  public static native void mapbas (String cmod);
  public static native void mapfil (String cfil, String copt);
  public static native void mapimg (String cfil, float x1, float x2, float x3,
                                    float x4, float x5, float x6);
  public static native void maplab (String copt, String ckey);
  public static native void maplev (String cmod);
  public static native void mapmod (String cmod);
  public static native void mappol (float xpol, float ypol);
  public static native void mapopt (String copt, String ckey);
  public static native void mapref (float ylower, float yupper);
  public static native void mapsph (float xrad);
  public static native void marker (int nsym);
  public static native void matop3 (float xr, float xg, float xb, String cstr);
  public static native void matopt (float x, String cstr);
  public static native void mdfmat (int nx, int ny, float weight);
  public static native void messag (String cstr, int nx, int ny);
  public static native void metafl (String cstr);
  public static native void mixalf ();
  public static native void mixleg ();
  public static native void mpaepl (int i);
  public static native void mplang (float x);
  public static native void mplclr (int nbg, int nfg);
  public static native void mplpos (int nx, int ny);
  public static native void mplsiz (int nsize);
  public static native void mpslogo (int i1, int i2, int i3, String s);
  public static native void mrkclr (int n);
  public static native void msgbox (String cstr);
  public static native void mshclr (int nclr);
  public static native void mshcrv (int nclr);
  public static native void mylab  (String cstr, int itick, String cax);
  public static native void myline (int [] nray, int n);
  public static native void mypat  (int iang, int itype, int idens, int icross);
  public static native void mysymb  (float [] xray, float [] yray, int n, 
                             int isym, int iflag);
  public static native void mysymb  (double [] xray, double [] yray, int n, 
                             int isym, int iflag);
  public static native void myvlt  (float [] xr, float [] xg, float [] xb, 
                             int n);
  public static native void myvlt  (double [] xr, double [] xg, double [] xb,
                             int n);
  public static native void namdis (int ndis, String cax);
  public static native void name   (String cstr, String cax);
  public static native void namjus (String copt, String cax);
  public static native void nancrv (String cmode);
  public static native void neglog (float eps);
  public static native void newmix ();
  public static native void newpag ();
  public static native int  nlmess (String cstr);
  public static native int  nlnumb (float x, int ndig);
  public static native void noarln ();
  public static native void nobar  ();
  public static native void nobgd  ();
  public static native void nochek ();
  public static native void noclip ();
  public static native void nofill ();
  public static native void nograf ();
  public static native void nohide ();
  public static native void noline (String cax);
  public static native void number (float x, int ndig, int nx, int ny);
  public static native void numfmt (String copt);
  public static native void numode (String cdec, String cgrp, String cpos, 
                             String cfix);
  public static native int  nwkday (int id, int im, int iy);
  public static native int  nxlegn (String cbuf);
  public static native int  nxpixl (int ix, int iy);
  public static native int  nxposn (float x);
  public static native int  nylegn (String cbuf);
  public static native int  nypixl (int ix, int iy);
  public static native int  nyposn (float y);
  public static native int  nzposn (float z);
  public static native int  openfl (String cfil, int nu, int irw); 
  public static native void opnwin (int id);
  public static native void origin (int nx0, int ny0);
  public static native void page   (int nw, int nh);
  public static native void pagera ();
  public static native void pagfll (int nclr);
  public static native void paghdr (String cstr1, String cstr2, int iopt, int idir);
  public static native void pagmod (String cmod);
  public static native void pagorg (String cmod);
  public static native void pagwin (int nw, int nh);
  public static native void patcyc (int index, int ipat);
  public static native int  pdfbuf (byte [] cbuf, int nbyte); 
  public static native void pdfmod (String c1, String c2);
  public static native void pdfmrk (String c1, String c2);
  public static native void penwid (float x);
  public static native void pie    (int nxm, int nym, int nr, float a, float b);
  public static native void piebor (int n);
  public static native void pieclr (int [] nxray, int [] nyray, int n);
  public static native void pieexp ();
  public static native void piegrf (String cbuf, int nlin, float [] xray, 
                                    int nseg);
  public static native void piegrf (String cbuf, int nlin, double [] xray, 
                                    int nseg);
  public static native void pielab (String clab, String cpos);
  public static native void pieopt (float x1, float x2);
  public static native void pierot (float angle);
  public static native void pietyp (String ctyp);
  public static native void pieval (float x, String copt);
  public static native void pievec (int ivec, String copt);
  public static native void pike3d (float x1, float y1, float z1, float x2,
                                    float y2, float z2, float r, int n, int m);  public static native void plat3d (float xm, float ym, float zm, float xl,
                                    String copt); 
  public static native void plyfin (String c1, String c2);
  public static native void plyini (String ctyp);
  public static native void pngmod (String c1, String c2);
  public static native void point  (int nx, int ny, int nb, int nh, int ncol);
  public static native void polar  (float xe, float xor, float xstp,
                              float yor, float ystp);
  public static native int  polclp (float [] xray, float [] yray, int n, 
              float [] xout, float [] yout, int nmax, float xv, String cedge);
  public static native int  polclp (double [] xray, double [] yray, int n, 
              double [] xout, double [] yout, int nmax, float xv, String cedge);
  public static native void polcrv (String cpol);
  public static native void polmod (String c1, String c2);
  public static native void pos2pt (float x, float y, float [] xray);
  public static native void pos3pt (float x, float y, float z, float [] xray);
  public static native void posbar (String copt);
  public static native int  posifl (int nu, int nbyte); 
  public static native void proj3d (String cproj);
  public static native void projct (String cproj);
  public static native void psfont (String cfont);
  public static native void psmode (String cfont);
  public static native void pt2pos (float x, float y, float [] xray);
  public static native void pyra3d (float xm, float ym, float zm, float xl, 
                                    float h1, float h2, int n); 
  public static native void qplbar (float [] xray, int n);
  public static native void qplbar (double [] xray, int n);
  public static native void qplclr (float [] xray, int n, int m);
  public static native void qplclr (double [] xray, int n, int m);
  public static native void qplcon (float [] xray, int n, int m, int nlev);
  public static native void qplcon (double [] xray, int n, int m, int nlev);
  public static native void qplcrv (float [] xray, float [] yray, int n,
                                    String s);
  public static native void qplcrv (double [] xray, double [] yray, int n,
                                    String s);
  public static native void qplot  (float [] xray, float [] yray, int n);
  public static native void qplot  (double [] xray, double [] yray, int n);
  public static native void qplpie (float [] xray, int n);
  public static native void qplpie (double [] xray, int n);
  public static native void qplsca (float [] xray, float [] yray, int n);
  public static native void qplsca (double [] xray, double [] yray, int n);
  public static native void qplscl (float xa, float xe, float xor, float xstp, 
                             String cstr);
  public static native void qplsur (float [] xray, int n, int m);
  public static native void qplsur (double [] xray, int n, int m);
  public static native void quad3d (float xm, float ym, float zm, 
                                    float xl, float yl, float zl);
  public static native int  rbfpng (byte [] cbuf, int nbyte); 
  public static native void rbmp   (String cfil);
  public static native void reawgt ();
  public static native int  readfl (int nu, byte [] cbuf, int nbyte); 
  public static native void recfll (int nx, int ny, int nw, int nh, int ncol);
  public static native void rectan (int nx, int ny, int nw, int nh);
  public static native void rel3pt (float x, float y, float z, float [] xray); 
  public static native void resatt ();
  public static native void reset  (String cname);
  public static native void revscr ();
  public static native void rgbhsv (float xr, float xg, float xb, 
                                    float [] xray); 
  public static native void rgif   (String cfil);
  public static native void rgtlab ();
  public static native void rimage (String cfil);
  public static native void rlarc  (float xm, float ym, float xa, float xb,
                             float a,  float b,  float t);
  public static native void rlarea (float [] xray, float [] yray, int n);
  public static native void rlarea (double [] xray, double [] yray, int n);
  public static native void rlcirc (float xm, float ym, float r);
  public static native void rlconn (float x, float y);
  public static native void rlell  (float xm, float ym, float a, float b);
  public static native void rline  (float x, float y, float u, float v);
  public static native void rlmess (String cstr, float x, float y);
  public static native void rlnumb (float x, int ndig, float xp, float yp);
  public static native void rlpie  (float xm, float ym, float r, float a, float b);
  public static native void rlpoin (float x, float y, int nb, int nh, int ncol);
  public static native void rlrec  (float x, float y, float xw, float xh);
  public static native void rlrnd  (float x, float y, float xb, float xh, int irnd);
  public static native void rlsec  (float xm, float ym, float r1, float r2,
                             float a,  float b,  int ncol);
  public static native void rlstrt (float x, float y);
  public static native void rlsymb (int nsym, float x, float y);
  public static native void rlvec  (float x1, float y1, float x2, float y2, int ivec);
  public static native void rlwind (float x, float xp, float yp, int n, float a);
  public static native void rndrec (int nx, int ny, int nb, int nh, int irnd);
  public static native void rot3d  (float xa, float ya, float za); 
  public static native int  rpixel (int ix, int iy);
  public static native void rpixls (byte [] iray, int ix, int iy, int nw, int nh);
  public static native void rpng   (String cfil);
  public static native void rppm   (String cfil);
  public static native void rpxrow (byte [] iray, int ix, int iy, int n);
  public static native void rtiff  (String cfil);
  public static native void rvynam ();
  public static native void scale  (String cscl, String cax);
  public static native void sclfac (float xfac);
  public static native void sclmod (String cmode);
  public static native void scrmod (String cmode);
  public static native void sector (int nx, int ny, int nr1, int nr2, 
                             float a, float b, int ncol);
  public static native void selwin (int id);
  public static native void sendbf ();
  public static native void sendmb ();
  public static native void sendok ();
  public static native void serif  ();
  public static native void setbas (float xfac);
  public static native void setclr (int ncol);
  public static native void setcsr (String copt);
  public static native void setexp (float fexp);
  public static native void setfce (String copt);
  public static native void setfil (String cfil);
  public static native void setgrf (String c1, String c2, String c3, 
                             String c4);
  public static native void setind (int index, float xr, float xg, float xb);
  public static native void setmix (String cstr, String cmix);
  public static native void setpag (String cpag);
  public static native void setres (int npb, int nph);
  public static native void setrgb(float xr, float xg, float xb);
  public static native void setscl (float [] xray, int n, String cax);
  public static native void setscl (double [] xray, int n, String cax);
  public static native void setvlt (String cvlt);
  public static native void setxid (int id, String copt);
  public static native void shdafr (int [] inray, int []ipray, int [] icray, 
                             int n);
  public static native void shdasi (int [] inray, int []ipray, int [] icray, 
                             int n);
  public static native void shdaus (int [] inray, int []ipray, int [] icray, 
                             int n);
  public static native void shdcha ();
  public static native void shdcrv (float [] x1ray, float [] y1ray, int n1,
                             float [] x2ray, float [] y2ray, int n2);
  public static native void shdcrv (double [] x1ray, double [] y1ray, int n1,
                             double [] x2ray, double [] y2ray, int n2);
  public static native void shdeur (int [] inray, int []ipray, int [] icray, 
                             int n);
  public static native void sdhfac (float xfac);
  public static native void shdmap (String cmap);
  public static native void shdmod (String copt, String ctype);
  public static native void shdnor (int [] inray, int []ipray, int [] icray, 
                             int n);
  public static native void shdpat (int ipat);
  public static native void shdsou (int [] inray, int []ipray, int [] icray, 
                             int n);
  public static native void shdusa (int [] inray, int []ipray, int [] icray, 
                             int n);
  public static native void shield (String carea, String cmode);
  public static native void shlcir (int nx, int ny, int nr);
  public static native void shldel (int id);
  public static native void shlell (int nx, int ny, int na, int nb, float t);
  public static native int  shlind ();
  public static native void shlpie (int nx, int ny, int nr, float a, float b);
  public static native void shlpol (int [] nxray, int [] nyray, int n);
  public static native void shlrct (int nx, int ny, int nw, int nh, float t);
  public static native void shlrec (int nx, int ny, int nw, int nh);
  public static native void shlres (int n);
  public static native void shlsur ();
  public static native void shlvis (int id, String cmode);
  public static native void simplx ();
  public static native int  skipfl (int nu, int nbyte); 
  public static native void smxalf (String calph, String c1, String c2, int n);
  public static native void solid  ();
  public static native void sortr1 (float [] xray, int n, String copt);
  public static native void sortr1 (double [] xray, int n, String copt);
  public static native void sortr2 (float [] xray, float [] yray, int n, 
                             String copt);
  public static native void sortr2 (double [] xray, double [] yray, int n, 
                             String copt);
  public static native void spcbar (int n);
  public static native void sphe3d (float xm, float ym, float zm, float r,
                             int n, int m);
  public static native int  spline (float [] xray,  float [] yray,  int n,
                             float [] xsray, float [] ysray);
  public static native int  spline (double [] xray,  double [] yray,  int n,
                             double [] xsray, double [] ysray);
  public static native void splmod (int ngrad, int npts);
  public static native void stmmod (String c1, String c2);
  public static native void stmopt (int n, String copt);
  public static native int  stmpts (float [] xv, float [] yv, int nx, int ny,
			    float [] xp, float [] yp, float x0, float y0, 
                            float [] xray, float [] yray, int nmax);
  public static native int  stmpts (double [] xv, double [] yv, int nx, int ny,
			    double [] xp, double [] yp, float x0, float y0, 
                            double [] xray, double [] yray, int nmax);
  public static native int  stmpts3d (float [] xv, float [] yv, float [] zv,
		            int nx, int ny, int nz,
			    float [] xp, float [] yp, float [] zp,
		            float x0, float y0, float z0, 
                            float [] xray, float [] yray, float [] zray, 
                            int nmax);
  public static native int  stmpts3d (double [] xv, double [] yv, double [] zv,
		            int nx, int ny, int nz,
			    double [] xp, double [] yp, double [] zp,
		            float x0, float y0, float z0, 
                            double [] xray, double [] yray, double [] zray, 
                            int nmax);
  public static native void stmtri (float [] xv, float [] yv, 
			            float [] xp, float [] yp, int n,
				    int [] i1ray, int [] i2ray, int [] i3ray,
                                    int ntri, float [] xs, float [] ys, 
                                    int nray);
  public static native void stmtri (double [] xv, double [] yv, 
			            double [] xp, double [] yp, int n,
				    int [] i1ray, int [] i2ray, int [] i3ray,
                                    int ntri, double [] xs, double [] ys, 
                                    int nray);
  public static native void stmval (float x, String copt);
  public static native void stream (float [] xv, float [] yv, int nx, int ny,
				    float [] xp, float [] yp, 
                                    float [] xs, float [] ys, int n);
  public static native void stream (double [] xv, double [] yv, int nx, int ny,
				    double [] xp, double [] yp, 
                                    double [] xs, double [] ys, int n);
  public static native void stream3d (float [] xv, float [] yv, float [] zv,
				    int nx, int ny, int nz,
				    float [] xp, float [] yp, float [] zp, 
                                    float [] xs, float [] ys, float [] zs, 
                                    int n);
  public static native void stream3d (double [] xv, double [] yv, double [] zv,
				    int nx, int ny, int nz,
				    double [] xp, double [] yp, double [] zp, 
                                    double [] xs, double [] ys, double [] zs, 
                                    int n);
  public static native void strt3d (float x, float y, float z);
  public static native void strtpt (float x, float y);
  public static native void surclr (int ictop, int icbot);
  public static native void surfce (float [] xray, int n, float [] yray, 
                             int m, float [] zmat);
  public static native void surfce (double [] xray, int n, double [] yray, 
                             int m, double [] zmat);
  public static native void surfcp (String zfun, 
                             float a1, float a2, float astp,
                             float b1, float b2, float bstp);
  public static native void surfun (String zfun, int ixpts, float xdel, 
                             int iypts, float ydel);
  public static native void suriso (float [] xray, int nx, float [] yray, int ny,
                             float [] zray, int nz, float [] wmat, float wlev);
  public static native void suriso (double [] xray, int nx, double [] yray, int ny,
                             double [] zray, int nz, double [] wmat, double wlev);
  public static native void surmat (float [] zmat, int nx, int ny, int ixpts, 
                             int iypts);
  public static native void surmat (double [] zmat, int nx, int ny, int ixpts, 
                             int iypts);
  public static native void surmsh (String copt);
  public static native void suropt (String copt);
  public static native void surshc (float [] xray, int n, float [] yray, int m,
                             float [] zmat, float [] wmat);
  public static native void surshc (double [] xray, int n, double [] yray, int m,
                             double [] zmat, double [] wmat);
  public static native void surshd (float [] xray, int n, float [] yray, int m,
                             float [] zmat);
  public static native void surshd (double [] xray, int n, double [] yray, int m,
                             double [] zmat);
  public static native void sursze (float xmin, float xmax, float ymin, 
                             float ymax);
  public static native void surtri (float [] xray, float [] yray, 
			     float [] zray, int n, int [] i1, int []i2,
                             int [] i3, int ntri);
  public static native void surtri (double [] xray, double [] yray, 
			     double [] zray, int n, int [] i1, int []i2,
                             int [] i3, int ntri);
  public static native void survis (String cvis);
  public static native void swgatt (int id, String ctype, String cwidg);
  public static native void swgbgd (int id, float xr, float xg, float xb);
  public static native void swgbox (int ip, int ival);
  public static native void swgbut (int ip, int ival);
  public static native void swgcb2 (int id, String cname);
  public static native void swgcb3 (int id, String cname);
  public static native void swgcbk (int id, String cname);
  public static native void swgclr (float xr, float xg, float xb, String copt);
  public static native void swgdrw (float x);
  public static native void swgfgd (int id, float xr, float xg, float xb);
  public static native void swgfil (int ip, String cval);
  public static native void swgflt (int ip, float xval, int ndig);
  public static native void swgfnt (String cfnt, int i);
  public static native void swgfoc (int id);
  public static native void swghlp (String cstr);
  public static native void swgint (int ip, int iv);
  public static native void swgiop (int ndig, String cax);
  public static native void swgjus (String ctype, String cwidg);
  public static native void swglis (int ip, int ival);
  public static native void swgmix (String c, String cstr);
  public static native void swgmod (String cmod);
  public static native void swgmrg (int ival, String cstr);
  public static native void swgoff (int nx, int ny);
  public static native void swgopt (String ctype, String cwidg);
  public static native void swgpop (String copt);
  public static native void swgpos (int nx, int ny);
  public static native void swgray (float [] xray, int n, String copt);
  public static native void swgray (double [] xray, int n, String copt);
  public static native void swgscl (int ip, float xval);
  public static native void swgsiz (int nx, int ny);
  public static native void swgspc (float x, float y);
  public static native void swgstp (float x);
  public static native void swgtbf (int id, float x, int ndig, int irow,
                                    int icol, String copt);
  public static native void swgtbi (int id, int ix, int irow, int icol, 
                                    String copt);
  public static native void swgtbl (int id, float [] xray, int n, int ndig,
                                    int idx, String copt);
  public static native void swgtbl (int id, double [] xray, int n, int ndig,
                                    int idx, String copt);
  public static native void swgtbs (int id, String cstr, int irow, int icol, 
                                    String copt);
  public static native void swgtit (String ctit);
  public static native void swgtxt (int ip, String cval);
  public static native void swgtyp (String ctype, String cwidg);
  public static native void swgval (int ip, float xval);
  public static native void swgwin (int nx, int ny, int nw, int nh);
  public static native void swgwth (int nchar);
  public static native void symb3d (int nsym, float x, float y, float z);
  public static native void symbol (int nsym, int nx, int ny);
  public static native void symfil (String cdev, String cstat);
  public static native void symrot (float angle);
  public static native int  tellfl (int nu); 
  public static native void thkc3d (float x);
  public static native void thkcrv (int nthk);
  public static native void thrfin ();
  public static native void thrini (int n);
  public static native void ticks  (int itick, String cax);
  public static native void ticlen (int nmaj, int nmin);
  public static native void ticmod (String copt, String cax);
  public static native void ticpos (String cpos, String cax);
  public static native void tifmod (int n, String cval, String copt);
  public static native void tiforg (int nx, int ny);
  public static native void tifwin (int nx, int ny, int nw, int nh);
  public static native void timopt ();
  public static native void titjus (String copt);
  public static native void title  ();
  public static native void titlin (String cstr, int n);
  public static native void titpos (String copt);
  public static native void torus3d (float xm, float ym, float zm, float r1,
                                    float r2, float h,
                                    float a1, float a2, int n, int m);
  public static native void tprini ();
  public static native void tprfin ();
  public static native void tprmod (String copt, String ckey);
  public static native void tprval (float x);
  public static native void tr3axs (float x, float y, float z, float a);
  public static native void tr3res ();
  public static native void tr3rot (float a, float b, float c);
  public static native void tr3scl (float x, float y, float z);
  public static native void tr3shf (float x, float y, float z);
  public static native void trfco1 (float [] xray, int n, String cfrom, 
                             String cto); 
  public static native void trfco1 (double [] xray, int n, String cfrom, 
                             String cto); 
  public static native void trfco2 (float [] xray, float [] yray, int n, 
                             String cfrom, String cto); 
  public static native void trfco2 (double [] xray, double [] yray, int n, 
                             String cfrom, String cto); 
  public static native void trfco3 (float [] xray, float [] yray, 
                             float [] zray, int n, String cfrom, String cto); 
  public static native void trfco3 (double [] xray, double [] yray, 
                             double [] zray, int n, String cfrom, String cto); 
  public static native void trfdat (int  n, int [] iray); 
  public static native void trfmat (float [] zmat, int nx, int ny, 
                             float [] zmat2, int nx2, int ny2);
  public static native void trfmat (double [] zmat, int nx, int ny, 
                             double [] zmat2, int nx2, int ny2);
  public static native void trfrel (float [] xray, float [] yray, int n);
  public static native void trfrel (double [] xray, double [] yray, int n);
  public static native void trfres ();
  public static native void trfrot (float xang, int nx, int ny);
  public static native void trfscl (float xscl, float yscl);
  public static native void trfshf (int nx, int ny);
  public static native void tria3d (float [] xtri, float [] ytri, 
                                    float [] ztri);
  public static native void tria3d (double [] xtri, double [] ytri, 
                                    double [] ztri);
  public static native int  triang (float [] xray, float [] yray, int n, 
                             int [] i1, int []i2, int [] i3, int nmax);
  public static native int  triang (double [] xray, double [] yray, int n, 
                             int [] i1, int []i2, int [] i3, int nmax);
  public static native void triflc (float [] xray, float [] yray, 
                                    int [] iray, int n);
  public static native void triflc (double [] xray, double [] yray, 
                                    int [] iray, int n);
  public static native void trifll  (float [] xray, float [] yray);
  public static native void trifll  (double [] xray, double [] yray);
  public static native void triplx ();
  public static native int  tripts (float [] xray, float [] yray, 
			     float [] zray, int n, int [] i1, int []i2,
                             int [] i3, int ntri, float zlev,
                             float [] xpts, float [] ypts, int maxpts, 
                             int [] nray, int maxray);
  public static native int  tripts (double [] xray, double [] yray, 
			     double [] zray, int n, int [] i1, int []i2,
                             int [] i3, int ntri, float zlev,
                             double [] xpts, double [] ypts, int maxpts, 
                             int [] nray, int maxray);
  public static native int  trmlen (String cstr);
  public static native void texmod (String copt);
  public static native void texopt (String copt, String ctype);
  public static native void texval (float x, String copt);
  public static native void ttfont (String copt);
  public static native void tube3d (float x1, float y1, float z1, 
              float x2, float y2, float z2, float r, int nsk1, int nsk2); 
  public static native void txtbgd (int nclr);
  public static native void txtjus (String copt);
  public static native void txture (int  [] izmat, int nx, int ny);
  public static native void unit   (int n);
  public static native void units  (String copt);
  public static native void upstr  (String cstr);
  public static native int  utfint (String cstr, int [] iray, int nray);
  public static native void vang3d (float a);
  public static native void vclp3d (float x1, float x2);
  public static native void vecclr (int n);
  public static native void vecf3d  (float [] xvray, float [] yvray,
                             float [] zvray, float [] xpray, float [] ypray,
                             float [] zpray, int n, int ivec);
  public static native void vecf3d  (double [] xvray, double [] yvray,
                             double [] zvray, double [] xpray, double [] ypray,
                             double [] zpray, int n, int ivec);
  public static native void vecfld  (float [] xvray, float [] yvray,
                             float [] xpay, float [] ypay, int n, int ivec);
  public static native void vecfld  (double [] xvray, double [] yvray,
                             double [] xpay, double [] ypay, int n, int ivec);
  public static native void vecmat (float [] xv, float [] yv, int nx, int ny,
				    float [] xp, float [] yp, int ivec); 
  public static native void vecmat (double [] xv, double [] yv, int nx, int ny,
				    double [] xp, double [] yp, int ivec); 
  public static native void vecmat3d (float [] xv, float [] yv, float [] zv,
				    int nx, int ny, int nz,
				    float [] xp, float [] yp, float [] zp, 
				    int ivec); 
  public static native void vecmat3d (double [] xv, double [] yv, double [] zv,
				    int nx, int ny, int nz,
				    double [] xp, double [] yp, double [] zp, 
				    int ivec); 
  public static native void vecopt (float x, String copt);
  public static native void vector (int nx1, int ny1, int nx2, int ny2, int ivec);
  public static native void vectr3 (float x1, float y1, float z1,
                             float x2, float y2, float z2, int ivec);
  public static native void vfoc3d (float x, float y, float z, String cview);
  public static native void view3d (float xvu, float yvu, float zvu, String cvu);
  public static native void vkxbar (int nvfx);
  public static native void vkybar (int nvfy);
  public static native void vkytit (int nvfy);
  public static native void vltfil (String cfil, String copt);
  public static native void vscl3d (float x);
  public static native void vtx3d  (float [] xray, float [] yray, 
              float  [] zray, int n, String copt);
  public static native void vtx3d  (double [] xray, double [] yray, 
              double  [] zray, int n, String copt);
  public static native void vtxc3d (float [] xray, float [] yray, 
              float [] zray, int [] ic, int n, String copt);
  public static native void vtxc3d (double [] xray, double [] yray, 
              double [] zray, int [] ic, int n, String copt);
  public static native void vtxn3d (float [] xray, float [] yray, 
              float [] zray, float [] xn, float [] yn, float [] zn, 
              int n, String copt);
  public static native void vtxn3d (double [] xray, double [] yray, 
              double [] zray, double [] xn, double [] yn, double [] zn, 
              int n, String copt);
  public static native void vup3d  (float a);
  public static native int  wgapp  (int ip, String clab);
  public static native int  wgappb (int ip, byte [] iray, int nw, int nh);
  public static native int  wgbas  (int ip, String ctype);
  public static native int  wgbox  (int ip, String cstr, int isel);
  public static native int  wgbut  (int ip, String cstr, int ival);
  public static native int  wgcmd  (int ip, String clab, String cmd);
  public static native int  wgdlis (int ip, String cstr, int isel);
  public static native int  wgdraw (int ip);
  public static native int  wgfil  (int ip, String clab, String cstr, 
                                    String cmask);
  public static native void wgfin  ();
  public static native int  wgicon (int ip, String clab, int nw, int nh, 
                                    String cfl);
  public static native int  wgimg  (int ip, String clab, byte [] iray, int nw,
                                    int nh); 
  public static native int  wgini  (String ctype);
  public static native int  wglab  (int ip, String cstr);
  public static native int  wglis  (int ip, String cstr, int isel);
  public static native int  wgok   (int ip);
  public static native int  wgpop  (int ip, String clab);
  public static native int  wgpopb (int ip, byte [] iray, int nw, int nh);
  public static native int  wgquit (int ip);
  public static native int  wgltxt (int ip, String clab, String ctext, 
                                    int iper);
  public static native int  wgpbar (int ip, float x1, float x2, float xstp);
  public static native int  wgpbut (int ip, String clab);
  public static native int  wgpicon (int ip, String clab, int nw, int nh, 
                                    String cfl);
  public static native int  wgpimg (int ip, String clab, byte [] iray, int nw,
                                   int nh); 
  public static native int  wgscl  (int ip, String cstr, float x1, float x2, 
                                    float xval, int ndez);
  public static native int  wgsep  (int ip);
  public static native int  wgstxt (int ip, int nsize, int nmax);
  public static native int  wgtbl  (int ip, int nrows, int ncols);
  public static native int  wgtxt  (int ip, String cstr);
  public static native void widbar (int n);
  public static native void wimage (String cfil);  
  public static native void winapp (String copt);  
  public static native void wincbk (String cname, String copt);
  public static native void winico (String copt);
  public static native void winjus (String copt);
  public static native void winkey (String copt);
  public static native void winmod (String copt);
  public static native void winopt (int iopt, String copt);
  public static native void windbr (float x, int nx, int ny, int n, float a);
  public static native void window (int nx, int ny, int nw, int nh);
  public static native void winfnt (String cfont);
  public static native int  winid  ();
  public static native void winsiz (int nw, int nh);
  public static native void wintit (String cstr);
  public static native void wintyp (String copt);
  public static native void wmfmod (String c1, String c2);
  public static native void world  ();
  public static native void wpixel (int ix, int iy, int iclr);
  public static native void wpixls (byte [] iray, int ix, int iy, int nw, int nh);
  public static native void wpxrow (byte [] iray, int ix, int iy, int n);
  public static native int  writfl (int nu, byte [] cbuf, int nbyte); 
  public static native void wtiff  (String cfil);
  public static native void x11fnt (String cfont, String ctail);
  public static native void x11mod (String copt);
  public static native float x2dpos (float x, float y);
  public static native float x3dabs (float x, float y, float z);
  public static native float x3dpos (float x, float y, float z);
  public static native float x3drel (float x, float y, float z);
  public static native void xaxgit ();
  public static native void xaxis  (float xa, float xe, float xor, float xstp, 
                             int nl, String cstr, int it, int nx, int ny);
  public static native void xaxlg  (float xa, float xe, float xor, float xstp,
                             int nl, String cstr, int it, int nx, int ny);
  public static native void xaxmap (float xa, float xe, float xor, float xstp,
                             String cstr, int it, int ny);
  public static native void xcross ();
  public static native void xdraw  (float x, float y);
  public static native float xinvrs (int n);
  public static native void xmove  (float x, float y);
  public static native float xposn  (float x);
  public static native float y2dpos (float x, float y);
  public static native float y3dabs (float x, float y, float z);
  public static native float y3dpos (float x, float y, float z);
  public static native float y3drel (float x, float y, float z);
  public static native void yaxgit  ();
  public static native void yaxis  (float ya, float ye, float yor, float ystp, 
                             int nl, String cstr, int it, int nx, int ny);
  public static native void yaxlg  (float ya, float ye, float yor, float ystp,
                             int nl, String cstr, int it, int nx, int ny);
  public static native void yaxmap (float ya, float ye, float yor, float ystp,
                             String cstr, int it, int ny);
  public static native void ycross ();
  public static native float yinvrs (int n);
  public static native float yposn  (float y);
  public static native float z3dpos (float x, float y, float z);
  public static native void zaxis  (float za, float ze, float zor, float zstp, 
                      int nl, String cstr, int it, int id, int nx, int ny);
  public static native void zaxlg  (float za, float ze, float zor, float zstp, 
                       int nl, String cstr, int it, int id, int nx, int ny);
  public static native void zbfers ();
  public static native void zbffin ();
  public static native int  zbfini ();
  public static native void zbflin (float x1, float y1, float z1, float x2,
                                    float y2, float z2);
  public static native void zbfmod (String cstr);
  public static native void zbfres ();
  public static native void zbfscl (float x);
  public static native void zbftri (float [] x, float [] y, float [] z, 
                                    int [] ic);
  public static native void zbftri (double [] x, double [] y, double [] z, 
                                    int [] ic);
  public static native void zscale (float za, float ze);

  static {System.loadLibrary ("disjava");
  }
}

