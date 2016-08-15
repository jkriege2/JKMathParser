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
#ifndef JKMATHPARSERFILENAMETOOLS_H
#define JKMATHPARSERFILENAMETOOLS_H

#include <string>
#include "jkmplib_imexport.h"
#include "jkmpbase.h"
#include "jkmpstringtools.h"

/* This just distinguishes between the different path formats on Windows and Unix:
 *   - on Windows you use a backslash '\' do separate directories
 *   - in Unix you use a slash '/' to separate directories
 */
#ifdef __WINDOWS__
  /** \brief a separator between two directories in a path between \c " quotes */
  #define JKMP_PATHSEPARATOR_STRING "\\"
  /** \brief a separator between two directories in a path between \c ' quotes */
  #define JKMP_PATHSEPARATOR_CHAR '\\'

#else
  /** \brief a separator between two directories in a path between \c " quotes */
  #define JKMP_PATHSEPARATOR_STRING "/"
  /** \brief a separator between two directories in a path between \c ' quotes */
  #define JKMP_PATHSEPARATOR_CHAR '/'
#endif

namespace JKMP {


    /**
     * \defgroup tools_string_filenames filename and path handling
     * \ingroup tools_string
     */
    /*@{*/

    /** \brief extracts the path from the given \a filename */
    JKMPLIB_EXPORT JKMP::stringType extract_file_path(JKMP::stringType filename);
    /** \brief replace slashes and backslashes by the system path separator in \a filename */
    JKMPLIB_EXPORT JKMP::stringType replace_to_system_pathseparator(JKMP::stringType filename);

    /** \brief extracts the name from the given \a filename */
    JKMPLIB_EXPORT JKMP::stringType extract_file_name(JKMP::stringType filename);

    /** \brief extracts the file extension from the given \a filename */
    JKMPLIB_EXPORT JKMP::stringType extract_file_ext(JKMP::stringType filename);

    /** \brief replaces the extension of the given \a filename with \a ext. \a ext may be given in
     *         the form \c ".ext" or \c "ext".
     */
    JKMPLIB_EXPORT JKMP::stringType change_file_ext(JKMP::stringType filename, JKMP::stringType ext);

    /** \brief add another part to the path */
    JKMPLIB_EXPORT JKMP::stringType extend_file_path(JKMP::stringType filename, JKMP::stringType addpath);

    /** \brief takes care that there is a backslash at the end of the given \a filename */
    JKMPLIB_EXPORT JKMP::stringType include_trailing_backslash(JKMP::stringType filename);

    /** \brief takes care that there is no backslash at the end of the given \a filename */
    JKMPLIB_EXPORT JKMP::stringType exclude_trailing_backslash(JKMP::stringType filename);

    /** \brief this method returns the full filename of the file specified in \a filename
     *
     * \attention This method uses system specific code!
     */
    JKMPLIB_EXPORT JKMP::stringType get_full_filename(JKMP::stringType filename);

    /** \brief return the current working directory */
    JKMPLIB_EXPORT JKMP::stringType get_currentworkingdirectory();



}

#endif // JKMATHPARSERFILENAMETOOLS_H

