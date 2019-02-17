// [[Rcpp::depends(BH)]]
#include <Rcpp.h>
#include <iostream>
#include <math.h>
#include <numeric>
#include <string>
// #include "exp.cpp"
// #include "opt.cpp"
#include <boost/math/special_functions/digamma.hpp>
#include <boost/math/special_functions/trigamma.hpp>

using namespace std;
using namespace Rcpp;
#define EPSILON 1e-6
#define EPSILON2 1e-6

// 1. Expt


long double l1(int x, int y, double a0, double a1, double a2, int k, int m, double adjj = 0)
{
  return(exp(lgamma(a1 + k)- lgamma(k+1)- lgamma(a1) + lgamma(x + y + a0 -m -k) - lgamma(x -k +1) - lgamma(a0 + y - m)
               + lgamma(m + a2) - lgamma(m+1) - lgamma(a2) + lgamma(y +a0 -m) - lgamma(y -m +1) - lgamma(a0) - adjj));
}
long double l1_c (double t1, double t2, int k, int m, double adjj = 0)
{
  return(exp(k * log(t1) + m * log(t2) - adjj));
}
long double l1_AC (double t1, double t2, int x, int y, double a0, double a1, double a2, int k, int m, double adjj = 0)
{
  return(exp(lgamma(a1 + k)- lgamma(k+1)- lgamma(a1) + lgamma(x + y + a0 -m -k) - lgamma(x -k +1) - lgamma(a0 + y - m)
               + lgamma(m + a2) - lgamma(m+1) - lgamma(a2) + lgamma(y +a0 -m) - lgamma(y -m +1) - lgamma(a0) + k *log(t1) + m *log(t2) - adjj));
}
long double l2_A (int x, double a0, double a1, double a2, int k, double adjj = 0)
{
  return(exp(lgamma(x +a0 -k) + lgamma(k + a1) - lgamma(a0) - lgamma(x-k+1) - lgamma(a1) - lgamma(k+1) - adjj));
}
long double l3_A (int y, double a0, double a1, double a2, int m, double adjj = 0)
{
  return(exp(lgamma(y +a0 -m) + lgamma(m + a2) - lgamma(a0) - lgamma(y-m+1) - lgamma(a2) - lgamma(m+1) - adjj));
}
long double R0_E1(int x, int y, int k, int m, double a0)
{
  return(x - k + y - m + a0);
}
long double log_R0_E1(int x, int y, int k, int m, double a0)
{
  return(boost::math::digamma(x - k + y - m + a0));
}
long double	log_R0_E2(int x, double a0, int k)
{
  return(boost::math::digamma(x - k + a0));
}
long double	log_R0_E3(int y, double a0, int m)
{
  return(boost::math::digamma(y - m + a0));
}
long double R1_E1(int k, double a1)
{
  return(k + a1);
}
long double log_R1_E1(int k, double a1)
{
  return(boost::math::digamma(k + a1));
}
long double log_R1_E2(int k, double a1)
{
  return(boost::math::digamma(k + a1));
}
long double R2_E1(int m, double a2)
{
  return(m + a2);
}
long double log_R2_E1(int m, double a2)
{
  return(boost::math::digamma(m + a2));
}
long double log_R2_E3(int m, double a2)
{
  return(boost::math::digamma(m + a2));
}

