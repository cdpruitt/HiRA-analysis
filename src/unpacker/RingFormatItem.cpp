#include "../../include/unpacker/RingFormatItem.h"

using namespace std;

RingFormatItem::RingFormatItem(string n) : RingItemBody(n)
{
    majorVersion = new SimpleDataChunk("Major Version Word", 4);
    majorVersion->add(Datum("Major Version", 0xFFFFFFFF, 0));
    add(majorVersion);

    minorVersion = new SimpleDataChunk("Minor Version Word", 4);
    minorVersion->add(Datum("Minor Version", 0xFFFFFFFF, 0));
    add(minorVersion);
}
