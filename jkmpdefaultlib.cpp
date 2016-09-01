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

#include "jkmathparser.h"
#include <typeinfo>
#include <regex>
#include "jkmpdefaultlib.h"
#include "./StatisticsTools/statistics_tools.h"


#define JKMATHPARSER_REGISTER_FUNC(FName, NAME_IN_PARSER) p->addFunction(#NAME_IN_PARSER, JKMathParser_DefaultLib::FName);


bool JKMathParser_DefaultLib::hasDefaultVariables(JKMathParser* p)
{
    return p->functionExists("pi")
            && p->functionExists("e")
            && p->functionExists("log10e")
            && p->functionExists("hbar")
            && p->functionExists("muB");
}

void JKMathParser_DefaultLib::addDefaultVariables(JKMathParser* p)
{

    p->addVariableString("stdlib_version", JKMP::string("0.4 (p%1)").arg(JKMATHPARSER_VERSION));


    p->addVariableDouble("pi", M_PI);
    p->addVariableDouble("e", M_E);
    p->addVariableDouble("sqrt2", sqrt(2));
    p->addVariableDouble("log2e", M_LOG2E);
    p->addVariableDouble("log10e", M_LOG10E);
    p->addVariableDouble("ln2", M_LN2);
    p->addVariableDouble("ln10", M_LN10);
    p->addVariableDouble("h", 6.6260689633E-34);
    p->addVariableDouble("hbar", 1.05457162853E-34);
    p->addVariableDouble("epsilon0", 8.854187817E-12);
    p->addVariableDouble("mu0", 12.566370614E-7);
    p->addVariableDouble("c", 299792458);
    p->addVariableDouble("ce", 1.60217648740E-19);
    p->addVariableDouble("muB", 927.40091523E-26);
    p->addVariableDouble("muB_eV", 5.788381755579E-5);
    p->addVariableDouble("muN", 5.0507832413E-27);
    p->addVariableDouble("muN_eV", 3.152451232645E-8);
    p->addVariableDouble("me", 9.1093821545E-31);
    p->addVariableDouble("mp", 1.67262163783E-27);
    p->addVariableDouble("mn", 1.67492721184E-27);
    p->addVariableDouble("NA", JKMP_NAVOGADRO);
    p->addVariableDouble("kB", JKMP_K_BOLTZ);
    p->addVariableDouble("kB_eV", 8.61734315E-5);
}

bool JKMathParser_DefaultLib::hasDefaultFunctions(JKMathParser* p)
{
    return p->functionExists("sinc")
            && p->functionExists("erf")
            && p->functionExists("polyval")
            && p->functionExists("randvec")
            && p->functionExists("bool2str");
}


void JKMathParser_DefaultLib::addDefaultFunctions(JKMathParser* p)
{
    p->addFunction("comment", JKMathParser_DefaultLib::fComment);
    p->addFunction("repeatstring", JKMathParser_DefaultLib::fRepeatString);
    p->addFunction("repeat", JKMathParser_DefaultLib::fRepeat);


    p->addFunction("doublematrix", JKMathParser_DefaultLib::fDoubleMatrix);
    p->addFunction("nummatrix", JKMathParser_DefaultLib::fDoubleMatrix);
    p->addFunction("boolmatrix", JKMathParser_DefaultLib::fBoolMatrix);

    p->addFunction("list", JKMathParser_DefaultLib::fList);
    p->addFunction("listappend", JKMathParser_DefaultLib::fListAppend);
    p->addFunction("listinsert", JKMathParser_DefaultLib::fListInsert);
    p->addFunction("listremove", JKMathParser_DefaultLib::fListRemove);
    p->addFunction("listget", JKMathParser_DefaultLib::fListGet);
    p->addFunction("listgetsave", JKMathParser_DefaultLib::fListGetSave);
    p->addFunction("vec2list", JKMathParser_DefaultLib::fVec2List);
    p->addFunction("dimensions", JKMathParser_DefaultLib::fDimensions);
    p->addFunction("size", JKMathParser_DefaultLib::fSize);
    p->addFunction("sizerows", JKMathParser_DefaultLib::fRows);
    p->addFunction("sizecols", JKMathParser_DefaultLib::fColumns);
    p->addFunction("sizecolumns", JKMathParser_DefaultLib::fColumns);
    p->addFunction("islist", JKMathParser_DefaultLib::fIsList);
    p->addFunction("vec2list", JKMathParser_DefaultLib::fVec2List);

    p->addFunction("vec2mat", JKMathParser_DefaultLib::fVec2Mat);
    p->addFunction("mat2vec", JKMathParser_DefaultLib::fMat2Vec);
    p->addFunction("reshape", JKMathParser_DefaultLib::fReshape);


    p->addFunction("struct", JKMathParser_DefaultLib::fStruct);
    p->addFunction("structkeys", JKMathParser_DefaultLib::fStructKeys);
    p->addFunction("structget", JKMathParser_DefaultLib::fStructGet);
    p->addFunction("structsaveget", JKMathParser_DefaultLib::fStructGetSave);
    p->addFunction("isstruct", JKMathParser_DefaultLib::fIsStruct);

    p->addFunction("sinc", JKMathParser_DefaultLib::fSinc, NULL, JKMP::sinc);
    p->addFunction("factorial", JKMathParser_DefaultLib::fFactorial, NULL, JKMP::factorial);
    p->addFunction("binomial", JKMathParser_DefaultLib::fBinom);
    p->addFunction("poisspdf", JKMathParser_DefaultLib::fPoissonPDF);
    p->addFunction("binopdf", JKMathParser_DefaultLib::fBinomialPDF);
    p->addFunction("poissonpdf", JKMathParser_DefaultLib::fPoissonPDF);
    p->addFunction("binomialpdf", JKMathParser_DefaultLib::fBinomialPDF);
    p->addFunction("asin", JKMathParser_DefaultLib::fASin, NULL, asin);
    p->addFunction("acos", JKMathParser_DefaultLib::fACos, NULL, acos);
    p->addFunction("atan", JKMathParser_DefaultLib::fATan, NULL, atan);
    p->addFunction("atan2", JKMathParser_DefaultLib::fATan2, NULL, NULL, atan2);
    p->addFunction("sin", JKMathParser_DefaultLib::fSin, NULL, sin);
    p->addFunction("cos", JKMathParser_DefaultLib::fCos, NULL, cos);
    p->addFunction("tan", JKMathParser_DefaultLib::fTan, NULL, tan);
    p->addFunction("sinh", JKMathParser_DefaultLib::fSinh, NULL, sinh);
    p->addFunction("cosh", JKMathParser_DefaultLib::fCosh, NULL, cosh);
    p->addFunction("tanh", JKMathParser_DefaultLib::fTanh, NULL, tanh);
    p->addFunction("asinh", JKMathParser_DefaultLib::fASinh, NULL, sinh);
    p->addFunction("acosh", JKMathParser_DefaultLib::fACosh, NULL, cosh);
    p->addFunction("atanh", JKMathParser_DefaultLib::fATanh, NULL, tanh);
    p->addFunction("log", JKMathParser_DefaultLib::fLog, NULL, log);
    p->addFunction("log2", JKMathParser_DefaultLib::fLog2, NULL, log2);
    p->addFunction("log10", JKMathParser_DefaultLib::fLog10, NULL, log10);
    p->addFunction("exp", JKMathParser_DefaultLib::fExp, NULL, exp);
    p->addFunction("sqrt", JKMathParser_DefaultLib::fSqrt, NULL, sqrt);
    p->addFunction("cbrt", JKMathParser_DefaultLib::fCbrt, NULL, cbrt);
    p->addFunction("sqr", JKMathParser_DefaultLib::fSqr, NULL, JKMP::sqrNR);
    p->addFunction("abs", JKMathParser_DefaultLib::fAbs, NULL, fabs);
    p->addFunction("erf", JKMathParser_DefaultLib::fErf, NULL, erf);
    p->addFunction("erfc", JKMathParser_DefaultLib::fErfc, NULL, erfc);
    p->addFunction("lgamma", JKMathParser_DefaultLib::flGamma, NULL, lgamma);
    p->addFunction("tgamma", JKMathParser_DefaultLib::ftGamma, NULL, tgamma);
    p->addFunction("j0", JKMathParser_DefaultLib::fJ0, NULL, j0);
    p->addFunction("j1", JKMathParser_DefaultLib::fJ1, NULL, j1);
    p->addFunction("jn", JKMathParser_DefaultLib::fJn, NULL, NULL, JKMathParser_DefaultLib::fJn);
    p->addFunction("y0", JKMathParser_DefaultLib::fY0, NULL, y0);
    p->addFunction("y1", JKMathParser_DefaultLib::fY1, NULL, y1);
    p->addFunction("yn", JKMathParser_DefaultLib::fYn, NULL, NULL, JKMathParser_DefaultLib::fYn);
    p->addFunction("rand", JKMathParser_DefaultLib::fRand, JKMathParser_DefaultLib::fRand, JKMathParser_DefaultLib::fRand, JKMathParser_DefaultLib::fRand);
    p->addFunction("randint", JKMathParser_DefaultLib::fRandInt, JKMathParser_DefaultLib::fRandInt, JKMathParser_DefaultLib::fRandInt, JKMathParser_DefaultLib::fRandInt);
    p->addFunction("randnorm", JKMathParser_DefaultLib::fRandNorm, JKMathParser_DefaultLib::fRandNorm, JKMathParser_DefaultLib::fRandNorm, JKMathParser_DefaultLib::fRandNorm);
    p->addFunction("randbool", JKMathParser_DefaultLib::fRandBool);
    p->addFunction("randvec", JKMathParser_DefaultLib::fRandVec);
    p->addFunction("randintvec", JKMathParser_DefaultLib::fRandIntVec);
    p->addFunction("randnormvec", JKMathParser_DefaultLib::fRandNormVec);
    p->addFunction("randboolvec", JKMathParser_DefaultLib::fRandBoolVec);
    p->addFunction("randvec_seed", JKMathParser_DefaultLib::fRandVecSeed);
    p->addFunction("randintvec_seed", JKMathParser_DefaultLib::fRandIntVecSeed);
    p->addFunction("randnormvec_seed", JKMathParser_DefaultLib::fRandNormVecSeed);
    p->addFunction("randboolvec_seed", JKMathParser_DefaultLib::fRandBoolVecSeed);
    p->addFunction("srand", JKMathParser_DefaultLib::fSRand, NULL, JKMathParser_DefaultLib::fSRand);
    p->addFunction("ceil", JKMathParser_DefaultLib::fCeil, NULL, ceil);
    p->addFunction("floor", JKMathParser_DefaultLib::fFloor, NULL, floor);
    p->addFunction("trunc", JKMathParser_DefaultLib::fTrunc, NULL, trunc);
    p->addFunction("round", JKMathParser_DefaultLib::fRound, NULL, round);
    p->addFunction("fmod", JKMathParser_DefaultLib::fFMod, NULL, NULL, fmod);
    p->addFunction("min", JKMathParser_DefaultLib::fMin,NULL,NULL,JKMathParser_DefaultLib::fMin);
    p->addFunction("max", JKMathParser_DefaultLib::fMax,NULL,NULL,JKMathParser_DefaultLib::fMax);
    p->addFunction("int2bin", JKMathParser_DefaultLib::fIntToBinStr);
    p->addFunction("int2oct", JKMathParser_DefaultLib::fIntToOctStr);
    p->addFunction("int2hex", JKMathParser_DefaultLib::fIntToHexStr);
    p->addFunction("int2str", JKMathParser_DefaultLib::fIntToStr);
    p->addFunction("num2str", JKMathParser_DefaultLib::fdoubleToStr);
    p->addFunction("bool2str", JKMathParser_DefaultLib::fboolToStr);
    p->addFunction("gaussnn", JKMathParser_DefaultLib::fGauss, NULL, JKMP::gaussSqrtE, JKMP::gaussSqrtE);
    p->addFunction("gauss", JKMathParser_DefaultLib::fGaussDist, NULL, JKMP::gaussNormSqrtE, JKMP::gaussNormSqrtE);
    p->addFunction("slit", JKMathParser_DefaultLib::fSlit, NULL, NULL,JKMP::slit);
    p->addFunction("theta", JKMathParser_DefaultLib::fTheta, NULL, JKMP::theta);
    p->addFunction("tanc", JKMathParser_DefaultLib::fTanc, NULL, JKMP::tanc);
    p->addFunction("sigmoid", JKMathParser_DefaultLib::fSigmoid, NULL, JKMP::sigmoid);
    p->addFunction("sign", JKMathParser_DefaultLib::fSign, NULL, JKMP::sign);
    p->addFunction("roundsig", JKMathParser_DefaultLib::fRoundSig, NULL, JKMP::roundError, JKMP::roundError);
    p->addFunction("tosystempathseparator", JKMathParser_DefaultLib::fToSystemPathSeparator);
    p->addFunction("trimm", JKMathParser_DefaultLib::fTrimm);
    p->addFunction("split", JKMathParser_DefaultLib::fSplit);
    p->addFunction("join", JKMathParser_DefaultLib::fJoin);
    p->addFunction("removeemtystrings", JKMathParser_DefaultLib::fRemoveEmpty);

    p->addFunction("toupper", JKMathParser_DefaultLib::fToUpper);
    p->addFunction("tolower", JKMathParser_DefaultLib::fToLower);
    p->addFunction("length", JKMathParser_DefaultLib::fLength);
    p->addFunction("sort", JKMathParser_DefaultLib::fSort);
    p->addFunction("dsort", JKMathParser_DefaultLib::fDSort);
    p->addFunction("count", JKMathParser_DefaultLib::fCountOccurences);
    p->addFunction("sum", JKMathParser_DefaultLib::fSum);
    p->addFunction("prod", JKMathParser_DefaultLib::fProd);
    p->addFunction("diff", JKMathParser_DefaultLib::fDiff);
    p->addFunction("cumsum", JKMathParser_DefaultLib::fCumSum);
    p->addFunction("cumprod", JKMathParser_DefaultLib::fCumProd);
    p->addFunction("sum2", JKMathParser_DefaultLib::fSum2);
    p->addFunction("mean", JKMathParser_DefaultLib::fMean);
    p->addFunction("corrcoeff", JKMathParser_DefaultLib::fCorrcoeff);
    p->addFunction("mandersoverlap", JKMathParser_DefaultLib::fMandersOverlapCoeff);
    p->addFunction("skewness", JKMathParser_DefaultLib::fSkewness);
    p->addFunction("moment", JKMathParser_DefaultLib::fCentralMoment);
    p->addFunction("ncmoment", JKMathParser_DefaultLib::fNonCentralMoment);
    p->addFunction("std", JKMathParser_DefaultLib::fStd);
    p->addFunction("var", JKMathParser_DefaultLib::fVar);
    p->addFunction("median", JKMathParser_DefaultLib::fMedian);
    p->addFunction("quantile", JKMathParser_DefaultLib::fQuantile);
    p->addFunction("quantile25", JKMathParser_DefaultLib::fQuantile25);
    p->addFunction("quantile75", JKMathParser_DefaultLib::fQuantile75);
    p->addFunction("remove", JKMathParser_DefaultLib::fRemove);
    p->addFunction("removeall", JKMathParser_DefaultLib::fRemoveAll);
    p->addFunction("shuffle", JKMathParser_DefaultLib::fShuffle);
    p->addFunction("reverse", JKMathParser_DefaultLib::fReverse);
    p->addFunction("find", JKMathParser_DefaultLib::fFind);
    p->addFunction("findfirst", JKMathParser_DefaultLib::fFindFirst);
    p->addFunction("select", JKMathParser_DefaultLib::fSelect);
    p->addFunction("concat", JKMathParser_DefaultLib::fConcat);
    p->addFunction("deg2rad", JKMathParser_DefaultLib::fDegToRad, NULL, JKMP::degToRad);
    p->addFunction("rad2deg", JKMathParser_DefaultLib::fRadToDeg, NULL, JKMP::radToDeg);
    p->addFunction("dot", JKMathParser_DefaultLib::fDot);
    p->addFunction("alltrue", JKMathParser_DefaultLib::fAllTrue);
    p->addFunction("allfalse", JKMathParser_DefaultLib::fAllFalse);
    p->addFunction("anytrue", JKMathParser_DefaultLib::fAnyTrue);
    p->addFunction("anyfalse", JKMathParser_DefaultLib::fAnyFalse);
    p->addFunction("countoccurences", JKMathParser_DefaultLib::fCountOccurences);
    p->addFunction("mad", JKMathParser_DefaultLib::fMAD);
    p->addFunction("nmad", JKMathParser_DefaultLib::fNMAD);
    p->addFunction("trapz", JKMathParser_DefaultLib::fTrapz);
    p->addFunction("trapez", JKMathParser_DefaultLib::fTrapz);

    p->addFunction("containssubstring", JKMathParser_DefaultLib::fContainsSubString);
    p->addFunction("contains", JKMathParser_DefaultLib::fContains);
    p->addFunction("containssubstr_caseinsensitive", JKMathParser_DefaultLib::fContainsSubStringLC);
    p->addFunction("contains_caseinsensitive", JKMathParser_DefaultLib::fContainsLC);
    p->addFunction("replace", JKMathParser_DefaultLib::fReplace);
    p->addFunction("multireplace", JKMathParser_DefaultLib::fReplaceMulti);

    p->addFunction("isnan", JKMathParser_DefaultLib::fIsNan);
    p->addFunction("isinf", JKMathParser_DefaultLib::fIsInf);
    p->addFunction("isfinite", JKMathParser_DefaultLib::fIsFinite);
    p->addFunction("isnumok", JKMathParser_DefaultLib::fIsFloatOK);
    p->addFunction("isvalid", JKMathParser_DefaultLib::fIsValid);
    p->addFunction("isvoid", JKMathParser_DefaultLib::fIsVoid);
    p->addFunction("isint", JKMathParser_DefaultLib::fIsInt);
    p->addFunction("isuint", JKMathParser_DefaultLib::fIsUInt);
    p->addFunction("isbool", JKMathParser_DefaultLib::fIsBool);
    p->addFunction("isstring", JKMathParser_DefaultLib::fIsString);
    p->addFunction("isnumber", JKMathParser_DefaultLib::fIsDouble);
    p->addFunction("isdouble", JKMathParser_DefaultLib::fIsDouble);
    p->addFunction("isdoublevector", JKMathParser_DefaultLib::fIsDoubleVector);
    p->addFunction("isnumbervector", JKMathParser_DefaultLib::fIsDoubleVector);
    p->addFunction("isboolvector", JKMathParser_DefaultLib::fIsBoolVector);
    p->addFunction("isstringvector", JKMathParser_DefaultLib::fIsStringVector);
    p->addFunction("ismatrix", JKMathParser_DefaultLib::fIsMatrix);
    p->addFunction("isvector", JKMathParser_DefaultLib::fIsVector);
    p->addFunction("isboolmatrix", JKMathParser_DefaultLib::fIsBoolMatrix);
    p->addFunction("isnumbermatrix", JKMathParser_DefaultLib::fIsDoubleMatrix);
    p->addFunction("isdoublematrix", JKMathParser_DefaultLib::fIsDoubleMatrix);

    p->addFunction("unique", JKMathParser_DefaultLib::fUnique);
    p->addFunction("indexedmean", JKMathParser_DefaultLib::fIndexedAvg);
    p->addFunction("indexedvar", JKMathParser_DefaultLib::fIndexedVar);
    p->addFunction("indexedstd", JKMathParser_DefaultLib::fIndexedStd);
    p->addFunction("indexedmin", JKMathParser_DefaultLib::fIndexedMin);
    p->addFunction("indexedmax", JKMathParser_DefaultLib::fIndexedMax);
    p->addFunction("indexedmedian", JKMathParser_DefaultLib::fIndexedMedian);
    p->addFunction("indexedsum", JKMathParser_DefaultLib::fIndexedSum);
    p->addFunction("indexedsum2", JKMathParser_DefaultLib::fIndexedSum2);
    p->addFunction("indexedquantile", JKMathParser_DefaultLib::fIndexedQuantile);
    p->addFunction("indexedmad", JKMathParser_DefaultLib::fIndexedMAD);
    p->addFunction("indexednmad", JKMathParser_DefaultLib::fIndexedNMAD);

    p->addFunction("indexedskewness", JKMathParser_DefaultLib::fIndexedSkewness);
    p->addFunction("indexedprod", JKMathParser_DefaultLib::fIndexedProd);
    p->addFunction("indexedcount", JKMathParser_DefaultLib::fIndexedCount);
    p->addFunction("indexedmoment", JKMathParser_DefaultLib::fIndexedMoment);
    p->addFunction("indexedncmoment", JKMathParser_DefaultLib::fIndexedNonCentralMoment);
    p->addFunction("indexedcorrcoeff", JKMathParser_DefaultLib::fIndexedCorrCoeff);

    p->addFunction("zeros", JKMathParser_DefaultLib::fZeros);
    p->addFunction("ones", JKMathParser_DefaultLib::fOnes);
    p->addFunction("vector", JKMathParser_DefaultLib::fVector);
    p->addFunction("linspace", JKMathParser_DefaultLib::fLinSpace);
    p->addFunction("logspace", JKMathParser_DefaultLib::fLogSpace);
    p->addFunction("meshgrid2d_x", JKMathParser_DefaultLib::fMeshGrid2Dx);
    p->addFunction("meshgrid2d_y", JKMathParser_DefaultLib::fMeshGrid2Dy);


    p->addFunction("polyval", JKMathParser_DefaultLib::fPolynom);
    p->addFunction("polyder", JKMathParser_DefaultLib::fDerivPolynom);

    p->addFunction("last", JKMathParser_DefaultLib::fLast);
    p->addFunction("first", JKMathParser_DefaultLib::fFirst);
    p->addFunction("lastinvector", JKMathParser_DefaultLib::fLastInVector);
    p->addFunction("firstinvector", JKMathParser_DefaultLib::fFirstInVector);
    p->addFunction("itemorfirst", JKMathParser_DefaultLib::fItemOrFirst);
    p->addFunction("returnlast", JKMathParser_DefaultLib::fReturnLast);
    p->addFunction("returnnth", JKMathParser_DefaultLib::fReturnNth);
    p->addFunction("returnfirst", JKMathParser_DefaultLib::fReturnFirst);
    p->addFunction("returnfirstvalid", JKMathParser_DefaultLib::fReturnFirstValid);
    p->addFunction("returnlastvalid", JKMathParser_DefaultLib::fReturnLastValid);
    p->addFunction("item", JKMathParser_DefaultLib::fItem);

    p->addFunction("runningaverage", JKMathParser_DefaultLib::fRunningAverage);
    p->addFunction("filterfinite", JKMathParser_DefaultLib::fFilterFinite);
    p->addFunction("filternumok", JKMathParser_DefaultLib::fFilterFinite);

    p->addFunction("regexpcap", JKMathParser_DefaultLib::fRegExpCaptureNCS);
    p->addFunction("regexpcontains", JKMathParser_DefaultLib::fRegExpContainsNCS);
    p->addFunction("regexpindexin", JKMathParser_DefaultLib::fRegExpIndexInNCS);

    p->addFunction("regexpcap_minimal", JKMathParser_DefaultLib::fRegExpCaptureMNCS);
    p->addFunction("regexpcontains_minimal", JKMathParser_DefaultLib::fRegExpContainsMNCS);
    p->addFunction("regexpindexin_minimal", JKMathParser_DefaultLib::fRegExpIndexInMNCS);

    p->addFunction("regexpcap_cs", JKMathParser_DefaultLib::fRegExpCaptureCS);
    p->addFunction("regexpcontains_cs", JKMathParser_DefaultLib::fRegExpContainsCS);
    p->addFunction("regexpindexin_cs", JKMathParser_DefaultLib::fRegExpIndexInCS);

    p->addFunction("regexpcap_cs_minimal", JKMathParser_DefaultLib::fRegExpCaptureMCS);
    p->addFunction("regexpcontains_cs_minimal", JKMathParser_DefaultLib::fRegExpContainsMCS);
    p->addFunction("regexpindexin_cs_minimal", JKMathParser_DefaultLib::fRegExpIndexInMCS);



    p->addFunction("str2bool", JKMathParser_DefaultLib::fStringToBool);
    p->addFunction("str2num", JKMathParser_DefaultLib::fStringToNum);


/*
    p->addFunction("now", JKMathParser_DefaultLib::fNow);
    p->addFunction("datenum", JKMathParser_DefaultLib::fDateNum);
    p->addFunction("datetimenum", JKMathParser_DefaultLib::fDateTimeNum);
    p->addFunction("date2str", JKMathParser_DefaultLib::fDate2Str);
    p->addFunction("datetime2str", JKMathParser_DefaultLib::fDateTime2Str);
    p->addFunction("datediff2secs", JKMathParser_DefaultLib::fDatediff2Secs);
    p->addFunction("datediff2mins", JKMathParser_DefaultLib::fDatediff2Mins);
    p->addFunction("datediff2hours", JKMathParser_DefaultLib::fDatediff2Hours);
    p->addFunction("datediff2days", JKMathParser_DefaultLib::fDatediff2Days);
    p->addFunction("dateday", JKMathParser_DefaultLib::fDateDay);
    p->addFunction("datemonth", JKMathParser_DefaultLib::fDateMonth);
    p->addFunction("dateyear", JKMathParser_DefaultLib::fDateYear);
    p->addFunction("datehour", JKMathParser_DefaultLib::fDateHour);
    p->addFunction("datemin", JKMathParser_DefaultLib::fDateMin);
    p->addFunction("datesec", JKMathParser_DefaultLib::fDateSec);
    */

    p->addFunction("varname", JKMathParser_DefaultLib::fVarname);
    p->addFunction("printexpression", JKMathParser_DefaultLib::fPrintExpression);
    p->addFunction("printexpressiontree", JKMathParser_DefaultLib::fPrintExpressionTree);

    p->addFunction("int", JKMathParser_DefaultLib::fInt);
    p->addFunction("num", JKMathParser_DefaultLib::fNum);
    p->addFunction("double", JKMathParser_DefaultLib::fNum);
    p->addFunction("bool", JKMathParser_DefaultLib::fBool);
    p->addFunction("strvec", JKMathParser_DefaultLib::fStrVec);
    p->addFunction("boolvec", JKMathParser_DefaultLib::fBoolVec);
    p->addFunction("numvec", JKMathParser_DefaultLib::fNumVec);
    p->addFunction("intvec", JKMathParser_DefaultLib::fIntVec);
    p->addFunction("ensuresize", JKMathParser_DefaultLib::fEnsureVectorSize);
    p->addFunction("ensuresize_start", JKMathParser_DefaultLib::fEnsureVectorSizeStart);
    p->addFunction("histogram", JKMathParser_DefaultLib::fHistogram);
    p->addFunction("histogrambins", JKMathParser_DefaultLib::fHistogramBins);
    p->addFunction("rangedhistogram", JKMathParser_DefaultLib::fRangedHistogram);
    p->addFunction("rangedhistogrambins", JKMathParser_DefaultLib::fRangedHistogramBins);

    p->addFunction("regression", JKMathParser_DefaultLib::fRegression);
    p->addFunction("polyfit", JKMathParser_DefaultLib::fPolyFit);
    p->addFunction("weighted_regression", JKMathParser_DefaultLib::fWeightedRegression);
    p->addFunction("irls", JKMathParser_DefaultLib::fIRLS);
    p->addFunction("erroradd", JKMathParser_DefaultLib::fErrorAdd);
    p->addFunction("errorasub", JKMathParser_DefaultLib::fErrorSub);
    p->addFunction("errormul", JKMathParser_DefaultLib::fErrorMul);
    p->addFunction("errordiv", JKMathParser_DefaultLib::fErrorDiv);
    p->addFunction("errorpow", JKMathParser_DefaultLib::fErrorPow);





    JKMATHPARSER_REGISTER_FUNC(fextractfilename, extract_filename)
    JKMATHPARSER_REGISTER_FUNC(fextractfilebasename, extract_basename)

    JKMATHPARSER_REGISTER_FUNC(fextractfileext, extract_ext)

    JKMATHPARSER_REGISTER_FUNC(fextractfilepath, extract_file_path)
    JKMATHPARSER_REGISTER_FUNC(fextractfileabspath, extract_absolute_path)
    JKMATHPARSER_REGISTER_FUNC(fextractfileabsfilepath, extract_absolute_file_path)


    JKMATHPARSER_REGISTER_FUNC(fReadFile, readfile)
    JKMATHPARSER_REGISTER_FUNC(fescapify, escapify)
    JKMATHPARSER_REGISTER_FUNC(fdeescapify, deescapify)


}

















