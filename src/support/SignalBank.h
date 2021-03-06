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

#ifndef SIGNALBANK_H
#define SIGNALBANK_H

#include "Common.h"
#include "UsefulFunctions.h"

namespace loudness{

    /**
     * @class SignalBank
     * 
     * @brief An object to store a bank of signals used as inputs and outputs of processing modules.
     *
     * A SignalBank consists of a number of ears, each with the same number of
     * signal channels and samples. Every signal must have the same number of samples. 
     * The sample data is therefore store as a 3D array.
     * SignalBanks carry metadata, such as the sampling frequency and the centre
     * frequencies of the channels; this is useful for sharing information
     * between processing modules. After instantiating a SignalBank, it must be
     * initialised using the initialize() function.
     *
     * SignalBank triggers are used to instruct modules when to process their
     * data. A trigger must be true (default) for a module to process the
     * SignalBank, otherwise the output will not updated. This is useful for
     * modules that process inputs at a rate lower than the host rate.
     * 
     * @author Dominic Ward
     */
    class SignalBank
    {
    public:
        SignalBank();
        ~SignalBank();


        /** Initialises the SignalBank with input arguments.
         *
         * @param nEars Number of ears.
         * @param nChannels Number of channels.
         * @param nSamples Number of samples per channel.
         * @param fs Sampling frequency.
         */
        void initialize(int nSources, int nEars, int nChannels, int nSamples, int fs);

        /** Initialises the SignalBank with the same parameters as the 
         * input SignalBank. The signals are initialised with zeros - the data
         * is not copied.
         */
        void initialize(const SignalBank &input);

        /**Zeros the all signals, sets trig to true and clears the aggregated
         * signals (if any).*/
        void reset();

        /** Returns true if the input SignalBank has the same number of ears,
         * channels and samples per signal as this SignalBank. */
        bool hasSameShape(const SignalBank& input) const;

        /** Sets every sample in the SignalBank to zero. */
        void zeroSignals();

        /** Multiplies the signal in the specified ear and channel by gainFactor. */
        void scale(int source, int ear, int channel, Real gainFactor);

        void scale(int source, int ear, Real gainFactor);

        /** Multiplies all signals in the specified ear by gainFactor. */
        void scale(int ear, Real gainFactor);

        /** Multiplies all signals in the SignalBank by gainFactor. */
        void scale(Real gainFactor);

        /** Removes all elements from the vector holding the aggregated signals.
         * */
        void clearAggregatedSignals();

        /** Sets the sampling frequency.*/
        void setFs(int fs);
 
        /** Sets the frame rate of the SignalBank.
         * The default is equal to the sampling frequency. This is typically
         * modified by modules that operate at a rate lower than the sampling
         * frequency, e.g. fs / hopSize.
         */
        void setFrameRate(Real frameRate);

        /** Sets the spacing between auditory channels in Cam units.
         * This can be useful for modules operating on auditory filterbanks.
         */
        void setChannelSpacingInCams(Real channelSpacingInCams);

        /** Sets the centre frequencies of all channels. The same frequencies
         * are used for *all* ears.
         *
         * @param centreFreqs A vector of centre frequencies in Hz.
         */
        void setCentreFreqs(const RealVec &centreFreqs);


        /** Sets the centre frequency of a single channel.
         *
         * @param channel Channel index.
         * @param freq The centre frequency (Hz).
         */
        inline void setCentreFreq(int channel, Real freq)
        {
            centreFreqs_[channel] = freq;
        }

       /** Sets the value of an individual sample in a specified ear and
        * channel.
         *
         * @param sourc Source index.
         * @param ear Ear index.
         * @param channel Channel index.
         * @param sample Sample index.
         * @param value Value of the sample.
         */

        inline void setSample(int source, int ear, int channel, int sample, Real value) 
        {
            LOUDNESS_ASSERT(
                    isPositiveAndLessThanUpper(source, nSources_) &&
                    isPositiveAndLessThanUpper(ear, nEars_) &&
                    isPositiveAndLessThanUpper(channel, nChannels_) &&
                    isPositiveAndLessThanUpper(sample, nSamples_));
            signals_[source * nTotalSamplesPerSource_
                     + ear * nTotalSamplesPerEar_ 
                     + channel * nSamples_ + sample] = value;
        }

        /** Copies nSamples from an array pointed to by source into a specified
         * signal. The ear, channel and sample index to write to must be specified,
         * along with the number of samples to copy. Watch your bounds.
         */
        void copySamples(int source, int ear, int channel, int writeSampleIndex, const Real* input, int nSamples);

