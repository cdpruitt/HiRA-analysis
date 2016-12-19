#ifndef SIMPLE_DETECTOR_H
#define SIMPLE_DETECTOR_H

class SimpleDetector : public Detector
{
    public:
        SimpleDetector(std::string n) : Detector(n) {}

        void add(Detector*)
        {
            cerr << "Error: tried to add a component to a SimpleDetector. "
                 << "Only CompositeDetectors (arrays of detectors) can add an "
                 << "detectors as components." << endl;
        }

        void calibrate(vector<double> calibration);

    protected:
    private:
}
#endif
