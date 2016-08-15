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

/** \file JKMathParserBase.h
 *  \ingroup jkmplib_mathtools_parser
 */
#ifndef JKMATHPARSERSTRINGTOOLS_H
#define JKMATHPARSERSTRINGTOOLS_H

#include <vector>
#include <string>
#include <stdint.h>
#include "jkmpbase.h"
#include "jkmplib_imexport.h"

namespace JKMP {
    class string; // forward
    class stringVector; // forward

    JKMPLIB_EXPORT string _(const string& s);

    JKMPLIB_EXPORT string charToStr(JKMP::charType v);
    JKMPLIB_EXPORT string intToStr(int64_t v, int fieldLEngth=-1, JKMP::charType fillc=' ');
    JKMPLIB_EXPORT string uintToStr(uint64_t v, int fieldLEngth=-1, JKMP::charType fillc=' ');
    JKMPLIB_EXPORT string intToHex(uint64_t v, int fieldLEngth=-1, JKMP::charType fillc='0');
    JKMPLIB_EXPORT string intToBin(uint64_t v, int fieldLEngth=-1, JKMP::charType fillc='0');
    JKMPLIB_EXPORT string intToOct(uint64_t v, int fieldLEngth=-1, JKMP::charType fillc='0');
    JKMPLIB_EXPORT string floatToStr(double v, int precision=-1, int fieldLEngth=-1, JKMP::charType fillc=' ', bool remove_trail0=false, double belowIsZero=1e-16);
    JKMPLIB_EXPORT string boolToStr(bool v);
    JKMPLIB_EXPORT bool strToBool(const JKMP::stringType& data);
    JKMPLIB_EXPORT double strToFloat(const JKMP::stringType& data);
    JKMPLIB_EXPORT int64_t strToInt(const JKMP::stringType& data);
    JKMPLIB_EXPORT int64_t hexToInt(const JKMP::stringType& data);
    JKMPLIB_EXPORT int64_t octToInt(const JKMP::stringType& data);
    JKMPLIB_EXPORT int64_t binToInt(const JKMP::stringType& data);

    JKMPLIB_EXPORT JKMP::stringType toLower(const JKMP::stringType& s);
    JKMPLIB_EXPORT JKMP::stringType toUpper(const JKMP::stringType& s);
    JKMPLIB_EXPORT JKMP::stringType escapify(JKMP::stringType text);
    JKMPLIB_EXPORT JKMP::stringType deescapify(JKMP::stringType text);
    JKMPLIB_EXPORT JKMP::stringType readFile(const JKMP::stringType& fn);

    /*! \brief convert a vector of numbers to a JKMP::stringType with a given decimalSeparator
        \ingroup qf3lib_tools

    */
    JKMPLIB_EXPORT JKMP::stringType doubleVecToStr(const std::vector<double>& value, int prec = 10, const JKMP::stringType itemSeparator=", " );
    /*! \brief convert a matrix of numbers to a JKMP::stringType with a given decimalSeparator
        \ingroup qf3lib_tools

    */
    JKMPLIB_EXPORT JKMP::stringType doubleMatrixToStr(const std::vector<double>& value, size_t columns, int prec = 10, const JKMP::stringType& itemSeparator=", ", const JKMP::stringType& columnSeparator="\n");
    /*! \brief convert a matrix of numbers to a JKMP::stringType with a given decimalSeparator
        \ingroup qf3lib_tools

    */
    JKMPLIB_EXPORT JKMP::stringType boolMatrixToStr(const std::vector<bool>& value, size_t columns, const JKMP::stringType& itemSeparator=", ", const JKMP::stringType& columnSeparator="\n", const JKMP::stringType& trueName=JKMP::stringType("true"), const JKMP::stringType& falseName=JKMP::stringType("false"));
    /*! \brief convert a vector of booleans to a JKMP::stringType
        \ingroup qf3lib_tools

    */
    JKMPLIB_EXPORT JKMP::stringType boolVecToStr(const std::vector<bool>& value, const JKMP::stringType itemSeparator=", ", const JKMP::stringType& trueName=JKMP::stringType("true"), const JKMP::stringType& falseName=JKMP::stringType("false") );


    class string: public JKMP::stringType {
        public:
            typedef typename JKMP::stringType my_base;
            typedef typename my_base::const_iterator const_iterator;
            typedef typename my_base::iterator iterator;

            inline explicit string(): my_base() {}

            inline explicit string(size_t n, const JKMP::charType& val = '\0'): my_base(n, val) { }

            template <class InputIterator>
            inline explicit string(InputIterator first, InputIterator last): my_base(first, last) {}

            inline string(const string& v): my_base(v) {}
            inline string(const JKMP::stringType& v): my_base(v) {}

            inline string (const string& str, size_t pos, size_t len = npos): my_base(str, pos, len) {}

            inline string (const JKMP::charType* s): my_base(s) {}
            inline string (const JKMP::charType* s, size_t n): my_base(s, n) {}

            JKMPLIB_EXPORT string trimmed() const;
            JKMPLIB_EXPORT string toLower() const;
            JKMPLIB_EXPORT string toUpper() const;


            inline string& operator=(const string& v) {
                my_base::operator=(v);
                return *this;
            }

            inline string& operator=(const JKMP::stringType& v) {
                my_base::operator=(v);
                return *this;
            }

