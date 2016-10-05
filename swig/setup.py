#!/usr/bin/python

from distutils.core import setup, Extension

# Third-party modules - we depend on numpy for everything
import numpy

# Obtain the numpy include directory.  This logic works across numpy versions.
try:
    numpy_include = numpy.get_include()
except AttributeError:
    numpy_include = numpy.get_numpy_include()

if __name__ == "__main__":

    setup(
        name="loudness",
        version="0.0.1",
        install_requires=[
            'numpy',
            'pysoundfile',
            'h5py'
        ],
        packages=['loudness', 'loudness.tools'],
        package_dir={'loudness': '.', 'loudness.tools': '../src/python'},
        ext_modules=[
            Extension(
                "loudness._core", [
                    "core.i",
                    "../src/thirdParty/cnpy/cnpy.cpp",
                    "../src/thirdParty/spline/Spline.cpp",
                    "../src/support/AuditoryTools.cpp",
                    "../src/support/SignalBank.cpp",
                    "../src/support/Module.cpp",
                    "../src/support/Model.cpp",
                    "../src/support/FFT.cpp",
                    "../src/support/Filter.cpp",
                    "../src/support/AudioFileProcessor.cpp",
                    "../src/modules/UnaryOperator.cpp",
                    "../src/modules/FIR.cpp",
                    "../src/modules/IIR.cpp",
                    "../src/modules/Butter.cpp",
                    "../src/modules/Biquad.cpp",
                    "../src/modules/SMA.cpp",
                    "../src/modules/EMA.cpp",
                    "../src/modules/AudioFileCutter.cpp",
                    "../src/modules/FrameGenerator.cpp",
                    "../src/modules/Window.cpp",
                    "../src/modules/PowerSpectrum.cpp",
                    "../src/modules/HoppingGoertzelDFT.cpp",
                    "../src/modules/WeightSpectrum.cpp",
                    "../src/modules/HighpassSpectrum.cpp",
                    "../src/modules/CompressSpectrum.cpp",
                    "../src/modules/RoexBankANSIS342007.cpp",
                    "../src/modules/FastRoexBank.cpp",
                    "../src/modules/MultiSourceRoexBank.cpp",
                    "../src/modules/FixedRoexBank.cpp",
                    "../src/modules/DoubleRoexBank.cpp",
                    "../src/modules/MultiSourceDoubleRoexBank.cpp",
                    "../src/modules/OctaveBank.cpp",
                    "../src/modules/ComplexGammatoneBank.cpp",
                    "../src/modules/MainLoudnessDIN456311991.cpp",
                    "../src/modules/SpecificLoudnessANSIS342007.cpp",
                    "../src/modules/SpecificPartialLoudnessMGB1997.cpp",
                    "../src/modules/SpecificPartialLoudnessCHGM2011.cpp",
                    "../src/modules/SpecificLoudnessModANSIS342007.cpp",
                    "../src/modules/BinauralInhibitionMG2007.cpp",
                    "../src/modules/InstantaneousLoudness.cpp",
                    "../src/modules/SimpleLoudness.cpp",
                    "../src/modules/ForwardMaskingPO1998.cpp",
                    "../src/modules/InstantaneousLoudnessDIN456311991.cpp",
                    "../src/modules/ARAverager.cpp",
                    "../src/modules/PeakFollower.cpp",
                    "../src/models/StationaryLoudnessANSIS342007.cpp",
                    "../src/models/StationaryLoudnessCHGM2011.cpp",
                    "../src/models/StationaryLoudnessDIN456311991.cpp",
                    "../src/models/DynamicLoudnessGM2002.cpp",
                    "../src/models/DynamicLoudnessCH2012.cpp",
                    "../src/models/DynamicLoudnessSimple.cpp",
                    "../src/models/StationaryLoudnessSimple.cpp",
                ],
                include_dirs=[numpy_include, "/usr/include"],
                library_dirs=['/usr/lib', '/usr/local/lib'],
                libraries=['fftw3', 'sndfile'],
                swig_opts=['-c++'],
                extra_compile_args=["-std=c++11", "-fPIC", "-O3"])
            ]
        )
