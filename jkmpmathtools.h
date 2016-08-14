/*
    Copyright (c) 2008-2015 Jan W. Krieger, German Cancer Research Center

    

    This file is part of QuickFit 3 (http://www.dkfz.de/Macromol/quickfit).

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef QFMATHTOOLS_H
#define QFMATHTOOLS_H


#include <cmath>
#include <cfloat>
#include <cstdlib>
#include <vector>
#include <stdint.h>
#include "jkmplib_imexport.h"
#include "jkmpstringtools.h"

/*!
    \defgroup tools_math_stat QuickFit Statistics tools
    \ingroup qf3lib_mathtools
  */

/*! \brief Avogadro's number \f$ N_A=6.02214179\cdot10^{23} \f$
    \ingroup tools_math_stat
 */
#define JKMP_NAVOGADRO (6.02214179e23)

/*! \brief Boltzman's constant number \f$ k_B=1.380648813\cdot10^{-23}J/K \f$
    \ingroup tools_math_stat
 */
#define JKMP_K_BOLTZ 1.380648813e-23

/*! \brief square root of pi \f$ \sqrt{\pi}=1.77245385091 \f$
    \ingroup tools_math_stat
 */
#define JKMP_SQRTPI (1.77245385091)

/*! \brief square root of pi \f$ \sqrt{2\pi}=2.50662827463 \f$
    \ingroup tools_math_stat
 */
#define JKMP_SQRT2PI (2.50662827463)

/*! \brief check whether the dlotaing point number is OK (i.e. non-inf, non-NAN)
    \ingroup tools_math_stat
 */
#define JKMP_FloatIsOK(v) (std::isfinite(v) && (!std::isinf(v)) && (!std::isnan(v)))


/*! \brief machine precision for double
    \ingroup tools_math_stat
 */
#define JKMP_DBL_EPSILON        2.2204460492503131e-16
/*! \brief square-root of the machine precision for double
    \ingroup tools_math_stat
 */
#define JKMP_SQRT_DBL_EPSILON   1.4901161193847656e-08

namespace JKMP {

    /*! \brief squares the argument
        \ingroup tools_math_stat
     */
    inline double sqr(const double& x) { return x*x; }
    inline float sqr(const float& x) { return x*x; }
    inline double sqrNR( double x) { return x*x; }
    inline float sqrNR( float x) { return x*x; }

    /*! \brief takes the argument to the third power
        \ingroup tools_math_stat
     */
    inline double cube(const double& x) { return x*x*x; }
    inline float cube(const float& x) { return x*x*x; }
    inline double cubeNR( double x) { return x*x*x; }
    inline float cubeNR( float x) { return x*x*x; }

    /*! \brief takes the argument to the fourth power
        \ingroup tools_math_stat
     */
    inline double pow4(const double& x) { double x2=x*x; return x2*x2; }
    inline float pow4(const float& x) { float x2=x*x; return x2*x2; }

    /*! \brief takes the argument to the fifth power
        \ingroup tools_math_stat
     */
    inline double pow5(const double& x) { double x2=x*x; return x2*x2*x; }
    inline float pow5(const float& x) { float x2=x*x; return x2*x2*x; }

    /** \brief sinc function \f$ \mbox{sinc}(x)=\frac{\sin(x)}{x} \f$
        \ingroup tools_math_stat
    */
    JKMPLIB_EXPORT double sinc( double x);
    /** \brief sinc function \f$ \mbox{tanc}(x)=\frac{\tan(x)}{x} \f$
        \ingroup tools_math_stat
    */
    JKMPLIB_EXPORT double tanc( double x);

