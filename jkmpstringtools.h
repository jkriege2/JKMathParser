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

namespace JKMP {
    class string; // forward
    class stringVector; // forward

    string _(const string& s);

    string chartostr(char v);
    string inttostr(int64_t v, int fieldLEngth=-1, char fillc=' ');
    string uinttostr(uint64_t v, int fieldLEngth=-1, char fillc=' ');
    string inttohex(uint64_t v, int fieldLEngth=-1, char fillc='0');
    string inttobin(uint64_t v, int fieldLEngth=-1, char fillc='0');
    string inttooct(uint64_t v, int fieldLEngth=-1, char fillc='0');
    string floattostr(double v, int past_comma=-1, bool remove_trail0=false, double belowIsZero=1e-16);
    string booltostr(bool v);
    bool strtobool(const std::string& data);
    double stringtofloat(const std::string& data);
	int64_t stringtoint(const std::string& data);
    int64_t hextoint(const std::string& data);
    int64_t octtoint(const std::string& data);
    int64_t bintoint(const std::string& data);

    std::string tolower(const std::string& s);
    std::string toupper(const std::string& s);
    std::string escapify(std::string text);
    std::string deescapify(std::string text);
    std::string readFile(const std::string& fn);

    class string: public std::string {
        public:
            typedef typename std::string my_base;
            typedef typename my_base::const_iterator const_iterator;
            typedef typename my_base::iterator iterator;

            inline explicit string(): my_base() {}

            inline explicit string(size_t n, const char& val = '\0'): my_base(n, val) { }

            template <class InputIterator>
            inline explicit string(InputIterator first, InputIterator last): my_base(first, last) {}

            inline string(const string& v): my_base(v) {}
            inline string(const std::string& v): my_base(v) {}

            inline string (const string& str, size_t pos, size_t len = npos): my_base(str, pos, len) {}

            inline string (const char* s): my_base(s) {}

            inline string (const char* s, size_t n): my_base(s, n) {}

            string trimmed() const;
            string toLower() const;
            string toUpper() const;


            inline string& operator=(const string& v) {
                my_base::operator=(v);
                return *this;
            }

            inline string& operator=(const std::string& v) {
                my_base::operator=(v);
                return *this;
            }

            inline string& operator=(const char* c) {
                my_base::operator=(c);
                return *this;
            }

            inline string& operator=(char c) {
                my_base::operator=(std::string(1,c));
                return *this;
            }

            inline bool is_empty() const {
                return this->size()<=0;
            }

            inline bool contains(char c, bool caseSensitive=true) const {
                if (caseSensitive) return this->find(c)!=npos;
                else this->toLower().contains(JKMP::tolower(std::string(1,c)), true);
            }


            inline bool contains(const std::string& s, bool caseSensitive=true) const {
                if (caseSensitive) return this->find(s)!=npos;
                else this->toLower().contains(JKMP::tolower(s), true);
            }

            string arg(const string& s1) const;

            inline string arg(int64_t s1, int fieldLEngth=-1, char fillc=' ') const {
                return arg(inttostr(s1, fieldLEngth, fillc));
            }
            inline string arg(uint64_t s1, int fieldLEngth=-1, char fillc=' ') const {
                return arg(inttostr(s1, fieldLEngth, fillc));
            }

            inline string arg(int32_t s1, int fieldLEngth=-1, char fillc=' ') const {
                return arg(inttostr(static_cast<int64_t>(s1), fieldLEngth, fillc));
            }
            inline string arg(uint32_t s1, int fieldLEngth=-1, char fillc=' ') const {
                return arg(uinttostr(static_cast<uint64_t>(s1), fieldLEngth, fillc));
            }

            inline string arg(int16_t s1, int fieldLEngth=-1, char fillc=' ') const {
                return arg(inttostr(static_cast<int64_t>(s1), fieldLEngth, fillc));
            }
            inline string arg(uint16_t s1, int fieldLEngth=-1, char fillc=' ') const {
                return arg(uinttostr(static_cast<uint64_t>(s1), fieldLEngth, fillc));
            }

            inline string arg(int8_t s1, int fieldLEngth=-1, char fillc=' ') const {
                return arg(inttostr(static_cast<int64_t>(s1), fieldLEngth, fillc));
            }
            inline string arg(uint8_t s1, int fieldLEngth=-1, char fillc=' ') const {
                return arg(uinttostr(static_cast<uint64_t>(s1), fieldLEngth, fillc));
            }

            inline string arg(double s1) const {
                return arg(floattostr(s1));
            }
            inline string arg(float s1) const {
                return arg(floattostr(s1));
            }
            inline string arg(bool s1) const {
                return arg(booltostr(s1));
            }
             inline void push_back(const string& v) {
                (*this) += v;
            }
            inline void push_back(const char& v) {
                (*this) += v;
            }
            inline void push_back(const char* v) {
                (*this) += v;
            }

            inline string& operator<<(const string& v) {
                (*this) += v;
                return *this;
            }
            inline string& operator<<(const char& v) {
                (*this) += v;
                return *this;
            }
            inline string& operator<<(const char* v) {
                (*this) += v;
                return *this;
            }

            inline string& replace(const string & before, const string & after, bool caseSensitive=true) ;

            stringVector split(const string& sep) const;
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
            inline stringVector(const std::string& v): my_base() { push_back(v); }
            inline stringVector(const char* v): my_base() { push_back(v); }

            inline stringVector& operator=(const stringVector& v) {
                my_base::operator=(v);
                return *this;
            }

            inline stringVector& operator=(const my_base& v) {
                my_base::operator=(v);
                return *this;
            }
            std::string join(const std::string& sep) const;
            inline bool contains(const std::string& v, bool caseSensitive=true) const {
                for (size_t i=0; i<this->size(); i++) {
                    if ((*this)[i].contains(v, caseSensitive)) return true;
                }
                return  false;
            }
            inline bool contains(const char& v, bool caseSensitive=true) const {
                for (size_t i=0; i<this->size(); i++) {
                    if ((*this)[i].contains(v, caseSensitive)) return true;
                }
                return  false;
            }

     };

}

#endif // JKMATHPARSERSTRINGTOOLS_H

