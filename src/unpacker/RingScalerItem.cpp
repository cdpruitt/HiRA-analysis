#include "../../include/unpacker/RingScalerItem.h"

using namespace std;

RingScalerItem::RingScalerItem(string n) : RingBodyItem(n)
{
    // Create word structure for a Scaler Item
    intervalStartOffset = new SimpleDataChunk("Interval Start Offset Word", 4);
    intervalStartOffset->add(Datum("Interval Start Offset", 0xFFFF, 0));
    add(intervalStartOffset);

    intervalEndOffset = new SimpleDataChunk("Interval End Offset Word", 4);
    intervalEndOffset->add(Datum("Interval End Offset", 0xFFFF, 0));
    add(intervalEndOffset);

    timestamp = new SimpleDataChunk("Timestamp Word", 4);
    timestamp->add(Datum("Timestamp", 0xFFFF, 0));
    add(timestamp);

    intervalDivisor = new SimpleDataChunk("Interval Divisor", 4);
    intervalDivisor->add(Datum("Interval Divisor", 0xFFFF, 0));
    add(intervalDivisor);

    numberOfScalers = new SimpleDataChunk("Number of Scalers Word", 4);
    numberOfScalers->add(Datum("Number of Scalers", 0xFFFF, 0));
    add(numberOfScalers);

    isIncremental = new SimpleDataChunk("Is Incremental? Word", 4);
    isIncremental->add(Datum("Is Incremental?", 0xFFFF, 0));
    add(isIncremental);

    scalerValues = new CompositeDataChunk("Scaler Values");
    add(scalerValues);
}

void RingScalerItem::extractData(ifstream& evtfile)
{
    for(int i=0; i<getScalerNumber(); i++)
    {
        string s = to_string(i);
        SimpleDataChunk* scalerValue = new SimpleDataChunk("Scaler " + s, 4);
        scalerValues->add(scalerValue);
    }
}

unsigned int RingScalerItem::getScalerNumber()
{
    return numberOfScalers->data[0].value;
}
