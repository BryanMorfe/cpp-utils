#include <iostream>
#include <iomanip>
#include <string>
#include "../data.h"

void printDataBuffer(data &d);
// Postcondition: Prints a buffer of bytes.

int main()
{
    std::string fName = "photo.jpeg";
    
    // File loading
    std::cout << "Loading a file named photo.jpeg." << std::endl;
    data d(fName);
    std::cout << "Photo loaded..." << std::endl;
    std::cout << d << std::endl;
    std::cout << std::endl;
    std::cout << "Different sizes: " << d.size() << " Bytes." << std::endl;
    std::cout << d.kSize() << " Kilobytes" << std::endl;
    std::cout << d.mSize() << " Megabytes" << std::endl;
    std::cout << d.gSize() << " Gigabytes" << std::endl;
    std::cout << std::endl;
    
    // Byte manipulation & file saving
    std::cout << "Let's append and prepend a byte and save it to a new file called mod_photo.jpeg" << std::endl;
    d.appendByte(56);
    d.prependByte(99);
    fName = "mod_photo.jpeg";
    d.saveToPath(fName);
    std::cout << "New photo saved." << std::endl;
    std::cout << std::endl;
    
    // Loading from an on-the-fly buffer of bytes and more byte manipulation
    std::cout << "Creating a data obj with a buffer..." << std::endl;
    byte bytes[] = {5, 99, 255, 33, 42, 65, 33};
    data d2(bytes, 7);
    std::cout << "Loaded..." << std::endl;
    std::cout << d2 << std::endl;
    std::cout << "The bytes look like this:" << std::endl;
    printDataBuffer(d2);
    std::cout << "Let's get a digest and hex digest representation:" << std::endl;
    std::cout << "Digest: " << d2.digest() << std::endl;
    std::cout << "Hex Digest: " << d2.hexDigest() << std::endl;
    std::cout << std::endl;
    std::cout << "Now let's insert {5, 8, 12} at i = 4:" << std::endl;
    byte b2[] = {5, 8, 12};
    d2.insertBytes(b2, 3, 4);
    std::cout << "Now the buffer of bytes looks like this: " << std::endl;
    printDataBuffer(d2);
    std::cout << std::endl;
    
    std::cout << "Now let's create a range(2, 5) and override those bytes with {11, 33, 22, 55, 44}: " << std::endl;
    data::range r(2, 5);
    byte b3[] = {11, 33, 22, 55, 44};
    
    std::cout << "The bytes in that range currently are:" << std::endl;
    byte *bInRange = d2.bytesInRange(r);
    for(int i = 0; i < r.rangeDistance() + 1; i++)
        std::cout << bInRange[i] << "  ";
    std::cout << std::endl;
    delete [] bInRange;
    
    d2.overrideBytes(b3, 5, r);
    std::cout << std::endl;
    std::cout << "After overriding, the buffer of bytes is now:" << std::endl;
    printDataBuffer(d2);
    std::cout << std::endl;
    
    // Testing capacities
    std::cout << "Now let's test capacities by creating a data obj with a capacity of 10 bytes:" << std::endl;
    data d3(10);
    std::cout << "The capacity of the buffer is " << d3.bufferCapacity() << " bytes." << std::endl;
    std::cout << "Let's add a buffer of bytes that looks like this: {5, 7, 3, 6}" << std::endl;
    byte b4[] = {5, 7, 3, 6};
    d3.insertBytes(b4, 4, 0); // Insert at the beginning
    std::cout << "Let's add another one like this {8, 145, 234, 22, 0, 1}" << std::endl;
    byte b5[] = {8, 145, 234, 22, 0, 1};
    d3.insertBytes(b5, 6, d3.size()); // Insert at the end
    std::cout << "The data now looks like this:" << std::endl;
    printDataBuffer(d3);
    std::cout << "Now we will attempt to append and prepend a byte:" << std::endl;
    d3.appendByte(3);
    d3.prependByte(3);
    std::cout << "Now let's remove the bytes at i = 3 and i = 4, and then the range(0,3)" << std::endl;
    d3.removeByteAt(3);
    d3.removeByteAt(4);
    data::range r2(0, 3);
    d3.removeBytesIn(r2);
    std::cout << "The data now looks like this:" << std::endl;
    printDataBuffer(d3);
    std::cout << std::endl;
    std::cout << "Now let's append 3 bytes: 55, 66, 77:" << std::endl;
    d3.appendByte(55);
    d3.appendByte(66);
    d3.appendByte(77);
    std::cout << "The data now looks like this:" << std::endl;
    printDataBuffer(d3);
    std::cout << std::endl;
    
    std::cout << "Now let's change the capacity to only 5 bytes." << std::endl;
    d3.setCapacity(5);
    std::cout << "The capacity of the buffer is now " << d3.bufferCapacity() << " bytes." << std::endl;
    std::cout << "The data now looks like this:" << std::endl;
    printDataBuffer(d3);
    std::cout << "Now we will attempt to append and prepend a byte:" << std::endl;
    d3.appendByte(3);
    d3.prependByte(3);
    
    return 0;
}

void printDataBuffer(data &d)
{
    for (int i = 0; i < d.size(); i++)
        std::cout << d[i] << "  ";
    std::cout << std::endl;
}
