/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

    This file is part of QuickFit 3 (http://www.dkfz.de/Macromol/quickfit).

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "jkmpmathtools.h"
#include <float.h>
#include <complex>
#include "./StatisticsTools/statistics_tools.h"

double JKMP::tanc(  double x ) {
    if (x==0) return 1;
    return tan(x)/x;
}


double JKMP::sinc(  double x )
{
    static double const    taylor_0_bound = 3*DBL_MIN ;
    static double const    taylor_2_bound = sqrt(taylor_0_bound);
    static double const    taylor_n_bound = sqrt(taylor_2_bound);

    if    (fabs(x) >= taylor_n_bound)
    {
        return sin(x)/x;
    }
    else
    {
        // approximation by taylor series in x at 0 up to order 0
        double    result = 1;

        if    (fabs(x) >= taylor_0_bound)
        {
            double    x2 = x*x;

            // approximation by taylor series in x at 0 up to order 2
            result -= x2/static_cast<double>(6);

            if    (fabs(x) >= taylor_2_bound)
            {
                // approximation by taylor series in x at 0 up to order 4
                result += (x2*x2)/static_cast<double>(120);
            }
        }

        return result;
    }
}


uint64_t JKMP::factorial( uint64_t kmax) {
    if (kmax<=1) return 1;
    else if (kmax==2) return 2;
    else if (kmax==3) return 6;
    else if (kmax==4) return 24;
    else if (kmax==5) return 120;
    else if (kmax==6) return 720;
    else if (kmax==7) return 5040;
    else if (kmax==8) return 40320;
    else if (kmax==9) return 362880;
    else if (kmax==10) return 3628800;
    uint64_t k=3628800;
    for (uint64_t i=11; i<=kmax; i++) {
        k=k*i;
    }
    return k;
}

uint64_t JKMP::binomialCoefficient(int64_t n, int64_t k) {
    if (n<0 || k<0) return 0;
    if (k>n) return 0;
    if (k==n) return 1;
    if (k==0) return 1;
    uint64_t r1=1;
    uint64_t r2=1;
    for (uint64_t j=1; j<=uint64_t(k); j++) {
        r1=r1*(n+1+j);
        r2=r2*j;
    }
    return r1/r2;
}

double JKMP::poissonDist(int64_t k, double lambda) {
    if (k<0) return 0;
    return pow(lambda, k)*exp(-lambda)/double(JKMP::factorial(uint64_t(k)));
}

double JKMP::binomialDist(int64_t k, int64_t N, double p) {
    if (k<0 || N<0) return 0;
    if (p<=0 || p>1) return 0;
    if (N<k) return 0;
    return double(JKMP::binomialCoefficient(N,k))*pow(p, k)*pow(1.0-p, N-k);
}

double JKMP::roundError(double error, int addSignifcant) {
    if (fabs(error)<DBL_MIN*10.0) return error;
    int sbits_error=ceil(log(fabs(error))/log(10.0));
    double f=pow(10.0, sbits_error-1-addSignifcant);
    //qDebug()<<"roundError("<<error<<addSignifcant<<")  sbits_error="<<sbits_error<<"  f="<<f;
    return round(error/f)*f;
}

double JKMP::roundWithError(double value, double error, int addSignifcant)  {
    if ((fabs(error)<DBL_MIN*10)||(!std::isfinite(error))) return value;

    int sbits_error=ceil(log(fabs(error))/log(10.0));
    if (sbits_error>=0) sbits_error=-1;
    double f=pow(10.0, sbits_error-(addSignifcant+1));

    return round(value/f)*f;
}

