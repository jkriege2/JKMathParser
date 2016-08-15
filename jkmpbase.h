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
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <typeinfo>
#include <map>
#include "jkmplib_imexport.h"

namespace JKMP {

    typedef void* GeneralData;
    JKMPLIB_EXPORT extern GeneralData GeneralDataDefault;

    typedef std::string stringType;
    typedef char charType;
    typedef std::stringstream stringstreamType;
    typedef std::istringstream istringstreamType;
    typedef std::ostringstream ostringstreamType;



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

            inline vector(const std::vector<T>& v): my_base(v) {}

            inline vector(const T& v1, const T& v2): my_base() { my_base::push_back(v1);my_base::push_back(v2); }
            inline vector(const T& v1, const T& v2, const T& v3): my_base() { my_base::push_back(v1);my_base::push_back(v2);my_base::push_back(v3); }
            inline vector(const T& v1, const T& v2, const T& v3, const T& v4): my_base() { my_base::push_back(v1);my_base::push_back(v2);my_base::push_back(v3);my_base::push_back(v4); }
            inline vector(const T& v1, const T& v2, const T& v3, const T& v4, const T& v5): my_base() { my_base::push_back(v1);my_base::push_back(v2);my_base::push_back(v3);my_base::push_back(v4);my_base::push_back(v5); }

            static inline vector construct(const T& data) {
                return vector<T>(1,data);
            }

            inline vector& operator=(const vector& v) {
                my_base::operator=(v);
                return *this;
            }

            inline bool is_empty() const {
                return this->size()<=0;
            }

            inline vector& operator<<(const vector& v) {
                for (const_iterator it=v.begin(); it!=v.end(); ++it) {
                    this->push_back(*it);
                }
                return *this;
            }
            inline vector& operator<<(const std::vector<T>& v) {
                for (size_t i=0; i<v.size(); i++) {
                    this->push_back(v[i]);
                }
                return *this;
            }
            inline vector& operator<<(const T& v) {
                this->push_back(v);
                return *this;
            }

            inline T value(size_t idx, const T& defaultVal=T()) const {
                if (idx<this->size()) return this->at(idx);
                return defaultVal;
            }

            inline T pop() {
                T res=this->back();
                this->pop_back();
                return res;
            }
            inline const T& top() const {
                return this->back();
            }
            inline T& top() {
                return this->back();
            }
            inline void push(const T& val) {
                this->push_back(val);
            }
            inline size_t count(const T& v) const {
                size_t N=0;
                for (size_t i=0; i<this->size(); i++) {
                    if ((*this)[i]==v) N++;
                }
                return  N;
            }
            inline bool contains(const T& v) const {
                for (size_t i=0; i<this->size(); i++) {
                    if ((*this)[i]==v) return true;
                }
                return  false;
            }
            inline void push_front(const T& v) {
                this->insert(this->begin(), v);
            }
            inline void pop_front() {
                this->erase(this->begin());
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

            inline T value(const Key& k, const T& defaultV=T()) const  {
                  const_iterator it=this->find(k);
                  if (it==this->end()) {
                      return defaultV;
                  }
                  return it->second;
            }

    };



    /*! \brief group the data in \a input according to the labels given in \a index. Then return a vector where the function \a func is applied to every vector of values from \input, which all have the same index in \a index.
        \ingroup qf3lib_tools

     */
    template <typename TVec, typename TIDXVec, typename TFUNC>
    inline TVec uniqueApplyFunction(const TVec& input, const TIDXVec& index, TFUNC func) {
        TVec res;
        JKMP::map<typename TIDXVec::value_type, TVec > dataset;
        JKMP::vector<typename TIDXVec::value_type> idxl;
        for (size_t i=0; i<std::min(input.size(), index.size()); i++) {
            dataset[index[i]].push_back(input[i]);
            if (!idxl.contains(index[i])) idxl<<index[i];
        }
        for (size_t i=0; i<idxl.size(); i++) {
            res<<func(dataset[idxl[i]]);
        }
        return res;
    }




    /*! \brief group the data in \a input according to the labels given in \a index. Then return a vector where the function \a func is applied to every vector of values from \input, which all have the same index in \a index.
        \ingroup qf3lib_tools

     */
    template <typename TVec, typename TIDXVec, typename TFUNC, typename TFUNCPARAM>
    inline TVec uniqueApplyFunction(const TVec& input, const TIDXVec& index, TFUNC func, TFUNCPARAM funcParam) {
        TVec res;
        JKMP::map<typename TIDXVec::value_type, TVec > dataset;
        JKMP::vector<typename TIDXVec::value_type> idxl;
        for (size_t i=0; i<std::min(input.size(), index.size()); i++) {
            dataset[index[i]].push_back(input[i]);
            if (!idxl.contains(index[i])) idxl<<index[i];
        }
        for (size_t i=0; i<idxl.size(); i++) {
            res<<func(dataset[idxl[i]], funcParam);
        }
        return res;
    }



    /*! \brief group the data in \a input and \a inputY according to the labels given in \a index. Then return a vector where the function \a func is applied to every vector of values from \a input and \a inputY, which all have the same index in \a index.
        \ingroup qf3lib_tools

     */
    template <typename TVec, typename TIDXVec, typename TFUNC>
    inline TVec uniqueApplyFunction2I(const TVec& input, const TVec& inputY, const TIDXVec& index, TFUNC func) {
        TVec res;
        JKMP::map<typename TIDXVec::value_type, std::pair<TVec , TVec > > dataset;
        JKMP::vector<typename TIDXVec::value_type> idxl;
        for (size_t i=0; i<std::min(input.size(), index.size()); i++) {
            dataset[index[i]].first.push_back(input[i]);
            dataset[index[i]].second.push_back(inputY[i]);
            if (!idxl.contains(index[i])) idxl<<index[i];
        }
        for (size_t i=0; i<idxl.size(); i++) {
            res<<func(dataset[idxl[i]].first, dataset[idxl[i]].second);
        }
        return res;
    }


    /** \brief convert a boolean-vector to a vector of doubles */
    JKMPLIB_EXPORT std::vector<double> boolvectorToNumVec(const std::vector<bool>& value, double trueValue=1.0, double falseValue=0.0);

}

template <class T>
inline std::ostream& operator <<(std::ostream& stream, const JKMP::vector<T>& v) {
    stream<<"JKMP::vector<"<<typeid(T).name()<<">[";
    for (size_t i=0; i<v.size(); i++) {
        if (i>0) stream<<", ";
        stream<<v[i];
    }
    stream<<"]";
    return stream;
}

#endif // JKMATHPARSERBASE_H

