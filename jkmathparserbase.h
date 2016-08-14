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
#ifndef JKMATHPARSERBASE_H
#define JKMATHPARSERBASE_H

#include <vector>
#include <map>


namespace JKMP {

    typedef void* GeneralData;

    extern GeneralData GeneralDataDefault;



    template <class T>
    class vector: public std::vector<T> {
        public:
            typedef typename std::vector<T> my_base;
            typedef typename my_base::const_iterator const_iterator;
            typedef typename my_base::iterator iterator;

            inline explicit vector(): my_base() {}

            inline explicit vector(size_t n, const T& val = T()): my_base(n, val) { }

            template <class InputIterator>
            inline explicit vector(InputIterator first, InputIterator last): my_base(first, last) {}

            inline vector(const vector& v): my_base(v) {}

            inline vector& operator=(const vector& v) {
                my_base::operator=(v);
                return *this;
            }

            inline bool is_empty() const {
                return this->size()<=0;
            }
    };





    template <class Key, class T>
    class map: public std::map<Key, T> {
        public:
            typedef typename std::map<Key, T> my_base;
            typedef typename my_base::const_iterator const_iterator;
            typedef typename my_base::iterator iterator;

            inline explicit map(): my_base() {}

            template <class InputIterator>
            inline explicit map(InputIterator first, InputIterator last): my_base(first, last) {}

            inline map(const map& v): my_base(v) {}

            inline JKMP::vector<Key> keys() const {
                JKMP::vector<Key> k;
                for (auto it=this->begin(); it!=this->end(); ++it) {
                    k.push_back(it->first);
                }
                return k;
            }

            inline bool is_empty() const {
                return this->size()<=0;
            }

            inline bool contains(const Key& key) const {
                return this->find(key)!=this->end();
            }

            inline T& operator[] (const Key& k) {
                return my_base::operator[](k);
            }
            inline const T& operator[] (const Key& k) const {
                const_iterator it=this->find(k);
                if (it==this->end()) {
                    throw std::out_of_range("key not found in map");
                }
                return it->second;
            }

    };
}

#endif // JKMATHPARSERBASE_H

