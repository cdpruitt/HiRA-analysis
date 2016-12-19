#ifndef COMPOSITE_DETECTOR_H
#define COMPOSITE_DETECTOR_H

#include "../include/Detector.h"

class CompositeDetector : public Detector
{
    public:
        CompositeDetector(std::string n) : Detector(n) {}
        ~CompositeDetector() {}

        void add(Detector* d)
        {
            components.push_back(d);
        }

        void calibrate(vector<double>)
        {
            cerr << "Error: tried to calibrate a CompositeDetector. "
                 << "Only SimpleDetectors (single channels) can be calibrated."
                 << endl;
        }

        unsigned int getSize()
        {
            return components.size();
        }

    protected:
        std::vector<Detector*> components;

    private:
};