            inline string& operator=(const JKMP::charType* c) {
                my_base::operator=(c);
                return *this;
            }

            inline string& operator=(JKMP::charType c) {
                my_base::operator=(JKMP::stringType(1,c));
                return *this;
            }

            inline bool is_empty() const {
                return this->size()<=0;
            }

            inline bool contains(JKMP::charType c, bool caseSensitive=true) const {
                if (caseSensitive) return this->find(c)!=npos;
                else return this->toLower().contains(JKMP::toLower(JKMP::stringType(1,c)), true);
            }


            inline bool contains(const JKMP::stringType& s, bool caseSensitive=true) const {
                if (caseSensitive) return this->find(s)!=npos;
                else return this->toLower().contains(JKMP::toLower(s), true);
            }

            JKMPLIB_EXPORT string arg(const string& s1) const;

            inline string arg(int64_t s1, int fieldLEngth=-1, JKMP::charType fillc=' ') const {
                return arg(intToStr(s1, fieldLEngth, fillc));
            }
            inline string arg(uint64_t s1, int fieldLEngth=-1, JKMP::charType fillc=' ') const {
                return arg(intToStr(s1, fieldLEngth, fillc));
            }

            inline string arg(int32_t s1, int fieldLEngth=-1, JKMP::charType fillc=' ') const {
                return arg(intToStr(static_cast<int64_t>(s1), fieldLEngth, fillc));
            }
            inline string arg(uint32_t s1, int fieldLEngth=-1, JKMP::charType fillc=' ') const {
                return arg(uintToStr(static_cast<uint64_t>(s1), fieldLEngth, fillc));
            }

            inline string arg(int16_t s1, int fieldLEngth=-1, JKMP::charType fillc=' ') const {
                return arg(intToStr(static_cast<int64_t>(s1), fieldLEngth, fillc));
            }
            inline string arg(uint16_t s1, int fieldLEngth=-1, JKMP::charType fillc=' ') const {
                return arg(uintToStr(static_cast<uint64_t>(s1), fieldLEngth, fillc));
            }

            inline string arg(int8_t s1, int fieldLEngth=-1, JKMP::charType fillc=' ') const {
                return arg(intToStr(static_cast<int64_t>(s1), fieldLEngth, fillc));
            }
            inline string arg(uint8_t s1, int fieldLEngth=-1, JKMP::charType fillc=' ') const {
                return arg(uintToStr(static_cast<uint64_t>(s1), fieldLEngth, fillc));
            }

            inline string arg(double s1) const {
                return arg(floatToStr(s1));
            }
            inline string arg(float s1) const {
                return arg(floatToStr(s1));
            }
            inline string arg(bool s1) const {
                return arg(boolToStr(s1));
            }
             inline void push_back(const string& v) {
                (*this) += v;
            }
            inline void push_back(const JKMP::charType& v) {
                (*this) += v;
            }
            inline void push_back(const JKMP::charType* v) {
                (*this) += v;
            }

            inline string& operator<<(const string& v) {
                (*this) += v;
                return *this;
            }
            inline string& operator<<(const JKMP::charType& v) {
                (*this) += v;
                return *this;
            }
            inline string& operator<<(const JKMP::charType* v) {
                (*this) += v;
                return *this;
            }

            using my_base::replace;
            string& replace(const string & before, const string & after, bool caseSensitive=true) ;

            JKMPLIB_EXPORT stringVector split(const string& sep) const;
            inline string left(size_t n) {
                return substr(0, n);
            }
            inline string right(size_t n) {
                return substr(size()-n, n);
            }

    };


    class stringVector: public JKMP::vector<JKMP::string> {
        public:
            typedef typename JKMP::vector<JKMP::string> my_base;
            typedef typename my_base::const_iterator const_iterator;
            typedef typename my_base::iterator iterator;

            inline explicit stringVector(): my_base() {}

            inline explicit stringVector(size_t n, const JKMP::string& val = JKMP::string()): my_base(n, val) { }

            template <class InputIterator>
            inline explicit stringVector(InputIterator first, InputIterator last): my_base(first, last) {}

            inline stringVector(const my_base& v): my_base(v) {}
            inline stringVector(const stringVector& v): my_base(v) {}
            inline stringVector(const string& v): my_base() { push_back(v); }
            inline stringVector(const JKMP::stringType& v): my_base() { push_back(v); }
            inline stringVector(const JKMP::charType* v): my_base() { push_back(v); }

            inline stringVector& operator=(const stringVector& v) {
                my_base::operator=(v);
                return *this;
            }

            inline stringVector& operator=(const my_base& v) {
                my_base::operator=(v);
                return *this;
            }
            JKMP::stringType join(const JKMP::stringType& sep) const;
            inline bool contains(const JKMP::stringType& v, bool caseSensitive=true) const {
                for (size_t i=0; i<this->size(); i++) {
                    if ((*this)[i].contains(v, caseSensitive)) return true;
                }
                return  false;
            }
            inline bool contains(const JKMP::charType& v, bool caseSensitive=true) const {
                for (size_t i=0; i<this->size(); i++) {
                    if ((*this)[i].contains(v, caseSensitive)) return true;
                }
                return  false;
            }

     };

}

#endif // JKMATHPARSERSTRINGTOOLS_H