// [[Rcpp::export]]
void dBvZINB_Expt(int &x, int &y, int &freq, double &a0, double &a1, double &a2,
                  double &b1, double &b2, double &p1, double &p2, double &p3, double &p4,
                  NumericVector &result)
{
  double t1 = (float)(b1 + b2 + 1) /(b1 + 1);
  double t2 = (float)(b1 + b2 + 1) /(b2 + 1);
  double adj_A = 0.0;
  double adj_B1 = 0.0;
  double adj_C = 0.0;
  double adj_sum = 0.0;
  long double l1_B = - (x + y + a0) * log(1.0 + b1 + b2) + x * log(b1) + y * log(b2) - a1 * log(1.0 + b1) - a2 * log(1.0 + b2);
  long double l2_B = (y!=0?0.0:(exp(- (x + a0 + a1) * log(1.0 + b1) + x * log(b1) + adj_B1) * p2));
  long double l3_B = (x!=0?0.0:(exp(- (y + a0 + a2) * log(1.0 + b2) + y * log(b2) + adj_B1) * p3));
  long double l4_B = ((x+y!=0)?0.0:( p4 * exp(adj_B1)));
  
  long double sum_A = 0,sum_AC = 0,sum_A_mat = 0,sum_C_mat = 0;
  
  long double R0_mat[x+1][y+1],R1_mat[x+1][y+1],R2_mat[x+1][y+1],log_R0_mat[x+1][y+1],log_R1_mat[x+1][y+1],log_R2_mat[x+1][y+1],log_R0_mat2[x+1],log_R0_mat3[y+1];
  long double l_A_mat[x+1][y+1],l2_A_mat[x+1],l3_A_mat[y+1],log_R1_mat2[x+1],log_R2_mat2[x+1],log_R1_mat3[y+1],log_R2_mat3[y+1];
  long double l_C_mat[x+1][y+1];
  long double l_AC_mat[x+1][y+1];
  
  
  for(int i = 0;i <= x;i++)
  {
    for(int j = 0;j <= y;j++)
    {
      l_A_mat[i][j] = l1(x,y,a0,a1,a2,i,j,adj_A);
      l_C_mat[i][j] = l1_c(t1,t2,i,j,adj_C);
      sum_A_mat = sum_A_mat + l_A_mat[i][j];
      sum_C_mat += l_C_mat[i][j];
      //cout << i <<" "<<j<<" "<< l_A_mat[i][j] << endl;
      //system("pause");
    }
  }
  
  if (l1_B < -200 && log(l2_B + l3_B + l4_B) < 0)
  {
    adj_B1 = ((-l1_B - 200)*1.0 / 100) * 100; // prevent exp(l1_B) from being 0
    l1_B = l1_B + adj_B1;
  }
  l1_B = exp(l1_B) * p1;
  //cout << l1_B <<endl;
  
  while (log(sum_A_mat) > 250)
  {
    sum_A_mat = 0;
    adj_A = adj_A + 200;
    for(int i = 0;i <= x;i++)
    {
      for(int j = 0;j <= y;j++)
      {
        l_A_mat[i][j] = l1(x,y,a0,a1,a2,i,j,adj_A);
        sum_A_mat += l_A_mat[i][j];
      }
    }
  }
  for(int i = 0;i <= x;i++)
  {
    l2_A_mat[i] = l2_A(x, a0, a1, a2, i, adj_A);
    //cout << "i = " << i << ", l2_A_mat[i] = " << l2_A_mat[i] << ", adj_A = "<< adj_A << endl;
  }
  for(int j = 0;j <= y;j++)
  {
    l3_A_mat[j] = l3_A(y, a0, a1, a2, j, adj_A) ;
  }
  while (log(sum_C_mat) > 250)
  {
    sum_C_mat = 0;
    adj_C = adj_C + 200;
    for(int i = 0;i <= x;i++)
    {
      for(int j = 0;j <= y;j++)
      {
        l_C_mat[i][j] = l1_c(t1,t2,i,j,adj_C);
        sum_C_mat += l_C_mat[i][j];
      }
    }
  }
  for(int i = 0;i <= x;i++)
  {
    for(int j = 0;j <= y;j++)
    {
      sum_AC += l_A_mat[i][j]*l_C_mat[i][j]; //code simplified;
      sum_A += l_A_mat[i][j];                //code missed;
    }
  }
  if(log(sum_AC) > 200)
  {
    adj_A = adj_A + 100;
    adj_C = adj_C + 100;
    sum_AC = 0;
    sum_A = 0;
    for(int i = 0;i <= x;i++)
    {
      for(int j = 0;j <= y;j++)
      {
        l_A_mat[i][j] = l1(x,y,a0,a1,a2,i,j,adj_A);
        l_C_mat[i][j] = l1_c(t1,t2,i,j,adj_C);
        sum_AC += l_A_mat[i][j]*l_C_mat[i][j];
        sum_A +=  l_A_mat[i][j];
      }
    }
  }
  else if(log(sum_AC) < - 100)
  {
    adj_A = adj_A - 200;
    adj_C = adj_C - 200;
    sum_AC=0;
    sum_A= 0;
    for(int i = 0;i <= x;i++)
    {
      for(int j = 0;j <= y;j++)
      {
        l_A_mat[i][j] = l1(x,y,a0,a1,a2,i,j,adj_A);
        l_C_mat[i][j] = l1_c(t1,t2,i,j,adj_C);
        l_AC_mat[i][j] = l1_AC(t1,t2,x,y,a0,a1,a2,i,j,adj_A+adj_C);
        sum_AC += l_AC_mat[i][j];
        sum_A +=  l_A_mat[i][j];
      }
    }
  }
  long double l_sum =0;
  //cout << sum_AC << " "<< l2_B << endl;
  l_sum = sum_AC * l1_B + sum_A * (l2_B +  l3_B +  l4_B) * exp(-adj_C);
  //cout << l_sum << " " << sum_AC << " " << l1_B << " " << sum_A << " " << l2_B << " " << l3_B << " " << l4_B << " " << adj_C << endl;
  if (l_sum == 0)
  {
    adj_sum = -floor(log(sum_AC)*2*1.0/3 + log(l1_B)*2*1.0/3);
    //cout << adj_sum<<"adj";
    //cout << sum_AC <<"AC"<< adj_sum <<"@"<< l1_B<<"c"<<adj_C<<endl;
    l_sum = sum_AC * exp(adj_sum) * l1_B + sum_A * (exp(adj_sum) * (l2_B +  l3_B +  l4_B)) * exp(-adj_C);
  }
  long double R0_E1_B,R0_E2_B,R0_E3_B,R0_E4_B,R1_E1_B,R1_E2_B,R1_E3_B,R1_E4_B,R2_E1_B,R2_E2_B,R2_E3_B,R2_E4_B;
  // expectation components
  R0_E1_B = (float)b1/(1 + b1 + b2);
  R0_E2_B = (float)b1/(1 + b1);
  R0_E3_B = (float)b1/(1 + b2);
  R0_E4_B = (float)b1;
  
  R1_E1_B = (float)b1/(1 + b1);
  R1_E2_B = (float)b1/(1 + b1);
  R1_E3_B = (float)b1;
  R1_E4_B = (float)b1;
  
  R2_E1_B = (float)b1/(1 + b2);
  R2_E2_B = (float)b1;
  R2_E3_B = (float)b1/(1 + b2);
  R2_E4_B = (float)b1;
  
  long double log_R0_E = 0,log_R1_E = 0,log_R2_E = 0,R0_E = 0,R1_E = 0,R2_E = 0;
  for(int i = 0;i <= x;i++)
  {
    log_R0_mat2[i] = l2_A_mat[i] * (log_R0_E2(x,a0,i) + log(R0_E2_B));
    log_R1_mat2[i] = l2_A_mat[i] * (log_R1_E1(i,a1) + log (R1_E2_B));
    //cout << "line 238; log_R1_mat2[i] = " << log_R1_mat2[i] << ", l2_A_mat[i] = " << l2_A_mat[i]  << ", logR1E2(x,a0,i) = " << log_R1_E2(i, a1) << ", log(R1E2B)= "<< log(R1_E2_B) << endl;
    log_R2_mat2[i] = l2_A_mat[i] * (boost::math::digamma(a2)+log(R2_E2_B));
    for(int j = 0;j <= y;j++)
    {
      R0_mat[i][j] = R0_E1(x,y,i,j,a0);
      R0_mat[i][j] = R0_mat[i][j]*l_A_mat[i][j];
      R0_E += R0_mat[i][j] * l_C_mat[i][j] * exp(adj_sum) * l1_B * R0_E1_B + R0_mat[i][j] * (l2_B * R0_E2_B + l3_B * R0_E3_B + l4_B * R0_E4_B)*exp(-adj_C + adj_sum);
      R1_mat[i][j] = R1_E1(i,a1);
      R2_mat[i][j] = R2_E1(j,a2);
      R1_mat[i][j] = R1_mat[i][j] * l_A_mat[i][j];
      R2_mat[i][j] = R2_mat[i][j] * l_A_mat[i][j];
      log_R0_mat[i][j] = (log_R0_E1(x,y,i,j,a0) + log(R0_E1_B)) * l_A_mat[i][j];
      log_R1_mat[i][j] = l_A_mat[i][j] * (log_R1_E1(i,a1) + log (R1_E1_B));
      log_R2_mat[i][j] = (log_R2_E1(j,a2) + log(R2_E1_B)) * l_A_mat[i][j];
      //cout << "line252; log_R1_mat[i][j]: " << log_R1_mat[i][j] << ", (log_R1_E1(i,a1) + log (R1_E1_B)): " << (log_R1_E1(i,a1) + log (R1_E1_B)) << ", log_R1_E1(i,a1):" << log_R1_E1(i,a1) << ", log (R1_E1_B): " << log (R1_E1_B) <<endl;
      log_R0_E += log_R0_mat[i][j] * l_C_mat[i][j] * exp(adj_sum - adj_C) * l1_B;
      //cout << "i = " << i << ", j = " << j << ", logR0E = " << log_R0_E << " " << endl;
      log_R1_E += log_R1_mat[i][j] * l_C_mat[i][j] * exp(adj_sum - adj_C) * l1_B;
      //cout << "line257; sum = " << log_R1_mat[i][0] * l_C_mat[i][0] * exp(adj_sum - adj_C) * l1_B << ", lCmat: " <<l_C_mat[i][0] << ", l1B: " << l1_B <<endl;
      //cout << "line257; log_R1_E = " << log_R1_E << endl;
      log_R2_E += log_R2_mat[i][j] * l_C_mat[i][j] * exp(adj_sum - adj_C) * l1_B;
      R1_E += R1_mat[i][j] * l_C_mat[i][j] * exp(adj_sum) * l1_B * R1_E1_B + R1_mat[i][j] * (l2_B * R1_E2_B + l3_B * R1_E3_B + l4_B * R1_E4_B)*exp(-adj_C + adj_sum);
      R2_E += R2_mat[i][j] * l_C_mat[i][j] * exp(adj_sum) * l1_B * R2_E1_B + R2_mat[i][j] * (l2_B * R2_E2_B + l3_B * R2_E3_B + l4_B * R2_E4_B)*exp(-adj_C + adj_sum);
    }
    log_R0_E += (log_R0_mat2[i] * l2_B) * exp(adj_sum);
    //cout << "line262; log_R1_E(cum1) = " << log_R1_E << ", line sum = " << (log_R1_mat2[i] * l2_B) * exp(adj_sum) << "log_R1_E(cum2) = " << log_R1_E + (log_R1_mat2[i] * l2_B) * exp(adj_sum) << endl;
    log_R1_E += (log_R1_mat2[i] * l2_B) * exp(adj_sum);
    log_R2_E += (log_R2_mat2[i] * l2_B) * exp(adj_sum);
    //cout << "line263; cum = " << log_R1_E << " sum = " << (log_R1_mat2[i] * l2_B) * exp(adj_sum) << ", logR1E = " << log_R1_E << ", l2_B = " << l2_B << ", exp(adj_sum) = " << exp(adj_sum) << endl;
    //cout << "logR1mat2.i = " << log_R1_mat2[i] << endl;
  }
  //cout << log_R0_E << " " << endl;
  for(int j = 0; j <= y;j++)
  {
    log_R0_mat3[j] = l3_A_mat[j] * (log_R0_E3(y,a0,j)+log(R0_E3_B));
    log_R1_mat3[j] = l3_A_mat[j] * (boost::math::digamma(a1)+log(R1_E3_B));
    log_R2_mat3[j] = l3_A_mat[j] * (log_R2_E1(j,a2) + log (R2_E3_B));
    log_R0_E += (log_R0_mat3[j] * l3_B) * exp(adj_sum);
    log_R1_E += (log_R1_mat3[j] * l3_B) * exp(adj_sum);
    //cout << "line275; cum = " << log_R1_E << endl;
    log_R2_E += (log_R2_mat3[j] * l3_B) * exp(adj_sum);
  }
  
  
  R0_E = R0_E*1.0 / l_sum;
  R1_E = R1_E*1.0 / l_sum;
  R2_E = R2_E*1.0 / l_sum;
  log_R0_E = log_R0_E + (boost::math::digamma(a0) + log(b1)) * exp(adj_sum) * l4_B;
  log_R0_E = log_R0_E * 1.0 / l_sum;
  log_R1_E = log_R1_E + (boost::math::digamma(a1) + log(b1)) * exp(adj_sum) * l4_B;
  // cout << "line276; sum = " << (log_R1_mat3[0] * l3_B) * exp(adj_sum) << endl;
  // cout << "line285; cum = " << log_R1_E << endl;
  
  // cout  << "log_R1_E : " <<  log_R1_E << endl;
  log_R1_E = log_R1_E * 1.0 / l_sum;
  // cout << ", l3_A_mat[0]: " << l3_A_mat[0]  << ", log(R1_E3_B): " << log(R1_E3_B) << endl;
  // cout << ", log_R1_mat3[0]: " <<  log_R1_mat3[0] << ", l3_B: " << l3_B << endl;
  // cout << ", digamma(a1): " << boost::math::digamma(a1)  << ", log(b1): " <<  log(b1) << ", exp(adj_sum): " <<  exp(adj_sum) << ", l4_B: " <<  l4_B << endl;
  // cout << ", (digamma(a1) + log(b1)): " << boost::math::digamma(a1)  + log(b1) << endl;
  // cout << ", (digamma(a1) + log(b1))*l4_B: " << (boost::math::digamma(a1)  + log(b1)) * l4_B << endl;
  // cout << "l.sum: " <<l_sum << endl;
  log_R2_E = log_R2_E + (boost::math::digamma(a2) + log(b1)) * exp(adj_sum) * l4_B;
  log_R2_E = log_R2_E * 1.0 / l_sum;
  // cout << sum_AC << l1_B << sum_A << l1_B << l2_B << l3_B << l4_B << adj_C << adj_sum << endl;
  double E_E1 = sum_AC * exp(adj_sum) * l1_B;
  double E_E2 = sum_A * l2_B *exp(-adj_C + adj_sum);
  double E_E3 = sum_A * l3_B *exp(-adj_C + adj_sum);
  double E_E4 = sum_A * l4_B *exp(-adj_C + adj_sum);
  
  double su = E_E1+E_E2+E_E3+E_E4;
  E_E1 = E_E1/su;
  E_E2 = E_E2/su;
  E_E3 = E_E3/su;
  E_E4 = E_E4/su;
  //E_E{2} <- E.E/sum(E.E)
  long double v_E = (y==0?0.0:y) + (a0 + a2) * b2 *(E_E2 +E_E4);
  
  /*long double xx = sum_AC * exp(adj_sum) * l1_B * y ;
  cout<<xx<<"&"<<endl;
  long double yy = sum_A * l2_B * a2 * b2*exp(-adj_C + adj_sum) +
  dnbinom(x, a0 + a1 + 1, b1*1.0/(1+b1)) * exp(-adj_A - adj_C + adj_sum) * a0 * b2 * p2 * y==0?1:0 +
  sum_A * l3_B * y *exp(-adj_C + adj_sum) +
  sum_A * l4_B * (a0 + a2) * b2 *exp(-adj_C + adj_sum);
  cout << yy <<"%"<<endl;
  long double v_E = xx + yy;
  v_E= v_E*1.0/l_sum;*/
  
  result[0] += (log(l_sum) + adj_A -adj_B1 + adj_C - adj_sum) * freq;
  result[1] += R0_E * freq ;
  result[2] += R1_E * freq;
  result[3] += R2_E * freq;
  result[4] += log_R0_E * freq;
  result[5] += log_R1_E * freq;
  result[6] += log_R2_E * freq;
  result[7] += E_E1 * freq;
  result[8] += E_E2 * freq;
  result[9] += E_E3 * freq;
  result[10] += E_E4 * freq;
  result[11] += v_E * freq;    //change lines between E_E and v_E;
  //cout << "result" << result << endl;
}
// [[Rcpp::export]]
void dBvZINB_Expt_vec(const IntegerVector &xvec, const IntegerVector &yvec, const IntegerVector &freq, 
                      int &n, double &a0, double &a1, double &a2,
                      double &b1, double &b2, double &p1, double &p2, double &p3, double &p4,
                      NumericVector &result) {
  //const int &XMAX, const int &YMAX) {
  int sumFreq = 0, x, y, f;
  // const int XMAX = 1001;
  // const int YMAX = 1001;
  
  // initialize result
  for (int i = 0; i < 12; i++) {
    result[i] = 0.0;
  }
  
  for (int i = 0; i < n; i++) {
    x = xvec[i]; 
    y = yvec[i];
    f = freq[i];
    sumFreq += freq[i];
    
    // add expectations with weights (freq)
    dBvZINB_Expt(x, y, f, a0, a1, a2,
                 b1, b2, p1, p2, p3, p4, result);
  }
  
  // from sum to mean
  for (int i = 0; i < 12; i++) {
    result[i] /= sumFreq;
  }
}