        /** Copies nSamples from an array pointed to by source into a specified
         * signal. The ear, channel and sample index to write to must be specified,
         * along with the number of samples to copy. Watch your bounds.
         */
        void copySamples(int source, int ear, int channel, int writeSampleIndex, const float* input, int nSamples);

        /** Copies nSamples from all signals of the input SignalBank into the
         * current SignalBank. Both the destination sample index and source
         * sample index must be specified, along with the number of samples to
         * copy.  Watch your bounds.
         */
        void copySamples(int writeSampleIndex, const SignalBank& input, int readSampleIndex, int nSamples);

        /** Copies nSamples from all signals of the input SignalBank into the
         * current SignalBank. Both the destination sample index and source.
         * Watch your bounds.
         */
        void copySamples(const SignalBank& input);

        /** Aggregates the vector signals_ on each call. */
        void aggregate();

        /** Pull all signals back by nSamples. */
        void pullBack(int nSamples);

        /** Sets the trigger state of the SignalBank.
         *
         * @param true if active, false otherwise.
         */
        inline void setTrig(bool trig)
        {
            trig_ = trig;
        }

        /** Returns the channel spacing in Cam units. */
        const Real getChannelSpacingInCams() const;

        /** Returns the trigger of the SignalBank.
         * @return true if active, false otherwise.
         */
        inline bool getTrig() const
        {
            return trig_;
        }

        /** Returns the number of sources in the SignalBank. */
        inline int getNSources() const
        {
            return nSources_;
        }

        /** Returns the number of ears in the SignalBank. */
        inline int getNEars() const
        {
            return nEars_;
        }

        /** Returns the number of channels in the SignalBank. */
        inline int getNChannels() const
        {
            return nChannels_;
        }

        /** Returns the number of samples per channel. */
        inline int getNSamples() const
        {
            return nSamples_;
        }

        inline int getNTotalSamplesPerEar() const
        {
            return nTotalSamplesPerEar_;
        }

        inline int getNTotalSamplesPerSource() const
        {
            return nTotalSamplesPerSource_;
        }

        /** Returns the total number of samples held by the SignalBank.
         * This value is equal to nEars * nChannels_ * nSamples_. 
         */
        inline int getNTotalSamples() const
        {
            return nTotalSamples_;
        }

        /** Get the value of a single sample in a given ear and channel. 
         * watch your bounds.
         */
        inline Real getSample(int source=0, int ear=0, int channel=0, int sample=0) const
        {
            LOUDNESS_ASSERT(
                    isPositiveAndLessThanUpper(source, nSources_) &&
                    isPositiveAndLessThanUpper(ear, nEars_) &&
                    isPositiveAndLessThanUpper(channel, nChannels_) &&
                    isPositiveAndLessThanUpper(sample, nSamples_));
            return signals_[source * nTotalSamplesPerSource_
                            + ear * nTotalSamplesPerEar_
                            + channel * nSamples_ + sample];
        }

        /** Get a pointer to the signal indexed by ear, channel and sample. Use
         * this for writing to the SignalBank starting from a specific sample
         * index. If the the number of samples is one and your iterating through
         * channels, then use getSingleSampleWritePointer. Watch your bounds.
         */
        Real* getSignalWritePointer(int source, int ear, int channel, int sample)
        {
             LOUDNESS_ASSERT(
                     isPositiveAndLessThanUpper(source, nSources_) &&
                     isPositiveAndLessThanUpper(ear, nEars_) &&
                     isPositiveAndLessThanUpper(channel, nChannels_) &&
                     isPositiveAndLessThanUpper(sample, nSamples_));
            return &signals_[source * nTotalSamplesPerSource_ +
                            ear * nTotalSamplesPerEar_ +
                            channel * nSamples_ + sample];
        }

        Real* getSignalWritePointer(int source, int ear, int channel)
        {
             LOUDNESS_ASSERT(
                     isPositiveAndLessThanUpper(source, nSources_) &&
                     isPositiveAndLessThanUpper(ear, nEars_) &&
                     isPositiveAndLessThanUpper(channel, nChannels_));
            return &signals_[source * nTotalSamplesPerSource_ +
                            ear * nTotalSamplesPerEar_ +
                            channel * nSamples_];
        }