    /** \brief 1/sqrt(e)-width gaussian function \f$ \mbox{g}(x, \sigma)=\exp\left(-\frac{1}{2}\cdot\frac{x^2}{\sigma^2}\right) \f$ with \f$ g(\sigma,\sigma)=1/\sqrt{e} \f$
        \ingroup tools_math_stat
     */
    inline double gaussSqrtE( double x,  double sigma) {
        return exp(-0.5*x*x/(sigma*sigma));
    }
    /** \brief 1/sqrt(e)-width gaussian function \f$ \mbox{g}(x, \sigma=1)=\exp\left(-\frac{1}{2}\cdot\frac{x^2}{\sigma^2}\right) \f$ with \f$ g(\sigma,\sigma)=1/\sqrt{e} \f$
        \ingroup tools_math_stat
     */
    inline double gaussSqrtE( double x) {
        return gaussSqrtE(x, 1);
    }
    /** \brief normalized 1/sqrt(e)-width gaussian function \f$ \mbox{g}(x, \sigma)=\frac{1}{w\cdot\sqrt{2\pi}}\cdot\exp\left(-\frac{1}{2}\cdot\frac{x^2}{\sigma^2}\right) \f$ with \f$ g(\sigma,\sigma)=1/\sqrt{e} \f$
        \ingroup tools_math_stat
     */
    inline double gaussNormSqrtE( double x,  double sigma){
        return exp(-0.5*x*x/(sigma*sigma))/(sigma*sqrt(2.0*M_PI));
    }
    /** \brief normalized 1/sqrt(e)-width gaussian function \f$ \mbox{g}(x, \sigma=1)=\frac{1}{w\cdot\sqrt{2\pi}}\cdot\exp\left(-\frac{1}{2}\cdot\frac{x^2}{\sigma^2}\right) \f$ with \f$ g(\sigma,\sigma)=1/\sqrt{e} \f$
        \ingroup tools_math_stat
     */
    inline double gaussNormSqrtE( double x){
        return gaussNormSqrtE(x,1);
    }
    /** \brief 1/e²-width gaussian function \f$ \mbox{g}(x, w)=\exp\left(-2\cdot\frac{x^2}{w^2}\right) \f$ with \f$ g(w,w)=1/e^2 \f$
        \ingroup tools_math_stat
     */
    inline double gaussE2(double x, double w) { return gaussSqrtE(x, w/2.0); }
    /** \brief 1/e²-width gaussian function \f$ \mbox{g}(x, w)=\exp\left(-2\cdot\frac{x^2}{w^2}\right) \f$ with \f$ g(w,w)=1/e^2 \f$
        \ingroup tools_math_stat
     */
    inline double gaussE2(double x) { return gaussSqrtE(x, 1.0/2.0); }
    /** \brief normalized 1/e²-width gaussian function \f$ \mbox{g}(x, w)=\frac{\sqrt{2}}{w\cdot\sqrt{\pi}}\cdot\exp\left(-2\cdot\frac{x^2}{w^2}\right) \f$
        \ingroup tools_math_stat
     */
    inline double gaussNormE2(double x, double w) { return gaussNormSqrtE(x, w/2.0); }
    /** \brief normalized 1/e²-width gaussian function \f$ \mbox{g}(x, w)=\frac{\sqrt{2}}{w\cdot\sqrt{\pi}}\cdot\exp\left(-2\cdot\frac{x^2}{w^2}\right) \f$
        \ingroup tools_math_stat
     */
    inline double gaussNormE2(double x) { return gaussNormSqrtE(x, 1.0/2.0); }
    /** \brief Theta step function \f$ \Theta(x)=\begin{cases}0&x<0\\1&\text{else}\end{cases} \f$
        \ingroup tools_math_stat
     */
    inline double theta(double x) { return (x>=0)?1.0:0.0; }
    /** \brief sigmoidal function \f$ \mbox{sigmoid}(x)=\frac{1}{1+\exp(-x)} \f$
        \ingroup tools_math_stat
     */
    inline double sigmoid(double x) { return 1.0/(1+exp(-1.0*x)); }
    /** \brief sigmoidal function \f$ \mbox{sigmoid}(x)=\frac{1}{1+\exp(-x)} \f$
        \ingroup tools_math_stat
     */
    inline double sigmoid(double x, double width, double x0=0) { return 1.0/(1+exp(-1.0*(x-x0)/width)); }
    /** \brief sign function \f$ \mbox{signa}(x)=\begin{cases}-1&x<0\\0&x=0\\1&x>0\end{cases} \f$
        \ingroup tools_math_stat
     */
    inline double sign(double x) {
        if (x<0) return -1;
        if (x>0) return 1;
        return 0;
    }