// 2. optimization
double func(double x, double y) 
{ 
  return (boost::math::digamma(x) - y);
}

double inv_digamma(double x, double y) 
{ 
  double h = func(x, y) / boost::math::trigamma(x);
  while (abs(h) >= EPSILON2)
  {
    h = func(x, y) / boost::math::trigamma(x);
    while (h > x) {
      // cout <<"inv_digamm hit zero. ";
      h /= 2.0;
    }
    x = x - h;
  }
  //cout << endl << "inv_digamm = " << x << endl;
  return(x);
  //cout << "The value of x is : " << x <<;
}


// optimization


void inv_digamma_vec(double lb, NumericVector &expt, NumericVector &a, NumericVector &idgam) 
{ 
  //double idgam[3];
  //double *idgam = new double[3];
  idgam[0] = inv_digamma(a[0], expt[4] + lb);
  idgam[1] = inv_digamma(a[1], expt[5] + lb);
  idgam[2] = inv_digamma(a[2], expt[6] + lb);
  //return(idgam);
}

// double objective(double lb, NumericVector &expt, NumericVector &a, NumericVector &idgam) 
// { 
//   inv_digamma_vec(lb, expt, a, idgam);
//   // cout << "idgam123: "<< idgam[0] << " "<< idgam[1] << " "<< idgam[2] << endl;
//   //  inv_digamma_vec(b1, expt, a, idgam);
//   // double result = (double);
//   return (log(idgam[0] + idgam[1] + idgam[2]) + lb - log(expt[1] + expt[2] + expt[3]));
// }
// 
// // Derivative
// double derivFunc(double lb, NumericVector &expt, NumericVector &a, NumericVector &idgam)
// {
//   inv_digamma_vec(lb, expt, a, idgam);
//   double result = 0.0;
//   for (int i = 0; i < 3; i++) {
//     result += 1/ boost::math::trigamma(idgam[i]);
//   }
//   return ((result)/(idgam[0] + idgam[1] + idgam[2]) + 1.0);
// }