        /** Get a pointer to the read-only signal indexed by ear,
         * channel and sample. Use this for reading samples of a signal from a
         * specific sample index. If the the number of samples is one and your
         * iterating through channels, then use getSingleSampleReadPointer.
         * Watch your bounds.
         */
        const Real* getSignalReadPointer(int source, int ear, int channel, int sample) const
        {
             LOUDNESS_ASSERT(
                     isPositiveAndLessThanUpper(source, nSources_) &&
                     isPositiveAndLessThanUpper(ear, nEars_) &&
                     isPositiveAndLessThanUpper(channel, nChannels_) &&
                     isPositiveAndLessThanUpper(sample, nSamples_));

            return &signals_[source * nTotalSamplesPerSource_ +
                            ear * nTotalSamplesPerEar_ +
                            channel * nSamples_ + sample];
        }

        const Real* getSignalReadPointer(int source, int ear, int channel) const
        {
             LOUDNESS_ASSERT(
                     isPositiveAndLessThanUpper(source, nSources_) &&
                     isPositiveAndLessThanUpper(ear, nEars_) &&
                     isPositiveAndLessThanUpper(channel, nChannels_));

            return &signals_[source * nTotalSamplesPerSource_ +
                            ear * nTotalSamplesPerEar_ +
                            channel * nSamples_];
        }

        /** Get a pointer to the first sample of a one sample signal indexed
         * by ear and channel. Incrementing this pointer will move you to the
         * sample zero of the next channel in a given ear. This is often used by
         * modules that process single frames of spectral data e.g. the
         * SignalBank's output by PowerSpectrum. Watch your bounds.
         */
        Real* getSingleSampleWritePointer(int source, int ear, int channel)
        {
            LOUDNESS_ASSERT(
                     isPositiveAndLessThanUpper(source, nSources_) &&
                     isPositiveAndLessThanUpper(ear, nEars_) &&
                     isPositiveAndLessThanUpper(channel, nChannels_) &&
                     (nSamples_ == 1));

            return &signals_[source * nTotalSamplesPerSource_ +
                            ear * nTotalSamplesPerEar_ + channel];
        }

        /** Get a pointer to the first sample of a one sample read-only signal indexed
         * by ear and channel. Incrementing this pointer will move you to the
         * sample zero of the next channel in a given ear. This is often used by
         * modules that process single frames of spectral data e.g. the
         * SignalBank's output by PowerSpectrum. Watch your bounds.
         */
        const Real* getSingleSampleReadPointer(int source, int ear, int channel) const
        {
            LOUDNESS_ASSERT(
                     isPositiveAndLessThanUpper(source, nSources_) &&
                     isPositiveAndLessThanUpper(ear, nEars_) &&
                     isPositiveAndLessThanUpper(channel, nChannels_) &&
                     (nSamples_ == 1));

            return &signals_[source * nTotalSamplesPerSource_ +
                            ear * nTotalSamplesPerEar_ + channel];
        }

        /** Returns a reference to all signals (as a flattened vector). */
        const RealVec& getSignals() const
        {
            return signals_;
        }

        /** Returns a reference to the aggregates signals (as a flattened
         * vector). */
        const RealVec& getAggregatedSignals() const
        {
            return aggregatedSignals_;
        }

        /** Returns the centre frequency in Hz of a specific channel. Watch your
         * bounds.
         */
        inline Real getCentreFreq(int channel) const
        {
            LOUDNESS_ASSERT(isPositiveAndLessThanUpper(channel, nChannels_));
            return centreFreqs_[channel];
        }

        /** Returns a reference to a vector of centre frequencies corresponding to each
         * channel.
         */
        const RealVec& getCentreFreqs() const;

        /** Get a pointer to the read-only centre frequencies of all channels. */
        const Real* getCentreFreqsReadPointer(int channel) const;

        /** Get a pointer to the centre frequency of a specific channel. Use
         * this for modifying the centre frequencies via a pointer. 
         */
        Real* getCentreFreqsWritePointer(int channel);

        void centreFreqsInCams() const;
        const RealVec& getCentreFreqsInCams() const;

        /** Returns the sampling frequency in Hz. */
        int getFs() const;

        /** Returns the frame rate in Hz.
         *
         * Default is fs_ / nSamples_ .
         */
        Real getFrameRate() const;
        
        /** Returns the state of the SignalBank.
         *
         * @return true if initialised, false otherwise.
         */
        bool isInitialized() const;

    private:

        int nSources_, nEars_, nChannels_, nSamples_;
        int nTotalSamples_, nTotalSamplesPerSource_, nTotalSamplesPerEar_;
        bool trig_, initialized_;
        int fs_;
        Real frameRate_, channelSpacingInCams_;
        long long reserveSamples_;
        RealVec signals_, aggregatedSignals_;
        RealVec centreFreqs_;
    }; 
}
#endif 