    /** \brief range/slit function \f$ \mbox{slit}(x)=\begin{cases}1&-a/2\leq x\leq a/2\\0&\text{else}\end{cases} \f$
        \ingroup tools_math_stat
     */
    inline double slit(double x, double a) {
        if (x>=-a/2.0 && x<=a/2.0) return 1;
        return 0;
    }

    /** \brief evaluate the polynomial defined by coefficients at the position x
        \ingroup tools_math_stat
     */
    inline double evalPolynomial(double x, const double* coefficients, int NC) {
        register double r=0;
        register double xf=1;
        for (register int i=0; i<NC; i++) {
            r+=coefficients[i]*xf;
            xf=xf*x;
        }
        return r;
    }

    /** \brief evaluate the polynomial defined by coefficients at the position x
        \ingroup tools_math_stat
     */
    inline double evalPolynomial(double x, const std::vector<double>& coefficients) {
        return evalPolynomial(x, coefficients.data(), coefficients.size());
    }


    /** \brief derive the polynomial defined by coefficients at the position x
        \ingroup tools_math_stat
     */
    inline std::vector<double> derivePolynomial(const std::vector<double>& coefficients) {
        std::vector<double> r;
        if (coefficients.size()<=1) {
            r.push_back(0.0);
            return r;
        }
        for (size_t i=1; i<coefficients.size(); i++) {
            r.push_back(coefficients[i]*double(i));
        }
        return r;
    }


    /** \brief convert degrees (0..360) ro radians (0..2pi)
        \ingroup tools_math_stat
     */
    inline double degToRad(double x) {
        return x/180.0*M_PI;
    }

    /** \brief convert degrees (0..360) ro radians (0..2pi)
        \ingroup tools_math_stat
     */
    inline double radToDeg(double x) {
        return x/M_PI*180.0;
    }

    /** \brief normalized cauchy distribution
        \ingroup tools_math_stat
     */
    inline double cauchy(double x, double fwhm, double center=0.0) {
        return 1.0/(sqr((x-center)/fwhm)+1.0)/M_PI/fwhm;
    }

    /** \brief cauchy distribution with given amplitude
        \ingroup tools_math_stat
     */
    inline double cauchy(double x, double A, double fwhm, double center) {
        return A/(sqr((x-center)/fwhm)+1.0);
    }



    /** \brief factorial \f$ k!=k\cdot (k-1)\cdot(k-2)\cdot...\cdot2\cdot1\f$ of a number k
        \ingroup tools_math_stat

     */
    JKMPLIB_EXPORT uint64_t factorial( uint64_t kmax);

    /** \brief factorial \f$ k!=k\cdot (k-1)\cdot(k-2)\cdot...\cdot2\cdot1\f$ of a number k
        \ingroup tools_math_stat

     */
    inline double factorial( double kmax) {
        if (kmax<=1) return 1;
        return factorial(uint64_t(round(kmax)));
    }


    /** \brief binomial coefficient \$\left(\stackrel{n}{k}\right)\f$
        \ingroup tools_math_stat

         \[ \left(\stackrel{n}{k}\right)=\frac{n!}{k!\cdot(n-k)!}=\prod\limits_{j=1}^k\frac{n+1-j}{j}  \f]
     */
    JKMPLIB_EXPORT uint64_t binomialCoefficient(int64_t n, int64_t k);


    /** \brief factorial \f$ k!=k\cdot (k-1)\cdot(k-2)\cdot...\cdot2\cdot1\f$ of a number k
        \ingroup tools_math_stat

     */
    inline double binomialCoefficient( double n, double k) {
        return binomialCoefficient(int64_t(round(n)), int64_t(round(k)));
    }


    /** \brief Poisson distribution
        \ingroup tools_math_stat

         \[ P_\lambda(k)=\frac{\lambda^k}{k!}\cdot\mathrm{e}^{-\lambda} \f]
     */
    JKMPLIB_EXPORT double poissonDist(int64_t k, double lambda);