namespace JKMathParser_DefaultLib {


    void fComment(jkmpResult &r, const jkmpResult */*params*/, unsigned int /*n*/, JKMathParser */*p*/)
    {
        r.setVoid();
    }

    void fRepeat(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==2) {
            if (params[0].isUInt()) {
                const uint32_t N=params[0].toUInt();
                if (params[1].type==jkmpDouble) {
                    r.setDoubleVec(N, params[1].num);
                } else if (params[1].type==jkmpDoubleVector) {
                    r.setDoubleVec(N*params[1].size());
                    for (uint32_t i=0; i<N*params[1].size(); i++) {
                        r.numVec[i]=params[1].numVec[i%params[1].numVec.size()];
                    }
                } else if (params[1].type==jkmpBool) {
                    r.setBoolVec(N, params[1].boolean);
                } else if (params[1].type==jkmpBoolVector) {
                    r.setBoolVec(N*params[1].size());
                    for (uint32_t i=0; i<N*params[1].size(); i++) {
                        r.boolVec[i]=params[1].boolVec[i%params[1].boolVec.size()];
                    }
                } else if (params[1].type==jkmpString) {
                    r.setStringVec(N, params[1].str);
                } else if (params[1].type==jkmpStringVector) {
                    r.setStringVec(N*params[1].size());
                    for (uint32_t i=0; i<N*params[1].size(); i++) {
                        r.strVec[i]=params[1].strVec[i%params[1].strVec.size()];
                    }
                } else {
                    p->jkmpError(JKMP::_("repeat(x,value) the argument value is of type '%1', which cannot be replicated into a vector").arg(params[1].typeName()));
                    r.setInvalid();
                    return;
                }
            } else {
                p->jkmpError(JKMP::_("repeat(x,value) needs a positive integer as first argument"));
                r.setInvalid();
                return;
            }
        } else {
            p->jkmpError(JKMP::_("repeat(x,value) needs at 2 arguments"));
            r.setInvalid();
            return;
        }
    }

    void fRepeatString(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==2) {
            if (params[0].isUInt()) {
                const uint32_t N=params[0].toUInt();
                if (params[1].type==jkmpString) {
                    r.setString("");
                    for (uint32_t i=0; i<N; i++) {
                        r.str.push_back(params[1].str);
                    }
                } else {
                    p->jkmpError(JKMP::_("repeatstring(x,text) the argument text is of type '%1', but has to be a string").arg(params[1].typeName()));
                    r.setInvalid();
                    return;
                }
            } else {
                p->jkmpError(JKMP::_("repeatstring(x,text) needs a positive integer as first argument"));
                r.setInvalid();
                return;
            }
        } else {
            p->jkmpError(JKMP::_("repeatstring(x,text) needs at 2 arguments"));
            r.setInvalid();
            return;
        }
    }


    JKMP::string ptosystempathseparator(const JKMP::string& param) {
        JKMP::string r;
        for (int i=0; i<param.size(); i++) {
            JKMP::charType ch=param[i];
            if (ch=='/' || ch=='\\') ch=JKMP_PATHSEPARATOR_CHAR;
            r+=ch;
        }
        return r;
    }




    void fMat2Vec(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==1 && params[0].type==jkmpDoubleMatrix) {
            r.setDoubleVec(params[0].numVec);
        } else if (n==1 && params[0].type==jkmpBoolMatrix) {
            r.setBoolVec(params[0].boolVec);
        } else {
            p->jkmpError(JKMP::_("mat2vec(x) needs one matrix argument"));
            r.setInvalid();
            return;
        }
    }
    void fVec2Mat(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        int cols=1;
        if (n>=1 && params[1].isUInt()) cols=params[1].toUInt();
        else if (n>=1) {
            p->jkmpError(JKMP::_("vec2mat(x, cols) cols has to be an unsigned inetegr!"));
            r.setInvalid();
            return;
        }
        if (cols<1) {
            p->jkmpError(JKMP::_("vec2mat(x, cols) cols has to be a positive integer, but was <=0 !"));
            r.setInvalid();
            return;
        }
        if (n>0 && params[0].type==jkmpDoubleVector) {
            int rows=params[0].numVec.size()/cols;
            if (params[0].numVec.size()!=rows*cols) {
                p->jkmpError(JKMP::_("vec2mat(x[, cols]) determined a matrix size of %1x%2 = %3 entries, but the vector has %4 elements").arg(rows).arg(cols).arg(rows*cols).arg(params[0].numVec.size()));
                r.setInvalid();
                return;
            }
            r=params[0];
            r.type=jkmpDoubleMatrix;
            r.matrix_columns=cols;
        } else if (n>0 && params[0].type==jkmpBoolVector) {
            int rows=params[0].boolVec.size()/cols;
            if (params[0].boolVec.size()!=rows*cols) {
                p->jkmpError(JKMP::_("vec2mat(x[, cols]) determined a matrix size of %1x%2 = %3 entries, but the vector has %4 elements").arg(rows).arg(cols).arg(rows*cols).arg(params[0].boolVec.size()));
                r.setInvalid();
                return;
            }
            r=params[0];
            r.type=jkmpBoolMatrix;
            r.matrix_columns=cols;
        } else {
            p->jkmpError(JKMP::_("vec2mat(x[, cols]) needs one number or boolean vector argument"));
            r.setInvalid();
            return;
        }
    }
    void fReshape(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {

        if (n==3 && (params[0].type==jkmpDoubleVector || params[0].type==jkmpDoubleMatrix || params[0].type==jkmpBoolVector || params[0].type==jkmpBoolMatrix) && params[1].isUInt() && params[2].isUInt()) {
            int rows=params[1].toUInt();
            int cols=params[2].toUInt();

            if (n>0 && (params[0].type==jkmpDoubleVector || params[0].type==jkmpDoubleMatrix)) {
                if (params[0].numVec.size()!=rows*cols) {
                    p->jkmpError(JKMP::_("reshape(x, rows, cols) requires a matrix of size %1x%2 = %3 entries, but the matrix has %4 elements").arg(rows).arg(cols).arg(rows*cols).arg(params[0].numVec.size()));
                    r.setInvalid();
                    return;
                }
                r=params[0];
                r.type=jkmpDoubleMatrix;
                r.matrix_columns=cols;
            } else if (n>0 &&(params[0].type==jkmpBoolVector || params[0].type==jkmpBoolMatrix)) {
                if (params[0].boolVec.size()!=rows*cols) {
                    p->jkmpError(JKMP::_("reshape(x, rows, cols) requires a matrix of size %1x%2 = %3 entries, but the matrix has %4 elements").arg(rows).arg(cols).arg(rows*cols).arg(params[0].boolVec.size()));
                    r.setInvalid();
                    return;
                }
                r=params[0];
                r.type=jkmpBoolMatrix;
                r.matrix_columns=cols;
            } else {
                p->jkmpError(JKMP::_("reshape(x, rows, cols) unsupported matrix type!"));
                r.setInvalid();
                return;
            }
        } else {
            p->jkmpError(JKMP::_("reshape(x, rows, cols) needs exactly three arguments, one matrix and two positive integers!"));
            r.setInvalid();
            return;
        }
    }
    void fPrintExpression(jkmpResult& r,  JKMathParser::jkmpNode** params, unsigned int n, JKMathParser* p){
        if (n==1) {
            r.setString(params[0]->print());
        } else if (n>1) {
            r.setStringVec();
            for (unsigned int i=0; i<n; i++) {
                r.strVec<<params[i]->print();
            }
        } else {
            p->jkmpError(JKMP::_("printexpression(x[,...]) needs at least 1 argument"));
            r.setInvalid();
            return;
        }
    }
    void fPrintExpressionTree(jkmpResult& r,  JKMathParser::jkmpNode** params, unsigned int n, JKMathParser* p){
        if (n==1) {
            r.setString(params[0]->printTree());
        } else if (n>1) {
            r.setStringVec();
            for (unsigned int i=0; i<n; i++) {
                r.strVec<<params[i]->printTree();
            }
        } else {
            p->jkmpError(JKMP::_("printexpressiontree(x[,...]) needs at least 1 argument"));
            r.setInvalid();
            return;
        }
    }


    void fShuffle(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("shuffle"));
            r.setInvalid();
            return;
        }
        if(params[0].type==jkmpDoubleVector) {
            //r.setDoubleVec(JKMathParser_shuffleD(params[0].numVec));
            r.setDoubleVec(params[0].numVec);
            for (int i=r.numVec.size()-1; i>=1; i--) {
                int j=p->get_rng()->randInt(i);
                std::swap(r.numVec[i], r.numVec[j]);
            }
        } else if(params[0].type==jkmpStringVector) {
            //r.setStringVec(JKMathParser_shuffleS(params[0].strVec));
            r.setStringVec(params[0].strVec);
            for (int i=r.strVec.size()-1; i>=1; i--) {
                int j=p->get_rng()->randInt(i);
                std::swap(r.strVec[i], r.strVec[j]);
            }
        } else if(params[0].type==jkmpString) {
            //r.setStringVec(JKMathParser_shuffleS(params[0].strVec));
            r.setString(params[0].str);
            for (int i=r.str.size()-1; i>=1; i--) {
                int j=p->get_rng()->randInt(i);
                JKMP::charType h=r.str[i];
                r.str[i]=r.str[j];
                r.str[j]=h;
            }
        } else if(params[0].type==jkmpBoolVector) {
            //r.setBoolVec(JKMathParser_shuffleB(params[0].boolVec));
            r.setBoolVec(params[0].boolVec);
            for (int i=r.boolVec.size()-1; i>=1; i--) {
                int j=p->get_rng()->randInt(i);
                std::swap(r.boolVec[i], r.boolVec[j]);
            }
        } else {
            p->jkmpError(JKMP::_("%1(x) argument has to be a vector of numbers/booleans/strings").arg("shuffle"));
            r.setInvalid();
        }
        return;
    }


    void fIsValid(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("isvalid"));
            r.setInvalid();
            return;
        }
        r.setBoolean(params[0].isValid);
    }

    void fIsVoid(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("isvoid"));
            r.setInvalid();
            return;
        }
        r.setBoolean(params[0].type==jkmpVoid);
    }


    void fIsInt(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("isint"));
            r.setInvalid();
            return;
        }
        r.setBoolean(params[0].isInteger());

    }

    void fIsUInt(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("isuint"));
            r.setInvalid();
            return;
        }
        r.setBoolean(params[0].isUInt());


    }

    void fIsDouble(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("isdouble"));
            r.setInvalid();
            return;
        }
        r.setBoolean(params[0].type==jkmpDouble);

    }

    void fIsString(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("isstring"));
            r.setInvalid();
            return;
        }
        r.setBoolean(params[0].type==jkmpString);
    }

    void fIsBool(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("isbool"));
            r.setInvalid();
            return;
        }
        r.setBoolean(params[0].type==jkmpBool);
    }

    void fIsVector(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("isvector"));
            r.setInvalid();
            return;
        }
        r.setBoolean(params[0].type==jkmpDoubleVector || params[0].type==jkmpBoolVector || params[0].type==jkmpStringVector);
    }

    void fIsBoolVector(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("isboolvector"));
            r.setInvalid();
            return;
        }
        r.setBoolean(params[0].type==jkmpBoolVector);
    }

    void fIsDoubleVector(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("isdoublevector"));
            r.setInvalid();
            return;
        }
        r.setBoolean(params[0].type==jkmpDoubleVector);
    }

    void fIsStringVector(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("isstringbool"));
            r.setInvalid();
            return;
        }
        r.setBoolean(params[0].type==jkmpStringVector);
    }






    void fAllTrue(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("alltrue"));
            r.setInvalid();
            return;
        }
        if(params[0].type==jkmpBoolVector) {
            bool res=true;
            for (int i=0; i<params[0].boolVec.size(); i++) {
                if (!params[0].boolVec[i]) {
                    res=false;
                    break;
                }
            }
            r.setBoolVec(res);
        } else if(params[0].type==jkmpBool) {
            r.setBoolVec(params[0].boolean);
         } else {
            p->jkmpError(JKMP::_("%1(x) argument has to be a vector of booleans").arg("alltrue"));
            r.setInvalid();
        }
        return;
    }

    void fAllFalse(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("allfalse"));
            r.setInvalid();
            return;
        }
        if(params[0].type==jkmpBoolVector) {
            bool res=true;
            for (int i=0; i<params[0].boolVec.size(); i++) {
                if (params[0].boolVec[i]) {
                    res=false;
                    break;
                }
            }
            r.setBoolVec(res);
        } else if(params[0].type==jkmpBool) {
            r.setBoolVec(!params[0].boolean);
         } else {
            p->jkmpError(JKMP::_("%1(x) argument has to be a vector of booleans").arg("allfalse"));
            r.setInvalid();
        }
        return;
    }


    void fAnyTrue(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("anytrue"));
            r.setInvalid();
            return;
        }
        if(params[0].type==jkmpBoolVector) {
            bool res=false;
            for (int i=0; i<params[0].boolVec.size(); i++) {
                if (params[0].boolVec[i]) {
                    res=true;
                    break;
                }
            }
            r.setBoolVec(res);
        } else if(params[0].type==jkmpBool) {
            r.setBoolVec(params[0].boolean);
         } else {
            p->jkmpError(JKMP::_("%1(x) argument has to be a vector of booleans").arg("anytrue"));
            r.setInvalid();
        }
        return;
    }

    void fAnyFalse(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(x) needs exacptly 1 argument").arg("anyfalse"));
            r.setInvalid();
            return;
        }
        if(params[0].type==jkmpBoolVector) {
            bool res=false;
            for (int i=0; i<params[0].boolVec.size(); i++) {
                if (!params[0].boolVec[i]) {
                    res=true;
                    break;
                }
            }
            r.setBoolVec(res);
        } else if(params[0].type==jkmpBool) {
            r.setBoolVec(!params[0].boolean);
         } else {
            p->jkmpError(JKMP::_("%1(x) argument has to be a vector of booleans").arg("anyfalse"));
            r.setInvalid();
        }
        return;
    }


    void fCountOccurences(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        if(n==1 && params[0].type==jkmpDoubleVector) {
            r.setDouble(statisticsCount(params[0].numVec));
        } else if(n==2 && params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble) {
            r.setDouble(params[0].numVec.count(params[1].num));
        } else if(n==2 && params[0].type==jkmpStringVector && params[1].type==jkmpString) {
            r.setDouble(params[0].strVec.count(params[1].str));
        } else if(n==2 && params[0].type==jkmpBoolVector && params[1].type==jkmpBool) {
            r.setDouble(params[0].boolVec.count(params[1].boolean));
        } else if(n==1 && params[0].type==jkmpBoolVector) {
            r.setDouble(params[0].boolVec.count(true));
        } else {
            p->jkmpError(JKMP::_("%1(x, value) argument 1 has to be a vector of numbers/booleans/strings and argument 2 the according item type number/string/boolean").arg("countoccurences"));
            r.setInvalid();
        }
    }

    jkmpResult fSRand(const jkmpResult* params, unsigned int  n, JKMathParser* p){
      jkmpResult r;
      r.type=jkmpDouble;
      //if (n!=1) p->jkmpError(JKMP::_("srand accepts 1 argument"));
      if (n>=1 && params[0].type!=jkmpDouble) p->jkmpError(JKMP::_("srand needs double argument"));
      r.num=0;
      //srand((unsigned int)params[0].num);
      if (n>=1) p->get_rng()->seed(uint32_t(params[0].num));
      else p->get_rng()->seed();
      return r;
    }


    jkmpResult fRand(const jkmpResult* params, unsigned int  n, JKMathParser* p){
      jkmpResult r;
      r.type=jkmpDouble;
      if (n>2) p->jkmpError(JKMP::_("rand accepts 0, 1 or 2 arguments"));
      //r.num=double(rand())/double(RAND_MAX);
      if (n==0) r.num=p->get_rng()->rand();
      else if (n==2) r.num=p->get_rng()->rand()*(params[1].num-params[0].num)+params[0].num;
      else r.num=p->get_rng()->rand(params[0].num);
      return r;
    }



    jkmpResult fRandNorm(const jkmpResult* params, unsigned int  n, JKMathParser* p){
      jkmpResult r;
      r.type=jkmpDouble;
      double mean=0;
      double var=1;
      if (n>2) p->jkmpError(JKMP::_("randnorm accepts 0,1 or 2 argument"));
      if (n>0 && (params[0].type!=jkmpDouble)) p->jkmpError(JKMP::_("randnorm needs a double as first argument (if any)"));
      if (n>1 && (params[1].type!=jkmpDouble)) p->jkmpError(JKMP::_("randnorm needs a double as second argument (if any)"));
      if (n==1) {
          var=params[0].num;
      } else if (n>1) {
          mean=params[0].num;
          var=params[1].num;
      }
      r.num=p->get_rng()->randNorm(mean, var);
      return r;
    }



    jkmpResult fRandInt(const jkmpResult* params, unsigned int  n, JKMathParser* p){
      jkmpResult r;
      if (n>2) p->jkmpError(JKMP::_("randint accepts 0, 1 or 2 argument"));
      r.type=jkmpDouble;
      if (n==0) r.num=p->get_rng()->randInt();
      else if (n==2) r.num=p->get_rng()->randInt((uint32_t)params[1].num-(uint32_t)params[0].num)+(uint32_t)params[0].num;
      else r.num=p->get_rng()->randInt((uint32_t)params[0].num);
      return r;
    }






    void fRandVec(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
      bool ok=true;
      if (n>3 || n<=0) { ok=false; p->jkmpError(JKMP::_("randvec accepts 1, 2 or 3 arguments")); }
      if (n>0 && (params[0].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randvec needs a double as first argument (if any)")); }
      if (n>1 && (params[1].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randvec needs a double as second argument (if any)")); }
      if (n>2 && (params[2].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randvec needs a double as third argument (if any)")); }
      int items=params[0].toInteger();
      if (items<=0) { ok=false; p->jkmpError(JKMP::_("randvec's first argument has to be a positive integer number")); }
      if (ok) {
          r.setDoubleVec(items, 0);

          if (n==1) {
              for (int i=0; i<items; i++)
                r.numVec[i]=p->get_rng()->rand();
          } else if (n==3) {
              for (int i=0; i<items; i++)
                r.numVec[i]=p->get_rng()->rand()*(params[2].num-params[1].num)+params[1].num;
          } else {
              for (int i=0; i<items; i++)
                r.numVec[i]=p->get_rng()->rand(params[1].num);
          }
      } else {
        r.setInvalid();
      }


    }


    void fRandIntVec(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
      bool ok=true;
      if (n>3 || n<=0) { ok=false; p->jkmpError(JKMP::_("randintvec accepts 1, 2 or 3 arguments")); }
      if (n>0 && (params[0].type!=jkmpDouble)) { ok=false; p->jkmpError(JKMP::_("randintvec needs a double as first argument (if any)")); }
      if (n>1 && (params[1].type!=jkmpDouble)) { ok=false; p->jkmpError(JKMP::_("randintvec needs a double as second argument (if any)")); }
      if (n>2 && (params[2].type!=jkmpDouble)) { ok=false; p->jkmpError(JKMP::_("randintvec needs a double as third argument (if any)")); }
      int items=params[0].toInteger();
      if (items<=0) { ok=false; p->jkmpError(JKMP::_("randintvec's first argument has to be a positive integer number")); }
      if (ok) {
          r.setDoubleVec(items, 0);

          if (n==1) {
              for (int i=0; i<items; i++)
                r.numVec[i]=p->get_rng()->randInt();
          } else if (n==3) {
              for (int i=0; i<items; i++)
                r.numVec[i]=p->get_rng()->randInt((uint32_t)params[2].num-(uint32_t)params[1].num)+(uint32_t)params[1].num;
          } else {
              for (int i=0; i<items; i++)
                r.numVec[i]=p->get_rng()->randInt((uint32_t)params[1].num);
          }
      } else {
          r.setInvalid();
      }


    }
    void fRandNormVec(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        bool ok=true;
      if (n>3 || n<=0) { ok=false; p->jkmpError(JKMP::_("randnormvec accepts 1, 2 or 3 arguments")); }
      if (n>0 && (params[0].type!=jkmpDouble)) { ok=false; p->jkmpError(JKMP::_("randnormvec needs a double as first argument (if any)")); }
      if (n>1 && (params[1].type!=jkmpDouble)) { ok=false; p->jkmpError(JKMP::_("randnormvec needs a double as second argument (if any)")); }
      if (n>2 && (params[2].type!=jkmpDouble)) { ok=false; p->jkmpError(JKMP::_("randnormvec needs a double as third argument (if any)")); }
      int items=params[0].toInteger();
      if (items<=0) { ok=false; p->jkmpError(JKMP::_("randnormvec's first argument has to be a positive integer number")); }
      if (ok) {

          r.setDoubleVec(items, 0);
          double mean=0;
          double var=1;
          if (n==2) {
              var=params[1].num;
          } else if (n>2) {
              mean=params[1].num;
              var=params[2].num;
          }

          for (int i=0; i<items; i++)
            r.numVec[i]=p->get_rng()->randNorm(mean, var);
      } else {
          r.setInvalid();
      }

    }



    void fRandBoolVec(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        bool ok=true;
      if (n>2 || n<=0) { ok=false; p->jkmpError(JKMP::_("randboolvec accepts 1 or 2 arguments")); }
      if (n>0 && (params[0].type!=jkmpDouble)) { ok=false; p->jkmpError(JKMP::_("randboolvec needs a double as first argument (if any)")); }
      if (n>1 && (params[1].type!=jkmpDouble)) { ok=false; p->jkmpError(JKMP::_("randboolvec needs a double as second argument (if any)")); }
      int items=params[0].toInteger();
      double pr=0.5;
      if (n>1) pr=params[1].num;
      if (items<=0) { ok=false; p->jkmpError(JKMP::_("randboolvec's first argument has to be a positive integer number")); }
      if (pr<0 || pr>1) { ok=false; p->jkmpError(JKMP::_("randboolvec's second argument has to be a number between 0 and 1")); }
      if (ok) {
          r.setBoolVec(items, false);

          for (int i=0; i<items; i++)
            r.boolVec[i]=(p->get_rng()->rand()<=pr);
      } else {
          r.setInvalid();
      }

    }












    void fRandVecSeed(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
      bool ok=true;
      if (n>4 || n<=1) { ok=false; p->jkmpError(JKMP::_("randvec_seed accepts 2, 3 or 4 arguments")); }
      if (n>0 && (params[0].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randvec_seed needs a double as first argument (if any)")); }
      if (n>1 && (params[1].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randvec_seed needs a double as second argument (if any)")); }
      if (n>2 && (params[2].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randvec_seed needs a double as third argument (if any)")); }
      if (n>4 && (params[3].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randvec_seed needs a double as fourth argument (if any)")); }
      int items=params[0].toInteger();
      if (items<=0) { ok=false; p->jkmpError(JKMP::_("randvec_seed's first argument has to be a positive integer number")); }
      if (ok) {
          r.setDoubleVec(items, 0);

          if (n==2) {
              MTRand rng;
              rng.seed(params[1].num);
              for (int i=0; i<items; i++)
                r.numVec[i]=rng.rand();
          } else if (n==4) {
              MTRand rng;
              rng.seed(params[3].num);
              for (int i=0; i<items; i++)
                r.numVec[i]=rng.rand()*(params[2].num-params[1].num)+params[1].num;
          } else if (n==3){
              MTRand rng;
              rng.seed(params[2].num);
              for (int i=0; i<items; i++)
                r.numVec[i]=rng.rand(params[1].num);
          }
      } else {
        r.setInvalid();
      }
    }


    void fRandIntVecSeed(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        bool ok=true;
        if (n>4 || n<=1) { ok=false; p->jkmpError(JKMP::_("randintvec_seed accepts 2, 3 or 4 arguments")); }
        if (n>0 && (params[0].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randintvec_seed needs a double as first argument (if any)")); }
        if (n>1 && (params[1].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randintvec_seed needs a double as second argument (if any)")); }
        if (n>2 && (params[2].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randintvec_seed needs a double as third argument (if any)")); }
        if (n>4 && (params[3].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randintvec_seed needs a double as fourth argument (if any)")); }
        int items=params[0].toInteger();
        if (items<=0) { ok=false; p->jkmpError(JKMP::_("randintvec_seed's first argument has to be a positive integer number")); }
        if (ok) {
            r.setDoubleVec(items, 0);

            if (n==2) {
                MTRand rng;
                rng.seed(params[1].num);
                for (int i=0; i<items; i++)
                  r.numVec[i]=rng.randInt();
            } else if (n==4) {
                MTRand rng;
                rng.seed(params[3].num);
                for (int i=0; i<items; i++)
                  r.numVec[i]=rng.randInt((uint32_t)params[2].num-(uint32_t)params[1].num)+(uint32_t)params[1].num;
            } else if (n==3){
                MTRand rng;
                rng.seed(params[2].num);
                for (int i=0; i<items; i++)
                  r.numVec[i]=rng.randInt((uint32_t)params[1].num);
            }
        } else {
          r.setInvalid();
        }
    }

    void fRandNormVecSeed(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        bool ok=true;
        if (n>4 || n<=1) { ok=false; p->jkmpError(JKMP::_("randnormvec_seed accepts 2, 3 or 4 arguments")); }
        if (n>0 && (params[0].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randnormvec_seed needs a double as first argument (if any)")); }
        if (n>1 && (params[1].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randnormvec_seed needs a double as second argument (if any)")); }
        if (n>2 && (params[2].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randnormvec_seed needs a double as third argument (if any)")); }
        if (n>4 && (params[3].type!=jkmpDouble)) { ok=false;  p->jkmpError(JKMP::_("randnormvec_seed needs a double as fourth argument (if any)")); }
        int items=params[0].toInteger();
        if (items<=0) { ok=false; p->jkmpError(JKMP::_("randnormvec_seed's first argument has to be a positive integer number")); }
        if (ok) {
            MTRand rng;

            r.setDoubleVec(items, 0);
            double mean=0;
            double var=1;
            if (n==3) {
                var=params[1].num;
                rng.seed(params[2].num);
            } else if (n==2) {
                rng.seed(params[1].num);
            } else if (n>3) {
                mean=params[1].num;
                var=params[2].num;
                rng.seed(params[3].num);
            }

            for (int i=0; i<items; i++)
              r.numVec[i]=rng.randNorm(mean, var);
        } else {
          r.setInvalid();
        }

}



    void fRandBoolVecSeed(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        bool ok=true;
      if (n>3 || n<=1) { ok=false; p->jkmpError(JKMP::_("randboolvec_seed accepts 2 or 3 arguments")); }
      if (n>0 && (params[0].type!=jkmpDouble)) { ok=false; p->jkmpError(JKMP::_("randboolvec_seed needs a double as first argument (if any)")); }
      if (n>1 && (params[1].type!=jkmpDouble)) { ok=false; p->jkmpError(JKMP::_("randboolvec_seed needs a double as second argument (if any)")); }
      if (n>2 && (params[2].type!=jkmpDouble)) { ok=false; p->jkmpError(JKMP::_("randboolvec_seed needs a double as third argument (if any)")); }
      int items=params[0].toInteger();
      double pr=0.5;
      double seed=params[1].num;
      if (n>2) {
          pr=params[1].num;
          seed=params[2].num;
      }
      if (items<=0) { ok=false; p->jkmpError(JKMP::_("randboolvec_seed's first argument has to be a positive integer number")); }
      if (pr<0 || pr>1) { ok=false; p->jkmpError(JKMP::_("randboolvec_seed's second argument has to be a number between 0 and 1")); }
      if (ok) {
          r.setBoolVec(items, false);
          MTRand rng;
          rng.seed(seed);
          for (int i=0; i<items; i++)
            r.boolVec[i]=(rng.rand()<=pr);
      } else {
          r.setInvalid();
      }

    }
















    void fRandBool(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        bool ok=true;
      if (n>1) { ok=false; p->jkmpError(JKMP::_("randbool accepts 0 or 1 arguments")); }
      if (n>0 && (params[0].type!=jkmpDouble)) { ok=false; p->jkmpError(JKMP::_("randboolvec needs a double as first argument (if any)")); }
      double pr=0.5;
      if (n>1) pr=params[0].num;
      if (pr<0 || pr>1) { ok=false; p->jkmpError(JKMP::_("randbool's second argument has to be a number between 0 and 1")); }
      if (ok) {
          r.setBoolean(p->get_rng()->rand()<=pr);
      } else {
          r.setInvalid();
      }
    }






    jkmpResult fMin(const jkmpResult* params, unsigned int  n, JKMathParser* p){
      jkmpResult r;
      r.type=jkmpDouble;
      if (n==1 && params[0].type==jkmpDoubleVector) {
          r.num=statisticsMin(params[0].numVec.data(), params[0].numVec.size());
      } else if (n==2 && params[0].type==jkmpDouble && params[1].type==jkmpDouble) {
          r.num=fmin(params[0].num, params[1].num);
      } else {
          p->jkmpError(JKMP::_("min(...) takes two number or 1 number vector argument"));
          r.isValid=false;
          return r;
      }
      return r;
    }



    jkmpResult fMax(const jkmpResult* params, unsigned int  n, JKMathParser* p){
        jkmpResult r;
        r.type=jkmpDouble;
        if (n==1 && params[0].type==jkmpDoubleVector) {
            r.num=statisticsMax(params[0].numVec.data(), params[0].numVec.size());
        } else if (n==2 && params[0].type==jkmpDouble && params[1].type==jkmpDouble) {
            r.num=fmax(params[0].num, params[1].num);
        } else {
            p->jkmpError(JKMP::_("max(...) takes two number or 1 number vector argument"));
            r.isValid=false;
            return r;
        }
        return r;
    }

    void fLength(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("length(x) need one argument"));
            r.setInvalid();
            return;
        }
        r.setDouble(params[0].size());
    }


    void fDimensions(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("dimensions(x) need one argument"));
            r.setInvalid();
            return;
        }
        r.setDouble(params[0].dimensions());
    }

    void fSize(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n!=1) {
            p->jkmpError(JKMP::_("size(x) need one argument"));
            r.setInvalid();
            return;
        }
        r.setDoubleVec(0);
        if (params[0].isMatrix()) {
            r.numVec<<params[0].sizeY()<<params[0].sizeX();
        } else {
            r.numVec<<params[0].sizeX();
        }
    }


    void fColumns(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("sizecols(x) need one argument"));
            r.setInvalid();
            return;
        }
        r.setDouble(params[0].columns());
    }

    void fRows(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("sizerows(x) need one argument"));
            r.setInvalid();
            return;
        }
        r.setDouble(params[0].rows());
    }

    void fRemove(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setInvalid();
        if (n==2 && params[0].type==jkmpDoubleVector && params[1].convertsToIntVector()) {
            JKMP::vector<int> ii=params[1].asIntVector();
            JKMP::vector<double> dat=params[0].numVec;
            r.setDoubleVec();
            for (size_t i=0; i<dat.size(); i++) {
                if (!ii.contains(i)) {
                    r.numVec<<dat[i];
                }
            }
        } else if (n==2 && params[0].type==jkmpStringVector && params[1].convertsToIntVector()) {
            JKMP::vector<int> ii=params[1].asIntVector();
            JKMP::stringVector dat=params[0].strVec;
            r.setStringVec();
            for (size_t i=0; i<dat.size(); i++) {
                if (!ii.contains(i)) {
                    r.strVec<<dat[i];
                }
            }
        } else if (n==2 && params[0].type==jkmpBoolVector && params[1].convertsToIntVector()) {
            JKMP::vector<int> ii=params[1].asIntVector();
            JKMP::vector<bool> dat=params[0].boolVec;
            r.setBoolVec();
            for (size_t i=0; i<dat.size(); i++) {
                if (!ii.contains(i)) {
                    r.boolVec<<dat[i];
                }
            }
        } else {
            p->jkmpError(JKMP::_("remove(x, idx) need one number vector and one number or number vector argument"));
            r.setInvalid();
            return;
        }
    }

    void fDot(jkmpResult &r,const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setInvalid();
        if (n==2 && params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector && params[0].numVec.size()==params[1].numVec.size()) {
            double sum=0;
            for (int i=0; i<params[0].numVec.size(); i++) {
                sum+=params[0].numVec[i]*params[1].numVec[i];
            }
            r.setDouble(sum);
        } else {
            p->jkmpError(JKMP::_("dot(x, y) needs two number vector arguments of equal size"));
            r.setInvalid();
            return;
        }
    }

    void fConcat(jkmpResult &r,const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setInvalid();
        if (n>0) {
            if (params[0].type==jkmpList) {
                r.setList(params[0].listData);
                for (unsigned int i=1; i<n; i++) {
                    if (params[i].type==jkmpList) {
                        r.listData<<params[i].listData;
                    } else {
                        r.listData<<params[i];
                    }
                }
            } else if (params[0].type==jkmpStruct) {
                r.setStruct();
                r.structData=params[0].structData;
                for (unsigned int i=1; i<n; i++) {
                    if (params[i].type==jkmpStruct) {
                        for (auto it=params[i].structData.begin(); it!=params[i].structData.end(); ++it) {
                            r.structData[it->first]=it->second;
                        }
                    } else {
                        p->jkmpError(JKMP::_("concat(x1, x2, ...) can only concatenate maps to  other maps, not to %1 in argument %2").arg(params[i].typeName()).arg(i+1));
                        r.setInvalid();
                        return;
                    }
                }
            } else if (params[0].type==jkmpDouble || params[0].type==jkmpDoubleVector) {
                r.setDoubleVec();
                for (unsigned int i=0; i<n; i++) {
                    if (params[i].convertsToVector()) {
                        r.numVec<<params[i].asVector();
                    } else {
                        p->jkmpError(JKMP::_("concat(x1, x2, ...) needs one or more vectors or vectorelements as arguments (all have to have the same type)"));
                        r.setInvalid();
                        return;
                    }
                }
            } else if (params[0].type==jkmpBool || params[0].type==jkmpBoolVector) {
                r.setBoolVec();
                for (unsigned int i=0; i<n; i++) {
                    if (params[i].convertsToBoolVector()) {
                        r.boolVec<<params[i].asBoolVector();
                    } else {
                        p->jkmpError(JKMP::_("concat(x1, x2, ...) needs one or more vectors or vectorelements as arguments (all have to have the same type)"));
                        r.setInvalid();
                        return;
                    }
                }
            } else if (params[0].type==jkmpString || params[0].type==jkmpStringVector) {
                r.setStringVec();
                for (unsigned int i=0; i<n; i++) {
                    if (params[i].convertsToStringVector()) {
                        r.strVec<<params[i].asStrVector();
                    } else {
                        p->jkmpError(JKMP::_("concat(x1, x2, ...) needs one or more vectors or vectorelements as arguments (all have to have the same type)"));
                        r.setInvalid();
                        return;
                    }
                }
            }
        } else {
            p->jkmpError(JKMP::_("concat(x1, x2, ...) needs one or more vectors or vectorelements as arguments (all have to have the same type)"));
            r.setInvalid();
            return;
        }
    }

    void fRemoveAll(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setInvalid();
        if (n==2 && params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble) {
            const JKMP::vector<double>& dat=params[0].numVec;
            r.type=jkmpDoubleVector;
            r.numVec.clear();
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                if (dat[i]!=params[1].num) {
                    r.numVec<<dat[i];
                }
            }
        } else if (n==2 && params[0].type==jkmpBoolVector && params[1].type==jkmpBool) {
            const JKMP::vector<bool>& dat=params[0].boolVec;
            r.type=jkmpBoolVector;
            r.boolVec.clear();
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                if (dat[i]!=params[1].boolean) {
                    r.boolVec<<dat[i];
                }
            }
        } else if (n==2 && params[0].type==jkmpStringVector && params[1].type==jkmpString) {
            const JKMP::stringVector& dat=params[0].strVec;
            r.type=jkmpStringVector;
            r.strVec.clear();
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                if (dat[i]!=params[1].str) {
                    r.strVec<<dat[i];
                }
            }
        } else {
            p->jkmpError(JKMP::_("removeall(x, value) need one number vector and one number argument"));
            r.setInvalid();
            return;
        }

    }


    void fFilterFinite(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setInvalid();
        if (n==1 && params[0].type==jkmpDoubleVector) {
            const JKMP::vector<double>& dat=params[0].numVec;
            r.type=jkmpDoubleVector;
            r.numVec.clear();
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                if (JKMP_FloatIsOK(dat[i])) {
                    r.numVec<<dat[i];
                }
            }
        } else {
            p->jkmpError(JKMP::_("filterfinite(x) need one number vector argument"));
            r.setInvalid();
            return;
        }
    }

    void fFind(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setInvalid();
        if (n==2 && params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble) {
            const JKMP::vector<double>& dat=params[0].numVec;
            r.type=jkmpDoubleVector;
            r.numVec.clear();
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                if (dat[i]==params[1].num) {
                    r.numVec<<i;
                }
            }
        } else if (n==2 && params[0].type==jkmpBoolVector && params[1].type==jkmpBool) {
            const JKMP::vector<bool>& dat=params[0].boolVec;
            r.type=jkmpDoubleVector;
            r.numVec.clear();
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                if (dat[i]==params[1].boolean) {
                    r.numVec<<i;
                }
            }
        } else if (n==1 && params[0].type==jkmpBoolVector) {
            const JKMP::vector<bool>& dat=params[0].boolVec;
            r.type=jkmpDoubleVector;
            r.numVec.clear();
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                if (dat[i]) {
                    r.numVec<<i;
                }
            }
        } else if (n==2 && params[0].type==jkmpStringVector && params[1].type==jkmpString) {
            const JKMP::stringVector& dat=params[0].strVec;
            r.type=jkmpDoubleVector;
            r.numVec.clear();
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                if (dat[i]==params[1].str) {
                    r.numVec<<i;
                }
            }
        } else {
            p->jkmpError(JKMP::_("find(x, value=true) needs two arguments: one vector x and a corresponding element value, if only a boolean vector is given, the function uses value=true"));
            r.setInvalid();
            return;
        }
    }


    void fFindFirst(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setInvalid();
        if (n==2 && params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble) {
            const JKMP::vector<double>& dat=params[0].numVec;
            r.type=jkmpDouble;
            r.num=-1;
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                if (dat[i]==params[1].num) {
                    r.num=i;
                    break;
                }
            }
        } else if (n==2 && params[0].type==jkmpBoolVector && params[1].type==jkmpBool) {
            const JKMP::vector<bool>& dat=params[0].boolVec;
            r.type=jkmpDoubleVector;
            r.type=jkmpDouble;
            r.num=-1;
            for (size_t i=0; i<dat.size(); i++) {
                if (dat[i]==params[1].boolean) {
                    r.num=i;
                    break;
                }
            }
        } else if (n==1 && params[0].type==jkmpBoolVector) {
            const JKMP::vector<bool>& dat=params[0].boolVec;
            r.type=jkmpDoubleVector;
            r.type=jkmpDouble;
            r.num=-1;
            for (size_t i=0; i<dat.size(); i++) {
                if (dat[i]) {
                    r.num=i;
                    break;
                }
            }
        } else if (n==2 && params[0].type==jkmpStringVector && params[1].type==jkmpString) {
            const JKMP::stringVector& dat=params[0].strVec;
            r.type=jkmpDoubleVector;
            r.type=jkmpDouble;
            r.num=-1;
            for (size_t i=0; i<dat.size(); i++) {
                if (dat[i]==params[1].str) {
                    r.num=i;
                    break;
                }
            }
        } else  if (n==2 && params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector) {
            const JKMP::vector<double>& dat=params[0].numVec;
            r.type=jkmpDoubleVector;
            r.numVec.clear();
            r.isValid=true;
            for (size_t j=0; j<params[1].numVec.size(); j++) {
                int add=-1;
                double v=params[1].numVec[j];
                for (size_t i=0; i<dat.size(); i++) {
                    if (dat[i]==v) {
                        add=i;
                        break;
                    }
                }
                r.numVec.push_back(add);
            }
        } else  if (n==2 && params[0].type==jkmpStringVector && params[1].type==jkmpStringVector) {
            const JKMP::stringVector& dat=params[0].strVec;
            r.type=jkmpDoubleVector;
            r.numVec.clear();
            r.isValid=true;
            for (size_t j=0; j<params[1].strVec.size(); j++) {
                int add=-1;
                JKMP::string v=params[1].strVec[j];
                for (size_t i=0; i<dat.size(); i++) {
                    if (dat[i]==v) {
                        add=i;
                        break;
                    }
                }
                r.numVec.push_back(add);
            }
        } else  if (n==2 && params[0].type==jkmpBoolVector && params[1].type==jkmpBoolVector) {
            const JKMP::vector<bool>& dat=params[0].boolVec;
            r.type=jkmpDoubleVector;
            r.numVec.clear();
            r.isValid=true;
            for (size_t j=0; j<params[1].numVec.size(); j++) {
                int add=-1;
                bool v=params[1].boolVec[j];
                for (size_t i=0; i<dat.size(); i++) {
                    if (dat[i]==v) {
                        add=i;
                        break;
                    }
                }
                r.numVec.push_back(add);
            }
        } else {
            p->jkmpError(JKMP::_("findfirst(x, value=true) needs two arguments: one vector x and a corresponding element value (or vector), if only a boolean vector is given, the function uses value=true"));
            r.setInvalid();
            return;
        }
    }


    void fContains(jkmpResult& r, const jkmpResult *params, unsigned int  n, JKMathParser* p){
        r.setInvalid();
        if (n==2 && params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble) {
            r.type=jkmpBool;
            r.boolean=params[0].numVec.contains(params[1].num);
            r.isValid=true;
        } else if (n==2 && params[0].type==jkmpBoolVector && params[1].type==jkmpBool) {
            r.type=jkmpBool;
            r.boolean=params[0].boolVec.contains(params[1].boolean);
            r.isValid=true;
        } else if (n==2 && params[0].type==jkmpStringVector && params[1].type==jkmpString) {
            r.type=jkmpBool;
            r.boolean=params[0].strVec.contains(params[1].str);
            r.isValid=true;
        } else if (n==2 && params[0].type==jkmpString && params[1].type==jkmpString) {
            const JKMP::string& dat=params[0].str;
            r.type=jkmpBool;
            r.boolean=dat.contains(params[1].str);
        } else {
            p->jkmpError(JKMP::_("contains(x, value) needs two arguments: one vector x and a corresponding element value (or vector)"));
            r.setInvalid();
            return;
        }
    }

    void fContainsLC(jkmpResult& r, const jkmpResult *params, unsigned int  n, JKMathParser* p){
        r.setInvalid();
        if (n==2 && params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble) {
            r.type=jkmpBool;
            r.boolean=params[0].numVec.contains(params[1].num);
            r.isValid=true;
        } else if (n==2 && params[0].type==jkmpBoolVector && params[1].type==jkmpBool) {
            r.type=jkmpBool;
            r.boolean=params[0].boolVec.contains(params[1].boolean);
            r.isValid=true;
        } else if (n==2 && params[0].type==jkmpStringVector && params[1].type==jkmpString) {
            r.type=jkmpBool;
            r.boolean=params[0].strVec.contains(params[1].str, false);
            r.isValid=true;
        } else if (n==2 && params[0].type==jkmpString && params[1].type==jkmpString) {
            const JKMP::string& dat=params[0].str;
            r.type=jkmpBool;
            r.boolean=dat.contains(params[1].str, false);
        } else {
            p->jkmpError(JKMP::_("contains_caseinsensitive(x, value) needs two arguments: one vector x and a corresponding element value (or vector)"));
            r.setInvalid();
            return;
        }
    }


    void fContainsSubString(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setInvalid();

        if (n==2 && params[0].type==jkmpStringVector && params[1].type==jkmpString) {
            const JKMP::stringVector& dat=params[0].strVec;
            r.type=jkmpBoolVector;
            r.boolVec.clear();
            for (size_t i=0; i<dat.size(); i++) {
                bool f=false;
                if (dat[i].contains(params[1].str)) {
                    f=true;
                    break;
                }
                r.boolVec.push_back(f);
            }
        } else if (n==2 && params[0].type==jkmpString && params[1].type==jkmpString) {
            const JKMP::string& dat=params[0].str;
            r.type=jkmpBool;
            r.boolean=dat.contains(params[1].str);
        } else  if (n==2 && params[0].type==jkmpStringVector && params[1].type==jkmpStringVector) {
            const JKMP::stringVector& dat=params[0].strVec;
            r.type=jkmpDoubleVector;
            r.numVec.clear();
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                int add=-1;
                for (int j=0; j<params[1].strVec.size(); j++) {
                    JKMP::string v=params[1].strVec[j];
                    if (dat[i].contains(v)) {
                        add=j;
                        break;
                    }
                }
                r.numVec.push_back(add);
            }
        } else  if (n==2 && params[0].type==jkmpString && params[1].type==jkmpStringVector) {
            r.type=jkmpDouble;
            r.num=-1;
            r.isValid=true;
            for (int j=0; j<params[1].strVec.size(); j++) {
                if (params[0].str.contains(params[1].strVec[j])) {
                    r.num=j;
                    break;
                }
            }
        }  else {
            p->jkmpError(JKMP::_("containssubstr(x, value) needs two arguments: stringVec/string, string/string or stringvec/string_vec"));
            r.setInvalid();
            return;
        }
    }

    void fContainsSubStringLC(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setInvalid();

        if (n==2 && params[0].type==jkmpStringVector && params[1].type==jkmpString) {
            const JKMP::stringVector& dat=params[0].strVec;
            r.type=jkmpBoolVector;
            r.boolVec.clear();
            for (size_t i=0; i<dat.size(); i++) {
                bool f=false;
                if (dat[i].contains(params[1].str, false)) {
                    f=true;
                    break;
                }
                r.boolVec.push_back(f);
            }
        } else if (n==2 && params[0].type==jkmpString && params[1].type==jkmpString) {
            const JKMP::string& dat=params[0].str;
            r.type=jkmpBool;
            r.boolean=dat.contains(params[1].str, false);
        } else  if (n==2 && params[0].type==jkmpStringVector && params[1].type==jkmpStringVector) {
            const JKMP::stringVector& dat=params[0].strVec;
            r.type=jkmpDoubleVector;
            r.numVec.clear();
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                int add=-1;
                for (size_t j=0; j<params[1].strVec.size(); j++) {
                    JKMP::string v=params[1].strVec[j];
                    if (dat[i].contains(v, false)) {
                        add=j;
                        break;
                    }
                }
                r.numVec.push_back(add);
            }
        } else  if (n==2 && params[0].type==jkmpString && params[1].type==jkmpStringVector) {
            r.type=jkmpDouble;
            r.num=-1;
            r.isValid=true;
            for (size_t j=0; j<params[1].strVec.size(); j++) {
                if (params[0].str.contains(params[1].strVec[j], false)) {
                    r.num=j;
                    break;
                }
            }
        }  else {
            p->jkmpError(JKMP::_("containssubstr_caseinsensitive(x, value) needs two arguments: stringVec/string, string/string or stringvec/string_vec"));
            r.setInvalid();
            return;
        }
    }
    void fSelect(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setInvalid();
        if (n==2 && params[0].type==jkmpDoubleVector && params[1].type==jkmpBoolVector) {
            const JKMP::vector<double>& dat=params[0].numVec;
            if (dat.size()!=params[1].boolVec.size()) {
                p->jkmpError(JKMP::_("find(x, criterion) vectors x and criterion have different size"));
                r.setInvalid();
                return ;
            }
            r.type=jkmpDoubleVector;
            r.numVec.clear();
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                if (params[1].boolVec[i]) {
                    r.numVec<<dat[i];
                }
            }
        } else if (n==2 && params[0].type==jkmpBoolVector && params[1].type==jkmpBoolVector) {
            const JKMP::vector<bool>& dat=params[0].boolVec;
            if (dat.size()!=params[1].boolVec.size()) {
                p->jkmpError(JKMP::_("find(x, criterion) vectors x and criterion have different size"));
                r.setInvalid();
                return ;
            }
            r.type=jkmpBoolVector;
            r.boolVec.clear();
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                if (params[1].boolVec[i]) {
                    r.boolVec<<dat[i];
                }
            }

        } else if (n==2 && params[0].type==jkmpStringVector && params[1].type==jkmpBoolVector) {
            const JKMP::stringVector& dat=params[0].strVec;
            if (dat.size()!=params[1].boolVec.size()) {
                p->jkmpError(JKMP::_("find(x, criterion) vectors x and criterion have different size"));
                r.setInvalid();
                return ;
            }
            r.type=jkmpStringVector;
            r.strVec.clear();
            r.isValid=true;
            for (size_t i=0; i<dat.size(); i++) {
                if (params[1].boolVec[i]) {
                    r.strVec<<dat[i];
                }
            }
        } else {
            p->jkmpError(JKMP::_("find(x, criterion) needs two arguments, a vector x and a boolean vector criterion of same length"));
            r.setInvalid();
            return;
        }
    }
     void fReverse(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setInvalid();
        if (n==1 && params[0].type==jkmpDoubleVector) {
            r.setDoubleVec(params[0].numVec);
            for (size_t i=0; i<r.numVec.size(); i++) {
                r.numVec[i]=params[0].numVec[r.numVec.size()-i-1];
            }
        } else if (n==1 && params[0].type==jkmpStringVector) {
            r.setStringVec(params[0].strVec);
            for (size_t i=0; i<r.strVec.size(); i++) {
                r.strVec[i]=params[0].strVec[r.strVec.size()-i-1];
            }
        } else if (n==1 && params[0].type==jkmpBoolVector) {
            r.setBoolVec(params[0].boolVec);
            for (size_t i=0; i<r.boolVec.size(); i++) {
                r.boolVec[i]=params[0].boolVec[r.boolVec.size()-i-1];
            }
        } else {
            p->jkmpError(JKMP::_("reverse(x) need one number vector argument"));
            r.setInvalid();
            return ;
        }
    }



    void fUnique(jkmpResult &res, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        res.setInvalid();

        if (n!=1) {
            p->jkmpError("unique(data) needs 1 argument");
            return ;
        }
        if ((params[0].type==jkmpDouble) || (params[0].type==jkmpString) || (params[0].type==jkmpBool)) {
            res=params[0];
            return;
        }

        if (params[0].type==jkmpDoubleVector) {
            JKMP::vector<double> out;
            const JKMP::vector<double>& in=params[0].numVec;

            for (size_t i=0; i<in.size(); i++) {
                if (!out.contains(in[i])) out<<in[i];
            }

            res.setDoubleVec(out);
            return;
        }

        if (params[0].type==jkmpStringVector) {
            JKMP::stringVector out;
            const JKMP::stringVector& in=params[0].strVec;

            for (size_t i=0; i<in.size(); i++) {
                if (!out.contains(in[i])) out<<in[i];
            }

            res.setStringVec(out);
            return;
        }

        if (params[0].type==jkmpBoolVector) {
            JKMP::vector<bool> out;
            const JKMP::vector<bool>& in=params[0].boolVec;

            for (size_t i=0; i<in.size(); i++) {
                if (!out.contains(in[i])) out<<in[i];
                if (out.size()>1) break;
            }

            res.setBoolVec(out);
            return;
        }
    }






    jkmpResult fIndexedAvg(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) p->jkmpError("indexedmean(data, index) needs 2 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size()) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsAverageV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsAverageV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsAverageV<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError("indexedmean(data, index) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }

    jkmpResult fIndexedVar(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) p->jkmpError("indexedvar(data, index) needs 2 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size()) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsVarianceV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsVarianceV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsVarianceV<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError("indexedvar(data, index) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }



    jkmpResult fIndexedStd(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) p->jkmpError("indexedstd(data, index) needs 2 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size()) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsStdDevV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsStdDevV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsStdDevV<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError("indexedstd(data, index) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }

    jkmpResult fIndexedSum(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) p->jkmpError("indexedsum(data, index) needs 2 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size()) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsSumV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsSumV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsSumV<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError("indexedsum(data, index) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }

    jkmpResult fIndexedSum2(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) p->jkmpError("indexedsum2(data, index) needs 2 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size()) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsSum2V<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsSum2V<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsSum2V<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError("indexedsum2(data, index) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }


    jkmpResult fIndexedMedian(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) p->jkmpError("indexedmedian(data, index) needs 2 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size()) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsMedianV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsMedianV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsMedianV<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError("indexedmedian(data, index) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }



    jkmpResult fIndexedMAD(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) p->jkmpError("indexedmad(data, index) needs 2 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size()) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsMADV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsMADV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsMADV<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError("indexedmad(data, index) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }

    jkmpResult fIndexedNMAD(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) p->jkmpError("indexedmad(data, index) needs 2 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size()) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsNMADV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsNMADV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsNMADV<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError("indexedmad(data, index) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }


    jkmpResult fIndexedMin(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) p->jkmpError("indexedmin(data, index) needs 2 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size()) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsMinV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsMinV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsMinV<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError("indexedmin(data, index) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }

    jkmpResult fIndexedMax(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) p->jkmpError("indexedmax(data, index) needs 2 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size()) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsMaxV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsMaxV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsMaxV<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError("indexedmax(data, index) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }

    jkmpResult fIndexedQuantile(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=3) p->jkmpError("indexedquantile(data, index, quantile) needs 3 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size() && params[2].type==jkmpDouble) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsQuantileV<JKMP::vector<double> >, params[2].num));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsQuantileV<JKMP::vector<double> >, params[2].num));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsQuantileV<JKMP::vector<double> >, params[2].num));
            }
        } else {
            p->jkmpError("indexedquantile(data, index, quantile) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }


    jkmpResult fIndexedSkewness(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) p->jkmpError("indexedskewness(data, index) needs 2 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size()) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsSkewnessV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsSkewnessV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsSkewnessV<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError("indexedskewness(data, index) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }

    jkmpResult fIndexedProd(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) p->jkmpError("indexedprod(data, index) needs 2 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size()) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsProdV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsProdV<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsProdV<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError("indexedprod(data, index) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }

    jkmpResult fIndexedCount(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) p->jkmpError("indexedcount(data, index) needs 2 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size()) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsCount<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsCount<JKMP::vector<double> >));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsCount<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError("indexedcount(data, index) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }


    jkmpResult fIndexedMoment(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=3) p->jkmpError("indexedmoment(data, index, order) needs 3 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size() && params[2].type==jkmpDouble) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsCentralMomentV<JKMP::vector<double> >, params[2].toInteger()));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsCentralMomentV<JKMP::vector<double> >, params[2].toInteger()));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsCentralMomentV<JKMP::vector<double> >, params[2].toInteger()));
            }
        } else {
            p->jkmpError("indexedmoment(data, index, order) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }

    jkmpResult fIndexedNonCentralMoment(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=3) p->jkmpError("indexedncmoment(data, index, order) needs 3 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && params[0].size()==params[1].size() && params[2].type==jkmpDouble) {
            JKMP::vector<double> d=params[0].asVector();

            if (params[1].type==jkmpDouble || params[1].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asVector(), statisticsMomentV<JKMP::vector<double> >, params[2].toInteger()));
            }
            if (params[1].type==jkmpBool || params[1].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asBoolVector(), statisticsMomentV<JKMP::vector<double> >, params[2].toInteger()));
            }
            if (params[1].type==jkmpString || params[1].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction(d, params[1].asStrVector(), statisticsMomentV<JKMP::vector<double> >, params[2].toInteger()));
            }
        } else {
            p->jkmpError("indexedncmoment(data, index, order) needs a number vector as data argument and an equal sized index array of any type");
        }

        return res;
    }

    jkmpResult fIndexedCorrCoeff(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=3) p->jkmpError("indexedcorrcoeff(dataX, dataY, index) needs 3 argument");
        else if (((params[0].type==jkmpDoubleVector)||(params[0].type==jkmpDouble)) && ((params[1].type==jkmpDoubleVector)||(params[1].type==jkmpDouble)) && params[0].size()==params[2].size() && params[1].size()==params[2].size()) {
            JKMP::vector<double> d=params[0].asVector();
            JKMP::vector<double> dy=params[1].asVector();

            if (params[2].type==jkmpDouble || params[2].type==jkmpDoubleVector) {
                return jkmpResult(JKMP::uniqueApplyFunction2I(d, dy, params[2].asVector(), statisticsCorrelationCoefficientV<JKMP::vector<double> >));
            }
            if (params[2].type==jkmpBool || params[2].type==jkmpBoolVector) {
                return jkmpResult(JKMP::uniqueApplyFunction2I(d, dy, params[2].asBoolVector(), statisticsCorrelationCoefficientV<JKMP::vector<double> >));
            }
            if (params[2].type==jkmpString || params[2].type==jkmpStringVector) {
                return jkmpResult(JKMP::uniqueApplyFunction2I(d, dy, params[2].asStrVector(), statisticsCorrelationCoefficientV<JKMP::vector<double> >));
            }
        } else {
            p->jkmpError(JKMP::_("indexedcorrcoeff(dataX, dataY, index) needs a number vector as data argument and an equal sized index array of any type [len(dataX)=%1, len(dataY)=%2, len(idx)=%3]").arg(params[0].size()).arg(params[1].size()).arg(params[2].size()));
        }

        return res;
    }














    jkmpResult fReturnLast(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n<1) {
            p->jkmpError("returnlast(x1,x2,x3,...) needs at least one argument");
            return res;
        }
        return params[n-1];
    }

    void fReturnNth(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        res.setInvalid();

        if (n<2) {
            p->jkmpError("returnnth(n,x1,x2,x3,...) needs at least two arguments");
            res.setInvalid();
            return;
        }
        int d=0;
        if (params[0].isInteger()) {
            d=params[0].toInteger();
        } else {
            p->jkmpError("returnnth(n,x1,x2,x3,...) needs an integer as first argument");
            res.setInvalid();
            return;
        }
        if (d<0 || d>=(long long)n-1) {
            p->jkmpError("returnnth(n,x0,x1,x2,...) parameter n has to index one of the elements x0,x1,...");
            res.setInvalid();
            return;
        }
        res=params[d+1];
    }


    jkmpResult fReturnFirst(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n<1) {
            p->jkmpError("returnfirst(x1,x2,x3,...) needs at least one argument");
            return res;
        }
        return params[0];
    }

    void fZeros(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        res.setInvalid();

        if (n==1 && params[0].isUInt())  {
            res.setDoubleVec(params[0].toUInt(), 0.0);
        } else {
            p->jkmpError("zeros(x) needs one unsigned integer argument");
            res.setInvalid();
            return ;
        }
    }


    void fOnes(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        res.setInvalid();

        if (n==1 && params[0].isUInt())  {
            res.setDoubleVec(params[0].toUInt(), 1.0);
        } else {
            p->jkmpError("ones(x) needs one unsigned integer argument");
            res.setInvalid();
            return ;
        }
    }
    void fVector(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        res.setInvalid();

        if (n==2 && params[0].isUInt())  {
            int items=params[0].toUInt();
            if (params[1].type==jkmpDouble) {
                res.setDoubleVec(items, params[1].num);
            } else if (params[1].type==jkmpString) {
                res.setStringVec(items, params[1].str);
            } else if (params[1].type==jkmpBool) {
                res.setBoolVec(items, params[1].boolean);
            } else if (params[1].type==jkmpDoubleVector) {
                res.setDoubleVec(items*params[1].numVec.size());
                int n=0;
                for (int i=0; i<items; i++) {
                    for (size_t j=0; j<params[1].numVec.size(); j++) {
                        res.numVec[n]=params[1].numVec[j];
                        n++;
                    }
                }
            } else if (params[1].type==jkmpStringVector) {
                res.setStringVec(items*params[1].strVec.size());
                int n=0;
                for (int i=0; i<items; i++) {
                    for (size_t j=0; j<params[1].strVec.size(); j++) {
                        res.strVec[n]=params[1].strVec[j];
                        n++;
                    }
                }
            } else if (params[1].type==jkmpBoolVector) {
                res.setBoolVec(items*params[1].boolVec.size());
                int n=0;
                for (int i=0; i<items; i++) {
                    for (size_t j=0; j<params[1].boolVec.size(); j++) {
                        res.boolVec[n]=params[1].boolVec[j];
                        n++;
                    }
                }
            }
        } else {
            p->jkmpError("vector(x, value) needs one unsigned integer argument and a non-vector argument");
            res.setInvalid();
            return ;
        }
    }

    void fLinSpace(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        res.setInvalid();
        int items=100;
        double x0=0;
        double x1=0;

        if (n>=2 && params[0].type==jkmpDouble && params[1].type==jkmpDouble)  {
            x0=params[0].num;
            x1=params[1].num;

            if (n==3 && params[2].isUInt()) {
                items=params[2].toUInt();
            } else if (n==3) {
                p->jkmpError("linspace(x0, x1[, items]) needs an unsigned integer as third (optional) parameter");
                res.setInvalid();
                return;
            }
            res.setDoubleVec(items, 0.0);
            for (int i=0; i<items; i++) {
                res.numVec[i]=x0+double(i)/double(items-1)*(x1-x0);
            }
        } else {
            p->jkmpError("linspace(x0, x1[, items]) needs two number parameters x0, x1");
            res.setInvalid();
            return ;
        }
    }

    void fLogSpace(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        res.setInvalid();
        int items=50;
        double x0=0;
        double x1=0;

        if (n>=2 && params[0].type==jkmpDouble && params[1].type==jkmpDouble)  {
            x0=params[0].num;
            x1=params[1].num;

            if (n==3 && params[2].isUInt()) {
                items=params[2].toUInt();
            } else if (n==3) {
                p->jkmpError("logspace(a, b[, items]) needs an unsigned integer as third (optional) parameter");
                res.setInvalid();
                return;
            }
            res.setDoubleVec(items, 0.0);
            for (int i=0; i<items; i++) {
                res.numVec[i]=pow(10.0, x0+double(i)/double(items-1)*(x1-x0));
            }
        } else {
            p->jkmpError("logspace(a, b[, items]) needs two number parameters x0, x1");
            res.setInvalid();
            return ;
        }
    }

    void fMeshGrid2Dx(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        res.setInvalid();
        if (n==2 && params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector)  {
            int Nx=params[0].numVec.size();
            int Ny=params[1].numVec.size();

            res.setDoubleVec(Nx*Ny, 0.0);
            int i=0;
            for (int y=0; y<Ny; y++) {
                for (int x=0; x<Nx; x++) {
                    res.numVec[i]=params[0].numVec[x];
                    i++;
                }
            }
        } else {
            p->jkmpError("meshgrid2d_x(X,Y) needs two number vector parameters");
            res.setInvalid();
            return ;
        }
    }


    void fMeshGrid2Dy(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        res.setInvalid();
        if (n==2 && params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector)  {
            int Nx=params[0].numVec.size();
            int Ny=params[1].numVec.size();

            res.setDoubleVec(Nx*Ny, 0.0);
            int i=0;
            for (int y=0; y<Ny; y++) {
                for (int x=0; x<Nx; x++) {
                    res.numVec[i]=params[1].numVec[y];
                    i++;
                }
            }
        } else {
            p->jkmpError("meshgrid2d_y(X,Y) needs two number vector parameters");
            res.setInvalid();
            return ;
        }
    }

    jkmpResult fLast(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=1) {
            p->jkmpError("last(x) needs one argument");
            return res;
        }
        if (params[0].type==jkmpDouble) return jkmpResult(params[0].num);
        if (params[0].type==jkmpDoubleVector && params[0].size()>0) return jkmpResult(params[0].numVec.back());
        if (params[0].type==jkmpBool) return jkmpResult(params[0].boolean);
        if (params[0].type==jkmpBoolVector && params[0].size()>0) return jkmpResult(params[0].boolVec.back());
        if (params[0].type==jkmpString) return jkmpResult(params[0].str.back());
        if (params[0].type==jkmpStringVector && params[0].size()>0) return jkmpResult(params[0].strVec.back());

        p->jkmpError("last(x): x had no entries or unrecognized type");
        return res;
    }


    jkmpResult fFirst(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=1) {
            p->jkmpError("first(x) needs one argument");
            return res;
        }
        if (params[0].type==jkmpDouble) return jkmpResult(params[0].num);
        if (params[0].type==jkmpDoubleVector && params[0].size()>0) return jkmpResult(params[0].numVec.front());
        if (params[0].type==jkmpBool) return jkmpResult(params[0].boolean);
        if (params[0].type==jkmpBoolVector && params[0].size()>0) return jkmpResult(params[0].boolVec.front());
        if (params[0].type==jkmpString) return jkmpResult(params[0].str.front());
        if (params[0].type==jkmpStringVector && params[0].size()>0) return jkmpResult(params[0].strVec.front());

        p->jkmpError("first(x): x had no entries or unrecognized type");
        return res;
    }


    jkmpResult fItem(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) {
            p->jkmpError("item(x, idx) needs two argument");
            return res;
        }
        JKMP::vector<int> idx=params[1].asIntVector();
        bool isNumber=true;
        JKMP::vector<bool> idxb=params[1].asBoolVector();
        if (idx.size()<=0 && idxb.size()<=0) {
            p->jkmpError("item(x, idx) parameter idx has to be a number, vector of numbers or vector of booleans");
            return res;
        } else if (idxb.size()>0) {
            isNumber=false;
            if (idxb.size()!=params[0].size()) {
                p->jkmpError(JKMP::_("item(x, boolidx) parameter boolidx has to be a vector of booleans with the same length as vector x [length(x)=%2, length(boolidx)=%1]").arg(idxb.size()).arg(params[0].size()));
                return res;
            }
        }
        if (isNumber) {
            if (params[0].type==jkmpDouble && idx.contains(0)) return jkmpResult(params[0].num);
            if (params[0].type==jkmpBool && idx.contains(0)) return jkmpResult(params[0].boolean);
            if (params[0].type==jkmpString) {
                JKMP::string r;
                for (size_t i=0; i<idx.size(); i++) {
                    if (i>=0 && i<params[0].str.size()) {
                        r+=params[0].str[idx[i]];
                    } else {
                        p->jkmpError(JKMP::_("item(x, idx) index i=%1 out of range").arg(idx[i]));
                        return res;
                    }
                }
                return jkmpResult(r);
            }
            if (params[0].type==jkmpStringVector) {
                JKMP::stringVector r;
                for (size_t i=0; i<idx.size(); i++) {
                    if (i>=0 && i<params[0].strVec.size()) {
                        r<<params[0].strVec[idx[i]];
                    } else {
                        p->jkmpError(JKMP::_("item(x, idx) index i=%1 out of range").arg(idx[i]));
                        return res;
                    }
                }
                if (r.size()==1) return jkmpResult(r[0]);
                return jkmpResult(r);
            }
            if (params[0].type==jkmpBoolVector) {
                JKMP::vector<bool> r;
                for (size_t i=0; i<idx.size(); i++) {
                    if (i>=0 && i<params[0].boolVec.size()) {
                        r<<params[0].boolVec[idx[i]];
                    } else {
                        p->jkmpError(JKMP::_("item(x, idx) index i=%1 out of range").arg(idx[i]));
                        return res;
                    }
                }
                if (r.size()==1) return jkmpResult(r[0]);
                return jkmpResult(r);
            }
            if (params[0].type==jkmpDoubleVector) {
                JKMP::vector<double> r;
                for (size_t i=0; i<idx.size(); i++) {
                    if (i>=0 && i<params[0].numVec.size()) {
                        r<<params[0].numVec[idx[i]];
                    } else {
                        p->jkmpError(JKMP::_("item(x, idx) index i=%1 out of range").arg(idx[i]));
                        return res;
                    }
                }
                if (r.size()==1) return jkmpResult(r[0]);
                return jkmpResult(r);
            }

        } else {
            if (params[0].type==jkmpString) {
                JKMP::string r;
                for (size_t i=0; i<idxb.size(); i++) {
                    if (idxb[i]) {
                        r+=params[0].str[i];
                    }
                }
                return jkmpResult(r);
            }
            if (params[0].type==jkmpStringVector) {
                JKMP::stringVector r;
                for (size_t i=0; i<idxb.size(); i++) {
                    if (idxb[i]) {
                        r<<params[0].strVec[i];
                    }
                }
                if (r.size()==1) return jkmpResult(r[0]);
                return jkmpResult(r);
            }
            if (params[0].type==jkmpBoolVector) {
                JKMP::vector<bool> r;
                for (size_t i=0; i<idxb.size(); i++) {
                    if (idxb[i]) {
                        r<<params[0].boolVec[i];
                    }
                }
                if (r.size()==1) return jkmpResult(r[0]);
                return jkmpResult(r);
            }
            if (params[0].type==jkmpDoubleVector) {
                JKMP::vector<double> r;
                for (size_t i=0; i<idxb.size(); i++) {
                    if (idxb[i]) {
                        r<<params[0].numVec[i];
                    }
                }
                if (r.size()==1) return jkmpResult(r[0]);
                return jkmpResult(r);
            }
        }

        p->jkmpError("item(x, idx): x had no entries or unrecognized type");
        return res;
    }


    jkmpResult fItemOrFirst(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) {
            p->jkmpError("itemorfirst(x, idx) needs two argument");
            return res;
        }
        JKMP::vector<int> idx=params[1].asIntVector();
        bool isNumber=true;
        JKMP::vector<bool> idxb=params[1].asBoolVector();
        if (idx.size()<=0 && idxb.size()>0) {
            isNumber=false;
            if (idxb.size()!=params[0].size()) {
                idxb.clear();
                idx.clear();
                isNumber=true;
            }
        }
        if (!isNumber) {
            idx.clear();
            for (size_t i=0; i<idxb.size(); i++) {
                if (idxb[i]) idx<<i;
            }
        }

        if (params[0].type==jkmpDouble && (idx.contains(0)||idx.is_empty())) return jkmpResult(params[0].num);
        if (params[0].type==jkmpBool && (idx.contains(0)||idx.is_empty())) return jkmpResult(params[0].boolean);
        if (params[0].type==jkmpString && (idx.contains(0)||idx.is_empty())) return jkmpResult(params[0].str);

        if (params[0].type==jkmpStringVector) {
            JKMP::stringVector r;
            for (size_t i=0; i<idx.size(); i++) {
                if (i>=0 && i<params[0].strVec.size()) {
                    r<<params[0].strVec[idx[i]];
                } else {
                    if (params[0].strVec.size()<=0) return jkmpResult::invalidResult();
                    return jkmpResult(params[0].strVec.value(0, ""));
                }
            }
            if (r.size()==1) return jkmpResult(r[0]);
            return jkmpResult(r);
        }
        if (params[0].type==jkmpBoolVector) {
            JKMP::vector<bool> r;
            for (size_t i=0; i<idx.size(); i++) {
                if (i>=0 && i<params[0].boolVec.size()) {
                    r<<params[0].boolVec[idx[i]];
                } else {
                    if (params[0].boolVec.size()<=0) return jkmpResult::invalidResult();
                    return jkmpResult(params[0].boolVec.value(0, false));
                }
            }
            if (r.size()==1) return jkmpResult(r[0]);
            return jkmpResult(r);
        }
        if (params[0].type==jkmpDoubleVector) {
            JKMP::vector<double> r;
            for (size_t i=0; i<idx.size(); i++) {
                if (i>=0 && i<params[0].numVec.size()) {
                    r<<params[0].numVec[idx[i]];
                } else {
                    if (params[0].numVec.size()<=0) return jkmpResult::invalidResult();
                    return jkmpResult(params[0].numVec.value(0, 0));
                }
            }
            if (r.size()==1) return jkmpResult(r[0]);
            return jkmpResult(r);
        }



        p->jkmpError("itemorfirst(x, idx): x had no entries or unrecognized type");
        return res;
    }
    jkmpResult fRunningAverage(const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=2) {
            p->jkmpError("runningaverage(x,n) needs two arguments");
            return res;
        }
        if (params[0].type==jkmpDoubleVector && params[1].isInteger() && params[1].toInteger()>0) {
            const JKMP::vector<double>& iv=params[0].numVec;
            int wid=params[1].toInteger();
            res.setDoubleVec(JKMP::vector<double>());
            for (size_t i=0; i<iv.size(); i+=wid) {
                double s=0;
                if (i+wid<iv.size()) {
                    for (int j=0; j<wid; j++) {
                        s+=iv[i+j];
                    }
                    res.numVec<<(s/double(wid));
                }
            }
            return res;
        } else {
            p->jkmpError("runningaverage(x,n) requires a number vector as first and an integer number >1 as second argument");
            return res;
        }
    }






    void fRegExpCapture(jkmpResult& r, const jkmpResult *params, unsigned int n, JKMathParser *p, bool minimal, bool casesens)
    {
        r.setInvalid();
        if (n<2 || n>4) {
            p->jkmpError(JKMP::_("regexpcap(regexp, strings, cap_id=1, default_string=\"\") needs 2 or 4 arguments"));
            r.setInvalid();
            return;
        }
        if(params[1].type==jkmpStringVector || params[1].type==jkmpString) {
            if(params[0].type==jkmpString) {
                int capid=1;
                JKMP::string defaultStr="";
                if (n>2) {
                    if(params[2].type==jkmpDouble && params[2].toInteger()>=0) {
                        capid=params[2].toInteger();
                    } else {
                        p->jkmpError(JKMP::_("regexpcap(regexp, strings, cap_id, default_string) argument cap_id has to be a number >=0"));
                        r.setInvalid();
                        return;
                    }
                }
                if (n>3) {
                    if(params[3].type==jkmpString) {
                        defaultStr=params[3].str;
                    } else {
                        p->jkmpError(JKMP::_("regexpcap(regexp, strings, cap_id, default_string) argument default_string has to be a string"));\
                        r.setInvalid();
                        return;
                    }
                }
                std::regex::flag_type flags=std::regex::ECMAScript;
                if (!casesens) flags=flags|std::regex::icase;
                std::regex rx(params[0].str, flags);
                std::smatch sm;

                if (params[1].type==jkmpStringVector) {
                    r.setStringVec(params[1].strVec);
                    for (size_t i=0; i<r.strVec.size(); i++) {
                        if (!std::regex_match(r.strVec[i],sm,rx)) {
                            r.strVec[i]=defaultStr;
                        } else {
                            r.strVec[i]=sm[capid];
                        }
                    }
                } else if (params[1].type==jkmpString) {
                    r.setString(params[1].str);
                    if (! std::regex_match (params[1].str,sm,rx)) {
                        r.str=defaultStr;
                    } else {
                        r.str=sm[capid];
                    }
                }
            } else {
                p->jkmpError(JKMP::_("regexpcap(regexp, strings, cap_id=1, default_string=\"\") argument regexp has to be a string"));
                r.setInvalid();
                return;
            }
        } else {
            p->jkmpError(JKMP::_("regexpcap(regexp, strings, cap_id, default_string=\"\") argument strings has to be a vector of strings and a string"));
            r.setInvalid();
            return;
        }
        return;
    }

    void fRegExpContains(jkmpResult& r, const jkmpResult *params, unsigned int n, JKMathParser *p, bool minimal, bool casesens)
    {
        if (n!=2) {\
            p->jkmpError(JKMP::_("regexpcontains(regexp, strings) needs 2 arguments"));\
            r.setInvalid();\
            return; \
        }\
        if(params[1].type==jkmpStringVector || params[1].type==jkmpString) {\
            if(params[0].type==jkmpString) {\
                std::regex::flag_type flags=std::regex::ECMAScript;
                if (!casesens) flags=flags|std::regex::icase;
                std::regex rx(params[0].str, flags);
                std::smatch sm;
                if (params[1].type==jkmpStringVector) {
                    JKMP::vector<bool> bv;
                    for (size_t i=0; i<params[1].strVec.size(); i++) {
                        bv.push_back(std::regex_search(params[1].strVec[i],sm,rx));
                    }
                    r.setBoolVec(bv);
                } else if (params[1].type==jkmpString) {
                    r.setBoolean(std::regex_search(params[1].str,sm,rx)>=0);
                }
            } else {
                p->jkmpError(JKMP::_("regexpcontains(regexp, strings) argument regexp has to be a string"));\
                r.setInvalid();\
                return;
            }
        } else {\
            p->jkmpError(JKMP::_("regexpcontains(regexp, strings) argument strings has to be a vector of strings and a string"));\
            r.setInvalid();\
            return;
        }\
        return; \
    }

    void fRegExpIndexIn(jkmpResult& r, const jkmpResult *params, unsigned int n, JKMathParser *p, bool minimal, bool casesens)
    {
        if (n!=2) {\
            p->jkmpError(JKMP::_("regexpindexin(regexp, strings) needs 2 arguments"));\
            r.setInvalid();\
            return; \
        }\
        if(params[1].type==jkmpStringVector || params[1].type==jkmpString) {\
            if(params[0].type==jkmpString) {\
                std::regex::flag_type flags=std::regex::ECMAScript;
                if (!casesens) flags=flags|std::regex::icase;
                std::regex rx(params[0].str, flags);
                std::smatch sm;
                if (params[1].type==jkmpStringVector) {
                    JKMP::vector<double> bv;
                    for (size_t i=0; i<params[1].strVec.size(); i++) {
                        if (std::regex_search(params[1].strVec[i],sm,rx)) {
                            bv.push_back(sm.position(0));
                        } else {
                            bv.push_back(-1);
                        }
                    }
                    r.setDoubleVec(bv);
                } else if (params[1].type==jkmpString) {
                    if (std::regex_search(params[1].str,sm,rx)) {
                        r.setDouble(sm.position(0));
                    } else {
                        r.setDouble(-1);
                    }
                }
            } else {
                p->jkmpError(JKMP::_("regexpindexin(regexp, strings) argument regexp has to be a string"));\
                r.setInvalid();\
                return;
            }
        } else {\
            p->jkmpError(JKMP::_("regexpindexin(regexp, strings) argument strings has to be a vector of strings and a string"));\
            r.setInvalid();\
            return;
        }\
        return; \
    }


    void fTrapz(jkmpResult& res, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        res.setInvalid();

        if (n==1) {
            if (params[0].type==jkmpDoubleVector) {
                res.setDouble(statisticsTrapz(params[0].numVec));
            } else {
                p->jkmpError("trapz(Y) needs 1 vector of numbers");
            }

        } else if (n==2) {
            if (params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector) {
                res.setDouble(statisticsTrapzXY(params[0].numVec, params[1].numVec));
            } else {
                p->jkmpError("trapz(X,Y) needs 2 vectors of numbers");
            }
        } else {
            p->jkmpError("trapz(Y)/trapz(X,Y) needs 1 or 2 argument");
        }
    }




