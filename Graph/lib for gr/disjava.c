/****************************************************************/
/**                        disjava.c                           **/
/**                                                            **/
/** Java Interface for Dislin.                                 **/
/**                                                            **/
/** Date     : 15.11.2013                                      **/
/** Version  : 10.3.6                                          **/
/****************************************************************/

#include <jni.h>
#include "Dislin.h"       /* created by javah from Dislin.java  */ 
#include "dislin_d.h"     /* identical with the C header file   */     
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CB 100
static char *clegbf;
static int ilegop = 0;

static int ncbray = 0;
static int icbray[MAX_CB];
static char *cbray[MAX_CB], *cbwin;
static JNIEnv *envcbk;
static char cbfunc[80];
static jmethodID midcbk;
static jclass clscbk;

/* Internal Dislin routines for error messages */
void *jqqlev (int i1, int i2, const char *s);
void qqerror (void *pd, int n, const char *s);

double *dbl_array (JNIEnv *env, jfloatArray arr, int n)
{ double *p;
  int i, m;
  jfloat *xray;

  m = (int) (*env)->GetArrayLength (env, arr);
  if (m < n) 
  { void *pd;
    pd = jqqlev (0, 3, "Java");
    qqerror (pd, 301, "Not enough array elements");
    return NULL;
  }

  p = (double *) calloc (n, sizeof (double));
  if (p == NULL)
  { void *pd;
    pd = jqqlev (0, 3, "Java");
    qqerror (pd, 302, "Not enough memory");
    return NULL;
  }

  xray = (*env)->GetFloatArrayElements(env, arr, 0);
  for (i = 0; i < n; i++)
    p[i] = xray[i];

  (*env)->ReleaseFloatArrayElements(env, arr, xray, 0);
  return p;
}

void copy_dblarray (JNIEnv *env, jfloatArray arr, double *p, int n)
{ int i;

  jfloat *xray = (*env)->GetFloatArrayElements(env, arr, 0);
  for (i = 0; i < n; i++)
    xray[i] = (jfloat) p[i];

  (*env)->ReleaseFloatArrayElements(env, arr, xray, 0);
}

void  dis_callback (int id)
{ char c1[80], c2[80], *p, *q;
  int i, idx = 0;  
  jmethodID mid;
  jclass cls;

  for (i = 0; i < ncbray; i++)
  { if (icbray[i] == id)
    { idx = i;
      break;
    }
  }

  p = cbray[idx];
  strcpy (c1, p);
  q = strstr (c1, ".");
  if (q != NULL) 
  { strcpy (c2, q+1);
    *q = '\0';
  }
  else
    return;

  cls = (*envcbk)->FindClass (envcbk, c1);
  if (cls == NULL) return;
  mid = (*envcbk)->GetStaticMethodID (envcbk, cls, c2, "(I)V");
  if (mid == 0) return;
  (*envcbk)-> CallStaticVoidMethod(envcbk, cls, mid, id);
}

void  dis_callback2 (int id, int irow, int icol)
{ char c1[80], c2[80], *p, *q;
  int i, idx = 0;  
  jmethodID mid;
  jclass cls;

  for (i = 0; i < ncbray; i++)
  { if (icbray[i] == id)
    { idx = i;
      break;
    }
  }

  p = cbray[idx];
  strcpy (c1, p);
  q = strstr (c1, ".");
  if (q != NULL) 
  { strcpy (c2, q+1);
    *q = '\0';
  }
  else
    return;

  cls = (*envcbk)->FindClass (envcbk, c1);
  if (cls == NULL) return;
  mid = (*envcbk)->GetStaticMethodID (envcbk, cls, c2, "(III)V");
  if (mid == 0) return;
  (*envcbk)-> CallStaticVoidMethod(envcbk, cls, mid, id, irow, icol);
}

void  dis_callback3 (int id, int ival)
{ char c1[80], c2[80], *p, *q;
  int i, idx = 0;  
  jmethodID mid;
  jclass cls;

  for (i = 0; i < ncbray; i++)
  { if (icbray[i] == id)
    { idx = i;
      break;
    }
  }

  p = cbray[idx];
  strcpy (c1, p);
  q = strstr (c1, ".");
  if (q != NULL) 
  { strcpy (c2, q+1);
    *q = '\0';
  }
  else
    return;

  cls = (*envcbk)->FindClass (envcbk, c1);
  if (cls == NULL) return;
  mid = (*envcbk)->GetStaticMethodID (envcbk, cls, c2, "(II)V");
  if (mid == 0) return;
  (*envcbk)-> CallStaticVoidMethod(envcbk, cls, mid, id, ival);
}

void  dis_wincbk (int id, int nx, int ny, int nw, int nh)
{ char c1[80], c2[80], *p, *q;
  jmethodID mid;
  jclass cls;

  p = cbwin;
  strcpy (c1, p);
  q = strstr (c1, ".");
  if (q != NULL) 
  { strcpy (c2, q+1);
    *q = '\0';
  }
  else
    return;

  cls = (*envcbk)->FindClass (envcbk, c1);
  if (cls == NULL) return;
  mid = (*envcbk)->GetStaticMethodID (envcbk, cls, c2, "(IIIII)V");
  if (mid == 0) return;
  (*envcbk)-> CallStaticVoidMethod(envcbk, cls, mid, id, nx, ny, nw, nh);
}

double dis_funcbck  (double x, double y, int iopt)
{ float xret;

  xret = (*envcbk)-> CallStaticFloatMethod(envcbk, clscbk, midcbk,
          (float)  x, (float) y, iopt);
  return (double) xret;
}