    /** \brief Poisson distribution
        \ingroup tools_math_stat

         \[ B(k|N,p)=\left(\stackrel{N}{k}\right)\cdotp^k\cdot(1-p)^{N-k} \f]
     */
    JKMPLIB_EXPORT double binomialDist(int64_t k, int64_t N, double p);


    /** \brief calculate the error propagation for <code>factorA*a+factorB*b</code> or <code>factorA*a-factorB*b</code> with errors \a ea and \a eb
        \ingroup tools_math_stat

        \f[ f=\alpha a\pm\beta b \f]
        \f[ \Delta f=\sqrt{\left(\Delta a\cdot\alpha\right)^2+\left(\Delta b\cdot\beta\right)^2} \f]
     */
    inline double errorSumMinus(const double& /*a*/, const double& ea, const double& /*b*/, const double& eb, const double& factorA=1.0, const double& factorB=1.0) {
        return sqrt(sqr(factorA*ea)+sqr(factorB*eb));
    }

    /** \brief calculate the error propagation for <code>factorA*a+factorB*b</code> or <code>factorA*a-factorB*b</code> with errors \a ea and \a eb
        \ingroup tools_math_stat

        \f[ f=\alpha a+\beta b \f]
        \f[ \Delta f=\sqrt{\left(\Delta a\cdot\alpha\right)^2+\left(\Delta b\cdot\beta\right)^2} \f]
     */
    inline double errorAdd(const double& /*a*/, const double& ea, const double& /*b*/, const double& eb) {
        return sqrt(sqr(ea)+sqr(eb));
    }

    /** \brief calculate the error propagation for <code>factorA*a+factorB*b</code> or <code>factorA*a-factorB*b</code> with errors \a ea and \a eb
        \ingroup tools_math_stat

        \f[ f=\alpha a-\beta b \f]
        \f[ \Delta f=\sqrt{\left(\Delta a\cdot\alpha\right)^2+\left(\Delta b\cdot\beta\right)^2} \f]
     */
    inline double errorSub(const double& /*a*/, const double& ea, const double& /*b*/, const double& eb) {
        return sqrt(sqr(ea)+sqr(eb));
    }

    /** \brief calculate the error propagation for <code>a*b</code> with errors \a ea and \a eb
        \ingroup tools_math_stat

        \f[ f=a\cdot b \f]
        \f[ \Delta f=\sqrt{\left(\Delta a\cdot b\right)^2+\left(\Delta b\cdot a\right)^2} \f]
     */
    inline double errorMul(const double& a, const double& ea, const double& b, const double& eb) {
        return sqrt(sqr(ea*b)+sqr(eb*a));
    }

    /** \brief calculate the error propagation for <code>a*b</code> with errors \a ea and \a eb
        \ingroup tools_math_stat

        \f[ f=a\cdot b\bcdot c \f]
        \f[ \Delta f=\sqrt{\left(\Delta a\cdot b\right)^2+\left(\Delta b\cdot a\right)^2} \f]
     */
    inline double errorMul(const double& a, const double& ea, const double& b, const double& eb, const double& c, const double& ec) {
        return sqrt(sqr(ea*b*c)+sqr(eb*a*c)+sqr(ec*b*a));
    }

    /** \brief calculate the error propagation for <code>a*b</code> with errors \a ea and \a eb
        \ingroup tools_math_stat

        \f[ f=a\cdot b\bcdot c+d\Cdot e\cdot f \f]
        \f[ \Delta f=\sqrt{\left(\Delta a\cdot b\right)^2+\left(\Delta b\cdot a\right)^2} \f]
     */
    inline double errorMulAdd(const double& a, const double& ea, const double& b, const double& eb, const double& c, const double& ec, const double& d, const double& ed, const double& e, const double& ee, const double& f, const double& ef) {
        return sqrt(sqr(ea*b*c)+sqr(eb*a*c)+sqr(ec*a*b)+sqr(ed*e*f)+sqr(ee*d*f)+sqr(ef*d*e));
    }