/*
    void fDateNum(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="datenum";
        r.setInvalid();
        if (n<1||n>2) {
            p->jkmpError(JKMP::_("%1(date[, formatstring]) needs 1 or 2 arguments").arg(iname));
            return;
        }
        if (n>=1) {
            JKMP::string format="yyyy-MM-dd";
            if (n==2 && params[1].type==jkmpString) {
                format=params[1].str;
            } else if (n==2) {
                p->jkmpError(JKMP::_("%1(x, formatstring) 2nd argument has to be a string").arg(iname));
                return;
            }

            if(params[0].type==jkmpString) {
                r.setDouble(QDateTime::fromString(params[0].str, format).toMSecsSinceEpoch());
                return;
            } else if(params[0].type==jkmpStringVector) {
                r.setDoubleVec(params[0].strVec.size());
                for (int i=0; i<params[0].strVec.size(); i++) {
                    r.numVec[i]=QDateTime::fromString(params[0].strVec[i], format).toMSecsSinceEpoch();
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a string or string vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }

    void fDateTimeNum(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="datetimenum";
        r.setInvalid();
        if (n<1||n>2) {
            p->jkmpError(JKMP::_("%1(date[, formatstring]) needs 1 or 2 arguments").arg(iname));
            return;
        }
        if (n>=1) {
            QDateTime dat;
            JKMP::string format="yyyy-MM-ddTHH:mm:ss";
            if (n==2 && params[1].type==jkmpString) {
                format=params[1].str;
                return;
            } else if (n==2) {
                p->jkmpError(JKMP::_("%1(x, formatstring) 2nd argument has to be a string").arg(iname));
                return;
            }

            if(params[0].type==jkmpString) {
                //qDebug()<<params[0].str<<QDateTime::fromString(params[0].str, format)<<format;
                r.setDouble(QDateTime::fromString(params[0].str, format).toMSecsSinceEpoch());
                return;
            } else if(params[0].type==jkmpStringVector) {
                r.setDoubleVec(params[0].strVec.size());
                for (int i=0; i<params[0].strVec.size(); i++) {
                    r.numVec[i]=QDateTime::fromString(params[0].strVec[i], format).toMSecsSinceEpoch();
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a string or string vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }


    void fDate2Str(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="date2str";
        r.setInvalid();
        if (n<1||n>2) {
            p->jkmpError(JKMP::_("%1(date[, formatstring]) needs 1 or 2 arguments").arg(iname));
            return;
        }
        if (n>=1) {
            QDateTime dat;
            JKMP::string format="yyyy-MM-dd";
            if (n==2 && params[1].type==jkmpString) {
                format=params[1].str;
            } else if (n==2) {
                p->jkmpError(JKMP::_("%1(x, formatstring) 2nd argument has to be a string").arg(iname));
                return;
            }

            if(params[0].type==jkmpDouble) {
                r.setString(QDateTime::fromMSecsSinceEpoch(params[0].num).toString(format));
                return ;
            } else if(params[0].type==jkmpDoubleVector) {
                r.setStringVec(params[0].numVec.size());
                for (int i=0; i<params[0].numVec.size(); i++) {
                    r.strVec[i]=QDateTime::fromMSecsSinceEpoch(params[0].numVec[i]).toString(format);
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a double or double vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }


    void fDateTime2Str(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="date2str";
        r.setInvalid();
        if (n<1||n>2) {
            p->jkmpError(JKMP::_("%1(date[, formatstring]) needs 1 or 2 arguments").arg(iname));
            return;
        }
        if (n>=1) {
            QDateTime dat;
            JKMP::string format="yyyy-MM-ddTHH:mm:ss";
            if (n==2 && params[1].type==jkmpString) {
                format=params[1].str;
            } else if (n==2) {
                p->jkmpError(JKMP::_("%1(x, formatstring) 2nd argument has to be a string").arg(iname));
                return;
            }

            if(params[0].type==jkmpDouble) {
                r.setString(QDateTime::fromMSecsSinceEpoch(params[0].num).toString(format));
                return ;
            } else if(params[0].type==jkmpDoubleVector) {
                r.setStringVec(params[0].numVec.size());
                for (int i=0; i<params[0].numVec.size(); i++) {
                    r.strVec[i]=QDateTime::fromMSecsSinceEpoch(params[0].numVec[i]).toString(format);
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a double or double vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }

    void fDatediff2Secs(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="datediff2secs";
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(date) needs 1 argument").arg(iname));
            return;
        }
        if (n>=1) {

            if(params[0].type==jkmpDouble) {
                r.setDouble(params[0].num/1e3);
                return ;
            } else if(params[0].type==jkmpDoubleVector) {
                r.setDoubleVec(params[0].numVec.size());
                for (int i=0; i<params[0].numVec.size(); i++) {
                    r.numVec[i]=params[0].numVec[i]/1e3;
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a double or double vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }

    void fDatediff2Mins(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="datediff2mins";
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(date) needs 1 argument").arg(iname));
            return;
        }
        if (n>=1) {

            if(params[0].type==jkmpDouble) {
                r.setDouble(params[0].num/1e3/60.0);
                return ;
            } else if(params[0].type==jkmpDoubleVector) {
                r.setDoubleVec(params[0].numVec.size());
                for (int i=0; i<params[0].numVec.size(); i++) {
                    r.numVec[i]=params[0].numVec[i]/1e3/60.0;
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a double or double vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }

    void fDatediff2Hours(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="datediff2hours";
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(date) needs 1 argument").arg(iname));
            return;
        }
        if (n>=1) {

            if(params[0].type==jkmpDouble) {
                r.setDouble(params[0].num/1e3/60.0/60.0);
                return ;
            } else if(params[0].type==jkmpDoubleVector) {
                r.setDoubleVec(params[0].numVec.size());
                for (int i=0; i<params[0].numVec.size(); i++) {
                    r.numVec[i]=params[0].numVec[i]/1e3/60.0/60.0;
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a double or double vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }

    void fDatediff2Days(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="datediff2hours";
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(date) needs 1 argument").arg(iname));
            return;
        }
        if (n>=1) {

            if(params[0].type==jkmpDouble) {
                r.setDouble(params[0].num/1e3/60.0/60.0/24.0);
                return ;
            } else if(params[0].type==jkmpDoubleVector) {
                r.setDoubleVec(params[0].numVec.size());
                for (int i=0; i<params[0].numVec.size(); i++) {
                    r.numVec[i]=params[0].numVec[i]/1e3/60.0/60.0/24.0;
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a double or double vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }

    void fNow(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        Q_UNUSED(params);
        JKMP::string iname="now";
        r.setInvalid();
        if (n!=0) {
            p->jkmpError(JKMP::_("%1() needs no arguments").arg(iname));
            return;
        }
        r.setDouble(QDateTime::currentDateTime().toMSecsSinceEpoch());
    }


    void fDateDay(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="dateday";
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(date) needs 1 argument").arg(iname));
            return;
        }
        if (n>=1) {

            if(params[0].type==jkmpDouble) {
                r.setDouble(QDateTime::fromMSecsSinceEpoch(params[0].num).date().day());
                return ;
            } else if(params[0].type==jkmpDoubleVector) {
                r.setDoubleVec(params[0].numVec.size());
                for (int i=0; i<params[0].numVec.size(); i++) {
                    r.numVec[i]=QDateTime::fromMSecsSinceEpoch(params[0].numVec[i]).date().day();
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a double or double vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }

    void fDateMonth(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="datemonth";
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(date) needs 1 argument").arg(iname));
            return;
        }
        if (n>=1) {

            if(params[0].type==jkmpDouble) {
                r.setDouble(QDateTime::fromMSecsSinceEpoch(params[0].num).date().month());
                return ;
            } else if(params[0].type==jkmpDoubleVector) {
                r.setDoubleVec(params[0].numVec.size());
                for (int i=0; i<params[0].numVec.size(); i++) {
                    r.numVec[i]=QDateTime::fromMSecsSinceEpoch(params[0].numVec[i]).date().month();
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a double or double vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }

    void fDateYear(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="dateyear";
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(date) needs 1 argument").arg(iname));
            return;
        }
        if (n>=1) {

            if(params[0].type==jkmpDouble) {
                r.setDouble(QDateTime::fromMSecsSinceEpoch(params[0].num).date().year());
                return ;
            } else if(params[0].type==jkmpDoubleVector) {
                r.setDoubleVec(params[0].numVec.size());
                for (int i=0; i<params[0].numVec.size(); i++) {
                    r.numVec[i]=QDateTime::fromMSecsSinceEpoch(params[0].numVec[i]).date().year();
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a double or double vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }

    void fDateHour(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="datehour";
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(date) needs 1 argument").arg(iname));
            return;
        }
        if (n>=1) {

            if(params[0].type==jkmpDouble) {
                r.setDouble(QDateTime::fromMSecsSinceEpoch(params[0].num).time().hour());
                return ;
            } else if(params[0].type==jkmpDoubleVector) {
                r.setDoubleVec(params[0].numVec.size());
                for (int i=0; i<params[0].numVec.size(); i++) {
                    r.numVec[i]=QDateTime::fromMSecsSinceEpoch(params[0].numVec[i]).time().hour();
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a double or double vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }
    void fDateMin(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="datemin";
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(date) needs 1 argument").arg(iname));
            return;
        }
        if (n>=1) {

            if(params[0].type==jkmpDouble) {
                r.setDouble(QDateTime::fromMSecsSinceEpoch(params[0].num).time().minute());
                return ;
            } else if(params[0].type==jkmpDoubleVector) {
                r.setDoubleVec(params[0].numVec.size());
                for (int i=0; i<params[0].numVec.size(); i++) {
                    r.numVec[i]=QDateTime::fromMSecsSinceEpoch(params[0].numVec[i]).time().minute();
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a double or double vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }


    void fDateSec(jkmpResult& r, const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        JKMP::string iname="datesec";
        r.setInvalid();
        if (n!=1) {
            p->jkmpError(JKMP::_("%1(date) needs 1 argument").arg(iname));
            return;
        }
        if (n>=1) {

            if(params[0].type==jkmpDouble) {
                r.setDouble(QDateTime::fromMSecsSinceEpoch(params[0].num).time().second());
                return ;
            } else if(params[0].type==jkmpDoubleVector) {
                r.setDoubleVec(params[0].numVec.size());
                for (int i=0; i<params[0].numVec.size(); i++) {
                    r.numVec[i]=QDateTime::fromMSecsSinceEpoch(params[0].numVec[i]).time().second();
                }
                return;
            } else {
                p->jkmpError(JKMP::_("%1(x) argument has to be a double or double vector").arg(iname));
                return;
            }
            return;
        }
        return;
    }*/


    void fVarname(jkmpResult& r,  JKMathParser::jkmpNode** nodes, unsigned int n, JKMathParser* p) {
        //qDebug()<<"fVarname "<<nodes<<n<<p;
        JKMP::string iname="varname";
        r.setInvalid();
        if (n<=0) {
            p->jkmpError(JKMP::_("%1(...) needs at least 1 argument").arg(iname));
            return;
        }
        if (n>=1) {
            r.setStringVec(n, "");
            for (unsigned int i=0; i<n; i++) {
                JKMathParser::jkmpVariableNode* vn=dynamic_cast<JKMathParser::jkmpVariableNode*>(nodes[i]);
                if (vn) {
                    r.strVec[i]=vn->getName();
                } else {
                    r.setInvalid();
                    p->jkmpError(JKMP::_("%1(...) needs variable names as input arguments, %2-th argument is of different type (%3)").arg(iname).arg(i+1).arg(typeid(*nodes[i]).name()));
                    return;
                }
            }
            return;
        }
        return;
    }

    void fReplace(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        r.setInvalid();
        if (n>0) r.set(params[0]);
        if (n==3 && params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble && params[2].type==jkmpDouble) {
            for (size_t i=0; i<r.numVec.size(); i++) {
                if (r.numVec[i]==params[1].num) r.numVec[i]=params[2].num;
            }
        } else if (n==3 && params[0].type==jkmpStringVector && params[1].type==jkmpString && params[2].type==jkmpString) {
            for (size_t i=0; i<r.strVec.size(); i++) {
                if (r.strVec[i]==params[1].str) r.strVec[i]=params[2].str;
            }
        } else if (n==3 && params[0].type==jkmpBoolVector && params[1].type==jkmpBool && params[2].type==jkmpBool) {
            for (size_t i=0; i<r.boolVec.size(); i++) {
                if (r.boolVec[i]==params[1].boolean) r.boolVec[i]=params[2].boolean;
            }
        } else if (n==3 && params[0].type==jkmpString && params[1].type==jkmpString && params[2].type==jkmpString) {
            r.str=r.str.replace(params[1].str, params[2].str);
        } else {
            p->jkmpError(JKMP::_("replace(x, old_value,new_value) needs 3 arguments: one vector x and two corresponding element values, or 3 strings"));
            r.setInvalid();
            return;
        }
    }

    void fReplaceMulti(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        const JKMP::string errorm=JKMP::_("multireplace(x, old_value,new_value,old2,new2,...) needs 3,5,7,... arguments: one vector x and pairs of corresponding element values, or 3/5/7/... strings");
        r.setInvalid();
        if (n>0) r.set(params[0]);
        if (n>=3 && n%2==1 && params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble && params[2].type==jkmpDouble) {
            for (unsigned int j=1; j<n; j+=2) {
                if (params[j].type!=jkmpDouble || params[j+1].type!=jkmpDouble) {
                    p->jkmpError(errorm);
                    r.setInvalid();
                    return;
                }
                for (size_t i=0; i<r.numVec.size(); i++) {
                    if (r.numVec[i]==params[j].num) r.numVec[i]=params[j+1].num;
                }
            }
        } else if (n>=3 && n%2==1 && params[0].type==jkmpStringVector && params[1].type==jkmpString && params[2].type==jkmpString) {
            for (unsigned int j=1; j<n; j+=2) {
                if (params[j].type!=jkmpString || params[j+1].type!=jkmpString) {
                    p->jkmpError(errorm);
                    r.setInvalid();
                    return;
                }
                for (size_t i=0; i<r.strVec.size(); i++) {
                    if (r.strVec[i]==params[j].str) r.strVec[i]=params[j+1].str;
                }
            }
        } else if (n>=3 && n%2==1 && params[0].type==jkmpBoolVector && params[1].type==jkmpBool && params[2].type==jkmpBool) {
            for (size_t j=1; j<n; j+=2) {
                if (params[j].type!=jkmpBool || params[j+1].type!=jkmpBool) {
                    p->jkmpError(errorm);
                    r.setInvalid();
                    return;
                }
                for (size_t i=0; i<r.boolVec.size(); i++) {
                    if (r.boolVec[i]==params[j].boolean) r.boolVec[i]=params[j+1].boolean;
                }
            }
        } else if (n>=3 && n%2==1 && params[0].type==jkmpString && params[1].type==jkmpString && params[2].type==jkmpString) {
            for (size_t j=1; j<n; j+=2) {
                if (params[j].type!=jkmpString || params[j+1].type!=jkmpString) {
                    p->jkmpError(errorm);
                    r.setInvalid();
                    return;
                }

                r.str=r.str.replace(params[j].str, params[j+1].str);
            }
        } else {
            p->jkmpError(errorm);
            r.setInvalid();
            return;
        }
    }

    void fSplit(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        r.setInvalid();
        if (n==2 && params[0].type==jkmpString && params[1].type==jkmpString) {
            r.setStringVec(params[0].str.split(params[1].str));
        } else {
            p->jkmpError(JKMP::_("split(x, param) needs 2 string arguments"));
            r.setInvalid();
            return;
        }
    }

    void fRemoveEmpty(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        r.setInvalid();
        if (n==1 && params[0].type==jkmpStringVector) {
            r.setStringVec(JKMP::stringVector());
            for (int i=0; i<params[0].strVec.size(); i++) {
                if (!params[0].strVec[i].is_empty()) r.strVec<<params[0].strVec[i];
            }
        } else {
            p->jkmpError(JKMP::_("removeemptystrings(x) needs 1 string vector arguments"));
            r.setInvalid();
            return;
        }
    }

    void fJoin(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        JKMP::string sep="";
        r.setString("");
        if (n==2) {
            if (params[1].type==jkmpString) {
                sep=params[1].str;
            } else {
                p->jkmpError(JKMP::_("join(x, separator) needs a string as second argument"));
                r.setInvalid();
                return;
            }
        }
        if (n>0 && params[0].type==jkmpStringVector) {
            r.setString(params[0].strVec.join(sep));
        } else {
            p->jkmpError(JKMP::_("join(x, separator) needs a string vector as first argument"));
            r.setInvalid();
            return;
        }
    }





    void fReturnFirstValid(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        res.setInvalid();

        if (n<1) {
            p->jkmpError("returnfirstvalid(x1,x2,x3,...) needs at least one argument");
            return ;
        }
        for (unsigned int i=0; i<n; i++) {
            if (params[i].isValid && params[i].type!=jkmpVoid) {
                res=params[i];
                return;
            }
        }
    }


    void fReturnLastValid(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        res.setInvalid();

        if (n<1) {
            p->jkmpError("returnlastvalid(x1,x2,x3,...) needs at least one argument");
            return ;
        }
        for ( long i=(long)n-1; i>=0; i--) {
            if (params[i].isValid && params[i].type!=jkmpVoid) {
                res=params[i];
                return;
            }
        }
    }

    void fType(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        //qDebug()<<"fVarname "<<nodes<<n<<p;
        JKMP::string iname="type";
        if (n<=0) {
            r.setInvalid();
            p->jkmpError(JKMP::_("%1(...) needs at least 1 argument").arg(iname));
            return;
        }
        if (n==1) {
            r.setString(params[0].typeName());
        } else if (n>=1) {
            r.setStringVec(n, "");
            for (unsigned int i=0; i<n; i++) {
                r.strVec[i]=params[i].typeName();
            }
        }
    }

    void fStrVec(jkmpResult &res, const jkmpResult* nodes, unsigned int n, JKMathParser *p)
    {
        const JKMP::string iname="strvec";
        const JKMP::string tname="string";
        res.setStringVec();
        for (unsigned int i=0; i<n; i++) {
            if (nodes[i].convertsToStringVector()) {
                res.strVec<<nodes[i].asStrVector();
            } else if (nodes[i].type==jkmpVoid || !nodes[i].isValid) {
            } else {
                res.setInvalid();
                p->jkmpError(JKMP::_("%1(...) encountered a datatype, which cannot be converted to a %3/%3 vector in argument %2").arg(iname).arg(i+1).arg(tname));
                return;
            }
        }
    }

    void fNumVec(jkmpResult &res, const jkmpResult* nodes, unsigned int n, JKMathParser *p)
    {
        const JKMP::string iname="numvec";
        const JKMP::string tname="number";
        res.setDoubleVec();
        for (unsigned int i=0; i<n; i++) {
            if (nodes[i].convertsToVector()) {
                res.numVec<<nodes[i].asVector();
            } else if (nodes[i].type==jkmpVoid || !nodes[i].isValid) {
            } else {
                res.setInvalid();
                p->jkmpError(JKMP::_("%1(...) encountered a datatype, which cannot be converted to a %3/%3 vector in argument %2").arg(iname).arg(i+1).arg(tname));
                return;
            }
        }
    }
    void fIntVec(jkmpResult &res, const jkmpResult* nodes, unsigned int n, JKMathParser *p)
    {
        const JKMP::string iname="intvec";
        const JKMP::string tname="integer";
        res.setDoubleVec();
        for (unsigned int i=0; i<n; i++) {
            if (nodes[i].convertsToVector()) {
                JKMP::vector<int> iv=nodes[i].asIntVector();
                for (int j=0; j<iv.size(); j++) res.numVec<<iv[j];
            } else if (nodes[i].type==jkmpVoid || !nodes[i].isValid) {
            } else {
                res.setInvalid();
                p->jkmpError(JKMP::_("%1(...) encountered a datatype, which cannot be converted to a %3/%3 vector in argument %2").arg(iname).arg(i+1).arg(tname));
                return;
            }
        }
    }
    void fInt(jkmpResult &res, const jkmpResult* nodes, unsigned int n, JKMathParser *p)
    {
        const JKMP::string iname="int";
        const JKMP::string tname="integer";
        if (n==0) {
            res.setInvalid();
            p->jkmpError(JKMP::_("%1(...) needs at least one argument").arg(iname));
            return;
        } else if (n==1) {
            int i=0;
            if (nodes[i].type==jkmpDouble || nodes[i].type==jkmpBool) {
                res.setDouble(nodes[i].toInteger());
            } else if (nodes[i].type==jkmpDoubleVector || nodes[i].type==jkmpBoolVector) {
                JKMP::vector<int> iv=nodes[i].asIntVector();
                res.setDoubleVec();
                for (int j=0; j<iv.size(); j++) res.numVec<<iv[j];
            } else {
                res.setInvalid();
                p->jkmpError(JKMP::_("%1(...) encountered a datatype, which cannot be converted to a %3 in argument %2").arg(iname).arg(1).arg(tname));
                return;
            }
        } else {
            res.setDoubleVec();
            for (unsigned int i=0; i<n; i++) {
                if (nodes[i].type==jkmpDouble || nodes[i].type==jkmpBool) {
                    res.numVec<<nodes[i].toInteger();
                } else if (nodes[i].convertsToIntVector()) {
                    JKMP::vector<int> iv=nodes[i].asIntVector();
                    res.setDoubleVec();
                    for (int j=0; j<iv.size(); j++) res.numVec<<iv[j];
                } else {
                    res.setInvalid();
                    p->jkmpError(JKMP::_("%1(...) encountered a datatype, which cannot be converted to a %3/%3 vector in argument %2").arg(iname).arg(i+1).arg(tname));
                    return;
                }
            }
        }
    }
    void fNum(jkmpResult &res, const jkmpResult* nodes, unsigned int n, JKMathParser *p)
    {
        const JKMP::string iname="num";
        const JKMP::string tname="number";
        if (n==0) {
            res.setInvalid();
            p->jkmpError(JKMP::_("%1(...) needs at least one argument").arg(iname));
            return;
        } else if (n==1) {
            int i=0;
            if (nodes[i].type==jkmpDouble || nodes[i].type==jkmpBool) {
                res.setDouble(nodes[i].asNumber());
            } else if (nodes[i].convertsToVector()) {
                res.setDoubleVec(nodes[i].asVector());
            } else {
                res.setInvalid();
                p->jkmpError(JKMP::_("%1(...) encountered a datatype, which cannot be converted to a %3 in argument %2").arg(iname).arg(1).arg(tname));
                return;
            }
        } else {
            res.setDoubleVec();
            for (unsigned int i=0; i<n; i++) {
                if (nodes[i].type==jkmpDouble || nodes[i].type==jkmpBool) {
                    res.numVec<<nodes[i].asNumber();
                } else if (nodes[i].convertsToVector()) {
                    res.numVec<<nodes[i].asVector();
                } else {
                    res.setInvalid();
                    p->jkmpError(JKMP::_("%1(...) encountered a datatype, which cannot be converted to a %3/%3 vector in argument %2").arg(iname).arg(i+1).arg(tname));
                    return;
                }
            }
        }
    }
    void fBool(jkmpResult &res, const jkmpResult* nodes, unsigned int n, JKMathParser *p)
    {
        const JKMP::string iname="bool";
        const JKMP::string tname="boolean";
        if (n==0) {
            res.setInvalid();
            p->jkmpError(JKMP::_("%1(...) needs at least one argument").arg(iname));
            return;
        } else if (n==1) {
            int i=0;
            if (nodes[i].type==jkmpDouble || nodes[i].type==jkmpBool) {
                res.setBoolean(nodes[i].asBool());
            } else if (nodes[i].type==jkmpDoubleVector || nodes[i].type==jkmpBoolVector || nodes[i].convertsToBoolVector()) {
                res.setBoolVec(nodes[i].asBoolVector());
            } else {
                res.setInvalid();
                p->jkmpError(JKMP::_("%1(...) encountered a datatype, which cannot be converted to a %3 in argument %2").arg(iname).arg(1).arg(tname));
                return;
            }
        } else {
            res.setBoolVec();
            for (unsigned int i=0; i<n; i++) {
                if (nodes[i].type==jkmpDouble || nodes[i].type==jkmpBool) {
                    res.boolVec<<nodes[i].asBool();
                } else if (nodes[i].convertsToIntVector()) {
                    res.boolVec<<nodes[i].asBoolVector();
                } else {
                    res.setInvalid();
                    p->jkmpError(JKMP::_("%1(...) encountered a datatype, which cannot be converted to a %3/%3 vector in argument %2").arg(iname).arg(i+1).arg(tname));
                    return;
                }
            }
        }
    }
    void fBoolVec(jkmpResult &res, const jkmpResult* nodes, unsigned int n, JKMathParser *p)
    {
        const JKMP::string iname="boolvec";
        const JKMP::string tname="boolean";
        res.setBoolVec();
        for (unsigned int i=0; i<n; i++) {
            if (nodes[i].convertsToBoolVector()) {
                res.boolVec<<nodes[i].asBoolVector();
            } else if (nodes[i].type==jkmpVoid || !nodes[i].isValid) {
            } else {
                res.setInvalid();
                p->jkmpError(JKMP::_("%1(...) encountered a datatype, which cannot be converted to a %3/%3 vector in argument %2").arg(iname).arg(i+1).arg(tname));
                return;
            }
        }
    }


    void fEnsureVectorSize(jkmpResult &res, const jkmpResult* params, unsigned int n, JKMathParser *p)
    {
        const JKMP::string iname="ensuresize";

        if (n==3 && (params[0].type==jkmpDoubleVector && params[1].isInteger() && params[2].type==jkmpDouble)) {
            int ts=params[1].toInteger();
            res=params[0];
            if (res.numVec.size()>ts) {
                res.numVec.resize(ts);
            } else if (res.numVec.size()<ts) {
                while (res.numVec.size()<ts) res.numVec.push_back(params[2].num);
            }
        } else if (n==3 && (params[0].type==jkmpStringVector && params[1].isInteger() && params[2].type==jkmpString)) {
            int ts=params[1].toInteger();
            res=params[0];
            if (res.strVec.size()>ts) {
                while (res.strVec.size()>ts) res.strVec.pop_back
();
            } else if (res.strVec.size()<ts) {
                while (res.strVec.size()<ts) res.strVec.push_back(params[2].str);
            }
        } else if (n==3 && (params[0].type==jkmpString && params[1].isInteger() && params[2].type==jkmpString && params[2].str.size()==1)) {
            int ts=params[1].toInteger();
            res=params[0];
            if (res.str.size()>ts) {
                res.str=res.str.left(ts);
            } else if (res.str.size()<ts) {
                while (res.str.size()<ts) res.str+=params[2].str;
            }
        } else if (n==3 && (params[0].type==jkmpBoolVector && params[1].isInteger() && params[2].type==jkmpBool)) {
            int ts=params[1].toInteger();
            res=params[0];
            if (res.boolVec.size()>ts) {
                res.boolVec.resize(ts);
            } else if (res.boolVec.size()<ts) {
                while (res.boolVec.size()<ts) res.boolVec.push_back(params[2].boolean);
            }
        } else {
            res.setInvalid();
            p->jkmpError(JKMP::_("%1(x,desiredsize,defaultvalue) needs three arguments: one vector, one integer and one argument copatible with x").arg(iname));
            return;
        }

    }



    void fEnsureVectorSizeStart(jkmpResult &res, const jkmpResult* params, unsigned int n, JKMathParser *p)
    {
        const JKMP::string iname="ensuresize_start";

        if (n==3 && (params[0].type==jkmpDoubleVector && params[1].isInteger() && params[2].type==jkmpDouble)) {
            int ts=params[1].toInteger();
            res=params[0];
            if (res.numVec.size()>ts) {
                while (res.numVec.size()>ts) res.numVec.pop_front();
            } else if (res.numVec.size()<ts) {
                while (res.numVec.size()<ts) res.numVec.push_front(params[2].num);
            }
        } else if (n==3 && (params[0].type==jkmpStringVector && params[1].isInteger() && params[2].type==jkmpString)) {
            int ts=params[1].toInteger();
            res=params[0];
            if (res.strVec.size()>ts) {
                while (res.strVec.size()>ts) res.strVec.pop_front();
            } else if (res.strVec.size()<ts) {
                while (res.strVec.size()<ts) res.strVec.push_front(params[2].str);
            }
        } else if (n==3 && (params[0].type==jkmpString && params[1].isInteger() && params[2].type==jkmpString && params[2].str.size()==1)) {
            int ts=params[1].toInteger();
            res=params[0];
            if (res.str.size()>ts) {
                res.str=res.str.right(ts);
            } else if (res.str.size()<ts) {
                while (res.str.size()<ts) res.str=params[2].str+res.str;
            }
        } else if (n==3 && (params[0].type==jkmpBoolVector && params[1].isInteger() && params[2].type==jkmpBool)) {
            int ts=params[1].toInteger();
            res=params[0];
            if (res.boolVec.size()>ts) {
                while (res.boolVec.size()>ts) res.boolVec.pop_front();
            } else if (res.boolVec.size()<ts) {
                while (res.boolVec.size()<ts) res.boolVec.push_front(params[2].boolean);
            }
        } else {
            res.setInvalid();
            p->jkmpError(JKMP::_("%1(x,desiredsize,defaultvalue) needs three arguments: one vector, one integer and one argument copatible with x").arg(iname));
            return;
        }
    }





    void fHistogram(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        const JKMP::string iname="histogram";
        int bins=11;
        if ((n==1 || n==2 || n==3) && params[0].type==jkmpDoubleVector) {
            bool norm=false;
            if (n==2 && params[1].isUInt())  {
                bins=std::max((uint32_t)2,params[1].toUInt());
            } else if (n==2 && params[1].type==jkmpBool)  {
                norm=params[1].boolean;
            } else if (n==3 && params[1].isUInt() && params[2].type==jkmpBool)  {
                bins=std::max((uint32_t)2,params[1].toUInt());
                norm=params[2].boolean;
            } else {
                res.setInvalid();
                p->jkmpError(JKMP::_("%1(data[,bins][,normalize]) needs an unsigned integer for bins and a boolean for normalize (both optional)").arg(iname));
                return;
            }
            JKMP::vector<double> temp;
            temp.resize(bins);
            res.setDoubleVec(bins);
            statisticsHistogram(params[0].numVec.data(), params[0].numVec.size(), temp.data(), res.numVec.data(), bins, norm);
        } else {
            res.setInvalid();
            p->jkmpError(JKMP::_("%1(data[,bins][,normalize]) needs at least one number vector argument").arg(iname));
            return;
        }
    }

    void fHistogramBins(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        const JKMP::string iname="histogrambins";
        int bins=11;
        if ((n==1 || n==2 || n==3) && params[0].type==jkmpDoubleVector) {
            bool norm=false;
            if (n==2 && params[1].isUInt())  {
                bins=std::max((uint32_t)2,params[1].toUInt());
            } else if (n==2 && params[1].type==jkmpBool)  {
                norm=params[1].boolean;
            } else if (n==3 && params[1].isUInt() && params[2].type==jkmpBool)  {
                bins=std::max((uint32_t)2,params[1].toUInt());
                norm=params[2].boolean;
            } else {
                res.setInvalid();
                p->jkmpError(JKMP::_("%1(data[,bins][,normalize]) needs an unsigned integer for bins and a boolean for normalize (both optional)").arg(iname));
                return;
            }
            JKMP::vector<double> temp;
            temp.resize(bins);
            res.setDoubleVec(bins);
            statisticsHistogram(params[0].numVec.data(), params[0].numVec.size(), res.numVec.data(), temp.data(), bins, norm);
        } else {
            res.setInvalid();
            p->jkmpError(JKMP::_("%1(data[,bins][,normalize]) needs at least one number vector argument").arg(iname));
            return;
        }

    }










    void fRangedHistogram(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        const JKMP::string iname="rangedhistogram";
        int bins=11;
        if ((n==3 || n==4 || n==5) && params[0].type==jkmpDoubleVector && params[1].type==jkmpDouble && params[2].type==jkmpDouble) {
            bool norm=false;
            double rmin=params[1].num;
            double rmax=params[2].num;
            if (n==4 && params[3].isUInt())  {
                bins=std::max((uint32_t)2,params[3].toUInt());
            } else if (n==4 && params[3].type==jkmpBool)  {
                norm=params[3].boolean;
            } else if (n==5 && params[3].isUInt() && params[4].type==jkmpBool)  {
                bins=std::max((uint32_t)2,params[3].toUInt());
                norm=params[4].boolean;
            } else {
                res.setInvalid();
                p->jkmpError(JKMP::_("%1(data,min,max[,bins][,normalize]) needs an unsigned integer for bins and a boolean for normalize (both optional)").arg(iname));
                return;
            }
            JKMP::vector<double> temp;
            temp.resize(bins);
            res.setDoubleVec(bins);
            statisticsHistogramRanged(params[0].numVec.data(), params[0].numVec.size(), rmin, rmax, temp.data(), res.numVec.data(), bins, norm);
        } else {
            res.setInvalid();
            p->jkmpError(JKMP::_("%1(data,min,max[,bins][,normalize]) needs at least one number vector argument").arg(iname));
            return;
        }
    }

    void fRangedHistogramBins(jkmpResult &res, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        const JKMP::string iname="rangedhistogrambins";
        int bins=11;
        unsigned int paramoffset=0;
        if (n>0 && params[0].type==jkmpDoubleVector) paramoffset=1;
        if ((n==2+paramoffset || n==3+paramoffset || n==4+paramoffset) && params[paramoffset+0].type==jkmpDouble && params[paramoffset+1].type==jkmpDouble) {
            bool norm=false;
            double rmin=params[paramoffset].num;
            double rmax=params[paramoffset+1].num;
            if (n==paramoffset+3 && params[paramoffset+2].isUInt())  {
                bins=std::max((uint32_t)2,params[paramoffset+2].toUInt());
            } else if (n==paramoffset+3 && params[paramoffset+2].type==jkmpBool)  {
                norm=params[paramoffset+2].boolean;
            } else if (n==paramoffset+4 && params[paramoffset+2].isUInt() && params[paramoffset+3].type==jkmpBool)  {
                bins=std::max((uint32_t)2,params[paramoffset+2].toUInt());
                norm=params[paramoffset+3].boolean;
            } else {
                res.setInvalid();
                p->jkmpError(JKMP::_("%1([data,]min,max[,bins][,normalize]) needs an unsigned integer for bins and a boolean for normalize (both optional)").arg(iname));
                return;
            }
            JKMP::vector<double> temp;
            temp.resize(bins);
            res.setDoubleVec(bins);
            JKMP::vector<double> dummy;
            dummy<<0<<1;
            statisticsHistogramRanged(dummy.data(), dummy.size(), rmin, rmax, res.numVec.data(), temp.data(), bins, norm);
        } else {
            res.setInvalid();
            p->jkmpError(JKMP::_("%1([data,]min,max[,bins][,normalize]) needs at least two number arguments").arg(iname));
            return;
        }

    }
















    void fPolyFit(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setDoubleVec();
        if (n!=3) {
            p->jkmpError("polyfit(X,Y,n) needs 3 arguments");
            r.setInvalid();
            return;

        }
        if (!(params[0].convertsToVector() && params[1].convertsToVector())) {
            p->jkmpError("polyfit(X,Y,n) arguments X and Y have to be of type number vector");
            r.setInvalid();
            return;

        }
        if (params[0].size()!= params[1].size()) {
            p->jkmpError("polyfit(X,Y,n) arguments X and Y have to have the same length");
            r.setInvalid();
            return;

        }
        if (!(params[2].isInteger())) {
            p->jkmpError("polyfit(X,Y,n) argument n has to be a positive integer number");
            r.setInvalid();
            return;

        }

        JKMP::vector<double> X=params[0].asVector();
        JKMP::vector<double> Y=params[1].asVector();
        int32_t np=params[2].toInteger();

        if (np<=0) {
            p->jkmpError("polyfit(X,Y,n) argument n has to be a positive integer number");
            r.setInvalid();
            return;

        }

        r.numVec.clear();
        r.numVec.resize(np+1);
        for (int i=0; i<np+1; i++) r.numVec[i]=0.0;
        statisticsPolyFit(X.data(), Y.data(), std::min(X.size(), Y.size()), np, r.numVec.data());

    }


    void fRegression(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setDoubleVec();
        if (n!=2 && n!=4) {
            p->jkmpError("regression(X,Y) needs 2 or 4 arguments");
            r.setInvalid();
            return;

        }
        if (!(params[0].convertsToVector() && params[1].convertsToVector())) {
            p->jkmpError("regression(X,Y) arguments X and Y have to be of type number vector");
            r.setInvalid();
            return;

        }
        if (params[0].size()!= params[1].size()) {
            p->jkmpError("regression(X,Y) arguments X and Y have to have the same length");
            r.setInvalid();
            return;

        }

        double a, b;
        bool fixA=false;
        bool fixB=false;
        if (n==4) {
            if (!(params[2].type==jkmpString && params[3].type==jkmpDouble)) {
                p->jkmpError(JKMP::_("regression(X,Y,'fixParameter',fixValue) argument fixParameter has to be a string and fixValue a number, but parameters are %1 and %2").arg(params[2].toTypeString()).arg(params[3].toTypeString()));
                r.setInvalid();
                return;

            }
            if (params[2].str.toLower()=="a") {
                fixA=true;
                a=params[3].num;
            } else if (params[2].str.toLower()=="b") {
                fixB=true;
                b=params[3].num;
            } else {
                p->jkmpError(JKMP::_("regression(X,Y,'fixParameter',fixValue) argument fixParameter has to be a string and fixValue a number 'a' or 'b', but was %1").arg(params[2].toTypeString()));
                r.setInvalid();
                return;

            }
        }
        JKMP::vector<double> X=params[0].asVector();
        JKMP::vector<double> Y=params[1].asVector();
        statisticsLinearRegression(X.data(), Y.data(), std::min(X.size(), Y.size()), a, b, fixA, fixB);

        r.numVec.clear();
        r.numVec<<a;
        r.numVec<<b;

    }

    void fWeightedRegression(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setDoubleVec();
        if (n!=3 && n!=5) {
            p->jkmpError("weighted_regression(X,Y,W) needs 3 or 5 arguments");
            r.setInvalid();
            return;

        }
        if (!(params[0].convertsToVector() && params[1].convertsToVector() && params[2].convertsToVector())) {
            p->jkmpError("weighted_regression(X,Y,W) arguments X, Y and W have to be of type number vector");
            r.setInvalid();
            return;

        }
        if (params[0].size()!= params[1].size() || params[0].size()!= params[2].size() || params[2].size()!= params[2].size()) {
            p->jkmpError("weighted_regression(X,Y,W) arguments X, Y and W have to have the same length");
            r.setInvalid();
            return;

        }

        double a, b;
        bool fixA=false;
        bool fixB=false;
        if (n==5) {
            if (!(params[3].type==jkmpString && params[4].type==jkmpDouble)) {
                p->jkmpError("weighted_regression(X,Y,W,'fixParameter',fixValue) argument fixParameter has to be a string and fixValue a number");
                r.setInvalid();
                return;

            }
            if (params[3].str.toLower()=="a") {
                fixA=true;
                a=params[4].num;
            } else if (params[3].str.toLower()=="b") {
                fixB=true;
                b=params[4].num;
            } else {
                p->jkmpError(JKMP::_("weighted_regression(X,Y,W,'fixParameter',fixValue) argument fixParameter has to be a string and fixValue a number 'a' or 'b', but was %1").arg(params[3].toTypeString()));
                r.setInvalid();
                return;

            }
        }
        JKMP::vector<double> X=params[0].asVector();
        JKMP::vector<double> Y=params[1].asVector();
        JKMP::vector<double> W=params[2].asVector();
        statisticsLinearWeightedRegression(X.data(), Y.data(), W.data(), std::min(X.size(), Y.size()), a, b, fixA, fixB);

        r.numVec.clear();
        r.numVec<<a;
        r.numVec<<b;

    }

    void fIRLS(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setDoubleVec();
        if (n<2 && n>6) {
            p->jkmpError("irls(X,Y) needs between 2 and 6 arguments");
            r.setInvalid();
            return;

        }
        if (!(params[0].convertsToVector() && params[1].convertsToVector())) {
            p->jkmpError("irls(X,Y) arguments X and Y have to be of type number vector");
            r.setInvalid();
            return;

        }
        if (params[0].size()!= params[1].size()) {
            p->jkmpError("irls(X,Y) arguments X and Y have to have the same length");
            r.setInvalid();
            return;

        }

        double a=0, b=0;
        bool fixA=false;
        bool fixB=false;
        double Lp=1.1;
        int iterations=100;
        int fixOffset=2;
        if (n>=3) {
            if (params[2].type==jkmpDouble) {
                fixOffset=3;
                Lp=params[2].asNumber();
                if (n>=4) {
                    if (params[3].type==jkmpDouble) {
                        fixOffset=4;
                        iterations=params[3].asNumber();
                    }
                }
            }
        }

        if ((int64_t)n==fixOffset+2) {
            if (!(params[fixOffset+0].type==jkmpString && params[fixOffset+1].type==jkmpDouble)) {
                p->jkmpError("irls(X,Y,...,'fixParameter',fixValue) argument fixParameter has to be a string and fixValue a number");
                r.setInvalid();
                return;

            }
            if (params[fixOffset+0].str.toLower()=="a") {
                fixA=true;
                a=params[fixOffset+1].num;
            } else if (params[fixOffset+0].str.toLower()=="b") {
                fixB=true;
                b=params[fixOffset+1].num;
            } else {
                p->jkmpError(JKMP::_("irls(X,Y,...,'fixParameter',fixValue) argument fixParameter has to be a string and fixValue a number 'a' or 'b', but was %1").arg(params[fixOffset+0].toTypeString()));
                r.setInvalid();
                return;

            }
        }
        JKMP::vector<double> X=params[0].asVector();
        JKMP::vector<double> Y=params[1].asVector();
        statisticsIterativelyReweightedLeastSquaresRegression(X.data(), Y.data(), std::min(X.size(), Y.size()), a, b, Lp, iterations, fixA, fixB);

        r.numVec.clear();
        r.numVec<<a;
        r.numVec<<b;

    }






















    void fErrorAdd(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setDoubleVec();
        if (n!=2) {
            p->jkmpError("erroradd(X,Y) needs 2 arguments");
            r.setInvalid();
            return;

        }
        if (!(params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector)) {
            p->jkmpError("erroradd(X,Y) arguments X and Y have to be of type number vector");
            r.setInvalid();
            return;

        }
        if (params[0].size()!=2 || params[1].size()!=2) {
            p->jkmpError("erroradd(X,Y) arguments X and Y have to have length 2");
            r.setInvalid();
            return;

        }

        double X=params[0].numVec[0];
        double Xe=params[0].numVec[1];
        double Y=params[1].numVec[0];
        double Ye=params[1].numVec[1];

        r.numVec.clear();
        r.numVec<<X+Y;
        r.numVec<<sqrt(Xe*Xe+Ye*Ye);

    }

    void fErrorSub(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setDoubleVec();
        if (n!=2) {
            p->jkmpError("errorsub(X,Y) needs 2 arguments");
            r.setInvalid();
            return;

        }
        if (!(params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector)) {
            p->jkmpError("errorsub(X,Y) arguments X and Y have to be of type number vector");
            r.setInvalid();
            return;

        }
        if (params[0].size()!=2 || params[1].size()!=2) {
            p->jkmpError("errorsub(X,Y) arguments X and Y have to have length 2");
            r.setInvalid();
            return;

        }

        double X=params[0].numVec[0];
        double Xe=params[0].numVec[1];
        double Y=params[1].numVec[0];
        double Ye=params[1].numVec[1];

        r.numVec.clear();
        r.numVec<<X-Y;
        r.numVec<<sqrt(Xe*Xe+Ye*Ye);

    }




    void fErrorMul(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setDoubleVec();
        if (n!=2) {
            p->jkmpError("errormul(X,Y) needs 2 arguments");
            r.setInvalid();
            return;

        }
        if (!(params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector)) {
            p->jkmpError("errormul(X,Y) arguments X and Y have to be of type number vector");
            r.setInvalid();
            return;

        }
        if (params[0].size()!=2 || params[1].size()!=2) {
            p->jkmpError("errormul(X,Y) arguments X and Y have to have length 2");
            r.setInvalid();
            return;

        }

        double X=params[0].numVec[0];
        double Xe=params[0].numVec[1];
        double Y=params[1].numVec[0];
        double Ye=params[1].numVec[1];

        r.numVec.clear();
        r.numVec<<X*Y;
        r.numVec<<sqrt(JKMP::sqr(Y*Xe)+JKMP::sqr(X*Ye));

    }

    void fErrorDiv(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setDoubleVec();
        if (n!=2) {
            p->jkmpError("errordiv(X,Y) needs 2 arguments");
            r.setInvalid();
            return;

        }
        if (!(params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector)) {
            p->jkmpError("errordiv(X,Y) arguments X and Y have to be of type number vector");
            r.setInvalid();
            return;

        }
        if (params[0].size()!=2 || params[1].size()!=2) {
            p->jkmpError("errordiv(X,Y) arguments X and Y have to have length 2");
            r.setInvalid();
            return;

        }

        double X=params[0].numVec[0];
        double Xe=params[0].numVec[1];
        double Y=params[1].numVec[0];
        double Ye=params[1].numVec[1];

        r.numVec.clear();
        r.numVec<<X/Y;
        r.numVec<<sqrt(JKMP::sqr(Xe/Y)+JKMP::sqr(X*Ye/Y/Y));

    }
    void fErrorPow(jkmpResult &r, const jkmpResult* params, unsigned int  n, JKMathParser* p){
        r.setDoubleVec();
        if (n!=2) {
            p->jkmpError("errorpow(X,Y) needs 2 arguments");
            r.setInvalid();
            return;
        }
        if (!(params[0].type==jkmpDoubleVector && params[1].type==jkmpDoubleVector)) {
            p->jkmpError("errorpow(X,Y) arguments X and Y have to be of type number vector");
            r.setInvalid();
            return;
        }
        if (params[0].size()!=2 || params[1].size()!=2) {
            p->jkmpError("errorpow(X,Y) arguments X and Y have to have length 2");
            r.setInvalid();
            return;
        }

        double X=params[0].numVec[0];
        double Xe=params[0].numVec[1];
        double Y=params[1].numVec[0];
        double Ye=params[1].numVec[1];

        r.numVec.clear();
        r.numVec<<pow(X,Y);
        r.numVec<<sqrt(JKMP::sqr(Xe*Y*pow(X,Y-1.0))+JKMP::sqr(Ye*pow(X,Y)*log(Y)));
    }

    void fStruct(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        unsigned int NN=0;
        if (n>0 && params[0].type==jkmpStruct) {
            r=params[0];
            NN=1;
        } else {
            r.setStruct();
        }
        if (n>=NN+2 && (n-NN)%2==0) {
            for (unsigned int i=NN; i<n; i=i+2) {

                if (params[i].type==jkmpString) {
                    //qDebug()<<"### "<<i<<": "<<params[i].str<<params[i].toTypeString()<<params[i+1].toTypeString();
                    r.structData[params[i].str]=params[i+1];
                } else {
                    p->jkmpError(JKMP::_("struct([struct_in,] name1, value1, ...) requires a string as parameter %2").arg(i+1));
                    r.setInvalid();
                    return;
                }
            }
        } else {
            p->jkmpError("struct([struct_in,] name1, value1, ...) requires an optinal struct as first argument and an even number of name/value pairs");
            r.setInvalid();
            return;
        }
    }

    void fStructKeys(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==1 && params[0].type==jkmpStruct) {
            r.setStringVec(params[0].structData.keys());
        } else {
            p->jkmpError("structkeys(struct_in) requires one struct argument");
            r.setInvalid();
            return;
        }
    }
    void fStructGet(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==2 && params[0].type==jkmpStruct && params[1].type==jkmpString) {
            if (params[0].structData.contains(params[1].str)) {
                r=params[0].structData.value(params[1].str, jkmpResult::invalidResult());
            } else {
                p->jkmpError(JKMP::_("structget(struct_in, item): the given element '%1' does not exist in struct_in").arg(params[1].str));
                r.setInvalid();
                return;
            }
        } else {
            p->jkmpError("structget(struct_in, item) requires one struct and one string argument");
            r.setInvalid();
            return;
        }
    }
    void fStructGetSave(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==2 && params[0].type==jkmpStruct && params[1].type==jkmpString) {
            r=params[0].structData.value(params[1].str, jkmpResult::invalidResult());
        } else {
            p->jkmpError("structsaveget(struct_in, item) requires one struct and one string argument");
            r.setInvalid();
            return;
        }
    }



    void fDoubleMatrix(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        r.setDoubleMatrix();
        if (n==1) {
            if (params[0].isUInt()) {
                int ro=params[0].toUInt();
                r.setDoubleMatrix(ro*ro, ro);
            } else {
                p->jkmpError("doublematrix(rows_or_cols) requires one unsigned integer argument ");
                r.setInvalid();
                return;
            }
        } else if (n==2) {
            if (params[0].isUInt() && params[1].isUInt()) {
                int ro=params[0].toUInt();
                int co=params[1].toUInt();
                r.setDoubleMatrix(ro*co, co);
            } else {
                p->jkmpError("doublematrix(rows,cols) requires two unsigned integer arguments");
                r.setInvalid();
                return;
            }
        } else if (n==3) {
            if (params[0].isUInt() && params[1].isUInt() && params[2].type==jkmpDouble) {
                int ro=params[0].toUInt();
                int co=params[1].toUInt();
                r.setDoubleMatrix(ro*co, co, params[2].num);
            } else {
                p->jkmpError("doublematrix(rows,cols, value) requires two unsigned integer arguments and one floating-point number");
                r.setInvalid();
                return;
            }
        }

    }

    void fBoolMatrix(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        r.setBoolMatrix();
        if (n==1) {
            if (params[0].isUInt()) {
                int ro=params[0].toUInt();
                r.setBoolMatrix(ro*ro, ro, false);
            } else {
                p->jkmpError("boolmatrix(rows_or_cols) requires one unsigned integer argument ");
                r.setInvalid();
                return;
            }
        } else if (n==2) {
            if (params[0].isUInt() && params[1].isUInt()) {
                int ro=params[0].toUInt();
                int co=params[1].toUInt();
                r.setBoolMatrix(ro*co, co, false);
            } else {
                p->jkmpError("boolmatrix(rows,cols) requires two unsigned integer arguments");
                r.setInvalid();
                return;
            }
        } else if (n==3) {
            if (params[0].isUInt() && params[1].isUInt() && params[2].type==jkmpBool) {
                int ro=params[0].toUInt();
                int co=params[1].toUInt();
                r.setBoolMatrix(ro*co, co, params[2].boolean);
            } else {
                p->jkmpError("boolmatrix(rows,cols, value) requires two unsigned integer arguments and one boolean");
                r.setInvalid();
                return;
            }
        }

    }



    void fList(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser */*p*/)
    {
        r.setList();

        for (unsigned int i=0; i<n; i++) {
            r.listData.push_back(params[i]);
        }
    }


    void fListAppend(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n>=2 && params[0].type==jkmpList) {
            r=params[0];
            for (unsigned int i=1; i<n; i++) {
                r.listData.push_back(params[i]);
            }
        } else {
            if (n<2) p->jkmpError(JKMP::_("listappend(list_in, item, ...) requires one list and at least one further argument, but only %1 arguments given").arg(n));
            else if (n>=2) p->jkmpError(JKMP::_("listappend(list_in, item, ...) requires one list and at least one further argument, but arguments of type %1, %2, ... given").arg(params[0].typeName()).arg(params[1].typeName()));
            r.setInvalid();
            return;
        }
    }

    void fListGet(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==2 && params[0].type==jkmpList) {
            r.setInvalid();
            if (params[1].isDoubleVector()) {
                r=params[0].getListItem(params[1].toUInt());
            } else if (params[1].isUIntVector()) {
                JKMP::vector<uint32_t> l=params[1].toUIntVector();
                r.setList(l.size());
                for (size_t i=0; i<l.size(); i++) {
                    r.listData[i]=params[0].getListItem(l[i]);
                }
            } else if (params[1].isBoolVector()) {
                JKMP::vector<bool> l=params[1].boolVec;
                r.setList(l.size());
                if (l.size()==r.listData.size()) {
                    for (size_t i=0; i<l.size(); i++) {
                        if (l[i]) r.listData[i]=params[0].getListItem(i);
                    }
                } else {
                    p->jkmpError(JKMP::_("listget(list_in, boolvec) wrong number of items in boolvec (#%1): requires boolvec to have the same number of entries, as list_in (#%2)").arg(l.size()).arg(r.listData.size()));
                    r.setInvalid();
                    return;
                }
            }
        } else {
            if (n<2) p->jkmpError(JKMP::_("listget(list_in, items) requires one list and one integer/boolean-vector/integer-vector arguments, but only %1 arguments given").arg(n));
            else if (n>=2) p->jkmpError(JKMP::_("listget(list_in, items) requires one list and one integer/boolean-vector/integer-vector arguments, but arguments of type %1, %2 given").arg(params[0].typeName()).arg(params[1].typeName()));
            r.setInvalid();
            return;
        }
    }

    void fListGetSave(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==3 && params[0].type==jkmpList) {
            r.setInvalid();
            if (params[1].isDoubleVector()) {
                r=params[0].getListItem(params[1].toUInt(), params[2]);
            } else if (params[1].isUIntVector()) {
                JKMP::vector<uint32_t> l=params[1].toUIntVector();
                r.setList(l.size());
                for (size_t i=0; i<l.size(); i++) {
                    r.listData[i]=params[0].getListItem(l[i], params[2]);
                }
            } else if (params[1].isBoolVector()) {
                JKMP::vector<bool> l=params[1].boolVec;
                r.setList(r.listData.size());
                if (l.size()==r.listData.size()) {
                    for (size_t i=0; i<l.size(); i++) {
                        if (l[i]) r.listData[i]=params[0].getListItem(i);
                        else r.listData[i]=params[2];
                    }
                } else {
                    p->jkmpError(JKMP::_("listgetsave(list_in, boolvec) wrong number of items in boolvec (#%1): requires boolvec to have the same number of entries, as list_in (#%2)").arg(l.size()).arg(r.listData.size()));
                    r.setInvalid();
                    return;
                }
            }
        } else {
            if (n<3) p->jkmpError(JKMP::_("listget(list_in, items) requires one list and one integer/boolean-vector/integer-vector arguments, but only %1 arguments given").arg(n));
            else if (n>=3) p->jkmpError(JKMP::_("listget(list_in, items) requires one list and one integer/boolean-vector/integer-vector arguments, but arguments of type %1, %2 given").arg(params[0].typeName()).arg(params[1].typeName()).arg(params[2].typeName()));
            r.setInvalid();
            return;
        }
    }

    void fVec2List(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==1) {
            r.setInvalid();
            if (params[0].type==jkmpDoubleVector) {
                r.setList(params[0].numVec.size());
                for (size_t i=0; i<params[0].numVec.size(); i++) {
                    r.listData[i].setDouble(params[0].numVec[i]);
                }
            } else if (params[0].type==jkmpStringVector) {
                r.setList(params[0].strVec.size());
                for (size_t i=0; i<params[0].strVec.size(); i++) {
                    r.listData[i].setString(params[0].strVec[i]);
                }
            } else if (params[0].type==jkmpBoolVector) {
                r.setList(params[0].boolVec.size());
                for (size_t i=0; i<params[0].boolVec.size(); i++) {
                    r.listData[i].setString(params[0].boolVec[i]);
                }
            } else {
                p->jkmpError(JKMP::_("vec2list(x) requires one vector of double/string/boolean as argument, but the argument x is a %1").arg(params[0].typeName()));
                r.setInvalid();
                return;
            }
        } else {
            p->jkmpError(JKMP::_("vec2list(x) requires one vector of double/string/boolean as argument, but only %1 arguments given").arg(n));
            r.setInvalid();
            return;
        }
    }
    void fIsList(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==1) {
            r.setBoolean(params[0].type==jkmpList);
        } else {
            p->jkmpError(JKMP::_("islist(x) requires one argument, but %1 arguments given").arg(n));
            r.setInvalid();
            return;
        }
    }

    void fIsStruct(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==1) {
            r.setBoolean(params[0].type==jkmpStruct);
        } else {
            p->jkmpError(JKMP::_("isstruct(x) requires one argument, but %1 arguments given").arg(n));
            r.setInvalid();
            return;
        }
    }

    void fIsMatrix(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==1) {
            r.setBoolean((params[0].type==jkmpBoolMatrix)||(params[0].type==jkmpDoubleMatrix));
        } else {
            p->jkmpError(JKMP::_("ismatrix(x) requires one argument, but %1 arguments given").arg(n));
            r.setInvalid();
            return;
        }
    }

    void fIsDoubleMatrix(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==1) {
            r.setBoolean(params[0].type==jkmpDoubleMatrix);
        } else {
            p->jkmpError(JKMP::_("isdoublematrix(x) requires one argument, but %1 arguments given").arg(n));
            r.setInvalid();
            return;
        }
    }

    void fIsBoolMatrix(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==1) {
            r.setBoolean(params[0].type==jkmpBoolMatrix);
        } else {
            p->jkmpError(JKMP::_("isboolmatrix(x) requires one argument, but %1 arguments given").arg(n));
            r.setInvalid();
            return;
        }
    }

    void fListRemove(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n==2 && params[0].type==jkmpList) {
            r=params[0];
            if (params[1].isDoubleVector()) {
                r.removeListItem(params[1].toUInt());
            } else if (params[1].isUIntVector()) {
                JKMP::vector<uint32_t> l=params[1].toUIntVector();
                std::sort(l.begin(), l.end());
                for (int i=l.size()-1; i>=0; i--) {
                    r.removeListItem(l[i]);
                }
            } else if (params[1].isBoolVector()) {
                JKMP::vector<bool> l=params[1].boolVec;
                if (l.size()==r.listData.size()) {
                    for (int i=l.size()-1; i>=0; i--) {
                        if (l[i]) r.removeListItem(i);
                    }
                } else {
                    p->jkmpError(JKMP::_("listaremove(list_in, boolvec) wrong number of items in boolvec (#%1): requires boolvec to have the same number of entries, as list_in (#%2)").arg(l.size()).arg(r.listData.size()));
                    r.setInvalid();
                    return;
                }
            }
        } else {
            if (n<2) p->jkmpError(JKMP::_("listaremove(list_in, items) requires one list and one integer/boolean-vector/integer-vector arguments, but only %1 arguments given").arg(n));
            else if (n>=2) p->jkmpError(JKMP::_("listaremove(list_in, items) requires one list and one integer/boolean-vector/integer-vector arguments, but arguments of type %1, %2 given").arg(params[0].typeName()).arg(params[1].typeName()));
            r.setInvalid();
            return;
        }
    }

    void fListInsert(jkmpResult &r, const jkmpResult *params, unsigned int n, JKMathParser *p)
    {
        if (n>=3 && params[0].type==jkmpList && params[1].isUInt()) {
            r=params[0];
            int idx=params[1].toUInt();
            for (unsigned int i=2; i<n; i++) {
                r.listData.insert(r.listData.begin()+(idx+i-2), params[i]);
            }
        } else {
            if (n<3) p->jkmpError(JKMP::_("listinsert(list_in, index, items) requires one list and one integer/boolean-vector/integer-vector arguments, but only %1 arguments given").arg(n));
            else if (n>=3) p->jkmpError(JKMP::_("listinsert(list_in, index, items) requires one list and one integer/boolean-vector/integer-vector arguments, but arguments of type %1, %2, %3 given").arg(params[0].typeName()).arg(params[1].typeName()).arg(params[2].typeName()));
            r.setInvalid();
            return;
        }
    }

    jkmpResult fLastInVector(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=1) {
            p->jkmpError("lastinvector(x) needs one argument");
            return res;
        }
        if (params[0].type==jkmpDouble) return jkmpResult(params[0].num);
        if (params[0].type==jkmpDoubleVector && params[0].size()>0) return jkmpResult(params[0].numVec.back());
        if (params[0].type==jkmpBool) return jkmpResult(params[0].boolean);
        if (params[0].type==jkmpBoolVector && params[0].size()>0) return jkmpResult(params[0].boolVec.back());
        if (params[0].type==jkmpString) return jkmpResult(params[0].str);
        if (params[0].type==jkmpStringVector && params[0].size()>0) return jkmpResult(params[0].strVec.back());

        p->jkmpError("lastinvector(x): x had no entries or unrecognized type");
        return res;
    }


    jkmpResult fFirstInVector(const jkmpResult* params, unsigned int  n, JKMathParser* p) {
        jkmpResult res=jkmpResult::invalidResult();

        if (n!=1) {
            p->jkmpError("firstinvector(x) needs one argument");
            return res;
        }
        if (params[0].type==jkmpDouble) return jkmpResult(params[0].num);
        if (params[0].type==jkmpDoubleVector && params[0].size()>0) return jkmpResult(params[0].numVec.front());
        if (params[0].type==jkmpBool) return jkmpResult(params[0].boolean);
        if (params[0].type==jkmpBoolVector && params[0].size()>0) return jkmpResult(params[0].boolVec.front());
        if (params[0].type==jkmpString) return jkmpResult(params[0].str);
        if (params[0].type==jkmpStringVector && params[0].size()>0) return jkmpResult(params[0].strVec.front());

        p->jkmpError("firstinvector(x): x had no entries or unrecognized type");
        return res;
    }














}





