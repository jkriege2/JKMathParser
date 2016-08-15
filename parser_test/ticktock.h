/*
    Copyright (c) 2008-2015 Jan W. Krieger (<jan@jkrieger.de>, <j.krieger@dkfz.de>), German Cancer Research Center (DKFZ) & IWR, University of Heidelberg

    

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


#ifndef TICKTOCK_H
#define TICKTOCK_H

#ifndef __WINDOWS__
#if defined(WIN32) || defined(WIN64) || defined(_MSC_VER) || defined(_WIN32)
#define __WINDOWS__
#endif
#endif

#ifdef __WINDOWS__
  #include "highrestimer.h"
#else
  #include <time.h>
#endif
#include<stdio.h>

#ifdef QT_CORE_LIB
#include <QDebug>
#endif


/** \brief this class provides a time measurement service
 * \ingroup tool_classes
 *
 * This provides timing services as tools for other classes simply make this the parent class
 * of another class and you can call tic() and toc(). After the call of toc() the protected member
 * runtime will contain the time between the last tic() and toc() in seconds.
 *
 * On Windows this implementation uses the class HighResTimer to get a high-resolution measurement of the
 * time. On other systems it uses the standard C library timing routines.
 */
class  TicToc
{
    public:
        /** Default constructor */
        inline TicToc() { runtime=0; intAvgTimingTicTock=0; }
        /** Default destructor */
        inline virtual ~TicToc() {}

        /** \return duration in seconds */
        inline double get_duration() { return runtime; }
    protected:

        /** \brief run-time of the run() method in seconds */
        double runtime;
        #ifdef __WINDOWS__
            /** \brief timer class for windows (high resolution) */
            HighResTimer ht;
        #else
            /** \brief used for runtim measurement */
            clock_t start;

            /** \brief used for runtim measurement */
            clock_t end;
        #endif

        /** \brief call this at the beginning of the runtime meausrement */
        inline void tic() {
            #ifdef __WINDOWS__
                ht.start();
            #else
                start=clock();
            #endif
        };

        /** \brief call this at the end of the runtime measurement. the time since the last call of tic() will be stored in runtime */
        inline double toc() {
            #ifdef __WINDOWS__
                runtime = ht.get_time()/1e6;
            #else
                end = clock();
                runtime = ((double) (end - start)) / CLOCKS_PER_SEC;
            #endif
            return runtime;
        };

        /** \brief call this at the end of the runtime measurement. the time since the last call of tic() will be stored in runtime */
        inline void tocPrint(const char* message=NULL) {
            toc();
#ifdef QT_CORE_LIB
            if (message) {
                qDebug()<<"toc ["<<message<<"]: "<<(get_duration()*1000.0)<<"ms";
            } else {
                qDebug()<<"toc: "<<(get_duration()*1000.0)<<"ms";
            }
#else
            if (message) {
                printf("toc [%s]: %.4lfms\n", message, get_duration()*1000.0);
            } else {
                printf("toc: %.4lfms\n", get_duration()*1000.0);
            }
#endif
        };

        /** \brief call this for averaging the times in runtime. the time since the last call of tic() will be stored in runtime */
        inline void tocAvg(double& averageTime) {
            toc();
            double t=get_duration()*1000.0;
            const double alpha=0.8;
            averageTime=averageTime*alpha+(1.0-alpha)*t;
        };

        /** \brief call this for averaging the times in runtime. the time since the last call of tic() will be stored in runtime */
        inline void tocAvg() {
            tocAvg(intAvgTimingTicTock);
        }

        /** \brief call this at the end of the runtime measurement. the time since the last call of tic() will be stored in runtime */
        inline void tocAvgPrint(double& averageTime, const char* message=NULL) {
            toc();
            double t=get_duration()*1000.0;
            const double alpha=0.8;
            averageTime=averageTime*alpha+(1.0-alpha)*t;
#ifdef QT_CORE_LIB
            if (message) {
                qDebug()<<"toc ["<<message<<"]: "<<t<<"ms   (avg="<<averageTime<<"ms)";
            } else {
//                qDebug()<<"toc: "<<t<<"ms   (avg="<<averageTime<<"ms)";
            }
#else
            if (message) {
                printf("toc [%s]: %8.4lfms   (avg=%.4lfms)\n", message,t,averageTime);
            } else {
                printf("toc: %8.4lfms  (avg=%.4lfms)\n", t,averageTime);
            }
#endif
        };
        /** \brief call this at the end of the runtime measurement. the time since the last call of tic() will be stored in runtime */
        inline void tocAvgPrint(const char* message=NULL) {
            tocAvgPrint(intAvgTimingTicTock, message);
        }
    private:
        double intAvgTimingTicTock;
};


/** \brief implementation of TicToc with public access
 * \ingroup tool_classes
 */
class  PublicTicToc: public TicToc {
    public:
        /** Default constructor */
        inline PublicTicToc(): TicToc() {}
        /** Default destructor */
        inline virtual ~PublicTicToc() {}

        inline void tic() {
            TicToc::tic();
        }

        inline double toc() {
            return TicToc::toc();
        }

        inline void tocPrint(const char* message=NULL) {
            TicToc::tocPrint(message);
        }

        inline void tocAvg() {
            TicToc::tocAvg();
        }

        inline void tocAvgPrint(const char* message=NULL) {
            TicToc::tocAvgPrint(message);
        }

};

#endif // TICKTOCK_H