    /** \brief calculate the error propagation for <code>a/b</code> with errors \a ea and \a eb
        \ingroup tools_math_stat
        \f[ f=\frac{a}{b} \f]
        \f[ \Delta f=\sqrt{\left(\Delta a\cdot b\right)^2+\left(\Delta b\cdot\frac{1}{b^2}\right)^2} \f]
     */
    inline double errorDiv(const double& a, const double& ea, const double& b, const double& eb) {
        return sqrt(sqr(ea*b)+sqr(eb*a/b/b));
    }

    /** \brief calculate the error propagation for <code>a^(factorB*b)</code> with errors \a ea and \a eb
        \ingroup tools_math_stat
        \f[ f=a^{\beta b} \f]
        \f[ \Delta f=\sqrt{\left(\Delta a\cdot a^{\beta b-1}\right)^2+\left(\Delta b\cdot a^{\beta b}\beta\cdot\log(a)\right)^2} \f]
     */
    inline double errorPow(const double& a, const double& ea, const double& b, const double& eb, const double& factorB=1.0) {
        return sqrt(sqr(ea*pow(a, factorB*b-1.0))+sqr(eb*pow(a, factorB*b)*factorB*log(a)));
    }

    /** \brief calculate the error propagation for <code>sqrt(factorA*a+offsetA)</code> with errors \a ea
        \ingroup tools_math_stat
        \f[ f=\sqrt{\alpha a+o} \f]
        \f[ \Delta f=\left|\left(\frac{\Delta a\cdot\alpha}{2\sqrt{\alpha a+o}}\right)\right| \f]
     */
    inline double errorSqrt(double a, double ea, double factorA=1.0, double offsetA=0.0) {
        return fabs(ea/sqrt(factorA*a+offsetA)*factorA/2.0);
    }

    /** \brief calculate the error propagation for <code>sqrt(factorA*a+factorB*b+offset)</code> with errors \a ea
        \ingroup tools_math_stat
        \f[ f=\sqrt{\alpha a+\beta b+o} \f]
        \f[ \Delta f=\sqrt{\left(\frac{\Delta a\cdot\alpha}{2\sqrt{\alpha a+\beta b+o}}\right)^2+\left(\frac{\Delta b\cdot\beta}{2\sqrt{\alpha a+\beta b+o}}\right)^2} \f]
     */
    inline double errorSqrt(double a, double ea, double b, double eb, double factorA=1.0, double factorB=1.0, double offset=0.0) {
        const double f=2.0*sqrt(factorA*a+factorB*b+offset);
        return sqrt(sqr(ea/f*factorA)+sqr(eb/f*factorB));
    }

    /** \brief calculate the error propagation for <code>sin(factorA*a+factorB*b+offset)</code> with errors \a ea
        \ingroup tools_math_stat
        \f[ f=\sin(\alpha a+\beta b+o) \f]
        \f[ \Delta f=\sqrt{\left(\Delta a\cdot\alpha\cdot\cos(\alpha a+\beta b+o)\right)^2+\left(\Delta b\cdot\beta\cdot\cos(\alpha a+\beta b+o)\right)^2} \f]
     */
    inline double errorSin(double a, double ea, double b, double eb, double factorA=1.0, double factorB=1.0, double offset=0.0) {
        const double f=cos(factorA*a+factorB*b+offset);
        return sqrt(sqr(ea*f*factorA)+sqr(eb*f*factorB));
    }

    /** \brief calculate the error propagation for <code>cos(factorA*a+factorB*b+offset)</code> with errors \a ea
        \ingroup tools_math_stat
        \f[ f=\cos(\alpha a+\beta b+o) \f]
        \f[ \Delta f=\sqrt{\left(\Delta a\cdot\alpha\cdot\sin(\alpha a+\beta b+o)\right)^2+\left(\Delta b\cdot\beta\cdot\sin(\alpha a+\beta b+o)\right)^2} \f]
     */
    inline double errorCos(double a, double ea, double b, double eb, double factorA=1.0, double factorB=1.0, double offset=0.0) {
        const double f=sin(factorA*a+factorB*b+offset);
        return sqrt(sqr(ea*f*factorA)+sqr(eb*f*factorB));
    }

