/*
    Copyright (c) 2008-2016 Jan W. Krieger (<jan@jkrieger.de>)


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

#ifndef JKMATHPARSERDEFAULTLIB_H
#define JKMATHPARSERDEFAULTLIB_H

#include "jkmathparsertools.h"
#include "jkmathparserbase.h"
#include "jkmathparserstringtools.h"


/******************************************************************************************
 * default-functions for farser
 ******************************************************************************************/


namespace JKMathParser_DefaultLib {

    JKMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC(fdoubleToStr, num2str, JKMP::floattostr)
    JKMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC(fIntToStr, int2str, JKMP::inttostr)
    JKMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC(fIntToBinStr, int2bin, JKMP::inttobin)
    JKMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC(fIntToHexStr, int2hex, JKMP::inttohex)
    JKMATHPARSER_DEFINE_1PARAM_NUM2STRING_FUNC(fIntToOctStr, int2oct, JKMP::inttooct)
    JKMATHPARSER_DEFINE_1PARAM_BOOL2STRING_FUNC(fboolToStr, bool2str, JKMP::booltostr)

    JKMATHPARSER_DEFINE_1PARAM_STRING2NUM_FUNC(fStringToNum, str2num, JKMP::stringtofloat)
    JKMATHPARSER_DEFINE_1PARAM_STRING2BOOL_FUNC(fStringToBool, str2bool, JKMP::strtobool)

    JKMATHPARSER_DEFINE_1PARAM_NUM2BOOL_FUNC(fIsNan, isnan, std::isnan)
    JKMATHPARSER_DEFINE_1PARAM_NUM2BOOL_FUNC(fIsInf, isinf, std::isinf)
    JKMATHPARSER_DEFINE_1PARAM_NUM2BOOL_FUNC(fIsFinite, isfinite, std::isfinite)
    JKMATHPARSER_DEFINE_1PARAM_NUM2BOOL_FUNC(fIsFloatOK, isnumok, statisticsFloatIsOK)

