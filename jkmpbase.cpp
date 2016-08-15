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

#include "jkmpbase.h"

namespace JKMP {
    GeneralData GeneralDataDefault = NULL;

    std::vector<double> boolvectorToNumVec(const std::vector<bool>& value, double trueValue, double falseValue) {
        std::vector<double> v;
        v.reserve(value.size());
        for (size_t i=0; i<value.size(); i++) {
            v.push_back(value[i]?trueValue:falseValue);
        }
        return v;
    }
}