    /** \brief calculate the error propagation for <code>tan(factorA*a+factorB*b+offset)</code> with errors \a ea
        \ingroup tools_math_stat
        \f[ f=\tan(\alpha a+\beta b+o) \f]
        \f[ \Delta f=\sqrt{\left(\Delta a\cdot\alpha\cdot\left[\cos(\alpha a+\beta b+o)\right]^{-2}\right)^2+\left(\Delta b\cdot\beta\cdot\left[\sin(\alpha a+\beta b+o)\right]^{-2}\right)^2} \f]
     */
    inline double errorTan(double a, double ea, double b, double eb, double factorA=1.0, double factorB=1.0, double offset=0.0) {
        const double f=1.0/sqr(cos(factorA*a+factorB*b+offset));
        return sqrt(sqr(ea*f*factorA)+sqr(eb*f*factorB));
    }


    /** \brief calculate the sqrt of a sum of squares
        \ingroup tools_math_stat
        \f[ f=\sqrt{\sum\limits_ia_i^2} \f]
     */
    inline double errorSqrtSumSqr(double a1, double a2) {
        return sqrt(sqr(a1)+sqr(a2));
    }

    /** \brief calculate the sqrt of a sum of squares
        \ingroup tools_math_stat
        \f[ f=\sqrt{\sum\limits_ia_i^2} \f]
     */
    inline double errorSqrtSumSqr(double a1, double a2, double a3) {
        return sqrt(sqr(a1)+sqr(a2)+sqr(a3));
    }
    /** \brief calculate the sqrt of a sum of squares
        \ingroup tools_math_stat
        \f[ f=\sqrt{\sum\limits_ia_i^2} \f]
     */
    inline double errorSqrtSumSqr(double a1, double a2, double a3, double a4) {
        return sqrt(sqr(a1)+sqr(a2)+sqr(a3)+sqr(a4));
    }

    /** \brief calculate the sqrt of a sum of squares
        \ingroup tools_math_stat
        \f[ f=\sqrt{\sum\limits_ia_i^2} \f]
     */
    inline double errorSqrtSumSqr(double a1, double a2, double a3, double a4, double a5) {
        return sqrt(sqr(a1)+sqr(a2)+sqr(a3)+sqr(a4)+sqr(a5));
    }


    /** \brief calculate the sqrt of a sum of squares
        \ingroup tools_math_stat
        \f[ f=\sqrt{\sum\limits_ia_i^2} \f]
     */
    inline double errorSqrtSumSqr(double a1, double a2, double a3, double a4, double a5, double a6) {
        return sqrt(sqr(a1)+sqr(a2)+sqr(a3)+sqr(a4)+sqr(a5)+sqr(a6));
    }
    /** \brief calculate the sqrt of a sum of squares
        \ingroup tools_math_stat
        \f[ f=\sqrt{\sum\limits_ia_i^2} \f]
     */
    inline double errorSqrtSumSqr(double a1, double a2, double a3, double a4, double a5, double a6, double a7) {
        return sqrt(sqr(a1)+sqr(a2)+sqr(a3)+sqr(a4)+sqr(a5)+sqr(a6)+sqr(a7));
    }
    /** \brief calculate the sqrt of a sum of squares
        \ingroup tools_math_stat
        \f[ f=\sqrt{\sum\limits_ia_i^2} \f]
     */
    inline double errorSqrtSumSqr(double a1, double a2, double a3, double a4, double a5, double a6, double a7, double a8) {
        return sqrt(sqr(a1)+sqr(a2)+sqr(a3)+sqr(a4)+sqr(a5)+sqr(a6)+sqr(a7)+sqr(a8));
    }
    /** \brief calculate the sqrt of a sum of squares
        \ingroup tools_math_stat
        \f[ f=\sqrt{\sum\limits_ia_i^2} \f]
     */
    inline double errorSqrtSumSqr(double a1, double a2, double a3, double a4, double a5, double a6, double a7, double a8, double a9) {
        return sqrt(sqr(a1)+sqr(a2)+sqr(a3)+sqr(a4)+sqr(a5)+sqr(a6)+sqr(a7)+sqr(a8)+sqr(a9));
    }

