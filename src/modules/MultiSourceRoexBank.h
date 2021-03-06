/*
 * Copyright (C) 2014 Dominic Ward <contactdominicward@gmail.com>
 *
 * This file is part of Loudness
 *
 * Loudness is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Loudness is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Loudness.  If not, see <http://www.gnu.org/licenses/>. 
 */

#ifndef MultiSourceRoexBank_H
#define MultiSourceRoexBank_H

#include "../support/Module.h"
#include "../thirdParty/spline/Spline.h"

namespace loudness{

    /**
     * @class MultiSourceRoexBank 
     *
     * @brief Applies a set of level dependent rounded exponential (roex)
     * filters to an input power spectrum to estimate the auditory
     * representation (excitation pattern). 
     *
     * Roex filters are spaced uniformly at intervals of camStep between 1.8 and
     * 38.9.
     *
     * A lookup table is employed for the computing the rounded exponential
     * filter.
     *
     * Cubinc spline interpolated can be applied to yield a 0.1 Cam resolution
     * excitation pattern.
     *
     * This implementation follows a combination of:
     *
     * Glasberg, B. R., & Moore, B. C. J. (1990).  Derivation of Auditory Filter
     * Shapes From Notched-Noise Data.  Hearing Research, 47, 103–138.
     *
     * ANSI S3.4-2007. (2007).  Procedure for the Computation of Loudness of
     * Steady Sounds.
     */
    class MultiSourceRoexBank : public Module
    {

    public:

        /** Constructs a MultiSourceRoexBank with roex filters equally spaced
         * on the Cam scale.
         * 
         * @param camStep Interval between adjacent filters on the cam Scale.
         */
        MultiSourceRoexBank(Real camStep = 0.1);

        virtual ~MultiSourceRoexBank();

    private:

        virtual bool initializeInternal(const SignalBank &input);
        virtual bool initializeInternal(){return 0;};
        virtual void processInternal(const SignalBank &input);
        virtual void processInternal(){};
        virtual void resetInternal();

        void generateRoexTable(int size = 1024);

        Real camStep_;
        int nFilters_, roexIdxLimit_;
        Real step_;
        vector<vector<int> > rectBinIndices_;
        RealVec pu_, pl_, roexTable_;
    };
}

#endif