// Derivative
double hfunc(double lb, NumericVector &expt, NumericVector &a, NumericVector &idgam)
{
  inv_digamma_vec(lb, expt, a, idgam);
  double result = 0.0;
  for (int i = 0; i < 3; i++) {
    // cout << "idgam[0:2]" << idgam[0] << " " << idgam[1] << " " << idgam[2] << endl;
    result += (1/ boost::math::trigamma(idgam[i]));
  }
  result = result / (idgam[0] + idgam[1] + idgam[2]) + 1.0;
  result = (log(idgam[0] + idgam[1] + idgam[2]) + lb - log(expt[1] + expt[2] + expt[3])) / result;
  return (result);
}

// [[Rcpp::export]]
void opt_lb(double& lb, NumericVector &expt, NumericVector &a,  NumericVector &idgam)
{
  //double* lb = log(b1);
  // double h =  objective(lb, expt, a, idgam) / derivFunc(lb, expt, a, idgam);
  double h = hfunc(lb, expt, a, idgam);
// cout << "h = " << h << " ";
  while (abs(h) >= EPSILON)
  {
    // h = objective(lb, expt, a, idgam)/derivFunc(lb, expt, a, idgam);
    // while (h > b1) {
    //   // cout << "opt_b1 hit zero. ";
    //   h /= 2.0;
    // }
// cout << "h = " << h << " ";
    lb -= h;
    h = hfunc(lb, expt, a, idgam);
  }
  //b1 = exp(lb);
  // cout << endl << "opt_b1 =" << b1 << endl;
  // double* idgam = inv_digamma_vec(lb, expt, a);
  a[0] = idgam[0];
  a[1] = idgam[1];
  a[2] = idgam[2];
  // cout << "a123: "<< a[0] << " "<< a[1] << " "<< a[2] << " " << exp(lb) << endl;
}