    /** \brief calculate the sqrt of a sum of squares
        \ingroup tools_math_stat
        \f[ f=\sqrt{\sum\limits_ia_i^2} \f]
     */
    inline double errorSqrtSumSqr(double a1, double a2, double a3, double a4, double a5, double a6, double a7, double a8, double a9, double a10) {
        return sqrt(sqr(a1)+sqr(a2)+sqr(a3)+sqr(a4)+sqr(a5)+sqr(a6)+sqr(a7)+sqr(a8)+sqr(a9)+sqr(a10));
    }



    /*! \brief fill the given vector with linear coordinates
        \ingroup tools_math_stat

    */
    template <typename T>
    inline void gridX(T& X, typename T::value_type x0, long long Nx, typename T::value_type dx) {
        typename T::value_type x;
        X.resize(Nx);
        x=x0;
        for (long long xi=0; xi<Nx; xi++) {
            X[xi]=x;
            x=x+dx;
        }
    }





    /*! \brief fill the given vector with (row-major) X,Y coordinates
        \ingroup tools_math_stat

    */
    template <typename T>
    inline void gridXY(T& X, T& Y, typename T::value_type x0, typename T::value_type y0, long long Nx, long long Ny, typename T::value_type dx, typename T::value_type dy) {
        typename T::value_type x;
        typename T::value_type y;
        X.resize(Nx*Ny);
        Y.resize(Nx*Ny);
        y=y0;
        for (long long yi=0; yi<Ny; yi++) {
            x=x0;
            for (long long xi=0; xi<Nx; xi++) {
                X[yi*Nx+xi]=x;
                Y[yi*Nx+xi]=y;
                x=x+dx;
            }
            y=y+dy;
        }
    }




    /*! \brief fill the given vector with (row-major) X,Y,Z coordinates
        \ingroup tools_math_stat

    */
    template <typename T>
    inline void gridXYZ(T& X, T& Y, T& Z, typename T::value_type x0, typename T::value_type y0, typename T::value_type z0, long long Nx, long long Ny, long long Nz, typename T::value_type dx, typename T::value_type dy, typename T::value_type dz) {
        typename T::value_type x;
        typename T::value_type y;
        typename T::value_type z;
        X.resize(Nx*Ny*Nz);
        Y.resize(Nx*Ny*Nz);
        Z.resize(Nx*Ny*Nz);
        z=z0;
        for (long long zi=0; zi<Nz; zi++) {
            y=y0;
            for (long long yi=0; yi<Ny; yi++) {
                x=x0;
                for (long long xi=0; xi<Nx; xi++) {
                    X[zi*Nx*Ny+yi*Nx+xi]=x;
                    Y[zi*Nx*Ny+yi*Nx+xi]=y;
                    Z[zi*Nx*Ny+yi*Nx+xi]=z;
                    x=x+dx;
                }
                y=y+dy;
            }
            z=z+dz;
        }
    }






    /*! \brief round a value according to the error
        \ingroup tools_math_stat

        this returns a rounded value that equals the input value with \a addSignifcant mor
        significant digits than the error

        So with \a addSignifcant = 2 The value 1.23456789&pm;0.001
        will be rounded to 1.23456&pm;0.001
    */
    JKMPLIB_EXPORT double roundWithError(double value, double error, int addSignifcant=1) ;

    /*! \brief round a value to its first 1+\a addSignifcant significant digits
        \ingroup tools_math_stat

    */
    JKMPLIB_EXPORT double roundError(double error, int addSignifcant) ;
    inline double roundError(double error) {
        return roundError(error, 1);
    }
    inline double roundError(double error, double addSignifcant) {
        return roundError(error, int(addSignifcant));
    }


}
#endif // QFMATHTOOLS_H