    JKMP::string ptosystempathseparator(const JKMP::string& param);
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fToSystemPathSeparator,tosystempathseparator, ptosystempathseparator )

    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(fFaddeevaRealW, faddeeva_real, qfFaddeevaRealW)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(fFactorial, factorial, qfFactorial)
    JKMATHPARSER_DEFINE_2PARAM12VEC_NUMERIC_FUNC(fBinom, binom, qfBinomialCoefficient)
    JKMATHPARSER_DEFINE_2PARAM1VEC_NUMERIC_FUNC(fPoissonPDF, poissonpdf, qfPoissonDist)
    JKMATHPARSER_DEFINE_3PARAM1VEC_NUMERIC_FUNC(fBinomialPDF, binomialpdf, qfBinomialDist)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(fSinc, sinc, qfSinc)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(fTanc, tanc, qfTanc)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fSin, sin)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fCos, cos)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fTan, tan)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fExp, exp)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fLog, log)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fLog10, log10)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fLog2, log2)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fSqrt, sqrt)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fCbrt, cbrt)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fSqr, qfSqr)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(fAbs, abs, fabs)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fASin, asin)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fACos, acos)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fATan, atan)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fSinh, sinh)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fCosh, cosh)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fTanh, tanh)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fASinh, asinh)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fACosh, acosh)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fATanh, atanh)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fErf, erf)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fErfc, erfc)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(flGamma, lgamma)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(ftGamma, tgamma)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fJ0, j0)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fJ1, j1)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fY0, y0)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fY1, y1)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fCeil, ceil)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fFloor, floor)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fTrunc, trunc)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC_SIMPLE(fRound, round)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(fTheta, theta, qfTheta)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(fSigmoid, sigmoid, qfSigmoid)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(fSign, sign, qfSign)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(fDegToRad, deg2rad, qfDegToRad)
    JKMATHPARSER_DEFINE_1PARAM_NUMERIC_FUNC(fRadToDeg, rad2deg, qfRadToDeg)
    JKMATHPARSER_DEFINE_2PARAM1NUM2VEC_VECTONUM_FUNC(fPolynom, polyval, qfEvalPolynomial)
    JKMATHPARSER_DEFINE_1PARAM_NUMERICVEC_FUNC(fDerivPolynom, polyder, qfDerivePolynomial)

    template <class T>
    inline T JKMathParser_sort(const T& value) {
        T m=value;
        qSort(m);
        return m;
    }

    JKMATHPARSER_DEFINE_1PARAM_VECTOR_FUNC(fSort, sort, JKMathParser_sort)

    template <class T>
    inline bool compareGreaterThan(const T &s1, const T &s2)
     {
         return s1 > s2;
     }

    static inline JKMP::vector<double> JKMathParser_dsort(const JKMP::vector<double>& value) {
        JKMP::vector<double> m=value;
        qSort(m.begin(), m.end(), compareGreaterThan<double>);
        return m;
    }

    static inline JKMP::vector<bool> JKMathParser_dsort(const JKMP::vector<bool>& value) {
        JKMP::vector<bool> m=value;
        qSort(m.begin(), m.end(), compareGreaterThan<bool>);
        return m;
    }
    static inline JKMP::stringVector JKMathParser_dsort(const JKMP::stringVector& value) {
        JKMP::stringVector m=value;
        qSort(m.begin(), m.end(), compareGreaterThan<JKMP::string>);
        return m;
    }


    JKMATHPARSER_DEFINE_1PARAM_VECTOR_FUNC(fDSort, dsort, JKMathParser_dsort)


    JKMP::vector<double> JKMathParser_shuffleD(const JKMP::vector<double>& value);
    JKMP::vector<bool> JKMathParser_shuffleB(const JKMP::vector<bool>& value);
    JKMP::stringVector JKMathParser_shuffleS(const JKMP::stringVector& value);

    void fIsValid(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsVoid(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsInt(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsUInt(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsDouble(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsString(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsBool(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsVector(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsBoolVector(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsDoubleVector(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsStringVector(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void fStruct(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fStructKeys(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fStructGet(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fStructGetSave(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void fList(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fListAppend(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fListRemove(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fListGet(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fListGetSave(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fListInsert(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fVec2List(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsList(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsStruct(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsMatrix(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsDoubleMatrix(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIsBoolMatrix(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fMat2Vec(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fVec2Mat(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fReshape(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void fDoubleMatrix(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fBoolMatrix(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void fShuffle(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void fComment(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fAllTrue(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void fAllFalse(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);


    void fAnyTrue(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void fAnyFalse(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void fContains(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fContainsLC(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fReplace(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fReplaceMulti(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fCountOccurences(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void fSplit(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fJoin(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRemoveEmpty(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRepeat(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRepeatString(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(fMean, mean, qfstatisticsAverage)

    JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(fSkewness, skewness, qfstatisticsSkewness)
    JKMATHPARSER_DEFINE_2PARAM1VEC_VECTONUM_FUNC(fCentralMoment, moment, qfstatisticsCentralMoment)
    JKMATHPARSER_DEFINE_2PARAM1VEC_VECTONUM_FUNC(fNonCentralMoment, ncmoment, qfstatisticsMoment)
    JKMATHPARSER_DEFINE_2PARAM2VEC_VECTONUM_FUNC(fCorrcoeff, corrcoeff, qfstatisticsCorrCoeff)
    JKMATHPARSER_DEFINE_2PARAM2VEC_VECTONUM_FUNC(fMandersOverlapCoeff, mandersoverlap, qfstatisticsMandersOverlapCoeff)

    JKMATHPARSER_DEFINE_1PARAM_NUMERICVEC_FUNC(fDiff, diff, qfstatisticsDiff)
    JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(fSum, sum, qfstatisticsSum)
    //JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(fCount, count, qfstatisticsCount)
    JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(fProd, prod, qfstatisticsProd)
    JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(fSum2, sum2, qfstatisticsSum2)
    JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(fVar, var, qfstatisticsVariance)
    JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(fStd, std, qfstatisticsStd)
    JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(fMedian, median, qfstatisticsMedian)
    JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(fMAD, mad, qfstatisticsMADS)
    JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(fNMAD, nmad, qfstatisticsNMADS)
    JKMATHPARSER_DEFINE_2PARAM1VEC_VECTONUM_FUNC(fQuantile, quantile, qfstatisticsQuantile)
    JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(fQuantile25, quantile25, qfstatisticsQuantile25)
    JKMATHPARSER_DEFINE_1PARAM_VECORNUMSTONUM_FUNC(fQuantile75, quantile75, qfstatisticsQuantile75)

    JKMATHPARSER_DEFINE_1PARAM_NUMERICVEC_FUNC(fCumSum, cumsum, qfstatisticsCumSum)
    JKMATHPARSER_DEFINE_1PARAM_NUMERICVEC_FUNC(fCumProd, cumprod, qfstatisticsCumProd)

    JKMATHPARSER_DEFINE_2PARAM2VEC_NUMERIC_FUNC_SIMPLE(fYn, yn)
    JKMATHPARSER_DEFINE_2PARAM2VEC_NUMERIC_FUNC_SIMPLE(fJn, jn)
    static inline double fJn(double a, double b) {
        return jn(a,b);
    }
    static inline double fYn(double a, double b) {
        return yn(a,b);
    }

    JKMATHPARSER_DEFINE_2PARAM1VEC_NUMERIC_FUNC(fSlit, slit, qfSlit)
    JKMATHPARSER_DEFINE_2PARAM12VEC_NUMERIC_FUNC_SIMPLE(fFMod, fmod)
    JKMATHPARSER_DEFINE_2PARAM12VEC_NUMERIC_FUNC_SIMPLE(fATan2, atan2)
    JKMATHPARSER_DEFINE_2PARAM1VEC_NUMERIC_FUNC(fGauss, gaussnn, qfGaussSqrtE)
    JKMATHPARSER_DEFINE_2PARAM1VEC_NUMERIC_FUNC(fGaussDist, gauss, qfGaussNormSqrtE)
    JKMATHPARSER_DEFINE_2PARAM1VEC_NUMERIC_FUNC(fRoundSig, roundsig, roundError)

    static inline JKMP::string JKMathParser_trimm(const JKMP::string& value) {
        return value.trimmed();
    }
    static inline JKMP::string JKMathParser_simplify(const JKMP::string& value) {
        return value.simplified();
    }
    static inline JKMP::string JKMathParser_tolower(const JKMP::string& value) {
        return value.toLower();
    }
    static inline JKMP::string JKMathParser_toupper(const JKMP::string& value) {
        return value.toUpper();
    }
    void fPrintExpression(jkmpResult& r,  JKMathParser::jkmpNode** params, unsigned int n, JKMathParser* p);
    void fPrintExpressionTree(jkmpResult& r,  JKMathParser::jkmpNode** params, unsigned int n, JKMathParser* p);

    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fTrimm, trimm, JKMathParser_trimm)
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fSimplify, simplify, JKMathParser_simplify)
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fToLower, tolower, JKMathParser_tolower)
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fToUpper, toupper, JKMathParser_toupper)

    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fReadFile, readfile, readFile)
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fescapify, escapify, escapify)
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fdeescapify, deescapify, deescapify)
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fcleanStringForFilename, clean_string_for_filename, cleanStringForFilename)


    static inline JKMP::string JKMathParser_extractfilename(const JKMP::string& value) {
        return QFileInfo(value).fileName();
    }
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fextractfilename, extract_filename, JKMathParser_extractfilename)
    static inline JKMP::string JKMathParser_extractfilepath(const JKMP::string& value) {
        return QFileInfo(value).path();
    }
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fextractfilepath, extract_file_path, JKMathParser_extractfilepath)
    static inline JKMP::string JKMathParser_extractfileabspath(const JKMP::string& value) {
        return QFileInfo(value).absolutePath();
    }
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fextractfileabspath, extract_absolute_path, JKMathParser_extractfileabspath)
    static inline JKMP::string JKMathParser_extractfileabsfilepath(const JKMP::string& value) {
        return QFileInfo(value).absoluteFilePath();
    }
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fextractfileabsfilepath, extract_absolute_file_path, JKMathParser_extractfileabsfilepath)
    static inline JKMP::string JKMathParser_extractfileext(const JKMP::string& value) {
        return QFileInfo(value).suffix();
    }
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fextractfileext, extract_ext, JKMathParser_extractfileext)
    static inline JKMP::string JKMathParser_extractfilefullext(const JKMP::string& value) {
        return QFileInfo(value).completeSuffix();
    }
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fextractfilefullext, extract_complete_ext, JKMathParser_extractfilefullext)
    static inline JKMP::string JKMathParser_extractfilebasename(const JKMP::string& value) {
        return QFileInfo(value).baseName();
    }
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fextractfilebasename, extract_basename, JKMathParser_extractfilebasename)
    static inline JKMP::string JKMathParser_extractfilefullbasename(const JKMP::string& value) {
        return QFileInfo(value).completeBaseName();
    }
    JKMATHPARSER_DEFINE_1PARAM_STRING_FUNC(fextractfilefullbasenam, extract_complete_basename, JKMathParser_extractfilefullbasename)

    jkmpResult fSRand(const jkmpResult* params, unsigned int  n, JKMathParser* p);

    static inline double fSRand(double rangeMax, JKMathParser* p) {
        p->get_rng()->seed(uint32_t(rangeMax));
        return 0;
    }

    jkmpResult fRand(const jkmpResult* params, unsigned int  n, JKMathParser* p);

    static inline double fRand(double rangeMin, double rangeMax, JKMathParser* p) {
        return p->get_rng()->rand()*(rangeMax-rangeMin)+rangeMin;
    }
    static inline double fRand(double rangeMax, JKMathParser* p) {
        return p->get_rng()->rand(rangeMax);
    }
    static inline double fRand(JKMathParser* p) {
        return p->get_rng()->rand();
    }

    jkmpResult fRandNorm(const jkmpResult* params, unsigned int  n, JKMathParser* p);

    static inline double fRandNorm(double mean, double var, JKMathParser* p) {
        return p->get_rng()->randNorm(mean,var);
    }
    static inline double fRandNorm(double var, JKMathParser* p) {
        return p->get_rng()->randNorm(0,var);
    }
    static inline double fRandNorm(JKMathParser* p) {
        return p->get_rng()->randNorm(0,1);
    }

    jkmpResult fRandInt(const jkmpResult* params, unsigned int  n, JKMathParser* p);

    static inline double fRandInt(double rangeMin, double rangeMax, JKMathParser* p) {
        return p->get_rng()->randInt()*((uint32_t)rangeMax-(uint32_t)rangeMin)+(uint32_t)rangeMin;
    }
    static inline double fRandInt(double rangeMax, JKMathParser* p) {
        return p->get_rng()->randInt((uint32_t)rangeMax);
    }
    static inline double fRandInt(JKMathParser* p) {
        return p->get_rng()->randInt();
    }




    void fEvalFitFunc(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRandVec(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);


    void fRandIntVec(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRandNormVec(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRandBoolVec(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRandVecSeed(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);


    void fRandIntVecSeed(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRandNormVecSeed(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRandBoolVecSeed(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void fRandBool(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    static inline double fMin(double a,double b) {
        return (a<b)?a:b;
    }

    static inline double fMax(double a,double b) {
        return (a>b)?a:b;
    }


    jkmpResult fMin(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fMax(const jkmpResult* params, unsigned int  n, JKMathParser* p);


    void fLength(jkmpResult &r,const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fSize(jkmpResult &r,const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDimensions(jkmpResult &r,const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRows(jkmpResult &r,const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fColumns(jkmpResult &r,const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRemove(jkmpResult &r,const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fReverse(jkmpResult &r,const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDot(jkmpResult &r,const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fConcat(jkmpResult &r,const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRemoveAll(jkmpResult &r,const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fFind(jkmpResult &r,const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fFindFirst(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fContainsSubString(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fContainsSubStringLC(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fFilterFinite(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fSelect(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fUnique(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedAvg(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedVar(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedStd(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedSum(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedSum2(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedMedian(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedMin(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedMax(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedQuantile(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedSkewness(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedProd(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedCount(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedMoment(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedNonCentralMoment(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fIndexedCorrCoeff(const jkmpResult* params, unsigned int  n, JKMathParser* p) ;
    jkmpResult fIndexedMAD(const jkmpResult* params, unsigned int  n, JKMathParser* p) ;
    jkmpResult fIndexedNMAD(const jkmpResult* params, unsigned int  n, JKMathParser* p) ;
    jkmpResult fReturnLast(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fReturnFirst(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fReturnFirstValid(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fReturnLastValid(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fReturnNth(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fZeros(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fOnes(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fVector(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fLinSpace(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fLogSpace(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fMeshGrid2Dx(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fMeshGrid2Dy(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fLast(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fFirst(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fLastInVector(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fFirstInVector(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fItem(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fItemOrFirst(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    jkmpResult fRunningAverage(const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fTrapz(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void fRegExpCapture(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p, bool minimal, Qt::CaseSensitivity casesens);

    inline void fRegExpCaptureMCS(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        fRegExpCapture(r, params, n, p, true, Qt::CaseSensitive);
    }
    inline void fRegExpCaptureMNCS(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        fRegExpCapture(r, params, n, p, true, Qt::CaseInsensitive);
    }
    inline void fRegExpCaptureCS(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        fRegExpCapture(r, params, n, p, false, Qt::CaseSensitive);
    }
    inline void fRegExpCaptureNCS(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        fRegExpCapture(r, params, n, p, false, Qt::CaseInsensitive);
    }


    void fRegExpContains(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p, bool minimal, Qt::CaseSensitivity casesens);

    inline void fRegExpContainsMCS(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        fRegExpContains(r, params, n, p, true, Qt::CaseSensitive);
    }
    inline void fRegExpContainsMNCS(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        fRegExpContains(r, params, n, p, true, Qt::CaseInsensitive);
    }
    inline void fRegExpContainsCS(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        fRegExpContains(r, params, n, p, false, Qt::CaseSensitive);
    }
    inline void fRegExpContainsNCS(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        fRegExpContains(r, params, n, p, false, Qt::CaseInsensitive);
    }

    void fRegExpIndexIn(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p, bool minimal, Qt::CaseSensitivity casesens);

    inline void fRegExpIndexInMCS(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        fRegExpIndexIn(r, params, n, p, true, Qt::CaseSensitive);
    }
    inline void fRegExpIndexInMNCS(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        fRegExpIndexIn(r, params, n, p, true, Qt::CaseInsensitive);
    }
    inline void fRegExpIndexInCS(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        fRegExpIndexIn(r, params, n, p, false, Qt::CaseSensitive);
    }
    inline void fRegExpIndexInNCS(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        fRegExpIndexIn(r, params, n, p, false, Qt::CaseInsensitive);
    }

    void fDateNum(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDateTimeNum(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDate2Str(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDateTime2Str(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void fDatediff2Secs(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDatediff2Mins(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDatediff2Hours(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDatediff2Days(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fNow(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDateDay(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDateMonth(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDateYear(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDateHour(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDateMin(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fDateSec(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void fVarname(jkmpResult& res, JKMathParser::jkmpNode **nodes, unsigned int n, JKMathParser* p);
    void fType(jkmpResult& res, const jkmpResult* nodes, unsigned int n, JKMathParser* p);
    void fStrVec(jkmpResult& res, const jkmpResult* nodes, unsigned int n, JKMathParser* p);
    void fNumVec(jkmpResult& res, const jkmpResult* nodes, unsigned int n, JKMathParser* p);
    void fBoolVec(jkmpResult& res, const jkmpResult* nodes, unsigned int n, JKMathParser* p);
    void fInt(jkmpResult& res, const jkmpResult* nodes, unsigned int n, JKMathParser* p);
    void fIntVec(jkmpResult& res, const jkmpResult* nodes, unsigned int n, JKMathParser* p);
    void fNum(jkmpResult& res, const jkmpResult* nodes, unsigned int n, JKMathParser* p);
    void fBool(jkmpResult& res, const jkmpResult* nodes, unsigned int n, JKMathParser* p);
    void fEnsureVectorSize(jkmpResult& res, const jkmpResult* nodes, unsigned int n, JKMathParser* p);
    void fEnsureVectorSizeStart(jkmpResult& res, const jkmpResult* nodes, unsigned int n, JKMathParser* p);


    void fHistogram(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fHistogramBins(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRangedHistogram(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fRangedHistogramBins(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);

    void addDefaultFunctions(JKMathParser *p);
    bool hasDefaultFunctions(JKMathParser* p);
    void addDefaultVariables(JKMathParser *p);
    bool hasDefaultVariables(JKMathParser* p);

    void fRegression(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fPolyFit(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fWeightedRegression(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fIRLS(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);



    void fErrorAdd(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fErrorSub(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fErrorMul(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fErrorDiv(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);
    void fErrorPow(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p);




#ifdef QFLIB_LIBRARY
    void fRDRIDs(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetResultIDs(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetEvaluationIDs(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetResult(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetResultError(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetName(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetType(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetFolder(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetGroup(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetDescription(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetRole(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetFiles(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetFileTypes(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetFileDescriptions(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDRGetProperty(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);

    void fEvalIDs(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fEvalGetName(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fEvalGetType(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fEvalGetProperty(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fEvalGetDescription(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p);

    void fProjectGetProperty(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *parser);

    void fRDR_istable(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fTable_column(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fTable_data(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fTable_numcolumn(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fTable_numdata(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fTable_columns(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fTable_columntitles(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fTable_rows(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);


    void fRDR_isoverview(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_overviewwidth(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_overviewheight(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_overviewsize(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_overviewchannels(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_overview(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);

    void fRDR_ismask(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fRDR_maskwidth(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_maskheight(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_masksize(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_mask(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);

    void fRDR_isimageselection(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fRDR_imageselectionwidth(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_imageselectionheight(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_imageselectionsize(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_imageselectioncount(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_imageselectionnames(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_imageselectionlastofname(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_imageselectionfirstofname(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_imageselection(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);

    void fRDR_isrunex(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fRDR_runex_runs(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_runex_isexcluded(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_runex_excluded(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_runex_included(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);
    void fRDR_runex_mask(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p);

    void fFitFunctionParameters(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionIsFit(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionInit(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionParamMins(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionParamMaxs(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionParamAbsMins(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionParamAbsMaxs(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionSetParam(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionSetParamMin(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionSetParamMax(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionSetFix(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionInitFix(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionIDs(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionEval(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionCalc(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionEstimate(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionParamCount(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);

    void fFitFunctionFit(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFitFunctionFitRanged(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);
    void fFit(jkmpResult& res, JKMathParser::jkmpNode **nodes, unsigned int n, JKMathParser* p);
    void fFitRanged(jkmpResult& res, JKMathParser::jkmpNode **nodes, unsigned int n, JKMathParser* p);


    void fFitAlgorithmsIDs(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *parser);

#endif

}


#endif // JKMATHPARSERDEFAULTLIB_H