// 3. EM

// [[Rcpp::export]]
void em(NumericVector& param, const IntegerVector &xvec, const IntegerVector &yvec, 
        const IntegerVector &freq, int &n, NumericVector &expt, 
        IntegerVector &iter,
        IntegerVector &maxiter, double &tol, int showFlag)
{
  //int iter = 0;
  double param_diff = 1.0;
  NumericVector param_old(9);
  NumericVector idgam(3);
  
  // cout << "tol = " << tol << endl;  
  while(maxiter[0] > iter[0] && param_diff > tol)
  {
    // cout << "param_diff = " << param_diff << " ";
    if (showFlag == 1) {
      cout << "iter = " << iter[0] << ", a0 = " << param[0] << ", a1 = " << param[1]
           << ", a2 = " << param[2] << ", b1 = " << param[3] << ", b2 = " << param[4] 
           << ", p1 = " << param[5] << ", p2 = " << param[6] << ", p3 = " <<  param[7]
           << ", p4 = " << param[8]  << endl;
    }
    iter[0] += 1;
    for(int i = 0;i < 9;i++)
    {
      param_old[i] = param[i];
    }
    
    dBvZINB_Expt_vec(xvec, yvec, freq, n, param[0], param[1], param[2], param[3], param[4], 
                     param[5], param[6], param[7], param[8], expt);
    double delta = expt[11]*1.0 / (expt[1] + expt[3]);
    for(int i = 0;i < 4; i++)
    {
      param[5 + i] = expt[7 + i];
    }
    double lb = log(param[3]);
    // Finding optimized a0, a1, a2, b1
    opt_lb(lb, expt, param, idgam);
    param[3] = exp(lb);
    param[4] = param[3] * delta;
    
    param_diff = 0.0;
    for(int i = 0;i < 9;i++)
    {
      double dif = abs(0.0 + param[i] - param_old[i]);
      // cout << &param << endl;
      //double dif = abs(0.0 + param[i] - param_old[i]);
      if( dif > param_diff)
      {
        param_diff = dif;
      }
    }
    //cout << "param_diff = " << param_diff << " ";
  }
// cout << "a " << param[0] << " " << param[1] << " " << param[2] << " b " << param[3] << " " << param[4] << " pi "
//      << param[5] << " " << param[6] << " "  << param[7] << " " << param[8] << endl;
}