#include "DetectorFactory.h"
#include "WCD.h"
#include "Scintillator.h"
#include "Hodoscope.h"
#include "Dummy.h"
#include "Munra.h"
#include "Detector.h"
#include "Counter.h"

std::unique_ptr<Detector> 
DetectorFactory::CreateDetector(const int aId, const Detector::DetectorType aType)
{
    switch(aType) {
        case Detector::eWCD:
            return std::make_unique_ptr<WCD>(aId, aType);
        case Detector::eScintillator:
            return std::make_unique<Scintillator>(aId, aType);
        case Detector::eHodoscope:
            return std::make_unique<Hodoscope>(aId, aType);
        case Detector::eMunra:
            return std::make_unique<Munra>(aId, aType);
        case Detector::eDummy:
            return std::make_unique<Dummy>(aId, aType);
        case Detector::eCounter:
            return std::make_unique<Counter>(aId, aType);

    }

    }
    throw std::invalid_argument("Invalid detector type.");
}