double dis_func2bck  (double x, double y)
{ float xret;

  xret = (*envcbk)-> CallStaticFloatMethod(envcbk, clscbk, midcbk, 
      (float) x, (float) y);
  return (double) xret;
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_abs3pt (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat z, jfloatArray arr1)
{ double xp, yp;
  jfloat *xray = (*env)->GetFloatArrayElements(env, arr1, 0);
  abs3pt ((double) x, (double) y, (double) z, &xp, &yp);
  xray[0] = (jfloat) xp;
  xray[1] = (jfloat) yp;
  (*env)->ReleaseFloatArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_addlab (JNIEnv *env, jobject object, jstring s1, 
        jfloat v, jint i, jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  addlab ((char *) str1, v, i, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_angle (JNIEnv *env, jobject object, jint i)
{ angle (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_arcell (JNIEnv *env, jobject object, jint nx, jint ny,
        jint na, jint nb, jfloat a, jfloat b, jfloat t)
{ arcell (nx, ny, na, nb, a, b, t);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_areaf (JNIEnv *env, jobject object, jintArray arr1, 
  jintArray arr2, jint n)
{ jint *nxray = (*env)->GetIntArrayElements(env, arr1, 0);
  jint *nyray = (*env)->GetIntArrayElements(env, arr2, 0);
  areaf ((int *) nxray, (int *) nyray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, nxray, 0);
  (*env)->ReleaseIntArrayElements(env, arr2, nyray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_autres (JNIEnv *env, jobject object, jint i, jint j)
{ autres (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_ax2grf (JNIEnv *env, jobject object)
{ ax2grf ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_ax3len (JNIEnv *env, jobject object, jint i, jint j, 
        jint k)
{ ax3len (i, j, k);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_axclrs (JNIEnv *env, jobject object, jint i, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  axclrs (i, (char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_axends (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  axends ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_axgit (JNIEnv *env, jobject object)
{ axgit ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_axis3d (JNIEnv *env, jobject object, jfloat x, 
        jfloat y, jfloat z)
{ axis3d (x, y, z);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_axsbgd (JNIEnv *env, jobject object, jint i)
{ axsbgd (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_axsers (JNIEnv *env, jobject object)
{ axsers ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_axslen (JNIEnv *env, jobject object, jint i, jint j)
{ axslen (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_axsorg (JNIEnv *env, jobject object, jint i, jint j)
{ axsorg (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_axspos (JNIEnv *env, jobject object, jint i, jint j)
{ axspos (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_axsscl (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  axsscl ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_axstyp (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  axstyp ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_barbor (JNIEnv *env, jobject object, jint n)
{ barbor (n);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_barclr (JNIEnv *env, jobject object, jint i1, 
        jint i2, jint i3)
{ barclr (i1, i2, i3);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_bargrp (JNIEnv *env, jobject object, jint i, jfloat x)
{ bargrp (i, x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_barmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  barmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_baropt (JNIEnv *env, jobject object, jfloat x1, 
        jfloat x2)
{ baropt (x1, x2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_barpos (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  barpos ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_bars___3D_3D_3DI
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *y1ray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *y2ray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  bars ((double *) xray, (double *) y1ray, (double *) y2ray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, y1ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, y2ray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_bars___3F_3F_3FI
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
  { bars (p1, p2, p3, n);
    copy_dblarray (env, arr1, p1, n);
    copy_dblarray (env, arr2, p2, n);
    copy_dblarray (env, arr3, p3, n);
  }
  free (p1);
  free (p2);
  free (p3);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_bars3d___3D_3D_3D_3D_3D_3D_3II 
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3, 
        jdoubleArray arr4, jdoubleArray arr5, jdoubleArray arr6, 
	jintArray arr7, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *z1ray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *z2ray = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jdouble *xwray = (*env)->GetDoubleArrayElements(env, arr5, 0);
  jdouble *ywray = (*env)->GetDoubleArrayElements(env, arr6, 0);
  jint *icray = (*env)->GetIntArrayElements(env, arr7, 0);
  bars3d ((double *) xray, (double *) yray, (double *) z1ray,
          (double *) z2ray, (double *) xwray, (double *) ywray,
          (int *) icray,  n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, z1ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, z2ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr5, xwray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr6, ywray, 0);
  (*env)->ReleaseIntArrayElements(env, arr7, icray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_bars3d___3F_3F_3F_3F_3F_3F_3II
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3, 
        jfloatArray arr4, jfloatArray arr5, jfloatArray arr6, 
	jintArray arr7, jint n)
{ double *p1, *p2, *p3, *p4, *p5, *p6;
  jint *icray;
  p1 = dbl_array (env, arr1, n);
  p2 = dbl_array (env, arr2, n);
  p3 = dbl_array (env, arr3, n);
  p4 = dbl_array (env, arr4, n);
  p5 = dbl_array (env, arr5, n);
  p6 = dbl_array (env, arr6, n);
  icray = (*env)->GetIntArrayElements(env, arr7, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL &&
      p4 != NULL && p5 != NULL && p6 != NULL)
    bars3d (p1, p2, p3, p4, p5, p6, (int *) icray,  n);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  free (p5);
  free (p6);
  (*env)->ReleaseIntArrayElements(env, arr7, icray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_bartyp (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  bartyp ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_barwth (JNIEnv *env, jobject object, jfloat x)
{ barwth (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_basalf (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  basalf ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_basdat (JNIEnv *env, jobject object, jint id, jint im,
       jint iy)
{ basdat (id, im, iy);
}


JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_bezier___3D_3DI_3D_3DI 
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jint nray, 
        jdoubleArray arr3, jdoubleArray arr4, jint n)
{ jdouble *xray  = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray  = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *x2ray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *y2ray = (*env)->GetDoubleArrayElements(env, arr4, 0);
  bezier ((double *) xray, (double *) yray, nray,
          (double *) x2ray, (double *) y2ray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, x2ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, y2ray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_bezier___3F_3FI_3F_3FI
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jint nray, 
        jfloatArray arr3, jfloatArray arr4, jint n)
{ double *p1, *p2, *p3, *p4;
  p1 = dbl_array (env, arr1, nray);
  p2 = dbl_array (env, arr2, nray);
  p3 = dbl_array (env, arr3, n);
  p4 = dbl_array (env, arr4, n);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL)
  { bezier (p1, p2, nray, p3, p4, n);
    copy_dblarray (env, arr3, p3, n);
    copy_dblarray (env, arr4, p4, n);
  }
  free (p1);
  free (p2);
  free (p3);
  free (p4);
}

JNIEXPORT jshort JNICALL 
Java_de_dislin_Dislin_bitsi2 (JNIEnv *env, jobject object, jint nbits, 
        jshort mher, jint iher, short mhin, jint ihin)
{ return  bitsi2 (nbits, mher, iher, mhin, ihin);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_bitsi4 (JNIEnv *env, jobject object, jint nbits, 
        jint mher, jint iher, jint mhin, jint ihin)
{ return  bitsi4 (nbits, mher, iher, mhin, ihin);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_bmpfnt (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  bmpfnt ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_bmpmod (JNIEnv *env, jobject object, jint i, jstring s1,
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  bmpmod (i, (char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_box2d (JNIEnv *env, jobject object)
{ box2d ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_box3d (JNIEnv *env, jobject object)
{ box3d ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_bufmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  bufmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_center (JNIEnv *env, jobject object)
{ center ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_cgmbgd (JNIEnv *env, jobject object, jfloat xr,
        jfloat xg, jfloat xb)
{ cgmbgd (xr, xg, xb);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_cgmpic (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  cgmpic ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_cgmver (JNIEnv *env, jobject object, jint i)
{ cgmver (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_chaang (JNIEnv *env, jobject object, jfloat x)
{ chaang (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_chacod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  chacod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_chaspc (JNIEnv *env, jobject object, jfloat x)
{ chaspc (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_chawth (JNIEnv *env, jobject object, jfloat x)
{ chawth (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_chnatt (JNIEnv *env, jobject object)
{ chnatt ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_chncrv (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  chncrv ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_chndot (JNIEnv *env, jobject object)
{ chndot ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_chndsh (JNIEnv *env, jobject object)
{ chndsh ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_chnbar (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  chnbar ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_chnpie (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  chnpie ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_circ3p (JNIEnv *env, jobject object, jfloat x1, 
        jfloat y1, jfloat x2, jfloat y2, jfloat x3, jfloat y3, 
        jfloatArray arr1)
{ double v1, v2, v3;
  jfloat *xray = (*env)->GetFloatArrayElements(env, arr1, 0);
  circ3p ((double) x1, (double) y1, (double) x2, (double) y2, (double) x3, 
          (double) y3, &v1, &v2, &v3);
  xray[0] = (jfloat) v1;
  xray[1] = (jfloat) v2;
  xray[2] = (jfloat) v3;
  (*env)->ReleaseFloatArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_circle (JNIEnv *env, jobject object, jint i, jint j, 
        jint k)
{ circle (i, j, k);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_circsp (JNIEnv *env, jobject object, jint i)
{ circsp (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_clip3d (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  clip3d ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_closfl (JNIEnv *env, jobject object, jint i)
{ return closfl (i);
}
 
JNIEXPORT void JNICALL
Java_de_dislin_Dislin_clpbor (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  clpbor ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_clpmod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  clpmod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_clpwin (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh)
{ clpwin (nx, ny, nw, nh);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_clrcyc (JNIEnv *env, jobject object, jint i, jint j)
{ clrcyc (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_clrmod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  clrmod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_clswin (JNIEnv *env, jobject object, jint i)
{ clswin (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_color (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  color ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_colran (JNIEnv *env, jobject object, jint i, jint j)
{ colran (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_colray___3D_3II (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jintArray arr2, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jint *nray = (*env)->GetIntArrayElements(env, arr2, 0);
  colray ((double *) xray, (int *) nray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseIntArrayElements(env, arr2, nray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_colray___3F_3II (JNIEnv *env, jobject object, 
        jfloatArray arr1, jintArray arr2, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  jint *nray = (*env)->GetIntArrayElements(env, arr2, 0);
  if (p1 != NULL)
    colray (p1, (int *) nray, n);
  free (p1);
  (*env)->ReleaseIntArrayElements(env, arr2, nray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_complx (JNIEnv *env, jobject object)
{ complx ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_conclr (JNIEnv *env, jobject object, jintArray arr1,
        jint n)
{ jint *nxray = (*env)->GetIntArrayElements(env, arr1, 0);
  conclr ((int *) nxray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, nxray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_concrv___3D_3DIF (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jint n, jfloat zlev)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  concrv ((double *) xray, (double *) yray, n, zlev);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_concrv___3F_3FIF (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jint n, jfloat zlev)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  if (p1 != NULL && p2 != NULL)
    concrv (p1, p2, n, zlev);
  free (p1);
  free (p2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_cone3d (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat z, jfloat r, jfloat h1, jfloat h2, jint n, jint m)
{ cone3d (x, y, z, r, h1, h2, n, m);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_confll___3D_3D_3DI_3I_3I_3II_3DI
       (JNIEnv *env, jobject object, jdoubleArray arr1, 
        jdoubleArray arr2, jdoubleArray arr3, jint n,
	jintArray arr4, jintArray arr5, jintArray arr6, jint ntri,
	jdoubleArray arr7, jint nlev)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr4, 0);
  jint *n2ray = (*env)->GetIntArrayElements(env, arr5, 0);
  jint *n3ray = (*env)->GetIntArrayElements(env, arr6, 0);
  jdouble *zlvray = (*env)->GetDoubleArrayElements(env, arr7, 0);
  confll ((double *) xray, (double *) yray, (double *) zray, n,
          (int *) n1ray, (int *) n2ray, (int *) n3ray, ntri, 
          (double *) zlvray, nlev);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseIntArrayElements(env, arr4, n1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, n2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr6, n3ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr7, zlvray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_confll___3F_3F_3FI_3I_3I_3II_3FI 
       (JNIEnv *env, jobject object, jfloatArray arr1, 
        jfloatArray arr2, jfloatArray arr3, jint n,
        jintArray arr4, jintArray arr5, jintArray arr6, jint ntri,
        jfloatArray arr7, jint nlev)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr7, n);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr4, 0);
  jint *n2ray = (*env)->GetIntArrayElements(env, arr5, 0);
  jint *n3ray = (*env)->GetIntArrayElements(env, arr6, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL)
    confll (p1, p2, p3, n, (int *) n1ray, (int *) n2ray, (int *) n3ray, ntri, 
            p4, nlev);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  (*env)->ReleaseIntArrayElements(env, arr4, n1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, n2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr6, n3ray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_congap (JNIEnv *env, jobject object, jfloat x)
{ congap (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_conlab (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  conlab ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_conmat___3DIIF (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint nx, jint ny, jfloat zlev)
{ jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr1, 0);
  conmat ((double *) zmat, nx, ny, zlev);
  (*env)->ReleaseDoubleArrayElements(env, arr1, zmat, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_conmat___3FIIF (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint nx, jint ny, jfloat zlev)
{ double *p1 = dbl_array (env, arr1, nx * ny);
  if (p1 != NULL)
    conmat (p1, nx, ny, zlev);
  free (p1);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_conmod (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ conmod (x, y);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_conn3d (JNIEnv *env, jobject object, jfloat x, 
        jfloat y, jfloat z)
{ conn3d (x, y, z);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_connpt (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ connpt (x, y);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_conpts___3DI_3DI_3DF_3D_3DI_3II 
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint n, jdoubleArray arr2, jint m,
        jdoubleArray arr3, jfloat zlev, jdoubleArray arr4,
        jdoubleArray arr5, jint maxpts, jintArray arr6, jint maxray)
{ int ncurvs;
  jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *xpts = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jdouble *ypts = (*env)->GetDoubleArrayElements(env, arr5, 0);
  jint *nray = (*env)->GetIntArrayElements(env, arr6, 0);
  conpts ((double *) xray, n, (double *) yray, m, (double *) zmat, zlev,
          (double *) xpts, (double *) ypts, maxpts, (int *) nray, maxray, 
          &ncurvs);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zmat, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, xpts, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr5, ypts, 0);
  (*env)->ReleaseIntArrayElements(env,   arr6, nray, 0);
  return ncurvs;
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_conpts___3FI_3FI_3FF_3F_3FI_3II
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint n, jfloatArray arr2, jint m,
        jfloatArray arr3, jfloat zlev, jfloatArray arr4,
        jfloatArray arr5, jint maxpts, jintArray arr6, jint maxray)
{ int ncurvs;
  double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, m);
  double *p3 = dbl_array (env, arr3, n * m);
  double *p4 = dbl_array (env, arr4, maxpts);
  double *p5 = dbl_array (env, arr5, maxpts);
  jint *nray = (*env)->GetIntArrayElements(env, arr6, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && p5 != NULL)
  { conpts (p1, n, p2, m, p3, zlev, p4, p5, maxpts, (int *) nray, maxray, 
            &ncurvs);
    copy_dblarray (env, arr4, p4, maxpts);
    copy_dblarray (env, arr5, p5, maxpts);
  }
  free (p1); 
  free (p2); 
  free (p3); 
  free (p4); 
  free (p5); 
  (*env)->ReleaseIntArrayElements(env, arr6, nray, 0);
  return ncurvs;
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_conshd___3DI_3DI_3D_3DI
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint n, jdoubleArray arr2, jint m,
        jdoubleArray arr3, jdoubleArray arr4, jint nlev)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *zlev = (*env)->GetDoubleArrayElements(env, arr4, 0);
  conshd ((double *) xray, n, (double *) yray, m, (double *) zmat, 
          (double *) zlev, nlev);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zmat, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, zlev, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_conshd___3FI_3FI_3F_3FI 
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint n, jfloatArray arr2, jint m,
        jfloatArray arr3, jfloatArray arr4, jint nlev)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, m);
  double *p3 = dbl_array (env, arr3, n * m);
  double *p4 = dbl_array (env, arr4, nlev);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL)
    conshd (p1, n, p2, m, p3, p4, nlev);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_conshd3d___3DI_3DI_3D_3DI
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint n, jdoubleArray arr2, jint m,
        jdoubleArray arr3, jdoubleArray arr4, jint nlev)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *zlev = (*env)->GetDoubleArrayElements(env, arr4, 0);
  conshd3d ((double *) xray, n, (double *) yray, m, (double *) zmat, 
          (double *) zlev, nlev);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zmat, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, zlev, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_conshd3d___3FI_3FI_3F_3FI 
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint n, jfloatArray arr2, jint m,
        jfloatArray arr3, jfloatArray arr4, jint nlev)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, m);
  double *p3 = dbl_array (env, arr3, n * m);
  double *p4 = dbl_array (env, arr4, nlev);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL)
    conshd3d (p1, n, p2, m, p3, p4, nlev);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_contri___3D_3D_3DI_3I_3I_3IIF 
       (JNIEnv *env, jobject object, jdoubleArray arr1, 
        jdoubleArray arr2, jdoubleArray arr3, jint n,
        jintArray arr4, jintArray arr5, jintArray arr6, jint ntri, jfloat zlev)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr4, 0);
  jint *n2ray = (*env)->GetIntArrayElements(env, arr5, 0);
  jint *n3ray = (*env)->GetIntArrayElements(env, arr6, 0);
  contri ((double *) xray, (double *) yray, (double *) zray, n,
          (int *) n1ray, (int *) n2ray, (int *) n3ray, ntri, zlev);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseIntArrayElements(env, arr4, n1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, n2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr6, n3ray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_contri___3F_3F_3FI_3I_3I_3IIF
       (JNIEnv *env, jobject object, jfloatArray arr1, 
        jfloatArray arr2, jfloatArray arr3, jint n,
	jintArray arr4, jintArray arr5, jintArray arr6, jint ntri, jfloat zlev)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr4, 0);
  jint *n2ray = (*env)->GetIntArrayElements(env, arr5, 0);
  jint *n3ray = (*env)->GetIntArrayElements(env, arr6, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    contri (p1, p2, p3, n, (int *) n1ray, (int *) n2ray, (int *) n3ray, 
            ntri, zlev);
  free (p1);
  free (p2);
  free (p3);
  (*env)->ReleaseIntArrayElements(env, arr4, n1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, n2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr6, n3ray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_contur___3DI_3DI_3DF (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint n, jdoubleArray arr2, jint m,
        jdoubleArray arr3, jfloat zlev)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr3, 0);
  contur ((double *) xray, n, (double *) yray, m, (double *) zmat, zlev);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zmat, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_contur___3FI_3FI_3FF (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint n, jfloatArray arr2, jint m,
        jfloatArray arr3, jfloat zlev)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, m);
  double *p3 = dbl_array (env, arr3, n * m);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    contur (p1, n, p2, m, p3, zlev);
  free (p1);
  free (p2);
  free (p3);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_cross (JNIEnv *env, jobject object)
{ cross ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_crvmat___3DIIII (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint nx, jint ny, jint ipts, jint jpts)
{ jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr1, 0);
  crvmat ((double *) zmat, nx, ny, ipts, jpts);
  (*env)->ReleaseDoubleArrayElements(env, arr1, zmat, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_crvmat___3FIIII (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint nx, jint ny, jint ipts, jint jpts)
{ double *p1 = dbl_array (env, arr1, nx * ny);
  if (p1 != NULL)
    crvmat (p1, nx, ny, ipts, jpts);
  free (p1);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_crvqdr___3D_3D_3DI 
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  crvqdr ((double *) xray, (double *) yray, (double *) zray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_crvqdr___3F_3F_3FI
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    crvqdr (p1, p2, p3, n);
  free (p1);
  free (p2);
  free (p3);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_crvt3d___3D_3D_3D_3D_3II
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3, 
        jdoubleArray arr4, jintArray arr5, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *rray = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr5, 0);
  crvt3d ((double *) xray, (double *) yray, (double *) zray, 
          (double *) rray, (int *) n1ray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, rray, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, n1ray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_crvt3d___3F_3F_3F_3F_3II
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3, 
        jfloatArray arr4, jintArray arr5, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr4, n);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr5, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL)
    crvt3d (p1, p2, p3, p4, (int *) n1ray, n);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_crvtri___3D_3D_3DI_3I_3I_3II
       (JNIEnv *env, jobject object, jdoubleArray arr1, 
        jdoubleArray arr2, jdoubleArray arr3, jint n,
	jintArray arr4, jintArray arr5, jintArray arr6, jint ntri)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr4, 0);
  jint *n2ray = (*env)->GetIntArrayElements(env, arr5, 0);
  jint *n3ray = (*env)->GetIntArrayElements(env, arr6, 0);
  crvtri ((double *) xray, (double *) yray, (double *) zray, n,
          (int *) n1ray, (int *) n2ray, (int *) n3ray, ntri);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseIntArrayElements(env, arr4, n1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, n2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr6, n3ray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_crvtri___3F_3F_3FI_3I_3I_3II 
       (JNIEnv *env, jobject object, jfloatArray arr1, 
        jfloatArray arr2, jfloatArray arr3, jint n,
        jintArray arr4, jintArray arr5, jintArray arr6, jint ntri)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr4, 0);
  jint *n2ray = (*env)->GetIntArrayElements(env, arr5, 0);
  jint *n3ray = (*env)->GetIntArrayElements(env, arr6, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    crvtri (p1, p2, p3, n, (int *) n1ray, (int *) n2ray, (int *) n3ray, ntri);
  free (p1);
  free (p2);
  free (p3);
  (*env)->ReleaseIntArrayElements(env, arr4, n1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, n2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr6, n3ray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_csrlin (JNIEnv *env, jobject object, jintArray arr1) 
{ jint *ixray = (*env)->GetIntArrayElements(env, arr1, 0);
  csrlin ((int *) &ixray[0], (int *) &ixray[1], (int *) &ixray[2],
          (int *) &ixray[3]);
  (*env)->ReleaseIntArrayElements(env, arr1, ixray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_csrmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  csrmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_csrmov (JNIEnv *env, jobject object, jintArray arr1, 
        jintArray arr2, jint nmax)
{ int iret, n;
  jint *ixray = (*env)->GetIntArrayElements(env, arr1, 0);
  jint *iyray = (*env)->GetIntArrayElements(env, arr2, 0);
  csrmov ((int *) ixray, (int *) iyray, nmax, &n, &iret);
  (*env)->ReleaseIntArrayElements(env, arr1, ixray, 0);
  (*env)->ReleaseIntArrayElements(env, arr2, iyray, 0);
  return n;
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_csrpt1 (JNIEnv *env, jobject object, jint i) 
{ int ix, iy;
  csrpt1 (&ix, &iy);
  if (i == 1)
    return ix;
  else if (i == 2)
    return iy;
  else
    return -1;
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_csrpos (JNIEnv *env, jobject object, jintArray arr1) 
{ int n;
  jint *ixray = (*env)->GetIntArrayElements(env, arr1, 0);
  n = csrpos ((int *) &ixray[0], (int *) &ixray[1]);
  (*env)->ReleaseIntArrayElements(env, arr1, ixray, 0);
  return n;
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_csrrec (JNIEnv *env, jobject object, jintArray arr1) 
{ jint *ixray = (*env)->GetIntArrayElements(env, arr1, 0);
  csrrec ((int *) &ixray[0], (int *) &ixray[1], (int *) &ixray[2],
          (int *) &ixray[3]);
  (*env)->ReleaseIntArrayElements(env, arr1, ixray, 0);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_csrkey (JNIEnv *env, jobject object) 
{ int n;
  n = csrkey ();
  return n;
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_csrpts (JNIEnv *env, jobject object, jintArray arr1, 
        jintArray arr2, jint nmax)
{ int iret, n;
  jint *ixray = (*env)->GetIntArrayElements(env, arr1, 0);
  jint *iyray = (*env)->GetIntArrayElements(env, arr2, 0);
  csrpts ((int *) ixray, (int *) iyray, nmax, &n, &iret);
  (*env)->ReleaseIntArrayElements(env, arr1, ixray, 0);
  (*env)->ReleaseIntArrayElements(env, arr2, iyray, 0);
  return n;
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_csrtyp (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  csrtyp ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_csruni (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  csruni ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curv3d___3D_3D_3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  curv3d ((double *) xray, (double *) yray, (double *) zray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curv3d___3F_3F_3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    curv3d (p1, p2, p3, n);
  free (p1);
  free (p2);
  free (p3);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curv4d___3D_3D_3D_3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3,
        jdoubleArray arr4, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *wray = (*env)->GetDoubleArrayElements(env, arr4, 0);
  curv4d ((double *) xray, (double *) yray, (double *) zray, (double *) wray, 
          n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, wray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curv4d___3F_3F_3F_3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3,
        jfloatArray arr4, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr4, n);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL)
    curv4d (p1, p2, p3, p4, n);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curve___3D_3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  curve ((double *) xray, (double *) yray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curve___3F_3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  if (p1 != NULL && p2 != NULL)
    curve (p1, p2, n);
  free (p1);
  free (p2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curve3___3D_3D_3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  curve3 ((double *) xray, (double *) yray, (double *) zray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curve3___3F_3F_3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    curve3 (p1, p2, p3, n);
  free (p1);
  free (p2);
  free (p3);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curvmp___3D_3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  curvmp ((double *) xray, (double *) yray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curvmp___3F_3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  if (p1 != NULL && p2 != NULL)
    curvmp (p1, p2, n);
  free (p1);
  free (p2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curvx3___3DF_3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jfloat y, jdoubleArray arr2, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  curvx3 ((double *) xray, y, (double *) yray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curvx3___3FF_3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloat y, jfloatArray arr2, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  if (p1 != NULL && p2 != NULL)
    curvx3 (p1, y, p2, n);
  free (p1);
  free (p2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curvy3__F_3D_3DI (JNIEnv *env, jobject object, jfloat x, 
        jdoubleArray arr1, jdoubleArray arr2, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  curvy3 (x, (double *) xray, (double *) yray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_curvy3__F_3F_3FI (JNIEnv *env, jobject object, jfloat x, 
        jfloatArray arr1, jfloatArray arr2, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  if (p1 != NULL && p2 != NULL)
    curvy3 (x, p1, p2, n);
  free (p1);
  free (p2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_cyli3d (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat z, jfloat r, jfloat h, jint n, jint m)
{ cyli3d (x, y, z, r, h, n, m);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_dash (JNIEnv *env, jobject object)
{ dash ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_dashl (JNIEnv *env, jobject object)
{ dashl ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_dashm (JNIEnv *env, jobject object)
{ dashm ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_dbffin (JNIEnv *env, jobject object)
{ dbffin ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_dbfini (JNIEnv *env, jobject object)
{ return dbfini ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_dbfmod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  dbfmod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_delglb (JNIEnv *env, jobject object)
{ delglb ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_digits (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  digits (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_disalf (JNIEnv *env, jobject object)
{ disalf ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_disenv (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  disenv ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_disfin (JNIEnv *env, jobject object)
{ disfin ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_disini (JNIEnv *env, jobject object)
{ disini ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_disk3d (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat z, jfloat r1, jfloat r2, jint n, jint m)
{ disk3d (x, y, z, r1, r2, n, m);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_doevnt (JNIEnv *env, jobject object)
{ doevnt ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_dot (JNIEnv *env, jobject object)
{ dot ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_dotl (JNIEnv *env, jobject object)
{ dotl ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_duplx (JNIEnv *env, jobject object)
{ duplx ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_dwgbut (JNIEnv *env, jobject object, jstring s, 
        jint ival)
{ int iret;
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  iret = dwgbut ((char *) str, ival);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return iret;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_dwgerr (JNIEnv *env, jobject object)
{ return dwgerr ();
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_dwgfil (JNIEnv *env, jobject object, jstring s1, 
        jstring s2, jstring s3)
{ char *p;
  jstring s4;
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  const char *str3 = (*env)->GetStringUTFChars(env, s3, 0);
  p = dwgfil ((char *) str1, (char *) str2, (char *) str3);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  (*env)->ReleaseStringUTFChars(env, s3, str3);
  s4 = (*env)->NewStringUTF(env, p);
  free (p);
  return s4;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_dwglis (JNIEnv *env, jobject object, jstring s1, 
        jstring s2, jint ilis)
{ int iret;
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  iret = dwglis ((char *) str1, (char *) str2, ilis);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  return iret;
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_dwgmsg (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  dwgmsg ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_dwgtxt (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ char *p;
  jstring s3;
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  p = dwgtxt ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  s3 = (*env)->NewStringUTF(env, p);
  free (p);
  return s3;
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_ellips (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh)
{ ellips (nx, ny, nw, nh);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_endgrf (JNIEnv *env, jobject object)
{ endgrf ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_erase (JNIEnv *env, jobject object)
{ erase ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_errbar___3D_3D_3D_3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, 
        jdoubleArray arr3, jdoubleArray arr4, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *err1 = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *err2 = (*env)->GetDoubleArrayElements(env, arr4, 0);
  errbar ((double *) xray, (double *) yray, (double *) err1, (double *) err2, 
          n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, err1, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, err2, 0);
}
JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_errbar___3F_3F_3F_3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, 
        jfloatArray arr3, jfloatArray arr4, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr4, n);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL)
    errbar (p1, p2, p3, p4, n);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_errdev (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  errdev ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}


JNIEXPORT void JNICALL
Java_de_dislin_Dislin_errfil (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  errfil ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_errmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  errmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_eushft (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  eushft ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_expimg (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  expimg ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_expzlb (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  expzlb ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_fbars___3D_3D_3D_3D_3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3, 
        jdoubleArray arr4, jdoubleArray arr5, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *y1ray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *y2ray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *y3ray = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jdouble *y4ray = (*env)->GetDoubleArrayElements(env, arr5, 0);
  fbars ((double *) xray, (double *) y1ray, (double *) y2ray, 
     (double *) y3ray, (double *) y4ray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, y1ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, y2ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, y3ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr5, y4ray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_fbars___3F_3F_3F_3F_3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3, 
        jfloatArray arr4, jfloatArray arr5, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr4, n);
  double *p5 = dbl_array (env, arr5, n);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && p5 != NULL)
    fbars (p1, p2, p3, p4, p5, n);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  free (p5);
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_fcha (JNIEnv *env, jobject object, jfloat x, jint ndig)
{ char cstr[80];
  int nl;
  nl = fcha (x, ndig, cstr);
  return (*env)->NewStringUTF(env, cstr);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_field___3D_3D_3D_3DII (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, 
        jdoubleArray arr3, jdoubleArray arr4, jint n, jint ivec)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *uray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *vray = (*env)->GetDoubleArrayElements(env, arr4, 0);
  field ((double *) xray, (double *) yray, (double *) uray, (double *) vray, n,
         ivec);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, uray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, vray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_field___3F_3F_3F_3FII (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, 
        jfloatArray arr3, jfloatArray arr4, jint n, jint ivec)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr4, n);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL)
    field (p1, p2, p3, p4, n, ivec);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_field3d___3D_3D_3D_3D_3D_3DII
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3, 
        jdoubleArray arr4, jdoubleArray arr5, jdoubleArray arr6, 
        jint n, jint ivec)
{ jdouble *x1ray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *y1ray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *z1ray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *x2ray = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jdouble *y2ray = (*env)->GetDoubleArrayElements(env, arr5, 0);
  jdouble *z2ray = (*env)->GetDoubleArrayElements(env, arr6, 0);
  field3d ((double *) x1ray, (double *) y1ray, (double *) z1ray, 
           (double *) x2ray, (double *) y2ray, (double *) z2ray,
           n, ivec);
  (*env)->ReleaseDoubleArrayElements(env, arr1, x1ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, y1ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, z1ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, x2ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr5, y2ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr6, z2ray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_field3d___3F_3F_3F_3F_3F_3FII
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3, 
        jfloatArray arr4, jfloatArray arr5, jfloatArray arr6, 
        jint n, jint ivec)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr4, n);
  double *p5 = dbl_array (env, arr5, n);
  double *p6 = dbl_array (env, arr6, n);
  if (p1 != NULL && p2 != NULL && p3 != NULL && 
      p4 != NULL && p5 != NULL && p6 != NULL)
    field3d (p1, p2, p3, p4, p5, p6, n, ivec);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  free (p5);
  free (p6);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_filbox (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh)
{ filbox (nx, ny, nw, nh);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_filclr (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  filclr ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_filmod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  filmod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_filopt (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  filopt ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_filsiz (JNIEnv *env, jobject object, jstring s, jint i)
{ int i1, i2, i3;
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  i3 = filsiz ((char *) str, &i1, &i2);
  (*env)->ReleaseStringUTFChars(env, s, str);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else if (i == 3)
    return i3;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_filtyp (JNIEnv *env, jobject object, jstring s)
{ int i1;
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  i1 = filtyp ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return i1;
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_filwin (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh)
{ filwin (nx, ny, nw, nh);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_fixspc (JNIEnv *env, jobject object, jfloat x)
{ fixspc (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_flab3d (JNIEnv *env, jobject object)
{ flab3d ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_flen (JNIEnv *env, jobject object, jfloat x, jint i)
{ return flen (x, i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_frame (JNIEnv *env, jobject object, jint i)
{ frame (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_frmclr (JNIEnv *env, jobject object, jint i)
{ frmclr (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_frmbar (JNIEnv *env, jobject object, jint i)
{ frmbar (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_frmess (JNIEnv *env, jobject object, jint i)
{ frmess (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_gapcrv (JNIEnv *env, jobject object, jfloat x)
{ gapcrv (x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_gapsiz (JNIEnv *env, jobject object, jfloat x, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  gapsiz (x, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gaxpar (JNIEnv *env, jobject object, jfloat v1,
        jfloat v2, jstring s1, jstring s2, jfloatArray arr1)
{ int ndig; 
  double x1, x2, x3, x4;
  jfloat *xray  = (*env)->GetFloatArrayElements(env, arr1, 0);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  gaxpar ((double) v1, (double) v2, (char *) str1, (char *) str2, 
           &x1, &x2, &x3, &x4, &ndig);
  xray[0] = (jfloat) x1;
  xray[1] = (jfloat) x2;
  xray[2] = (jfloat) x3;
  xray[3] = (jfloat) x4;
  (*env)->ReleaseFloatArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  return ndig; 
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_getalf (JNIEnv *env, jobject object)
{ char *p;
  p = getalf ();
  return (*env)->NewStringUTF(env, p);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getang (JNIEnv *env, jobject object)
{ return getang ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getbpp (JNIEnv *env, jobject object)
{ return getbpp ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getclr (JNIEnv *env, jobject object)
{ return getclr ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getclp (JNIEnv *env, jobject object, jint i)
{ int i1, i2, i3, i4;
  getclp (&i1, &i2, &i3, &i4);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else if (i == 3)
    return i3;
  else if (i == 4)
    return i4;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getdig (JNIEnv *env, jobject object, jint i)
{ int i1, i2, i3;
  getdig (&i1, &i2, &i3);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else if (i == 3)
    return i3;
  else 
    return -1;
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_getdsp (JNIEnv *env, jobject object)
{ char *p;
  p = getdsp ();
  return (*env)->NewStringUTF(env, p);
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_getfil (JNIEnv *env, jobject object)
{ char *p;
  p = getfil ();
  return (*env)->NewStringUTF(env, p);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_getgrf (JNIEnv *env, jobject object, jint i, jstring s)
{ double a, e, or, step;
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  getgrf (&a, &e, &or, &step, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  if (i == 1)
    return (jfloat) a;
  else if (i == 2)
    return (jfloat) e;
  else if (i == 3)
    return (jfloat) or;
  else if (i == 4)
    return (jfloat) step;
  else
    return (jfloat) -1.0;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gethgt (JNIEnv *env, jobject object)
{ return gethgt ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gethnm (JNIEnv *env, jobject object)
{ return gethnm ();
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_getind (JNIEnv *env, jobject object, jint idx, jint i)
{ double x1, x2, x3;
  getind (idx, &x1, &x2, &x3);
  if (i == 1)
    return (jfloat) x1;
  else if (i == 2)
    return (jfloat) x2;
  else if (i == 3)
    return (jfloat) x3;
  else 
    return (jfloat) -1.0;
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_getlab (JNIEnv *env, jobject object, jint i)
{ char cx[5], cy[5], cz[5];
  getlab (cx, cy, cz);
  if (i == 1)
    return (*env)->NewStringUTF(env, cx);
  else if (i == 2)
    return (*env)->NewStringUTF(env, cy);
  else
    return (*env)->NewStringUTF(env, cz);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getlen (JNIEnv *env, jobject object, jint i)
{ int i1, i2, i3;
  getlen (&i1, &i2, &i3);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else if (i == 3)
    return i3;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getlev (JNIEnv *env, jobject object)
{ return getlev ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getlin (JNIEnv *env, jobject object)
{ return getlin ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getlit (JNIEnv *env, jobject object, 
        jfloat xp, jfloat yp, jfloat zp,
        jfloat xn, jfloat yn, jfloat zn)
{ return getlit (xp, yp, zp, xn, yn, zn);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_getmat___3D_3D_3DI_3DIIF_3I_3D 
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3,
        jint n, jdoubleArray arr4, jint nx, jint ny, jfloat zval,
        jintArray arr5, jdoubleArray arr6)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jint   *imat = (*env)->GetIntArrayElements(env, arr5, 0);
  jdouble *wmat = (*env)->GetDoubleArrayElements(env, arr6, 0);
  getmat ((double *) xray, (double *) yray, (double *) zray, n, 
          (double *) zmat, 
          nx, ny, zval, (int *) imat, (double *) wmat);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, zmat, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, imat, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr6, wmat, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_getmat___3F_3F_3FI_3FIIF_3I_3F 
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3,
        jint n, jfloatArray arr4, jint nx, jint ny, jfloat zval,
        jintArray arr5, jfloatArray arr6)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr4, nx * ny);
  double *p5 = dbl_array (env, arr6, nx * ny);
  jint   *imat = (*env)->GetIntArrayElements(env, arr5, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && p5 != NULL)
  { getmat (p1, p2, p3, n, p4, nx, ny, zval, (int *) imat, p5);
    copy_dblarray (env, arr4, p4, nx * ny);
  }
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  free (p5);
  (*env)->ReleaseIntArrayElements(env, arr5, imat, 0);
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_getmfl (JNIEnv *env, jobject object)
{ char *p;
  p = getmfl ();
  return (*env)->NewStringUTF(env, p);
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_getmix (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  char *p;
  p = getmix ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return (*env)->NewStringUTF(env, p);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getor (JNIEnv *env, jobject object, jint i)
{ int i1, i2;
  getor (&i1, &i2);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getpag (JNIEnv *env, jobject object, jint i)
{ int i1, i2;
  getpag (&i1, &i2);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getpat (JNIEnv *env, jobject object)
{ return (int) getpat ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getplv (JNIEnv *env, jobject object)
{ return getplv ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getpos (JNIEnv *env, jobject object, jint i)
{ int i1, i2;
  getpos (&i1, &i2);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else 
    return -1;
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_getrgb (JNIEnv *env, jobject object, jint i)
{ double x1, x2, x3;
  getrgb (&x1, &x2, &x3);
  if (i == 1)
    return (jfloat) x1;
  else if (i == 2)
    return (jfloat) x2;
  else if (i == 3)
    return (jfloat) x3;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getran (JNIEnv *env, jobject object, jint i)
{ int i1, i2;
  getran (&i1, &i2);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getres (JNIEnv *env, jobject object, jint i)
{ int i1, i2;
  getres (&i1, &i2);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getscl (JNIEnv *env, jobject object, jint i)
{ int i1, i2, i3;
  getscl (&i1, &i2, &i3);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else if (i == 3)
    return i3;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getscm (JNIEnv *env, jobject object, jint i)
{ int i1, i2, i3;
  getscm (&i1, &i2, &i3);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else if (i == 3)
    return i3;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getscr (JNIEnv *env, jobject object, jint i)
{ int i1, i2;
  getscr (&i1, &i2);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else 
    return -1;
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_getshf (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  char *p;
  p = getshf ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return (*env)->NewStringUTF(env, p);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getsp1 (JNIEnv *env, jobject object, jint i)
{ int i1, i2, i3;
  getsp1 (&i1, &i2, &i3);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else if (i == 3)
    return i3;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getsp2 (JNIEnv *env, jobject object, jint i)
{ int i1, i2, i3;
  getsp2 (&i1, &i2, &i3);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else if (i == 3)
    return i3;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getsym (JNIEnv *env, jobject object, jint i)
{ int i1, i2;
  getsym (&i1, &i2);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gettcl (JNIEnv *env, jobject object, jint i)
{ int i1, i2;
  gettcl (&i1, &i2);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gettic (JNIEnv *env, jobject object, jint i)
{ int i1, i2, i3;
  gettic (&i1, &i2, &i3);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else if (i == 3)
    return i3;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gettyp (JNIEnv *env, jobject object)
{ return gettyp ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getuni (JNIEnv *env, jobject object)
{ FILE *fp; 
  fp = (FILE *) getuni ();
  if (fp == NULL)
    return 0;
  else
    return 6;
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_getver (JNIEnv *env, jobject object)
{ return getver ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getvk (JNIEnv *env, jobject object, jint i)
{ int i1, i2, i3;
  getvk (&i1, &i2, &i3);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else if (i == 3)
    return i3;
  else 
    return -1;
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_getvlt (JNIEnv *env, jobject object)
{ 
  char *p;
  p = getvlt ();
  return (*env)->NewStringUTF(env, p);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getwid (JNIEnv *env, jobject object)
{ return getwid ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getwin (JNIEnv *env, jobject object, jint i)
{ int i1, i2, i3, i4;
  getwin (&i1, &i2, &i3, &i4);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else if (i == 3)
    return i3;
  else if (i == 4)
    return i4;
  else 
    return -1;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_getxid (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int nl;
  nl = getxid ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return nl;
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_gifmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  gifmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_gothic (JNIEnv *env, jobject object)
{ gothic ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_grace (JNIEnv *env, jobject object, jint i)
{ grace (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_graf (JNIEnv *env, jobject object, 
                  jfloat xa, jfloat xe, jfloat xor, jfloat xstp,
                  jfloat ya, jfloat ye, jfloat yor, jfloat ystp)
{ graf (xa, xe, xor, xstp, ya, ye, yor, ystp);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_graf3 (JNIEnv *env, jobject object, 
        jfloat xa, jfloat xe, jfloat xor, jfloat xstp,
        jfloat ya, jfloat ye, jfloat yor, jfloat ystp,
        jfloat za, jfloat ze, jfloat zor, jfloat zstp)
{ graf3 (xa, xe, xor, xstp, ya, ye, yor, ystp, za, ze, zor, zstp);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_graf3d (JNIEnv *env, jobject object, 
        jfloat xa, jfloat xe, jfloat xor, jfloat xstp,
        jfloat ya, jfloat ye, jfloat yor, jfloat ystp,
        jfloat za, jfloat ze, jfloat zor, jfloat zstp)
{ graf3d (xa, xe, xor, xstp, ya, ye, yor, ystp, za, ze, zor, zstp);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_grafmp (JNIEnv *env, jobject object, 
        jfloat xa, jfloat xe, jfloat xor, jfloat xstp,
        jfloat ya, jfloat ye, jfloat yor, jfloat ystp)
{ grafmp (xa, xe, xor, xstp, ya, ye, yor, ystp);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_grffin (JNIEnv *env, jobject object)
{ grffin ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_grfini (JNIEnv *env, jobject object, 
        jfloat x1, jfloat y1, jfloat z1,
        jfloat x2, jfloat y2, jfloat z2,
        jfloat x3, jfloat y3, jfloat z3)
{ grfini (x1, y1, z1, x2, y2, z2, x3, y3, z3);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_grdpol (JNIEnv *env, jobject object, jint i, jint j)
{ grdpol (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_grid (JNIEnv *env, jobject object, jint i, jint j)
{ grid (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_grid3d (JNIEnv *env, jobject object, jint i, jint j, 
        jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  grid3d (i, j, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_gridmp (JNIEnv *env, jobject object, jint i, jint j)
{ gridmp (i, j);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gwgatt (JNIEnv *env, jobject object, jint i, jstring s)
{ int iret;
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  iret = gwgatt (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return iret;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gwgbox (JNIEnv *env, jobject object, jint i)
{ return gwgbox (i);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gwgbut (JNIEnv *env, jobject object, jint i)
{ return gwgbut (i);
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_gwgfil (JNIEnv *env, jobject object, jint id)
{ char cstr[257];
  gwgfil (id, cstr);
  return (*env)->NewStringUTF(env, cstr);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_gwgflt (JNIEnv *env, jobject object, jint id)
{ return gwgflt (id);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gwggui (JNIEnv *env, jobject object)
{ return gwggui ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gwgint (JNIEnv *env, jobject object, jint id)
{ return gwgint (id);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gwglis (JNIEnv *env, jobject object, jint i)
{ return gwglis (i);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_gwgscl (JNIEnv *env, jobject object, jint id)
{ return gwgscl (id);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gwgsiz (JNIEnv *env, jobject object, jint id, jint i)
{ int i1, i2;
  gwgsiz (id, &i1, &i2);
  if (i == 1)
    return i1;
  else if (i == 2)
    return i2;
  else 
    return -1;
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_gwgtbf (JNIEnv *env, jobject object, jint id,
        jint irow, jint icol)
{ return gwgtbf (id, irow, icol);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gwgtbi (JNIEnv *env, jobject object, jint id,
                              jint irow, jint icol)
{ return gwgtbi (id, irow, icol);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_gwgtbl__I_3DIILjava_lang_String_2 
       (JNIEnv *env, jobject object, jint id,
        jdoubleArray arr1, jint n, jint idx, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  jdouble *xray  = (*env)->GetDoubleArrayElements(env, arr1, 0);
  gwgtbl (id, (double *) xray, n, idx, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_gwgtbl__I_3FIILjava_lang_String_2 
       (JNIEnv *env, jobject object, jint id,
        jfloatArray arr1, jint n, jint idx, jstring s)
{ double *p1 = dbl_array (env, arr1, n);
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  if (p1 != NULL)
  { gwgtbl (id, p1, n, idx, (char *) str);
    copy_dblarray (env, arr1, p1, n);
  }
  free (p1);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_gwgtbs (JNIEnv *env, jobject object, jint id,
        jint irow, jint icol)
{ char cstr[257];
  gwgtbs (id, irow, icol, cstr);
  return (*env)->NewStringUTF(env, cstr);
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_gwgtxt (JNIEnv *env, jobject object, jint id)
{ char cstr[257];
  gwgtxt (id, cstr);
  return (*env)->NewStringUTF(env, cstr);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_gwgxid (JNIEnv *env, jobject object, jint i)
{ return gwgxid (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_height (JNIEnv *env, jobject object, jint i)
{ height (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_helve (JNIEnv *env, jobject object)
{ helve ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_helves (JNIEnv *env, jobject object)
{ helves ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_helvet (JNIEnv *env, jobject object)
{ helvet ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_hidwin (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  hidwin (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_histog___3DI_3D_3D 
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint nray, 
        jdoubleArray arr2, jdoubleArray arr3)
{ int n;
  jdouble *xray  = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *x2ray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *y2ray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  histog ((double *) xray, nray, (double *) x2ray, (double *) y2ray, &n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, x2ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, y2ray, 0);
  return n;
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_histog___3FI_3F_3F 
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint nray, 
        jfloatArray arr2, jfloatArray arr3)
{ int n;
  double *p1 = dbl_array (env, arr1, nray);
  int m = (int) (*env)->GetArrayLength (env, arr2);
  double *p2 = dbl_array (env, arr2, m);
  double *p3 = dbl_array (env, arr3, m);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
  { histog (p1, nray, p2, p3, &n);
    copy_dblarray (env, arr2, p2, n);
    copy_dblarray (env, arr3, p3, n);
  }
  free (p1);
  free (p2);
  free (p3);
  return n;
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_hname (JNIEnv *env, jobject object, jint i)
{ hname (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_hpgmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  hpgmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_hsvrgb (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat z, jfloatArray arr1)
{ double x1, x2, x3;
  jfloat *xray = (*env)->GetFloatArrayElements(env, arr1, 0);
  hsvrgb ((double) x, (double) y, (double) z, &x1, &x2, &x3);
  xray[0] = (jfloat) x1;
  xray[1] = (jfloat) x2;
  xray[2] = (jfloat) x3;
  (*env)->ReleaseFloatArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_hsym3d (JNIEnv *env, jobject object, jfloat x)
{ hsym3d (x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_hsymbl (JNIEnv *env, jobject object, jint i)
{ hsymbl (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_htitle (JNIEnv *env, jobject object, jint i)
{ htitle (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_hwfont (JNIEnv *env, jobject object)
{ hwfont ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_hwmode (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  hwmode ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_hworig (JNIEnv *env, jobject object, jint i, jint j)
{ hworig (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_hwpage (JNIEnv *env, jobject object, jint i, jint j)
{ hwpage (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_hwscal (JNIEnv *env, jobject object, jfloat x)
{ hwscal (x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_imgbox (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh)
{ imgbox (nx, ny, nw, nh);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_imgclp (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh)
{ imgclp (nx, ny, nw, nh);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_imgfin (JNIEnv *env, jobject object)
{ imgfin ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_imgfmt (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  imgfmt ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_imgini (JNIEnv *env, jobject object)
{ imgini ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_imgmod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  imgmod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_imgsiz (JNIEnv *env, jobject object, jint i, jint j)
{ imgsiz (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_imgtpr (JNIEnv *env, jobject object, jint i)
{ imgtpr (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_inccrv (JNIEnv *env, jobject object, jint i)
{ inccrv (i);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_incdat (JNIEnv *env, jobject object, jint id, 
        jint im, jint iy)
{ return incdat (id, im, iy);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_incfil (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  incfil ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_incmrk (JNIEnv *env, jobject object, jint i)
{ incmrk (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_intax (JNIEnv *env, jobject object)
{ intax ();
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_intcha (JNIEnv *env, jobject object, jint n)
{ char cstr[80];
  int nl;
  nl = intcha (n, cstr);
  return (*env)->NewStringUTF(env, cstr);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_intlen (JNIEnv *env, jobject object, jint i)
{ return intlen (i);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_intutf (JNIEnv *env, jobject object, 
        jintArray arr1, jint nray, jstring s, jint nmax)
{ int n;
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  jint *iray = (*env)->GetIntArrayElements(env, arr1, 0);
  n = intutf ((int *) iray, nray, (char *) str, nmax);
  (*env)->ReleaseStringUTFChars(env, s, str);
  (*env)->ReleaseIntArrayElements(env, arr1, iray, 0);
  return n;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_indrgb (JNIEnv *env, jobject object, jfloat xr,
        jfloat xg, jfloat xb)
{ return indrgb (xr, xg, xb);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_intrgb (JNIEnv *env, jobject object, jfloat xr,
        jfloat xg, jfloat xb)
{ return intrgb (xr, xg, xb);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_isopts___3DI_3DI_3DI_3DF_3D_3D_3DI
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint nx, jdoubleArray arr2, jint ny,
        jdoubleArray arr3, jint nz, jdoubleArray arr4, jfloat wlev,
        jdoubleArray arr5, jdoubleArray arr6, jdoubleArray arr7, jint nmax)
{ int ntri;
  jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *wmat = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jdouble *xtri = (*env)->GetDoubleArrayElements(env, arr5, 0);
  jdouble *ytri = (*env)->GetDoubleArrayElements(env, arr6, 0);
  jdouble *ztri = (*env)->GetDoubleArrayElements(env, arr7, 0);
  isopts ((double *) xray, nx, (double *) yray, ny, (double *) zray, nz,
          (double *) wmat, wlev, (double *) xtri, (double *) ytri,
          (double *) ztri, nmax, &ntri);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, wmat, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr5, xtri, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr6, ytri, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr7, ztri, 0);
  return ntri;
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_isopts___3FI_3FI_3FI_3FF_3F_3F_3FI 
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint nx, jfloatArray arr2, jint ny,
        jfloatArray arr3, jint nz, jfloatArray arr4, jfloat wlev,
        jfloatArray arr5, jfloatArray arr6, jfloatArray arr7, jint nmax)
{ int ntri;
  double *p1 = dbl_array (env, arr1, nx);
  double *p2 = dbl_array (env, arr2, ny);
  double *p3 = dbl_array (env, arr3, nz);
  double *p4 = dbl_array (env, arr4, nx * ny * nz);
  double *p5 = dbl_array (env, arr5, nmax);
  double *p6 = dbl_array (env, arr6, nmax);
  double *p7 = dbl_array (env, arr7, nmax);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL &&
      p5 != NULL && p6 != NULL && p7 != NULL)
  { isopts (p1, nx, p2, ny, p3, nz, p4,
            wlev, p5, p6, p7, nmax, &ntri);
    copy_dblarray (env, arr5, p5, nmax);
    copy_dblarray (env, arr6, p6, nmax);
    copy_dblarray (env, arr7, p7, nmax);
  }
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  free (p5);
  free (p6);
  free (p7);
  return ntri;
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_itmcat (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ char *p;
  int n1, n2;
  jstring s3;
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  n1 = trmlen ((char *) str1);
  n2 = trmlen ((char *) str2);
  p = (char *) malloc (n1 + n2 + 2);
  strcpy (p, (char *) str1);
  itmcat (p, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  s3 = (*env)->NewStringUTF(env, p);
  free (p);
  return s3;
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_itmncat (JNIEnv *env, jobject object, jstring s1, 
        jint nmx, jstring s2)
{ char *p;
  int n1, n2, n;
  jstring s3;
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  n1 = trmlen ((char *) str1);
  n2 = trmlen ((char *) str2);
  n = n1 + n2 + 2;
  if (n > nmx) n = nmx;
  if (nmx < n1) return s1;
  p = (char *) malloc (n);
  strcpy (p, (char *) str1);
  itmncat (p, nmx, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  s3 = (*env)->NewStringUTF(env, p);
  free (p);
  return s3;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_itmcnt(JNIEnv *env, jobject object, jstring s)
{ int i;
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  i = itmcnt ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return i;
}

JNIEXPORT jstring JNICALL
Java_de_dislin_Dislin_itmstr (JNIEnv *env, jobject object, jstring s, jint i)
{ char *p;
  jstring s2;
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  p = itmstr ((char *) str, i);
  (*env)->ReleaseStringUTFChars(env, s, str);
  s2 = (*env)->NewStringUTF(env, p);
  free (p);
  return s2;
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_jusbar (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  jusbar ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_labclr (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  labclr (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_labdig (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  labdig (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_labdis (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  labdis (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_labels (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  labels ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_labjus (JNIEnv *env, jobject object, jstring s1,
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  labjus ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_labl3d (JNIEnv *env, jobject object, jstring s1)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  labl3d ((char *) str1);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_labmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2, jstring s3)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  const char *str3 = (*env)->GetStringUTFChars(env, s3, 0);
  labmod ((char *) str1, (char *) str2, (char *) str3);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  (*env)->ReleaseStringUTFChars(env, s3, str3);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_labpos (JNIEnv *env, jobject object, jstring s1, jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  labpos ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_labtyp (JNIEnv *env, jobject object, jstring s1, jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  labtyp ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_ldimg (JNIEnv *env, jobject object, jstring s1,
        jshortArray arr1, jint nmax, jint nc )
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  jshort *iray = (*env)->GetShortArrayElements(env, arr1, 0);
  int n;
  n = ldimg (str1, (unsigned short *) iray, nmax, nc);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseShortArrayElements(env, arr1, iray, 0);
  return n;
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_legbgd (JNIEnv *env, jobject object, jint i)
{
  legbgd (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_legclr (JNIEnv *env, jobject object)
{
  legclr ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_legend (JNIEnv *env, jobject object, jstring s, jint n)
{ 
  legend (clegbf, n);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_legini (JNIEnv *env, jobject object, jstring s, jint nlin,
                    jint nmaxln)
{ int i;

  if (ilegop != 0) free (clegbf);
  clegbf = (char *) malloc (nlin * nmaxln + 1);

  for (i = 0; i < nlin * nmaxln; i++)
     clegbf[i] = ' ';
  clegbf[nlin*nmaxln] = '\0';

  ilegop = 1;
  legini (clegbf, nlin, nmaxln);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_leglin (JNIEnv *env, jobject object, jstring s1, 
                    jstring s2, jint ilin)
{ const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  leglin (clegbf, (char *) str2, ilin);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_legopt (JNIEnv *env, jobject object, jfloat x, jfloat y, jfloat z)
{
  legopt (x, y, z);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_legpat (JNIEnv *env, jobject object, jint ityp, jint ithk,
                    jint isym, jint ipat, jint iclr, jint ilin)
{
  legpat (ityp, ithk, isym, ipat, iclr, ilin);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_legpos (JNIEnv *env, jobject object, jint i, jint j)
{
  legpos (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_legsel (JNIEnv *env, jobject object, jintArray arr1, jint n)
{ jint *nxray = (*env)->GetIntArrayElements(env, arr1, 0);
  legsel ((int *) nxray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, nxray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_legtit (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  legtit ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_legtyp (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  legtyp ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_legval (JNIEnv *env, jobject object, jfloat x, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  legval (x, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_lfttit (JNIEnv *env, jobject object)
{
  lfttit ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_licmod (JNIEnv *env, jobject object, jstring s1, jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  licmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_licpts___3D_3DII_3I_3I_3D 
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jint nx, jint ny,
        jintArray arr3, jintArray arr4,  jdoubleArray arr5)
{ jdouble *xv = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yv = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jint *itmat = (*env)->GetIntArrayElements(env, arr3, 0);
  jint *iwmat = (*env)->GetIntArrayElements(env, arr4, 0);
  jdouble *wmat = (*env)->GetDoubleArrayElements(env, arr5, 0);
  licpts ((double *) xv, (double *) yv, nx, ny, (int *) itmat, (int *) iwmat,
          (double *) wmat);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yv, 0);
  (*env)->ReleaseIntArrayElements(env, arr3, itmat, 0);
  (*env)->ReleaseIntArrayElements(env, arr4, iwmat, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr5, wmat, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_licpts___3F_3FII_3I_3I_3F 
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jint nx, jint ny,
        jintArray arr3, jintArray arr4, jfloatArray arr5)
{ double *p1 = dbl_array (env, arr1, nx * ny);
  double *p2 = dbl_array (env, arr2, nx * ny);
  double *p3 = dbl_array (env, arr5, nx * ny);
  jint *itmat = (*env)->GetIntArrayElements(env, arr3, 0);
  jint *iwmat = (*env)->GetIntArrayElements(env, arr4, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
  { licpts (p1, p2, nx, ny, (int *) itmat, (int *) iwmat, p3);
    copy_dblarray (env, arr5, p3, nx * ny);
  }
  free (p1);
  free (p2);
  free (p3);
  (*env)->ReleaseIntArrayElements(env, arr3, itmat, 0);
  (*env)->ReleaseIntArrayElements(env, arr4, iwmat, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_light (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  light ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_linclr (JNIEnv *env, jobject object, jintArray arr1, 
        jint n)
{ jint *nxray = (*env)->GetIntArrayElements(env, arr1, 0);
  linclr ((int *) nxray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, nxray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_lincyc (JNIEnv *env, jobject object, jint i, jint j)
{ lincyc (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_line (JNIEnv *env, jobject object, jint nx, jint ny,
        jint nu, jint nv)
{ line (nx, ny, nu, nv);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_linesp (JNIEnv *env, jobject object, jfloat x)
{ linesp (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_linmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  linmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_lintyp (JNIEnv *env, jobject object, jint i)
{ lintyp (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_linwid (JNIEnv *env, jobject object, jint i)
{ linwid (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_litmod (JNIEnv *env, jobject object, jint id, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  litmod (id, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_litop3 (JNIEnv *env, jobject object, jint id, 
        jfloat xr, jfloat xg, jfloat xb, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  litop3 (id, xr, xg, xb, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_litopt (JNIEnv *env, jobject object, jint id, 
        jfloat x, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  litopt (id, x, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_litpos (JNIEnv *env, jobject object, jint id, 
        jfloat x, jfloat y, jfloat z, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  litpos (id, x, y, z, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_lncap (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  lncap ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_lnjoin (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  lnjoin ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_lnmlt (JNIEnv *env, jobject object, jfloat x)
{ lnmlt (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_logtic (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  logtic ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_mapbas (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  mapbas ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_mapfil (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  mapfil ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_mapimg (JNIEnv *env, jobject object, jstring s1, 
	jfloat x1, jfloat x2, jfloat x3, 
        jfloat x4, jfloat x5, jfloat x6) 
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  mapimg ((char *) str1, x1, x2, x3, x4, x5, x6);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_maplab (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  maplab ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_maplev (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  maplev ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_mapmod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  mapmod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_mapopt (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  mapopt ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mappol (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ mappol (x, y);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mapref (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ mapref (x, y);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mapsph (JNIEnv *env, jobject object, jfloat x)
{ mapsph (x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_marker (JNIEnv *env, jobject object, jint i)
{ marker (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_matop3 (JNIEnv *env, jobject object, 
        jfloat xr, jfloat xg, jfloat xb, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  matop3 (xr, xg, xb, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_matopt (JNIEnv *env, jobject object, jfloat x, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  matopt (x, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mdfmat (JNIEnv *env, jobject object, jint i, jint j, 
        jfloat x)
{ mdfmat (i, j, x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_messag (JNIEnv *env, jobject object, jstring s, 
        jint i, jint j)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  messag ((char *) str, i, j);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_metafl (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  metafl ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_mixalf (JNIEnv *env, jobject object)
{
  mixalf ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_mixleg (JNIEnv *env, jobject object)
{
  mixleg ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_mpaepl (JNIEnv *env, jobject object, jint i)
{
  mpaepl (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_mplang (JNIEnv *env, jobject object, jfloat x)
{ mplang (x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mplclr (JNIEnv *env, jobject object, jint i, jint j)
{
  mplclr (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mplpos (JNIEnv *env, jobject object, jint i, jint j)
{
  mplpos (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mplsiz (JNIEnv *env, jobject object, jint i)
{
  mplsiz (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mpslogo (JNIEnv *env, jobject object, jint i1, 
                jint i2, jint i3, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  mpslogo (i1, i2, i3, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);

}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mrkclr (JNIEnv *env, jobject object, jint i)
{
  mrkclr (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_msgbox (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  msgbox ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mshclr (JNIEnv *env, jobject object, jint i)
{
  mshclr (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mshcrv (JNIEnv *env, jobject object, jint i)
{
  mshcrv (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_mylab (JNIEnv *env, jobject object, jstring s1, jint i, 
                    jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  mylab ((char *) str1, i, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_myline (JNIEnv *env, jobject object, jintArray arr1, jint n)
{ jint *nxray = (*env)->GetIntArrayElements(env, arr1, 0);
  myline ((int *) nxray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, nxray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mypat (JNIEnv *env, jobject object, jint nx, jint ny, 
                                                 jint nw, jint nh)
{
  mypat (nx, ny, nw, nh);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mysymb___3D_3DIII (JNIEnv *env, jobject object, 
                   jdoubleArray arr1, jdoubleArray arr2, jint n, jint isym, 
                   jint iflag)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  mysymb ((double *) xray, (double *) yray, n, isym, iflag);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_mysymb___3F_3FIII (JNIEnv *env, jobject object, 
                   jfloatArray arr1, jfloatArray arr2, jint n, jint isym, 
                   jint iflag)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  if (p1 != NULL && p2 != NULL)
    mysymb (p1, p2, n, isym, iflag);
  free (p1);
  free (p2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_myvlt___3D_3D_3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3, jint n)
{
  jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  myvlt ((double *) xray, (double *) yray, (double *) zray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_myvlt___3F_3F_3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    myvlt (p1, p2, p3, n);
  free (p1);
  free (p2);
  free (p3);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_namdis (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  namdis (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_name (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  name ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_namjus (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  namjus ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_nancrv (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  nancrv ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_neglog (JNIEnv *env, jobject object, jfloat x)
{ neglog (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_newmix (JNIEnv *env, jobject object)
{ newmix ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_newpag (JNIEnv *env, jobject object)
{ newpag ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_nlmess (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int nl;
  nl = nlmess ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return nl;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_nlnumb (JNIEnv *env, jobject object, jfloat x, jint i)
{ return nlnumb (x, i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_noarln (JNIEnv *env, jobject object)
{ noarln ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_nobar (JNIEnv *env, jobject object)
{ nobar ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_nobgd (JNIEnv *env, jobject object)
{ nobgd ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_nochek (JNIEnv *env, jobject object)
{ nochek ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_noclip (JNIEnv *env, jobject object)
{ noclip ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_nofill (JNIEnv *env, jobject object)
{ nofill ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_nograf (JNIEnv *env, jobject object)
{ nograf ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_nohide (JNIEnv *env, jobject object)
{ nohide ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_noline (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  noline ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_number (JNIEnv *env, jobject object, jfloat x, jint n, 
        jint i, jint j)
{ number (x, n, i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_numfmt (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  numfmt ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_numode (JNIEnv *env, jobject object, jstring s1, 
        jstring s2, jstring s3, jstring s4)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  const char *str3 = (*env)->GetStringUTFChars(env, s3, 0);
  const char *str4 = (*env)->GetStringUTFChars(env, s4, 0);
  numode ((char *) str1, (char *) str2, (char *) str3, (char *) str4);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  (*env)->ReleaseStringUTFChars(env, s3, str3);
  (*env)->ReleaseStringUTFChars(env, s4, str4);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_nwkday (JNIEnv *env, jobject object, jint id, 
        jint im, jint iy)
{ return nwkday (id, im, iy);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_nxlegn (JNIEnv *env, jobject object, jstring s)
{ return nxlegn (clegbf);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_nxpixl (JNIEnv *env, jobject object, jint ix,
        jint iy)
{ return nxpixl (ix, iy);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_nxposn (JNIEnv *env, jobject object, jfloat x)
{ return nxposn (x);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_nylegn (JNIEnv *env, jobject object, jstring s)
{ return nylegn (clegbf);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_nypixl (JNIEnv *env, jobject object, jint ix,
        jint iy)
{ return nypixl (ix, iy);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_nyposn (JNIEnv *env, jobject object, jfloat x)
{ return nyposn (x);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_nzposn (JNIEnv *env, jobject object, jfloat x)
{ return nzposn (x);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_openflfl (JNIEnv *env, jobject object, jstring s, 
        jint nu, jint irw)
{ int iret;
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  iret = openfl ((char *) str, nu, irw);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return iret;
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_opnwin (JNIEnv *env, jobject object, jint i)
{ opnwin (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_origin (JNIEnv *env, jobject object, jint i, jint j)
{ origin (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_page (JNIEnv *env, jobject object, jint i, jint j)
{ page (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_pagera (JNIEnv *env, jobject object)
{ pagera ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_pagfll (JNIEnv *env, jobject object, jint i)
{ pagfll (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_paghdr (JNIEnv *env, jobject object, jstring s1, 
        jstring s2, jint i, jint j)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  paghdr ((char *) str1, (char *) str2, i, j);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_pagmod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  pagmod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_pagorg (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  pagorg ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_pagwin (JNIEnv *env, jobject object, jint i, jint j)
{ pagwin (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_patcyc (JNIEnv *env, jobject object, jint i, jint j)
{ patcyc (i, j);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_pdfbuf (JNIEnv *env, jobject object, jbyteArray arr1, 
        jint n)
{ int iret;
  jbyte *cbuf = (*env)->GetByteArrayElements(env, arr1, 0);
  iret = pdfbuf ((char * ) cbuf, n);
  (*env)->ReleaseByteArrayElements(env, arr1, cbuf, 0);
  return iret;
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_pdfmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  pdfmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_pdfmrk (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  pdfmrk ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_penwid (JNIEnv *env, jobject object, jfloat x)
{ penwid (x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_pie (JNIEnv *env, jobject object, jint nxm, jint nym,
        jint nr, jfloat a, jfloat b)
{ pie (nxm, nym, nr, a, b);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_piebor (JNIEnv *env, jobject object, jint n)
{ piebor (n);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_pieclr (JNIEnv *env, jobject object, jintArray arr1, 
        jintArray arr2, jint n)
{ jint *nxray = (*env)->GetIntArrayElements(env, arr1, 0);
  jint *nyray = (*env)->GetIntArrayElements(env, arr2, 0);
  pieclr ((int *) nxray, (int *) nyray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, nxray, 0);
  (*env)->ReleaseIntArrayElements(env, arr2, nyray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_pieexp (JNIEnv *env, jobject object)
{ pieexp ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_piegrf__Ljava_lang_String_2I_3DI 
       (JNIEnv *env, jobject object, jstring s, jint nlin,
        jdoubleArray arr1, jint nseg)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  if (nlin == 0)
    piegrf (" ", nlin, (double *) xray, nseg);
  else 
    piegrf (clegbf, nlin, (double *) xray, nseg);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_piegrf__Ljava_lang_String_2I_3FI 
       (JNIEnv *env, jobject object, jstring s, jint nlin,
        jfloatArray arr1, jint nseg)
{ double *p1 = dbl_array (env, arr1, nseg);
  if (p1 != NULL)  
  { if (nlin == 0)
      piegrf (" ", nlin, p1, nseg);
    else 
      piegrf (clegbf, nlin, p1, nseg);
  }
  free (p1);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_pielab (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  pielab ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_pieopt (JNIEnv *env, jobject object, jfloat x1, 
        jfloat x2)
{ pieopt (x1, x2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_pierot (JNIEnv *env, jobject object, jfloat x)
{ pierot (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_pietyp (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  pietyp ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_pieval (JNIEnv *env, jobject object, jfloat x, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  pieval (x, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_pievec (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  pievec (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_pike3d (JNIEnv *env, jobject object, 
        jfloat x1, jfloat y1, jfloat z1, 
        jfloat x2, jfloat y2, jfloat z2, jfloat r, jint n, jint m)
{ pike3d (x1, y1, z1, x2, y2, z2, r, n, m);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_plat3d (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat z, jfloat xl, jstring s1)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  plat3d (x, y, z, xl, (char *) str1);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_plyfin (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  plyfin ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_plyini (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  plyini ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_pngmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  pngmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_point (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh, jint ncol)
{ point (nx, ny, nw, nh, ncol);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_polar (JNIEnv *env, jobject object, 
        jfloat xe, jfloat xor, jfloat xstp, jfloat yor, jfloat ystp)
{ grafp (xe, xor, xstp, yor, ystp);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_grafp (JNIEnv *env, jobject object, 
        jfloat xe, jfloat xor, jfloat xstp,
        jfloat yor, jfloat ystp)
{ grafp (xe, xor, xstp, yor, ystp);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_polclp___3D_3DI_3D_3DIFLjava_lang_String_2
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jint n,
        jdoubleArray arr3, jdoubleArray arr4,
        jint nmax, jfloat xv, jstring s1)
{ int nout;
  jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *xout = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *yout = (*env)->GetDoubleArrayElements(env, arr4, 0);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  nout = polclp ((double *) xray, (double *) yray, n, 
          (double *) xout, (double *) yout, nmax, xv, (char *) str1);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, xout, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, yout, 0);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  return nout;
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_polclp___3F_3FI_3F_3FIFLjava_lang_String_2
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jint n,
        jfloatArray arr3, jfloatArray arr4,
        jint nmax, jfloat xv, jstring s1)
{ int nout;
  double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, nmax);
  double *p4 = dbl_array (env, arr4, nmax);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  if (p1 != 0 && p2 != NULL && p3 != NULL && p4 != NULL)
  { nout = polclp (p1, p2, n, p3, p4, nmax, xv, (char *) str1);
    copy_dblarray (env, arr3, p3, nmax);
    copy_dblarray (env, arr4, p4, nmax);
  }
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  return nout;
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_polcrv (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  polcrv ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_polmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  polmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_pos2pt (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloatArray arr1)
{ double xp, yp;
  jfloat *xray = (*env)->GetFloatArrayElements(env, arr1, 0);
  pos2pt ((double) x, (double) y, &xp, &yp);
  xray[0] = (jfloat) xp;
  xray[1] = (jfloat) yp;
  (*env)->ReleaseFloatArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_pos3pt (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat z, jfloatArray arr1)
{ double xp, yp, zp;
  jfloat *xray = (*env)->GetFloatArrayElements(env, arr1, 0);
  pos3pt ((double) x, (double) y, (double) z, &xp, &yp, &zp);
  xray[0] = (jfloat) xp;
  xray[1] = (jfloat) yp;
  xray[2] = (jfloat) zp;
  (*env)->ReleaseFloatArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_posbar (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  posbar ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_posifl (JNIEnv *env, jobject object, jint i, jint j)
{ return posifl (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_proj3d (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  proj3d ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_projct (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  projct ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_psfont (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  psfont ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_psmode (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  psmode ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_pt2pos (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloatArray arr1)
{ double xp, yp;
  jfloat *xray = (*env)->GetFloatArrayElements(env, arr1, 0);
  pt2pos ((double) x, (double) y, &xp, &yp);
  xray[0] = (jfloat) xp;
  xray[1] = (jfloat) yp;
  (*env)->ReleaseFloatArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_pyra3d (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat z, jfloat xl, jfloat h1, jfloat h2, jint n)
{ pyra3d (x, y, z, xl, h1, h2, n);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplbar___3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  qplbar ((double *) xray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplbar___3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  if (p1 != NULL)
    qplbar (p1, n);
  free (p1);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplclr___3DII (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint nx, jint ny)
{ jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr1, 0);
  qplclr ((double *) zmat, nx, ny);
  (*env)->ReleaseDoubleArrayElements(env, arr1, zmat, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplclr___3FII (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint nx, jint ny)
{ double *p1 = dbl_array (env, arr1, nx * ny);
  if (p1 != NULL)
     qplclr (p1, nx, ny);
  free (p1);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplcon___3DIII (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint nx, jint ny, jint nlev)
{ jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr1, 0);
  qplcon ((double *) zmat, nx, ny, nlev);
  (*env)->ReleaseDoubleArrayElements(env, arr1, zmat, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplcon___3FIII (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint nx, jint ny, jint nlev)
{ double *p1 = dbl_array (env, arr1, nx * ny);
  if (p1 != NULL)
    qplcon (p1, nx, ny, nlev);
  free (p1);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplcrv___3D_3DILjava_lang_String_2
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jint n, jstring s)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  qplcrv ((double *) xray, (double *) yray, n, (char *) str);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplcrv___3F_3FILjava_lang_String_2 
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jint n, jstring s)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  if (p1 != NULL && p2 != NULL)
    qplcrv (p1, p2, n, (char *) str);
  free (p1);
  free (p2);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplot___3D_3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  qplot ((double *) xray, (double *) yray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplot___3F_3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  if (p1 != NULL && p2 != NULL)
    qplot (p1, p2, n);
  free (p1);
  free (p2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplpie___3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  qplpie ((double *) xray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplpie___3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  if (p1 != NULL)
    qplpie (p1, n);
  free (p1);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplsca___3D_3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  qplsca ((double *) xray, (double *) yray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplsca___3F_3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  if (p1 != NULL && p2 != NULL)
    qplsca (p1, p2, n);
  free (p1);
  free (p2); 
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_qplscl (JNIEnv *env, jobject object, jfloat a, jfloat b,
        jfloat or, jfloat step, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  qplscl (a, b, or, step, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplsur___3DII (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint nx, jint ny)
{ jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr1, 0);
  qplsur ((double *) zmat, nx, ny);
  (*env)->ReleaseDoubleArrayElements(env, arr1, zmat, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_qplsur___3FII (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint nx, jint ny)
{ double *p1 = dbl_array (env, arr1, nx * ny);
  if (p1 != NULL)
    qplsur (p1, nx, ny);
  free (p1);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_quad3d (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat z, jfloat xl, jfloat yl, jfloat zl)
{ quad3d (x, y, z, xl, yl, zl);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_rbfpng (JNIEnv *env, jobject object, jbyteArray arr1, 
        jint n)
{ int iret;
  jbyte *cbuf = (*env)->GetByteArrayElements(env, arr1, 0);
  iret = rbfpng ((char * ) cbuf, n);
  (*env)->ReleaseByteArrayElements(env, arr1, cbuf, 0);
  return iret;
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_rbmp (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  rbmp ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_readfl (JNIEnv *env, jobject object, jint nu, 
        jbyteArray arr1, jint n)
{ int iret;
  jbyte *cbuf = (*env)->GetByteArrayElements(env, arr1, 0);
  iret = readfl (nu, (unsigned char * ) cbuf, n);
  (*env)->ReleaseByteArrayElements(env, arr1, cbuf, 0);
  return iret;
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_reawgt (JNIEnv *env, jobject object)
{ reawgt ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_recfll (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh, jint ncol)
{ recfll (nx, ny, nw, nh, ncol);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rectan (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh)
{ rectan (nx, ny, nw, nh);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rel3pt (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat z, jfloatArray arr1)
{ double xp, yp;
  jfloat *xray = (*env)->GetFloatArrayElements(env, arr1, 0);
  rel3pt ((double) x, (double) y, (double) z, &xp, &yp);
  xray[0] = (float) xp;
  xray[1] = (float) yp;
  (*env)->ReleaseFloatArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_resatt (JNIEnv *env, jobject object)
{ resatt ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_reset (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  reset ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_revsrc (JNIEnv *env, jobject object)
{ revscr ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rgbhsv (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat z, jfloatArray arr1)
{ double x1, x2, x3;
  jfloat *xray = (*env)->GetFloatArrayElements(env, arr1, 0);
  rgbhsv (x, y, z, &x1, &x2, &x3);
  xray[0] = (jfloat) x1;
  xray[1] = (jfloat) x2;
  xray[2] = (jfloat) x3;
  (*env)->ReleaseFloatArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_rgif (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  rgif ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_rgtlab (JNIEnv *env, jobject object)
{ rgtlab ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_rimage (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  rimage ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlarc (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat xa, jfloat xb, jfloat a, jfloat b, jfloat t)
{ rlarc (x, y, xa, xb, a, b, t);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlarea___3D_3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  rlarea ((double *) xray, (double *) yray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlarea___3F_3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  if (p1 != NULL && p2 != NULL)
    rlarea (p1, p2, n);
  free (p1);
  free (p2);
}


JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlcirc (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat r)
{ rlcirc (x, y, r);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlconn (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ rlconn (x, y);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlell (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat a, jfloat b)
{ rlell (x, y, a, b);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rline (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat u, jfloat v)
{ rline (x, y, u, v);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_rlmess (JNIEnv *env, jobject object, jstring s, 
        jfloat x, jfloat y)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  rlmess ((char *) str, x, y);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlnumb (JNIEnv *env, jobject object, jfloat x, jint ndig,
        jfloat xp, jfloat yp)
{ rlnumb (x, ndig, xp, yp);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlpie (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat r, jfloat a, jfloat b)
{ rlpie (x, y, r, a, b);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlpoin (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jint nb, jint nh, jint ncol)
{ rlpoin (x, y, nb, nh, ncol);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlrec (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat xw, jfloat xh)
{ rlrec (x, y, xw, xh);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlrnd (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat xw, jfloat xh, jint i)
{ rlrnd (x, y, xw, xh, i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlsec (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat r1, jfloat r2, jfloat a, jfloat b, jint i)
{ rlsec (x, y, r1, r2, a, b, i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlstrt (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ rlstrt (x, y);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlsymb (JNIEnv *env, jobject object, jint i, jfloat x,
        jfloat y)
{ rlsymb (i, x, y);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlvec (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat u, jfloat v, jint i)
{ rlvec (x, y, u, v, i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rlwind (JNIEnv *env, jobject object, jfloat x, jfloat xp,
        jfloat yp, jint n, jfloat a)
{ rlwind (x, xp, yp, n, a);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rndrec (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh, jint irnd)
{ rndrec (nx, ny, nw, nh, irnd);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rot3d (JNIEnv *env, jobject object, jfloat x,
        jfloat y, jfloat z)
{ rot3d (x, y, z);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_rpixel (JNIEnv *env, jobject object, jint i, jint j)
{ int iret;
  rpixel (i, j, &iret);
  return iret;
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rpixls (JNIEnv *env, jobject object, jbyteArray arr1, 
        jint ix, jint iy, jint nw, jint nh)
{ jbyte *cbuf = (*env)->GetByteArrayElements(env, arr1, 0);
  rpixls ((unsigned char * ) cbuf, ix, iy, nw, nh);
  (*env)->ReleaseByteArrayElements(env, arr1, cbuf, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_rpng (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  rpng ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_rppm (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  rppm ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_rpxrow (JNIEnv *env, jobject object, jbyteArray arr1, 
        jint ix, jint iy, jint n)
{ jbyte *cbuf = (*env)->GetByteArrayElements(env, arr1, 0);
  rpxrow ((unsigned char * ) cbuf, ix, iy, n);
  (*env)->ReleaseByteArrayElements(env, arr1, cbuf, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_rtiff (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  rtiff ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_rvynam (JNIEnv *env, jobject object)
{ rvynam ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_scale (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  scale ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_sclfac (JNIEnv *env, jobject object, jfloat x)
{ sclfac (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_sclmod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  sclmod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_scrmod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  scrmod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_sector (JNIEnv *env, jobject object, jint nx, jint ny,
        jint nr1, jint nr2, jfloat a, jfloat b, jint ncol)
{ sector (nx, ny, nr1, nr2, a, b, ncol);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_selwin (JNIEnv *env, jobject object, jint i)
{ selwin (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_sendbf (JNIEnv *env, jobject object)
{ sendbf ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_sendmb (JNIEnv *env, jobject object)
{ sendmb ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_sendok (JNIEnv *env, jobject object)
{ sendok ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_serif (JNIEnv *env, jobject object)
{ serif ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_setbas (JNIEnv *env, jobject object, jfloat x)
{ setbas (x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_setclr (JNIEnv *env, jobject object, jint i)
{ setclr (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_setcsr (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  setcsr ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_setexp (JNIEnv *env, jobject object, jfloat x)
{ setexp (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_setfce (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  setfce ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_setfil (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  setfil ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_setgrf (JNIEnv *env, jobject object, jstring s1, 
        jstring s2, jstring s3, jstring s4)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  const char *str3 = (*env)->GetStringUTFChars(env, s3, 0);
  const char *str4 = (*env)->GetStringUTFChars(env, s4, 0);
  setgrf ((char *) str1, (char *) str2, (char *) str3, (char *) str4);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  (*env)->ReleaseStringUTFChars(env, s3, str3);
  (*env)->ReleaseStringUTFChars(env, s4, str4);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_setind (JNIEnv *env, jobject object, jint i, jfloat xr,
        jfloat xg, jfloat xb)
{ setind (i, xr, xg, xb);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_setmix (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  setmix ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_setpag (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  setpag ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_setres (JNIEnv *env, jobject object, jint i, jint j)
{ setres (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_setrgb (JNIEnv *env, jobject object, jfloat r, jfloat g, 
        jfloat b)
{ setrgb (r, g, b);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_setscl___3DILjava_lang_String_2
       (JNIEnv *env, jobject object, jdoubleArray arr1, jint n, jstring s)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  setscl ((double *) xray, n, (char *) str);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_setscl___3FILjava_lang_String_2
       (JNIEnv *env, jobject object, jfloatArray arr1, jint n, jstring s)
{ double *p1 = dbl_array (env, arr1, n);
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  if (p1 != NULL)
    setscl (p1, n, (char *) str);
  free (p1);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_setvlt (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  setvlt ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_setxid (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  setxid (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shdafr (JNIEnv *env, jobject object, jintArray arr1, 
        jintArray arr2, jintArray arr3, jint n)
{ jint *inray = (*env)->GetIntArrayElements(env, arr1, 0);
  jint *ipray = (*env)->GetIntArrayElements(env, arr2, 0);
  jint *icray = (*env)->GetIntArrayElements(env, arr2, 0);
  long *p;
  int i;
  p = (long *) calloc (n, sizeof (long));
  for (i = 0; i < n; i++)
    p[i] = ipray[i];
  shdafr ((int *) inray, p, (int *) icray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, inray, 0);
  (*env)->ReleaseIntArrayElements(env, arr2, ipray, 0);
  (*env)->ReleaseIntArrayElements(env, arr3, icray, 0);
  free (p);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shdasi (JNIEnv *env, jobject object, jintArray arr1, 
        jintArray arr2, jintArray arr3, jint n)
{ jint *inray = (*env)->GetIntArrayElements(env, arr1, 0);
  jint *ipray = (*env)->GetIntArrayElements(env, arr2, 0);
  jint *icray = (*env)->GetIntArrayElements(env, arr2, 0);
  long *p;
  int i;
  p = (long *) calloc (n, sizeof (long));
  for (i = 0; i < n; i++)
    p[i] = ipray[i];
  shdasi ((int *) inray, p, (int *) icray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, inray, 0);
  (*env)->ReleaseIntArrayElements(env, arr2, ipray, 0);
  (*env)->ReleaseIntArrayElements(env, arr3, icray, 0);
  free (p);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shdaus (JNIEnv *env, jobject object, jintArray arr1, 
        jintArray arr2, jintArray arr3, jint n)
{ jint *inray = (*env)->GetIntArrayElements(env, arr1, 0);
  jint *ipray = (*env)->GetIntArrayElements(env, arr2, 0);
  jint *icray = (*env)->GetIntArrayElements(env, arr2, 0);
  long *p;
  int i;
  p = (long *) calloc (n, sizeof (long));
  for (i = 0; i < n; i++)
    p[i] = ipray[i];
  shdaus ((int *) inray, p, (int *) icray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, inray, 0);
  (*env)->ReleaseIntArrayElements(env, arr2, ipray, 0);
  (*env)->ReleaseIntArrayElements(env, arr3, icray, 0);
  free (p);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_shdcha (JNIEnv *env, jobject object)
{ shdcha ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shdcrv___3D_3DI_3D_3DI
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jint n1,
        jdoubleArray arr3, jdoubleArray arr4, jint n2)
{ jdouble *x1ray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *y1ray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *x2ray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *y2ray = (*env)->GetDoubleArrayElements(env, arr4, 0);
  shdcrv ((double *) x1ray, (double *) y1ray, n1,
          (double *) x2ray, (double *) y2ray, n2);
  (*env)->ReleaseDoubleArrayElements(env, arr1, x1ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, y1ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, x2ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, y2ray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shdcrv___3F_3FI_3F_3FI
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jint n1,
        jfloatArray arr3, jfloatArray arr4, jint n2)
{ double *p1 = dbl_array (env, arr1, n1);
  double *p2 = dbl_array (env, arr2, n1);
  double *p3 = dbl_array (env, arr3, n2);
  double *p4 = dbl_array (env, arr4, n2);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL)
    shdcrv (p1, p2, n1, p3, p4, n2);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shdeur (JNIEnv *env, jobject object, jintArray arr1, 
        jintArray arr2, jintArray arr3, jint n)
{ jint *inray = (*env)->GetIntArrayElements(env, arr1, 0);
  jint *ipray = (*env)->GetIntArrayElements(env, arr2, 0);
  jint *icray = (*env)->GetIntArrayElements(env, arr2, 0);
  long *p;
  int i;
  p = (long *) calloc (n, sizeof (long));
  for (i = 0; i < n; i++)
    p[i] = ipray[i];
  shdeur ((int *) inray, p, (int *) icray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, inray, 0);
  (*env)->ReleaseIntArrayElements(env, arr2, ipray, 0);
  (*env)->ReleaseIntArrayElements(env, arr3, icray, 0);
  free (p);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_shdfac (JNIEnv *env, jobject object, jfloat x)
{ shdfac (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_shdmap (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  shdmap ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_shdmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  shdmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shdnor (JNIEnv *env, jobject object, jintArray arr1, 
        jintArray arr2, jintArray arr3, jint n)
{ jint *inray = (*env)->GetIntArrayElements(env, arr1, 0);
  jint *ipray = (*env)->GetIntArrayElements(env, arr2, 0);
  jint *icray = (*env)->GetIntArrayElements(env, arr2, 0);
  long *p;
  int i;
  p = (long *) calloc (n, sizeof (long));
  for (i = 0; i < n; i++)
    p[i] = ipray[i];
  shdnor ((int *) inray, p, (int *) icray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, inray, 0);
  (*env)->ReleaseIntArrayElements(env, arr2, ipray, 0);
  (*env)->ReleaseIntArrayElements(env, arr3, icray, 0);
  free (p);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shdpat (JNIEnv *env, jobject object, jint i)
{ shdpat ((long) i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shdsou (JNIEnv *env, jobject object, jintArray arr1, 
        jintArray arr2, jintArray arr3, jint n)
{ jint *inray = (*env)->GetIntArrayElements(env, arr1, 0);
  jint *ipray = (*env)->GetIntArrayElements(env, arr2, 0);
  jint *icray = (*env)->GetIntArrayElements(env, arr2, 0);
  long *p;
  int i;
  p = (long *) calloc (n, sizeof (long));
  for (i = 0; i < n; i++)
    p[i] = ipray[i];
  shdsou ((int *) inray, p, (int *) icray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, inray, 0);
  (*env)->ReleaseIntArrayElements(env, arr2, ipray, 0);
  (*env)->ReleaseIntArrayElements(env, arr3, icray, 0);
  free (p);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shdusa (JNIEnv *env, jobject object, jintArray arr1, 
        jintArray arr2, jintArray arr3, jint n)
{ jint *inray = (*env)->GetIntArrayElements(env, arr1, 0);
  jint *ipray = (*env)->GetIntArrayElements(env, arr2, 0);
  jint *icray = (*env)->GetIntArrayElements(env, arr2, 0);
  long *p;
  int i;
  p = (long *) calloc (n, sizeof (long));
  for (i = 0; i < n; i++)
    p[i] = ipray[i];
  shdusa ((int *) inray, p, (int *) icray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, inray, 0);
  (*env)->ReleaseIntArrayElements(env, arr2, ipray, 0);
  (*env)->ReleaseIntArrayElements(env, arr3, icray, 0);
  free (p);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_shield (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  shield ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shlcir (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nr)
{ shlcir (nx, ny, nr);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shldel (JNIEnv *env, jobject object, jint i)
{ shldel (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shlell (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint na, jint nb, jfloat t)
{ shlell (nx, ny, na, nb, t);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_shlind (JNIEnv *env, jobject object)
{ return shlind ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shlpie (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nr, jfloat a, jfloat b)
{ shlpie (nx, ny, nr, a, b);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shlpol (JNIEnv *env, jobject object, jintArray arr1, 
        jintArray arr2, jint n)
{ jint *nxray = (*env)->GetIntArrayElements(env, arr1, 0);
  jint *nyray = (*env)->GetIntArrayElements(env, arr2, 0);
  shlpol ((int *) nxray, (int *) nyray, n);
  (*env)->ReleaseIntArrayElements(env, arr1, nxray, 0);
  (*env)->ReleaseIntArrayElements(env, arr2, nyray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shlrct (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh, jfloat t)
{ shlrct (nx, ny, nw, nh, t);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shlrec (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh)
{ shlrec (nx, ny, nw, nh);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_shlres (JNIEnv *env, jobject object, jint i)
{ shlres (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_shlsur (JNIEnv *env, jobject object)
{ shlsur ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_shlvis (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  shlvis (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_simplx (JNIEnv *env, jobject object)
{ simplx ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_skipfl (JNIEnv *env, jobject object, jint i, jint j)
{ return skipfl (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_smxalf (JNIEnv *env, jobject object, jstring s1, 
        jstring s2, jstring s3, jint i)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  const char *str3 = (*env)->GetStringUTFChars(env, s3, 0);
  smxalf ((char *) str1, (char *) str2, (char *) str3, i);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  (*env)->ReleaseStringUTFChars(env, s3, str3);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_solid (JNIEnv *env, jobject object)
{ solid ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_sortr1___3DILjava_lang_String_2 
       (JNIEnv *env, jobject object, jdoubleArray arr1, jint n, jstring s)
{ jdouble *xray  = (*env)->GetDoubleArrayElements(env, arr1, 0);
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  sortr1 ((double *) xray, n, (char *) str);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_sortr1
       (JNIEnv *env, jobject object, jfloatArray arr1, jint n, jstring s)
{ double *p1 = dbl_array (env, arr1, n);
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  if (p1 != NULL)
  { sortr1 (p1, n, (char *) str);
    copy_dblarray (env, arr1, p1, n);
  }
  free (p1);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_sortr2___3D_3DILjava_lang_String_2
       (JNIEnv *env, jobject object, jdoubleArray arr1, 
        jdoubleArray arr2, jint n, jstring s)
{ jdouble *xray  = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray  = (*env)->GetDoubleArrayElements(env, arr2, 0);
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  sortr2 ((double *) xray, (double *) yray, n, (char *) str);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_sortr2___3F_3FILjava_lang_String_2
       (JNIEnv *env, jobject object, jfloatArray arr1, 
        jfloatArray arr2, jint n, jstring s)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  if (p1 != NULL && p2 != NULL)
  { sortr2 (p1, p2, n, (char *) str);
    copy_dblarray (env, arr1, p1, n);
    copy_dblarray (env, arr2, p2, n);
  }
  free (p1);
  free (p2);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_spcbar (JNIEnv *env, jobject object, jint i)
{ spcbar (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_sphe3d (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat z, jfloat r, jint n, jint m)
{ sphe3d (x, y, z, r, n, m);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_spline___3D_3DI_3D_3D
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jint nray, 
        jdoubleArray arr3, jdoubleArray arr4)
{ int n;
  jdouble *xray  = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray  = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *x2ray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *y2ray = (*env)->GetDoubleArrayElements(env, arr4, 0);
  spline ((double *) xray, (double *) yray, nray,
          (double *) x2ray, (double *) y2ray, &n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, x2ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, y2ray, 0);
  return n;
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_spline___3F_3FI_3F_3F
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jint nray, 
        jfloatArray arr3, jfloatArray arr4)
{ int n;
  double *p1 = dbl_array (env, arr1, nray);
  double *p2 = dbl_array (env, arr2, nray);
  int m1 = (int) (*env)->GetArrayLength (env, arr3);
  int m2 = (int) (*env)->GetArrayLength (env, arr4);
  double *p3 = dbl_array (env, arr3, m1);
  double *p4 = dbl_array (env, arr4, m2);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL)
  { spline (p1, p2, nray, p3, p4, &n);
    copy_dblarray (env, arr3, p3, m1);
    copy_dblarray (env, arr4, p4, m2);
  }
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  return n;
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_splmod (JNIEnv *env, jobject object, jint i, jint j)
{ splmod (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_stmmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  stmmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_stmopt (JNIEnv *env, jobject object, jint n, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  stmopt (n, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_stmpts___3D_3DII_3D_3DFF_3D_3DI
       (JNIEnv *env, jobject object, jdoubleArray arr1, jdoubleArray arr2,
        jint nx, jint ny, jdoubleArray arr3, jdoubleArray arr4, jfloat x0,
        jfloat y0, jdoubleArray arr5, jdoubleArray arr6, jint nmax)
{ int n;
  jdouble *xv = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yv = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *xp = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *yp = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jdouble *xray = (*env)->GetDoubleArrayElements(env, arr5, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr6, 0);
  stmpts ((double *) xv, (double *) yv, nx, ny, (double *) yp, (double *) yp,
          x0, y0, (double *) xray, (double *) yray, nmax, &n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, xp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, yp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr5, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr6, yray, 0);
  return n;
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_stmpts___3F_3FII_3F_3FFF_3F_3FI
       (JNIEnv *env, jobject object, jfloatArray arr1, jfloatArray arr2, 
        jint nx, jint ny, jfloatArray arr3, jfloatArray arr4, jfloat x0,
        jfloat y0,  jfloatArray arr5, jfloatArray arr6, jint nmax)
{ int n;
  int nn = nx * ny;
  double *p1 = dbl_array (env, arr1, nn);
  double *p2 = dbl_array (env, arr2, nn);
  double *p3 = dbl_array (env, arr3, nx);
  double *p4 = dbl_array (env, arr4, ny);
  double *p5 = dbl_array (env, arr5, nmax);
  double *p6 = dbl_array (env, arr6, nmax);
  if (p1 != NULL && p2 != NULL && p3 != NULL && 
      p4 != NULL && p5 != NULL && p6 != NULL)
  { stmpts (p1, p2, nx, ny, p3, p4, x0, y0, p5, p6, nmax, &n);
    copy_dblarray (env, arr5, p5, nmax);
    copy_dblarray (env, arr6, p6, nmax);
  }
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  free (p5);
  free (p6);
  return n;
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_stmpts3d___3D_3D_3DIII_3D_3D_3DFFF_3D_3D_3DI
       (JNIEnv *env, jobject object, jdoubleArray arr1, jdoubleArray arr2,
        jdoubleArray arr3, jint nx, jint ny, jint nz, jdoubleArray arr4, 
        jdoubleArray arr5, jdoubleArray arr6, jfloat x0, jfloat y0, jfloat z0,
        jdoubleArray arr7, jdoubleArray arr8, jdoubleArray arr9, jint nmax)
{ int n;
  jdouble *xv = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yv = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zv = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *xp = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jdouble *yp = (*env)->GetDoubleArrayElements(env, arr5, 0);
  jdouble *zp = (*env)->GetDoubleArrayElements(env, arr6, 0);
  jdouble *xray = (*env)->GetDoubleArrayElements(env, arr7, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr8, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr9, 0);
  stmpts3d ((double *) xv, (double *) yv, (double *) zv, nx, ny, nz,
	    (double *) xp, (double *) yp, (double *) zp, x0, y0, z0,
            (double *) xray, (double *) yray, (double *) zray, nmax, &n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, xp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr5, yp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr6, zp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr7, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr8, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr9, zray, 0);
  return n;
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_stmpts3d___3F_3F_3FIII_3F_3F_3FFFF_3F_3F_3FI
       (JNIEnv *env, jobject object, jfloatArray arr1, jfloatArray arr2, 
        jfloatArray arr3, jint nx, jint ny, jint nz, jfloatArray arr4,
        jfloatArray arr5, jfloatArray arr6, jfloat x0, jfloat y0, jfloat z0,
        jfloatArray arr7, jfloatArray arr8, jfloatArray arr9, jint nmax)
{ int n;
  int nn = nx * ny * nz;
  double *p1 = dbl_array (env, arr1, nn);
  double *p2 = dbl_array (env, arr2, nn);
  double *p3 = dbl_array (env, arr3, nn);
  double *p4 = dbl_array (env, arr4, nx);
  double *p5 = dbl_array (env, arr5, ny);
  double *p6 = dbl_array (env, arr6, nz);
  double *p7 = dbl_array (env, arr7, nmax);
  double *p8 = dbl_array (env, arr8, nmax);
  double *p9 = dbl_array (env, arr9, nmax);
  if (p1 != NULL && p2 != NULL && p3 != NULL && 
      p4 != NULL && p5 != NULL && p6 != NULL &&
      p7 != NULL && p8 != NULL && p9 != NULL) 
  { stmpts3d (p1, p2, p3, nx, ny, nz, p4, p5, p6, x0, y0, z0,
              p7, p8, p9, nmax, &n);
    copy_dblarray (env, arr7, p7, nmax);
    copy_dblarray (env, arr8, p8, nmax);
    copy_dblarray (env, arr9, p9, nmax);
  }
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  free (p5);
  free (p6);
  free (p7);
  free (p8);
  free (p9);
  return n;
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_stmtri___3D_3D_3D_3DI_3I_3I_3II_3D_3DI
       (JNIEnv *env, jobject object, jdoubleArray arr1, jdoubleArray arr2, 
  	jdoubleArray arr3, jdoubleArray arr4, jint n,
   	jintArray arr5, jintArray arr6, jintArray arr7, jint ntri,
        jdoubleArray arr8, jdoubleArray arr9, jint nray)
{ jdouble *xs = NULL, *ys = NULL;
  jdouble *xv = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yv = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *xp = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *yp = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jint *i1ray = (*env)->GetIntArrayElements(env, arr5, 0);
  jint *i2ray = (*env)->GetIntArrayElements(env, arr6, 0);
  jint *i3ray = (*env)->GetIntArrayElements(env, arr7, 0);
  if (nray > 0)
  { xs = (*env)->GetDoubleArrayElements(env, arr8, 0);
    ys = (*env)->GetDoubleArrayElements(env, arr9, 0);
  }
  stmtri ((double *) xv, (double *) yv, (double *) yp, (double *) yp, n,
          (int *) i1ray, (int *) i2ray, (int *) i3ray, ntri,
          (double *) xs, (double *) ys, nray);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, xp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, yp, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, i1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr6, i2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr7, i3ray, 0);
  if (nray > 0)
  { (*env)->ReleaseDoubleArrayElements(env, arr8, xs, 0);
    (*env)->ReleaseDoubleArrayElements(env, arr9, ys, 0);
  }
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_stmtri___3F_3F_3F_3FI_3I_3I_3II_3F_3FI
       (JNIEnv *env, jobject object, jfloatArray arr1, jfloatArray arr2, 
  	jfloatArray arr3, jfloatArray arr4, jint n,
   	jintArray arr5, jintArray arr6, jintArray arr7, jint ntri,
        jfloatArray arr8, jfloatArray arr9, jint nray)
{ double *p5 = NULL, *p6 = NULL;
  double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr4, n);
  jint *i1ray = (*env)->GetIntArrayElements(env, arr5, 0);
  jint *i2ray = (*env)->GetIntArrayElements(env, arr6, 0);
  jint *i3ray = (*env)->GetIntArrayElements(env, arr7, 0);
  if (nray > 0)
  { p5 = dbl_array (env, arr8, nray);
    p6 = dbl_array (env, arr9, nray);
  }
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && 
      (nray == 0 || (p5 != NULL && p6 != NULL))) 
    stmtri (p1, p2, p3, p4, n, (int *) i1ray, (int *) i2ray, (int *) i3ray, 
            ntri, p5, p6, nray);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  if (nray > 0)
  { free (p5);
    free (p6);
  }
  (*env)->ReleaseIntArrayElements(env, arr5, i1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr6, i2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr7, i3ray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_stmval (JNIEnv *env, jobject object, jfloat x, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  stmval (x, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_stream___3D_3DII_3D_3D_3D_3DI
       (JNIEnv *env, jobject object, jdoubleArray arr1, jdoubleArray arr2, 
        jint nx, jint ny, jdoubleArray arr3, jdoubleArray arr4, 
        jdoubleArray arr5, jdoubleArray arr6, jint n)
{ jdouble *xv = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yv = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *xp = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *yp = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jdouble *xs = (*env)->GetDoubleArrayElements(env, arr5, 0);
  jdouble *ys = (*env)->GetDoubleArrayElements(env, arr6, 0);
  stream ((double *) xv, (double *) yv, nx, ny, (double *) yp, (double *) yp,
          (double *) xs, (double *) ys, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, xp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, yp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr5, xs, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr6, ys, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_stream___3F_3FII_3F_3F_3F_3FI
       (JNIEnv *env, jobject object, jfloatArray arr1, jfloatArray arr2, 
        jint nx, jint ny, jfloatArray arr3, jfloatArray arr4, 
        jfloatArray arr5, jfloatArray arr6, jint n)
{ double *p5 = NULL, *p6 = NULL;
  int nn = nx * ny;
  double *p1 = dbl_array (env, arr1, nn);
  double *p2 = dbl_array (env, arr2, nn);
  double *p3 = dbl_array (env, arr3, nx);
  double *p4 = dbl_array (env, arr4, ny);
  if (n > 0)
  { p5 = dbl_array (env, arr5, n);
    p6 = dbl_array (env, arr6, n);
  }
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL &&
      (n == 0 || (p5 != NULL && p6 != NULL))) 
    stream (p1, p2, nx, ny, p3, p4, p5, p6, n);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  if (n > 0)
  { free (p5);
    free (p6);
  }
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_stream3d___3D_3D_3DIII_3D_3D_3D_3D_3D_3DI
       (JNIEnv *env, jobject object, jdoubleArray arr1, jdoubleArray arr2, 
        jdoubleArray arr3, jint nx, jint ny, jint nz, jdoubleArray arr4,
        jdoubleArray arr5, jdoubleArray arr6, jdoubleArray arr7, 
        jdoubleArray arr8, jdoubleArray arr9, jint n)
{ jdouble *xv = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yv = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zv = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *xp = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jdouble *yp = (*env)->GetDoubleArrayElements(env, arr5, 0);
  jdouble *zp = (*env)->GetDoubleArrayElements(env, arr6, 0);
  jdouble *xs = (*env)->GetDoubleArrayElements(env, arr7, 0);
  jdouble *ys = (*env)->GetDoubleArrayElements(env, arr8, 0);
  jdouble *zs = (*env)->GetDoubleArrayElements(env, arr9, 0);
  stream3d ((double *) xv, (double *) yv, (double *) zv, nx, ny, nz,
            (double *) yp, (double *) yp, (double *) zp,
            (double *) xs, (double *) ys, (double *) zs, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, xp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr5, yp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr6, zp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr7, xs, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr8, ys, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr9, zs, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_stream3d___3F_3F_3FIII_3F_3F_3F_3F_3F_3FI
       (JNIEnv *env, jobject object, jfloatArray arr1, jfloatArray arr2, 
        jfloatArray arr3, jint nx, jint ny, jint nz, jfloatArray arr4,
        jfloatArray arr5, jfloatArray arr6, jfloatArray arr7, jfloatArray arr8,
        jfloatArray arr9, jint n)
{ double *p7 = NULL, *p8 = NULL, *p9 = NULL;
  int nn = nx * ny * nz;
  double *p1 = dbl_array (env, arr1, nn);
  double *p2 = dbl_array (env, arr2, nn);
  double *p3 = dbl_array (env, arr3, nn);
  double *p4 = dbl_array (env, arr4, nx);
  double *p5 = dbl_array (env, arr5, ny);
  double *p6 = dbl_array (env, arr6, nz);
  if (n > 0)
  { p7 = dbl_array (env, arr7, n);
    p8 = dbl_array (env, arr8, n);
    p9 = dbl_array (env, arr9, n);
  }
  if (p1 != NULL && p2 != NULL && p3 != NULL && 
      p4 != NULL && p5 != NULL && p6 != NULL &&
      (n == 0 || (p7 != NULL && p8 != NULL && p9 != NULL))) 
    stream3d (p1, p2, p3, nx, ny, nz, p4, p5, p6, p7, p8, p9, n);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  free (p5);
  free (p6);
  if (n > 0)
  { free (p7);
    free (p8);
    free (p9);
  }
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_strt3d (JNIEnv *env, jobject object, jfloat x, jfloat y, 
        jfloat z)
{ strt3d (x, y, z);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_strtpt (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ strtpt (x, y);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_surclr (JNIEnv *env, jobject object, jint i, jint j)
{ surclr (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_surfce___3DI_3DI_3D (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint n, jdoubleArray arr2, jint m,
        jdoubleArray arr3)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr3, 0);
  surfce ((double *) xray, n, (double *) yray, m, (double *) zmat);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zmat, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_surfce___3FI_3FI_3F (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint n, jfloatArray arr2, jint m,
        jfloatArray arr3)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, m);
  double *p3 = dbl_array (env, arr3, n * m);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    surfce (p1, n, p2, m, p3);
  free (p1);
  free (p2);
  free (p3);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_surfcp (JNIEnv *env, jobject object, jstring s,
                    jfloat a1, jfloat a2, jfloat astp,
                    jfloat b1, jfloat b2, jfloat bstp)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  char c1[80], c2[80], *q;
  strcpy (cbfunc , str);
  envcbk = env;

  strcpy (c1, cbfunc);
  q = strstr (c1, ".");
  if (q != NULL) 
  { strcpy (c2, q+1);
    *q = '\0';
  }
  else
    return;

  clscbk = (*env)->FindClass (env, c1);
  if (clscbk == NULL) return;
  midcbk = (*env)->GetStaticMethodID (env, clscbk, c2, "(FFI)F");
  if (midcbk == 0) return;

  surfcp (dis_funcbck, a1, a2, astp, b1, b2, bstp);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_surfun (JNIEnv *env, jobject object, jstring s,
        jint ixpts, jfloat xdel, jint iypts, jfloat ydel)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  char c1[80], c2[80], *q;
  strcpy (cbfunc , str);
  envcbk = env;

  strcpy (c1, cbfunc);
  q = strstr (c1, ".");
  if (q != NULL) 
  { strcpy (c2, q+1);
    *q = '\0';
  }
  else
    return;

  clscbk = (*env)->FindClass (env, c1);
  if (clscbk == NULL) return;
  midcbk = (*env)->GetStaticMethodID (env, clscbk, c2, "(FF)F");
  if (midcbk == 0) return;
  surfun (dis_func2bck, ixpts, (double) xdel, iypts, (double) ydel);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_suriso___3DI_3DI_3DI_3DF
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint nx, jdoubleArray arr2, jint ny,
        jdoubleArray arr3, jint nz, jdoubleArray arr4, jfloat wlev)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *wmat = (*env)->GetDoubleArrayElements(env, arr4, 0);
  suriso ((double *) xray, nx, (double *) yray, ny, (double *) zray, nz,
          (double *) wmat, wlev);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, wmat, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_suriso___3FI_3FI_3FI_3FF
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint nx, jfloatArray arr2, jint ny,
        jfloatArray arr3, jint nz, jfloatArray arr4, jfloat wlev)
{ double *p1 = dbl_array (env, arr1, nx);
  double *p2 = dbl_array (env, arr2, ny);
  double *p3 = dbl_array (env, arr3, nz);
  double *p4 = dbl_array (env, arr4, nx * ny * nz);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL)
    suriso (p1, nx, p2, ny, p3, nz, p4, wlev);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_surmat___3DIIII (JNIEnv *env, jobject object, 
                   jdoubleArray arr1, jint nx, jint ny, jint ipts, jint jpts)
{
  jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr1, 0);
  surmat ((double *) zmat, nx, ny, ipts, jpts);
  (*env)->ReleaseDoubleArrayElements(env, arr1, zmat, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_surmat___3FIIII (JNIEnv *env, jobject object, 
                   jfloatArray arr1, jint nx, jint ny, jint ipts, jint jpts)
{ double *p1 = dbl_array (env, arr1, nx * ny);
  if (p1 != NULL)
    surmat (p1, nx, ny, ipts, jpts);
  free (p1);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_surtri___3D_3D_3DI_3I_3I_3II
       (JNIEnv *env, jobject object, jdoubleArray arr1, 
        jdoubleArray arr2, jdoubleArray arr3, jint n,
        jintArray arr4, jintArray arr5, jintArray arr6, jint ntri)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr4, 0);
  jint *n2ray = (*env)->GetIntArrayElements(env, arr5, 0);
  jint *n3ray = (*env)->GetIntArrayElements(env, arr6, 0);
  surtri ((double *) xray, (double *) yray, (double *) zray, n,
          (int *) n1ray, (int *) n2ray, (int *) n3ray, ntri);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseIntArrayElements(env, arr4, n1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, n2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr6, n3ray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_surtri___3F_3F_3FI_3I_3I_3II
       (JNIEnv *env, jobject object, jfloatArray arr1, 
        jfloatArray arr2, jfloatArray arr3, jint n,
        jintArray arr4, jintArray arr5, jintArray arr6, jint ntri)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr4, 0);
  jint *n2ray = (*env)->GetIntArrayElements(env, arr5, 0);
  jint *n3ray = (*env)->GetIntArrayElements(env, arr6, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    surtri (p1, p2, p3, n, (int *) n1ray, (int *) n2ray, (int *) n3ray, ntri);
  free (p1);
  free (p2);
  free (p3);
  (*env)->ReleaseIntArrayElements(env, arr4, n1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, n2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr6, n3ray, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_surmsh (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  surmsh ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_suropt (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  suropt ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_surshc___3DI_3DI_3D_3D (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint n, jdoubleArray arr2, jint m,
        jdoubleArray arr3, jdoubleArray arr4)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *wmat = (*env)->GetDoubleArrayElements(env, arr4, 0);
  surshc ((double *) xray, n, (double *) yray, m, (double *) zmat, 
          (double *) wmat);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zmat, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, wmat, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_surshc___3FI_3FI_3F_3F (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint n, jfloatArray arr2, jint m,
        jfloatArray arr3, jfloatArray arr4)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, m);
  double *p3 = dbl_array (env, arr3, n * m);
  double *p4 = dbl_array (env, arr4, n * m);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
     surshc (p1, n, p2, m, p3, p4);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_surshd___3DI_3DI_3D (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint n, jdoubleArray arr2, jint m, 
        jdoubleArray arr3)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr3, 0);
  surshd ((double *) xray, n, (double *) yray, m, (double *) zmat);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zmat, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_surshd___3FI_3FI_3F (JNIEnv *env, jobject object, 
        jfloatArray arr1, jint n, jfloatArray arr2, jint m, jfloatArray arr3)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, m);
  double *p3 = dbl_array (env, arr3, n * m);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    surshd (p1, n, p2, m, p3);
  free (p1);
  free (p2);
  free (p3);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_sursze (JNIEnv *env, jobject object, jfloat xmin, 
        jfloat xmax, jfloat ymin, jfloat ymax)
{ sursze (xmin, xmax, ymin, ymax);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_survis (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  survis ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgatt (JNIEnv *env, jobject object, jint id, jstring s1,
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  swgatt (id, (char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgbox (JNIEnv *env, jobject object, jint i, jint j)
{ swgbox (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgbut (JNIEnv *env, jobject object, jint i, jint j)
{ swgbut (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgcb2 (JNIEnv *env, jobject object, jint id, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int n;
  char *p;
  n = strlen (str);
  p = (char *) malloc (n+1);
  strcpy (p , str);
  envcbk = env;

  if (ncbray < MAX_CB)
  { cbray[ncbray] = p;
    icbray[ncbray] = id;
    ncbray++;
    swgcb2 (id, dis_callback2);
  } 
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgcb3 (JNIEnv *env, jobject object, jint id, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int n;
  char *p;
  n = strlen (str);
  p = (char *) malloc (n+1);
  strcpy (p , str);
  envcbk = env;

  if (ncbray < MAX_CB)
  { cbray[ncbray] = p;
    icbray[ncbray] = id;
    ncbray++;
    swgcb3 (id, dis_callback3);
  } 
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgcbk (JNIEnv *env, jobject object, jint id, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int n;
  char *p;
  n = strlen (str);
  p = (char *) malloc (n+1);
  strcpy (p , str);
  envcbk = env;

  if (ncbray < MAX_CB)
  { cbray[ncbray] = p;
    icbray[ncbray] = id;
    ncbray++;
    swgcbk (id, dis_callback);
  } 
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgclr (JNIEnv *env, jobject object, jfloat xr,
        jfloat xg, jfloat xb, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  swgclr (xr, xg, xb, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgbgd (JNIEnv *env, jobject object, jint id, jfloat xr,
        jfloat xg, jfloat xb)
{
  swgbgd (id, xr, xg, xb);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgfgd (JNIEnv *env, jobject object, jint id, jfloat xr,
        jfloat xg, jfloat xb)
{
  swgfgd (id, xr, xg, xb);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgdrw (JNIEnv *env, jobject object, jfloat x)
{ swgdrw (x);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgfil (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  swgfil (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgflt (JNIEnv *env, jobject object, jint i, jfloat x,
        jint ndig)
{ swgflt (i, x, ndig);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgfnt (JNIEnv *env, jobject object, jstring s, jint i)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  swgfnt ((char *) str, i);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgfoc (JNIEnv *env, jobject object, jint i)
{ swgfoc (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swghlp (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  swghlp ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgint (JNIEnv *env, jobject object, jint i, jint iv)
{ swgint (i, iv);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgiop (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  swgiop (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgjus (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  swgjus ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swglis (JNIEnv *env, jobject object, jint i, jint j)
{ swglis (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgmix (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  swgmix ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}


JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgmrg (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  swgmrg (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgoff (JNIEnv *env, jobject object, jint i, jint j)
{ swgoff (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgopt (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  swgopt ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgpop (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  swgpop ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgpos (JNIEnv *env, jobject object, jint i, jint j)
{ swgpos (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgray___3DILjava_lang_String_2
       (JNIEnv *env, jobject object, jdoubleArray arr1, jint n, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  swgray ((double *) xray, n, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgray___3FILjava_lang_String_2
       (JNIEnv *env, jobject object, jfloatArray arr1, jint n, jstring s)
{ double *p1 = dbl_array (env, arr1, n);
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  if (p1 != NULL)
    swgray (p1, n, (char *) str);
  free (p1);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgscl (JNIEnv *env, jobject object, jint i, jfloat x)
{ swgscl (i, x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgsiz (JNIEnv *env, jobject object, jint i, jint j)
{ swgsiz (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgspc (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ swgspc (x, y);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgstp (JNIEnv *env, jobject object, jfloat x)
{ swgstp (x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgtbf (JNIEnv *env, jobject object, jint i, jfloat x,
        jint ndig, jint irow, jint icol, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  swgtbf (i, x, ndig, irow, icol, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgtbi (JNIEnv *env, jobject object, jint i, jint ix,
        jint irow, jint icol, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  swgtbi (i, ix, irow, icol, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgtbl__I_3DIIILjava_lang_String_2
       (JNIEnv *env, jobject object, jint i, jdoubleArray arr1, jint n, 
        jint ndig, jint idx, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  swgtbl (i, (double *) xray, n, ndig, idx, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgtbl__I_3FIIILjava_lang_String_2
       (JNIEnv *env, jobject object, jint i, jfloatArray arr1, jint n, 
        jint ndig, jint idx, jstring s)
{ double *p1 = dbl_array (env, arr1, n);
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  if (p1 != NULL)
    swgtbl (i, p1, n, ndig, idx, (char *) str);
  free (p1);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgtbs (JNIEnv *env, jobject object, jint i, jstring s1,
        jint irow, jint icol, jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  swgtbs (i, (char *) str1, irow, icol, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgtit (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  swgtit ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgtxt (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  swgtxt (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_swgtyp (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  swgtyp ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgval (JNIEnv *env, jobject object, jint i, jfloat x)
{ swgval (i, x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgwin (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh)
{ swgwin (nx, ny, nw, nh);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_swgwth (JNIEnv *env, jobject object, jint i)
{ swgwth (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_symb3d (JNIEnv *env, jobject object, jint n, jfloat x, 
        jfloat y, jfloat z)
{ symb3d (n, x, y, z);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_symbol (JNIEnv *env, jobject object, jint i, jint nx, 
        jint ny)
{ symbol (i, nx, ny);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_symfil (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  symfil ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_symrot (JNIEnv *env, jobject object, jfloat x)
{ symrot (x);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_tellfl (JNIEnv *env, jobject object, jint i)
{ return tellfl (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_thkc3d (JNIEnv *env, jobject object, jfloat x)
{ thkc3d (x);
}
 
JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_thkcrv (JNIEnv *env, jobject object, jint i)
{ thkcrv (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_thrfin (JNIEnv *env, jobject object)
{ thrfin ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_thrini (JNIEnv *env, jobject object, jint i)
{ thrini (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_ticks (JNIEnv *env, jobject object, jint i, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  ticks (i, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_ticlen (JNIEnv *env, jobject object, jint i, jint j)
{ ticlen (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_ticmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  ticmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_ticpos (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  ticpos ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_tifmod (JNIEnv *env, jobject object, jint i, jstring s1,
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  tifmod (i, (char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_tiforg (JNIEnv *env, jobject object, jint i, jint j)
{ tiforg (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_tifwin (JNIEnv *env, jobject object, jint nx, jint ny, 
        jint nw, jint nh)
{ tifwin (nx, ny, nw, nh);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_timopt (JNIEnv *env, jobject object)
{ timopt ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_titjus (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  titjus ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_title (JNIEnv *env, jobject object)
{ title ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_titlin (JNIEnv *env, jobject object, jstring s, jint i)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  titlin ((char *) str, i);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_titpos (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  titpos ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_torus3d (JNIEnv *env, jobject object, jfloat x, jfloat y,
        jfloat z, jfloat r1, jfloat r2, jfloat h, jfloat a1, jfloat a2, jint n,
        jint m)
{ torus3d (x, y, z, r1, r2, h, a1, a2, n, m);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_tprini (JNIEnv *env, jobject object)
{ tprini ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_tprfin (JNIEnv *env, jobject object)
{ tprfin ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_tprmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  tprmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_tprval (JNIEnv *env, jobject object, jfloat x)
{ tprval (x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_tr3axs (JNIEnv *env, jobject object, jfloat x,
        jfloat y, jfloat z, jfloat a)
{ tr3axs (x, y, z, a);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_tr3res (JNIEnv *env, jobject object)
{ tr3res ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_tr3rot (JNIEnv *env, jobject object, jfloat x,
        jfloat y, jfloat z)
{ tr3rot (x, y, z);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_tr3scl (JNIEnv *env, jobject object, jfloat x,
        jfloat y, jfloat z)
{ tr3scl (x, y, z);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_tr3shf (JNIEnv *env, jobject object, jfloat x,
                              jfloat y, jfloat z)
{
  tr3shf (x, y, z);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_trfco1___3DILjava_lang_String_2Ljava_lang_String_2 
       (JNIEnv *env, jobject object, jdoubleArray arr1, jint n,
        jstring s1, jstring s2)
{ jdouble *xray  = (*env)->GetDoubleArrayElements(env, arr1, 0);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  trfco1 ((double *) xray, n, (char *) str1, (char *) str2);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_trfco1___3FILjava_lang_String_2Ljava_lang_String_2 
       (JNIEnv *env, jobject object, jfloatArray arr1, jint n,
        jstring s1, jstring s2)
{ double *p1 = dbl_array (env, arr1, n);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  if (p1 != NULL)
  { trfco1 (p1, n, (char *) str1, (char *) str2);
    copy_dblarray (env, arr1, p1, n);
  } 
  free (p1);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_trfco2___3D_3DILjava_lang_String_2Ljava_lang_String_2 
       (JNIEnv *env, jobject object, jdoubleArray arr1, 
        jdoubleArray arr2, jint n, jstring s1, jstring s2)
{ jdouble *xray  = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray  = (*env)->GetDoubleArrayElements(env, arr2, 0);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  trfco2 ((double *) xray, (double *) yray, n, (char *) str1, (char *) str2);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_trfco2___3F_3FILjava_lang_String_2Ljava_lang_String_2
       (JNIEnv *env, jobject object, jfloatArray arr1, 
        jfloatArray arr2, jint n, jstring s1, jstring s2)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  if (p1 != NULL && p2 != NULL)
  { trfco2 (p1, p2, n, (char *) str1, (char *) str2);
    copy_dblarray (env, arr1, p1, n);
    copy_dblarray (env, arr2, p2, n);
  }
  free (p1);
  free (p2); 
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_trfco3___3D_3D_3DILjava_lang_String_2Ljava_lang_String_2 
       (JNIEnv *env, jobject object, jdoubleArray arr1, jdoubleArray arr2, 
        jdoubleArray arr3, jint n, jstring s1, jstring s2)
{ jdouble *xray  = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray  = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray  = (*env)->GetDoubleArrayElements(env, arr3, 0);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  trfco3 ((double *) xray, (double *) yray, (double *) zray, n, 
          (char *) str1, (char *) str2);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_trfco3___3F_3F_3FILjava_lang_String_2Ljava_lang_String_2 
       (JNIEnv *env, jobject object, jfloatArray arr1, jfloatArray arr2, 
        jfloatArray arr3, jint n, jstring s1, jstring s2)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
  { trfco3 (p1, p2, p3, n, (char *) str1, (char *) str2);
    copy_dblarray (env, arr1, p1, n);
    copy_dblarray (env, arr2, p2, n);
    copy_dblarray (env, arr3, p3, n);
  }
  free (p1);
  free (p2);
  free (p3);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_trfmat___3DII_3DII (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jint nx, jint ny, 
        jdoubleArray arr2, jint nx2, jint ny2)
{ jdouble *zmat = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *zmat2 = (*env)->GetDoubleArrayElements(env, arr2, 0);
  trfmat ((double *) zmat, nx, ny, (double *) zmat2, nx2, ny2);
  (*env)->ReleaseDoubleArrayElements(env, arr1, zmat, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, zmat2, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_trfmat___3FII_3DII (JNIEnv *env, jobject object, 
                   jfloatArray arr1, jint nx, jint ny, 
                   jfloatArray arr2, jint nx2, jint ny2)
{ double *p1 = dbl_array (env, arr1, nx * ny);
  double *p2 = dbl_array (env, arr2, nx2 * ny2);
  if (p1 != NULL && p2 != NULL)
  { trfmat (p1, nx, ny, p2, nx2, ny2);
    copy_dblarray (env, arr2, p2, nx2 * ny2);
  }
  free (p1);
  free (p2);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_trfrel___3D_3DI (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  trfrel ((double *) xray, (double *) yray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_trfrel___3F_3FI (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  if (p1 != NULL && p2 != NULL)
  { trfrel (p1, p2, n);
    copy_dblarray (env, arr1, p1, n);
    copy_dblarray (env, arr2, p2, n);
  }
  free (p1);
  free (p2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_trfres (JNIEnv *env, jobject object)
{ trfres ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_trfrot (JNIEnv *env, jobject object, jfloat x, jint i,
        jint j)
{ trfrot (x, i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_trfscl (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ trfscl (x, y);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_trfshf (JNIEnv *env, jobject object, jint i, jint j)
{ trfshf (i, j);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_tria3d___3D_3D_3D (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  tria3d ((double *) xray, (double *) yray, (double *) zray);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
}
JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_tria3d___3F_3F_3F (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3)
{ double *p1 = dbl_array (env, arr1, 3);
  double *p2 = dbl_array (env, arr2, 3);
  double *p3 = dbl_array (env, arr3, 3);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    tria3d (p1, p2, p3);
  free (p1);
  free (p2);
  free (p3);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_triang___3D_3DI_3I_3I_3II
       (JNIEnv *env, jobject object, jdoubleArray arr1, jdoubleArray arr2, 
        jint n, jintArray arr3, jintArray arr4, jintArray arr5, jint nmax)
{ int ntri;
  jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr3, 0);
  jint *n2ray = (*env)->GetIntArrayElements(env, arr4, 0);
  jint *n3ray = (*env)->GetIntArrayElements(env, arr5, 0);
  ntri = triang ((double *) xray, (double *) yray, n,
          (int *) n1ray, (int *) n2ray, (int *) n3ray, nmax);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseIntArrayElements(env, arr3, n1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr4, n2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, n3ray, 0);
  return ntri;
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_triang___3F_3FI_3I_3I_3II
       (JNIEnv *env, jobject object, jfloatArray arr1, jfloatArray arr2, 
        jint n, jintArray arr3, jintArray arr4, jintArray arr5, jint nmax)
{ int ntri;
  double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr3, 0);
  jint *n2ray = (*env)->GetIntArrayElements(env, arr4, 0);
  jint *n3ray = (*env)->GetIntArrayElements(env, arr5, 0);
  if (p1 != NULL && p2 != NULL)
    ntri = triang (p1, p2, n, (int *) n1ray, (int *) n2ray, (int *) n3ray, 
                   nmax);
  free (p1);
  free (p2); 
  (*env)->ReleaseIntArrayElements(env, arr3, n1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr4, n2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, n3ray, 0);
  return ntri;
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_triflc___3D_3D_3II (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jintArray arr3, jint n)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jint *iray = (*env)->GetIntArrayElements(env, arr3, 0);
  triflc ((double *) xray, (double *) yray, (int *) iray, n);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseIntArrayElements(env, arr3, iray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_triflc___3F_3F_3II (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jintArray arr3, jint n)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  jint *iray = (*env)->GetIntArrayElements(env, arr3, 0);
  if (p1 != NULL && p2 != NULL)
    triflc (p1, p2, (int *) iray, n);
  free (p1);
  free (p2);
  (*env)->ReleaseIntArrayElements(env, arr3, iray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_trifll___3D_3D (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  trifll ((double *) xray, (double *) yray);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_trifll___3F_3F (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2)
{ double *p1 = dbl_array (env, arr1, 3);
  double *p2 = dbl_array (env, arr2, 3);
  if (p1 != NULL && p2 != NULL)
    trifll (p1, p2);
  free (p1);
  free (p2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_triplx (JNIEnv *env, jobject object)
{ triplx ();
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_tripts___3D_3D_3DI_3I_3I_3IIF_3D_3DI_3II
       (JNIEnv *env, jobject object, jdoubleArray arr1, 
        jdoubleArray arr2, jdoubleArray arr3, jint n,
        jintArray arr4, jintArray arr5, jintArray arr6, jint ntri,
	jfloat zlev, jdoubleArray arr7, jdoubleArray arr8, 
        jint maxpts, jintArray arr9, jint maxray)
{ int ncurvs;
  jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr4, 0);
  jint *n2ray = (*env)->GetIntArrayElements(env, arr5, 0);
  jint *n3ray = (*env)->GetIntArrayElements(env, arr6, 0);
  jdouble *xpts = (*env)->GetDoubleArrayElements(env, arr7, 0);
  jdouble *ypts = (*env)->GetDoubleArrayElements(env, arr8, 0);
  jint *nray = (*env)->GetIntArrayElements(env, arr9, 0);
  tripts ((double *) xray, (double *) yray, (double *) zray, n,
          (int *) n1ray, (int *) n2ray, (int *) n3ray, ntri, zlev,
          (double *) xpts, (double *) ypts, maxpts, (int *) nray, maxray, 
          &ncurvs);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseIntArrayElements(env, arr4, n1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, n2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr6, n3ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr7, xpts, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr8, ypts, 0);
  (*env)->ReleaseIntArrayElements(env,   arr9, nray, 0);
  return ncurvs;
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_tripts___3F_3F_3FI_3I_3I_3IIF_3F_3FI_3II
       (JNIEnv *env, jobject object, jfloatArray arr1, 
        jfloatArray arr2, jfloatArray arr3, jint n,
        jintArray arr4, jintArray arr5, jintArray arr6, jint ntri,
	jfloat zlev, jfloatArray arr7, jfloatArray arr8, 
        jint maxpts, jintArray arr9, jint maxray)
{ int ncurvs;
  double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr7, maxpts);
  double *p5 = dbl_array (env, arr8, maxpts);
  jint *n1ray = (*env)->GetIntArrayElements(env, arr4, 0);
  jint *n2ray = (*env)->GetIntArrayElements(env, arr5, 0);
  jint *n3ray = (*env)->GetIntArrayElements(env, arr6, 0);
  jint *nray = (*env)->GetIntArrayElements(env, arr9, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL && p5 != NULL)
  { tripts (p1, p2, p3, n, (int *) n1ray, (int *) n2ray, (int *) n3ray, ntri, 
            zlev, p4, p5, maxpts, (int *) nray, maxray, &ncurvs);
    copy_dblarray (env, arr7, p4, maxpts);
    copy_dblarray (env, arr8, p5, maxpts);
  }
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  free (p5);
  (*env)->ReleaseIntArrayElements(env, arr4, n1ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr5, n2ray, 0);
  (*env)->ReleaseIntArrayElements(env, arr6, n3ray, 0);
  (*env)->ReleaseIntArrayElements(env,   arr9, nray, 0);
  return ncurvs;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_trmlen (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int nl;
  nl = trmlen((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return nl;
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_texmod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  texmod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_texopt (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  texopt ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_texval (JNIEnv *env, jobject object, jfloat x, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  texval (x, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_ttfont (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  ttfont ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_tube3d (JNIEnv *env, jobject object, jfloat x1, 
        jfloat y1, jfloat z1, jfloat x2, jfloat y2, jfloat z2, jfloat r,
        jint n, jint m)
{ tube3d (x1, y1, z1, x2, y2, z2, r, n, m);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_txtbgd (JNIEnv *env, jobject object, jint i)
{ txtbgd (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_txtjus (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  txtjus ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_txture (JNIEnv *env, jobject object, 
	jintArray arr1, jint nx, jint ny) 
{ jint *izmat = (*env)->GetIntArrayElements(env, arr1, 0);
  txture ((int *) izmat, nx, ny);
  (*env)->ReleaseIntArrayElements(env, arr1, izmat, 0);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_unit (JNIEnv *env, jobject object, jint i)
{ if (i == 0)
    unit (NULL);
  else
    unit ((void *) stdout);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_units (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  units ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_upstr (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  upstr ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_utfint (JNIEnv *env, jobject object, jstring s,
        jintArray arr1, jint nray)
{ int n;
  const char *str = (*env)->GetStringUTFChars(env, s, 0);
  jint *iray = (*env)->GetIntArrayElements(env, arr1, 0);
  n = utfint ((char *) str, (int *) iray, nray);
  (*env)->ReleaseStringUTFChars(env, s, str);
  (*env)->ReleaseIntArrayElements(env, arr1, iray, 0);
  return n;
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_vang3d (JNIEnv *env, jobject object, jfloat x)
{ vang3d (x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vclp3d (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ vclp3d (x, y);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vecclr (JNIEnv *env, jobject object, jint i)
{ vecclr (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vecf3d___3D_3D_3D_3D_3D_3DII 
       (JNIEnv *env, jobject object, jdoubleArray arr1, jdoubleArray arr2, 
        jdoubleArray arr3, jdoubleArray arr4, jdoubleArray arr5, 
        jdoubleArray arr6, jint n, jint ivec)
{ jdouble *x1ray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *y1ray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *z1ray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *x2ray = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jdouble *y2ray = (*env)->GetDoubleArrayElements(env, arr5, 0);
  jdouble *z2ray = (*env)->GetDoubleArrayElements(env, arr6, 0);
  vecf3d ((double *) x1ray, (double *) y1ray, (double *) z1ray, 
           (double *) x2ray, (double *) y2ray, (double *) z2ray,
           n, ivec);
  (*env)->ReleaseDoubleArrayElements(env, arr1, x1ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, y1ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, z1ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, x2ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr5, y2ray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr6, z2ray, 0);
}
JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vecf3d___3F_3F_3F_3F_3F_3FII 
       (JNIEnv *env, jobject object, jfloatArray arr1, jfloatArray arr2,
        jfloatArray arr3, jfloatArray arr4, jfloatArray arr5, 
        jfloatArray arr6, jint n, jint ivec)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr4, n);
  double *p5 = dbl_array (env, arr5, n);
  double *p6 = dbl_array (env, arr6, n);
  if (p1 != NULL && p2 != NULL && p3 != NULL && 
      p4 != NULL && p5 != NULL && p6 != NULL)
    vecf3d (p1, p2, p3, p4, p5, p6, n, ivec);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  free (p5);
  free (p6);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vecfld___3D_3D_3D_3DII
       (JNIEnv *env, jobject object, jdoubleArray arr1, jdoubleArray arr2, 
        jdoubleArray arr3, jdoubleArray arr4, jint n, jint ivec)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *uray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *vray = (*env)->GetDoubleArrayElements(env, arr4, 0);
  vecfld ((double *) xray, (double *) yray, (double *) uray, 
     (double *) vray, n, ivec);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, uray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, vray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vecfld___3F_3F_3F_3FII 
       (JNIEnv *env, jobject object, jfloatArray arr1, jfloatArray arr2, 
        jfloatArray arr3, jfloatArray arr4, jint n, jint ivec)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr4, n);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL)
    vecfld (p1, p2, p3, p4, n, ivec);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vecmat___3D_3DII_3D_3DI
       (JNIEnv *env, jobject object, jdoubleArray arr1, jdoubleArray arr2, 
        jint nx, jint ny, jdoubleArray arr3, jdoubleArray arr4, jint ivec)
{ jdouble *xv = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yv = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *xp = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *yp = (*env)->GetDoubleArrayElements(env, arr4, 0);
  vecmat ((double *) xv, (double *) yv, nx, ny, (double *) yp, (double *) yp,
          ivec);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, xp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, yp, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vecmat___3F_3FII_3F_3FI
       (JNIEnv *env, jobject object, jfloatArray arr1, jfloatArray arr2, 
        jint nx, jint ny, jfloatArray arr3, jfloatArray arr4, jint ivec)
{ int n = nx * ny;
  double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, nx);
  double *p4 = dbl_array (env, arr4, ny);
  if (p1 != NULL && p2 != NULL && p3 != NULL && p4 != NULL)
    vecmat (p1, p2, nx, ny, p3, p4, ivec);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vecmat3d___3D_3D_3DIII_3D_3D_3DI 
       (JNIEnv *env, jobject object, jdoubleArray arr1, jdoubleArray arr2, 
        jdoubleArray arr3, jint nx, jint ny, jint nz,
        jdoubleArray arr4, jdoubleArray arr5, jdoubleArray arr6, jint ivec)
{ jdouble *xv = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yv = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zv = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *xp = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jdouble *yp = (*env)->GetDoubleArrayElements(env, arr5, 0);
  jdouble *zp = (*env)->GetDoubleArrayElements(env, arr6, 0);
  vecmat3d ((double *) xv, (double *) yv, (double *) zv, nx, ny, nz,
            (double *) yp, (double *) yp, (double *) zp, ivec);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zv, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, xp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr5, yp, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr6, zp, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vecmat3d___3F_3F_3FIII_3F_3F_3FI
       (JNIEnv *env, jobject object, jfloatArray arr1, jfloatArray arr2, 
        jfloatArray arr3, jint nx, jint ny, jint nz,
	jfloatArray arr4, jfloatArray arr5, jfloatArray arr6, jint ivec)
{ int n = nx * ny *nz;
  double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr3, nx);
  double *p5 = dbl_array (env, arr3, ny);
  double *p6 = dbl_array (env, arr3, nz);
  if (p1 != NULL && p2 != NULL && p3 != NULL &&
      p4 != NULL && p5 != NULL && p6 != NULL)
    vecmat3d (p1, p2, p3, nx, ny, nz, p4, p5, p6, ivec);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  free (p5);
  free (p6);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_vecopt (JNIEnv *env, jobject object, jfloat x, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  vecopt (x, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vector (JNIEnv *env, jobject object, jint nx1, jint ny1,
        jint nx2, jint ny2, jint ivec)
{ vector (nx1, ny1, nx2, ny2, ivec);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vectr3 (JNIEnv *env, jobject object, jfloat x1, 
        jfloat y1, jfloat z1, jfloat x2, jfloat y2, jfloat z2, jint i)
{ vectr3 (x1, y1, z1, x2, y2, z2, i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_vfoc3d (JNIEnv *env, jobject object, jfloat x, jfloat y, 
                    jfloat z, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  vfoc3d (x, y, z, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_view3d (JNIEnv *env, jobject object, jfloat x, jfloat y, 
        jfloat z, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  view3d (x, y, z, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vkxbar (JNIEnv *env, jobject object, jint i)
{
  vkxbar (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vkybar (JNIEnv *env, jobject object, jint i)
{
  vkybar (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vkytit (JNIEnv *env, jobject object, jint i)
{
  vkytit (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_vltfil (JNIEnv *env, jobject object, jstring s1,
               jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  vltfil ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_vscl3d (JNIEnv *env, jobject object, jfloat x)
{ vscl3d (x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vtx3d___3D_3D_3D_3IILjava_lang_String_2 
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3, jint n,
        jstring s1)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  vtx3d ((double *) xray, (double *) yray, (double *) zray, n, (char *) str1);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vtx3d___3F_3F_3FILjava_lang_String_2 
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3, jint n,
        jstring s1)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    vtx3d (p1, p2, p3, n, (char *) str1);
  free (p1);
  free (p2);
  free (p3);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vtxc3d___3D_3D_3D_3IILjava_lang_String_2 
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3, 
        jintArray arr4, jint n, jstring s1)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jint *iray = (*env)->GetIntArrayElements(env, arr4, 0);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  vtxc3d ((double *) xray, (double *) yray, (double *) zray, (int *) iray, 
          n, (char *) str1);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, zray, 0);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vtxc3d___3F_3F_3F_3IILjava_lang_String_2 
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3, 
        jintArray arr4, jint n, jstring s1)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  jint *iray = (*env)->GetIntArrayElements(env, arr4, 0);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    vtxc3d (p1, p2, p3, (int *) iray, n, (char *) str1);
  free (p1);
  free (p2);
  free (p3);
  (*env)->ReleaseIntArrayElements(env, arr4, iray, 0);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vtxn3d___3D_3D_3D_3D_3D_3DILjava_lang_String_2
       (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3, 
        jdoubleArray arr4, jdoubleArray arr5, jdoubleArray arr6, jint n,
        jstring s1)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jdouble *xn = (*env)->GetDoubleArrayElements(env, arr4, 0);
  jdouble *yn = (*env)->GetDoubleArrayElements(env, arr5, 0);
  jdouble *zn = (*env)->GetDoubleArrayElements(env, arr6, 0);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  vtxn3d ((double *) xray, (double *) yray, (double *) zray, 
          (double *) xn, (double *) yn, (double *) zn, n, (char *) str1);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr4, xn, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr5, yn, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr6, zn, 0);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_vtxn3d___3F_3F_3F_3F_3F_3FILjava_lang_String_2 
       (JNIEnv *env, jobject object, 
        jfloatArray arr1, jfloatArray arr2, jfloatArray arr3, 
        jfloatArray arr4, jfloatArray arr5, jfloatArray arr6, jint n,
        jstring s1)
{ double *p1 = dbl_array (env, arr1, n);
  double *p2 = dbl_array (env, arr2, n);
  double *p3 = dbl_array (env, arr3, n);
  double *p4 = dbl_array (env, arr4, n);
  double *p5 = dbl_array (env, arr5, n);
  double *p6 = dbl_array (env, arr6, n);
  const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL &&
      p4 != NULL && p5 != NULL && p6 != NULL)
    vtxn3d (p1, p2, p3, p4, p5, p6, n, (char *) str1);
  free (p1);
  free (p2);
  free (p3);
  free (p4);
  free (p5);
  free (p6);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_vup3d (JNIEnv *env, jobject object, jfloat x)
{ vup3d (x);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgapp (JNIEnv *env, jobject object, jint ip, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int id;
  id = wgapp (ip, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgappb (JNIEnv *env, jobject object, jint ip, 
        jbyteArray arr1, jint nw, jint nh)
{ jbyte *cbuf = (*env)->GetByteArrayElements(env, arr1, 0);
  int id;
  id = wgappb (ip, (unsigned char *) cbuf, nw, nh);
  (*env)->ReleaseByteArrayElements(env, arr1, cbuf, 0);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgbas (JNIEnv *env, jobject object, jint ip, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int id;
  id = wgbas (ip, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgbox (JNIEnv *env, jobject object, jint ip, 
        jstring s, jint isel)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int id;
  id = wgbox (ip, (char *) str, isel);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgbut (JNIEnv *env, jobject object, jint ip, 
        jstring s, jint isel)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int id;
  id = wgbut (ip, (char *) str, isel);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgcmd (JNIEnv *env, jobject object, jint ip, jstring s1,
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  int id;
  id = wgcmd (ip, (char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgdlis (JNIEnv *env, jobject object, jint ip, 
        jstring s, jint isel)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int id;
  id = wgdlis (ip, (char *) str, isel);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgdraw (JNIEnv *env, jobject object, jint i)
{ return wgdraw (i);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgfil (JNIEnv *env, jobject object, jint ip, jstring s1,
        jstring s2, jstring s3)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  const char *str3 = (*env)->GetStringUTFChars(env, s3, 0);
  int id;
  id = wgfil (ip, (char *) str1, (char *) str2, (char *) str3);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  (*env)->ReleaseStringUTFChars(env, s3, str3);
  return id;
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_wgfin (JNIEnv *env, jobject object)
{
  wgfin ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgini (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int id;
  id = wgini ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgicon (JNIEnv *env, jobject object, jint ip, jstring s1,
                              jint nw, jint nh, jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  int id;
  id = wgicon (ip, (char *) str1, nw, nh, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgimg (JNIEnv *env, jobject object, jint ip, 
        jstring s1, jbyteArray arr1, jint nw, jint nh)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  jbyte *iray = (*env)->GetByteArrayElements(env, arr1, 0);
  int id;
  id = wgimg (ip, (char *) str1, (unsigned char *) iray, nw, nh);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseByteArrayElements(env, arr1, iray, 0);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wglab (JNIEnv *env, jobject object, jint ip, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int id;
  id = wglab (ip, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wglis (JNIEnv *env, jobject object, jint ip, 
        jstring s, jint isel)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int id;
  id = wglis (ip, (char *) str, isel);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgok (JNIEnv *env, jobject object, jint i)
{ return wgok (i);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgpop (JNIEnv *env, jobject object, jint ip, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int id;
  id = wgpop (ip, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgpopb (JNIEnv *env, jobject object, jint ip, 
        jbyteArray arr1, jint nw, jint nh)
{ jbyte *cbuf = (*env)->GetByteArrayElements(env, arr1, 0);
  int id;
  id = wgpopb (ip, (unsigned char *) cbuf, nw, nh);
  (*env)->ReleaseByteArrayElements(env, arr1, cbuf, 0);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgquit (JNIEnv *env, jobject object, jint i)
{ return wgquit (i);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgsep (JNIEnv *env, jobject object, jint i)
{ return wgsep (i);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgltxt (JNIEnv *env, jobject object, jint ip, jstring s1,
        jstring s2, jint iper)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  int id;
  id = wgltxt (ip, (char *) str1, (char *) str2, iper);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgpbar (JNIEnv *env, jobject object, jint ip, 
        jfloat x1, jfloat x2, jfloat xstp)
{ int id;
  id = wgpbar (ip, x1, x2, xstp);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgpbut (JNIEnv *env, jobject object, jint ip, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int id;
  id = wgpbut (ip, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgpicon (JNIEnv *env, jobject object, jint ip, 
                    jstring s1, jint nw, jint nh, jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  int id;
  id = wgpicon (ip, (char *) str1, nw, nh, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgpimg (JNIEnv *env, jobject object, jint ip, 
        jstring s1, jbyteArray arr1, jint nw, jint nh)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  jbyte *iray = (*env)->GetByteArrayElements(env, arr1, 0);
  int id;
  id = wgpimg (ip, (char *) str1, (unsigned char *) iray, nw, nh);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseByteArrayElements(env, arr1, iray, 0);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgscl (JNIEnv *env, jobject object, jint ip, jstring s,
                   jfloat x1, jfloat x2, jfloat xval, jint ndez)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int id;
  id = wgscl (ip, (char *) str, x1, x2, xval, ndez);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return id;
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgstxt (JNIEnv *env, jobject object, jint i, 
        jint nsize, jint nmax)
{ return wgstxt (i, nsize, nmax);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgtbl (JNIEnv *env, jobject object, jint ip, 
                              jint nrows, jint ncols)
{ return wgtbl (ip, nrows, ncols);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_wgtxt (JNIEnv *env, jobject object, jint ip, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  int id;
  id = wgtxt (ip, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
  return id;
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_widbar (JNIEnv *env, jobject object, jint i)
{
  widbar (i);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_wimage (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  wimage ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_winapp (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  winapp ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_wincbk (JNIEnv *env, jobject object, jstring s1,
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  int n;
  char *p;
  n = strlen (str1);
  p = (char *) malloc (n+1);
  strcpy (p , str1);
  envcbk = env;

  cbwin = p;
  wincbk (dis_wincbk, str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_winico (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  winico ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_winjus (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  winjus ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_winkey (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  winkey ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_winmod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  winmod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_winopt (JNIEnv *env, jobject object, jint iopt, 
        jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  winopt (iopt, (char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_windbr (JNIEnv *env, jobject object, jfloat x, 
        jint nx, jint ny, jint n, jfloat a)
{ windbr (x, nx, ny, n, a);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_window (JNIEnv *env, jobject object, jint nx, jint ny, 
                                                 jint nw, jint nh)
{ window (nx, ny, nw, nh);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_winfnt (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  winfnt ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_winid (JNIEnv *env, jobject object)
{ return winid ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_winsiz (JNIEnv *env, jobject object, jint i, jint j)
{ winsiz (i, j);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_wintit (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  wintit ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_wintyp (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  wintyp ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_wmfmod (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  wmfmod ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_world (JNIEnv *env, jobject object)
{ world ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_wpixel (JNIEnv *env, jobject object, jint i, jint j, 
        jint k)
{ wpixel (i, j, k);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_wpixls (JNIEnv *env, jobject object, jbyteArray arr1, 
        jint ix, jint iy, jint nw, jint nh)
{ jbyte *cbuf = (*env)->GetByteArrayElements(env, arr1, 0);
  wpixls ((unsigned char * ) cbuf, ix, iy, nw, nh);
  (*env)->ReleaseByteArrayElements(env, arr1, cbuf, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_wpxrow (JNIEnv *env, jobject object, jbyteArray arr1, 
        jint ix, jint iy, jint n)
{ jbyte *cbuf = (*env)->GetByteArrayElements(env, arr1, 0);
  wpxrow ((unsigned char * ) cbuf, ix, iy, n);
  (*env)->ReleaseByteArrayElements(env, arr1, cbuf, 0);
}

JNIEXPORT jint JNICALL 
Java_de_dislin_Dislin_writfl (JNIEnv *env, jobject object, jint nu, 
        jbyteArray arr1, jint n)
{ int iret;
  jbyte *cbuf = (*env)->GetByteArrayElements(env, arr1, 0);
  iret = writfl (nu, (unsigned char * ) cbuf, n);
  (*env)->ReleaseByteArrayElements(env, arr1, cbuf, 0);
  return iret;
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_wtiff (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  wtiff ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_x11fnt (JNIEnv *env, jobject object, jstring s1, 
        jstring s2)
{ const char *str1 = (*env)->GetStringUTFChars(env, s1, 0);
  const char *str2 = (*env)->GetStringUTFChars(env, s2, 0);
  x11fnt ((char *) str1, (char *) str2);
  (*env)->ReleaseStringUTFChars(env, s1, str1);
  (*env)->ReleaseStringUTFChars(env, s2, str2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_x11mod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  x11mod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_x2dpos (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ return x2dpos (x, y);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_x3dabs (JNIEnv *env, jobject object, jfloat x, 
        jfloat y, jfloat z)
{ return x3dabs (x, y, z);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_x3dpos (JNIEnv *env, jobject object, jfloat x, 
        jfloat y, jfloat z)
{ return x3dpos (x, y, z);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_x3drel (JNIEnv *env, jobject object, jfloat x, 
        jfloat y, jfloat z)
{ return x3drel (x, y, z);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_xaxgit (JNIEnv *env, jobject object)
{ xaxgit ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_xaxis (JNIEnv *env, jobject object, jfloat a, jfloat b,
        jfloat or, jfloat step, jint nl, jstring s, jint it, jint nx, jint ny)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  xaxis (a, b, or, step, nl, (char *) str, it, nx, ny);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_xaxlg (JNIEnv *env, jobject object, jfloat a, jfloat b,
        jfloat or, jfloat step, jint nl, jstring s, jint it, jint nx, jint ny)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  xaxlg (a, b, or, step, nl, (char *) str, it, nx, ny);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_xaxmap (JNIEnv *env, jobject object, jfloat a, jfloat b,
        jfloat or, jfloat step, jstring s, jint it, jint ny)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  xaxmap (a, b, or, step, (char *) str, it, ny);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_xcross (JNIEnv *env, jobject object)
{ xcross ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_xdraw (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ xdraw (x, y);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_xinvrs (JNIEnv *env, jobject object, jint i)
{ return xinvrs (i);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_xmove (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ xmove (x, y);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_xposn (JNIEnv *env, jobject object, jfloat x)
{ return xposn (x);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_y2dpos (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ return y2dpos (x, y);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_y3dabs (JNIEnv *env, jobject object, jfloat x, 
        jfloat y, jfloat z)
{ return y3dabs (x, y, z);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_y3dpos (JNIEnv *env, jobject object, jfloat x, 
        jfloat y, jfloat z)
{ return x3dpos (x, y, z);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_y3drel (JNIEnv *env, jobject object, jfloat x, 
        jfloat y, jfloat z)
{ return y3drel (x, y, z);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_yaxgit (JNIEnv *env, jobject object)
{ yaxgit ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_yaxis (JNIEnv *env, jobject object, jfloat a, jfloat b,
        jfloat or, jfloat step, jint nl, jstring s, jint it, jint nx, jint ny)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  yaxis (a, b, or, step, nl, (char *) str, it, nx, ny);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_yaxlg (JNIEnv *env, jobject object, jfloat a, jfloat b,
        jfloat or, jfloat step, jint nl, jstring s, jint it, jint nx, jint ny)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  yaxlg (a, b, or, step, nl, (char *) str, it, nx, ny);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_yaxmap (JNIEnv *env, jobject object, jfloat a, jfloat b,
        jfloat or, jfloat step, jstring s, jint it, jint ny)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  yaxmap (a, b, or, step, (char *) str, it, ny);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_ycross (JNIEnv *env, jobject object)
{ ycross ();
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_yinvrs (JNIEnv *env, jobject object, jint i)
{ return yinvrs (i);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_yposn (JNIEnv *env, jobject object, jfloat x)
{ return yposn (x);
}

JNIEXPORT jfloat JNICALL
Java_de_dislin_Dislin_z3dpos (JNIEnv *env, jobject object, jfloat x, 
        jfloat y, jfloat z)
{ return z3dpos (x, y, z);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_zaxis (JNIEnv *env, jobject object, jfloat a, jfloat b,
        jfloat or, jfloat step, jint nl, jstring s,
        jint it, jint id, jint nx, jint ny)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  zaxis (a, b, or, step, nl, (char *) str, it, id, nx, ny);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_zaxlg (JNIEnv *env, jobject object, jfloat a, jfloat b,
        jfloat or, jfloat step, jint nl, jstring s,
        jint it, jint id, jint nx, jint ny)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  zaxlg (a, b, or, step, nl, (char *) str, it, id, nx, ny);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_zbfers (JNIEnv *env, jobject object)
{ zbfers ();
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_zbffin (JNIEnv *env, jobject object)
{ zbffin ();
}

JNIEXPORT jint JNICALL
Java_de_dislin_Dislin_zbfini (JNIEnv *env, jobject object)
{ return zbfini ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_zbflin (JNIEnv *env, jobject object, jfloat x1, 
        jfloat y1, jfloat z1, jfloat x2, jfloat y2, jfloat z2)
{ zbflin (x1, y1, z1, x2, y2, z2);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_zbfmod (JNIEnv *env, jobject object, jstring s)
{ const char *str = (*env)->GetStringUTFChars(env, s, 0);
  zbfmod ((char *) str);
  (*env)->ReleaseStringUTFChars(env, s, str);
}

JNIEXPORT void JNICALL
Java_de_dislin_Dislin_zbfres (JNIEnv *env, jobject object)
{ zbfres ();
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_zbfscl (JNIEnv *env, jobject object, jfloat x)
{ zbfscl (x);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_zbftri___3D_3D_3D_3I (JNIEnv *env, jobject object, 
        jdoubleArray arr1, jdoubleArray arr2, jdoubleArray arr3, 
        jintArray arr4)
{ jdouble *xray = (*env)->GetDoubleArrayElements(env, arr1, 0);
  jdouble *yray = (*env)->GetDoubleArrayElements(env, arr2, 0);
  jdouble *zray = (*env)->GetDoubleArrayElements(env, arr3, 0);
  jint   *iray = (*env)->GetIntArrayElements(env, arr4, 0);
  zbftri ((double *) xray, (double *) yray, (double *) zray, (int *) iray);
  (*env)->ReleaseDoubleArrayElements(env, arr1, xray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr2, yray, 0);
  (*env)->ReleaseDoubleArrayElements(env, arr3, zray, 0);
  (*env)->ReleaseIntArrayElements(env, arr4, iray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_zbftri___3F_3F_3F_3I (JNIEnv *env, jobject object, 
  jfloatArray arr1, jfloatArray arr2, jfloatArray arr3, jintArray arr4)
{ double *p1 = dbl_array (env, arr1, 3);
  double *p2 = dbl_array (env, arr2, 3);
  double *p3 = dbl_array (env, arr3, 3);
  jint *iray = (*env)->GetIntArrayElements(env, arr4, 0);
  if (p1 != NULL && p2 != NULL && p3 != NULL)
    zbftri (p1, p2, p3, (int *) iray);
  free (p1);
  free (p2);
  free (p3);
  (*env)->ReleaseIntArrayElements(env, arr4, iray, 0);
}

JNIEXPORT void JNICALL 
Java_de_dislin_Dislin_zscale (JNIEnv *env, jobject object, jfloat x, jfloat y)
{ zscale (x, y);
}
