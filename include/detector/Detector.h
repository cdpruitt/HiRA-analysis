#ifndef DETECTOR_H
#define DETECTOR_H

class Detector
{
    public:
        virtual ~DataChunk() {}
        virtual void add(Detector*) = 0;
        virtual void calibrate(vector<double>) = 0;
        virtual void iterate() = 0;

    protected:
        Detector(std::string n);
        vector<double> absolutePosition(3);
        vector<double> relativePosition(3);
        unsigned int numberComponents; // number of detectors that comprise this detector
    private:
        std::string name;
}

#endif
