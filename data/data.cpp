#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "data.h"

/** Public Class Objects **/
data::range::range(int l, int u)
{
    if (l < 0) {
        std::cout << "Fatal error: Attempted to start a range at a value less than 0." << std::endl;
        exit(1);
    } else if (u <= l) {
        std::cout << "Fatal error: Attempted to create an upper bound less or equal to lower bound." << std::endl;
        exit(1);
    }
    
    lower = l;
    upper = u;
    distance = u - l;
}

int data::range::lowerBound()
{
    return lower;
}

int data::range::upperBound()
{
    return upper;
}

int data::range::rangeDistance()
{
    return distance;
}

/** Public Member Functions **/

/*** Constructors & Destructor ***/
data::data()
{
    bytes = new byte;
    count = -1;
    maxCapacity = -1;
    internalCapacity = 1;
    filePath = "";
}

data::data(std::string fpath)
{
    std::ifstream ifile(fpath.c_str(), std::ifstream::binary);
    
    if (ifile)
    {
        ifile.seekg(0, ifile.end);
        int length = ifile.tellg();
        ifile.seekg(0, ifile.beg);
        
        bytes = new byte[length];
        for(int i = 0; i < length; i++)
        {
            int b = ifile.get();
            if (b < 0)
                b += 256;
            bytes[i] = b;
        }
        
        ifile.close();
        count = length;
    }
    else
    {
        std::cout << "Fatal error: Could not read file: " << fpath << std::endl;
        exit(1);
    }
    
    filePath = fpath;
    maxCapacity = -1;
    internalCapacity = count + 1;
}

data::data(byte *b, int n)
{
    bytes = new byte[n];
    maxCapacity = -1;
    internalCapacity = n;
    filePath = "";
    for(int i = 0; i < n; i++)
        bytes[i] = b[i];
    count = n - 1;
}

data::data(data &d)
{
    count = d.count;
    maxCapacity = d.maxCapacity;
    internalCapacity = d.internalCapacity;
    filePath = d.filePath;
    bytes = new byte[d.internalCapacity];
    for(int i = 0; i < count + 1; i++)
        bytes[i] = d.bytes[i];
}

data::data(int n)
{
    
    if (n < -1 || n == 0)
    {
        std::cout << "Fatal error: Attempted to set an invalid capacity." << std::endl;
        exit(1);
    }
    
    maxCapacity = n;
    internalCapacity = n;
    bytes = new byte[n];
    count = -1;
    filePath = "";
}

//data::data(int nBytes)
//{
//    bytes = new byte[n];
//    count = n - 1;
//    for (int i = 0; i < n; i++)
//        bytes[i] = 0;
//
//    filePath = "";
//    maxCapacity = -1;
//    internalCapacity = n;
//}

data::~data()
{
    if (internalCapacity == 1)
        delete bytes;
    else
        delete [] bytes;
}

/*** Representation of buffer of bytes ***/
std::string data::digest()
{
    std::string d = "";
    for(int i = 0; i <= count; i++) {
        d += char(bytes[i]);
    }
    
    return d;
}

std::string data::hexDigest()
{
    std::stringstream ss;
    
    for(int i = 0; i <= count; i++)
        ss << std::hex << std::setfill('0') << std::setw(2) << bytes[i];
    
    return ss.str();
}

/*** File manipulation ***/
void data::save()
{
    if (filePath == "")
    {
        std::cout << "Error: Attempted to save when file is not loaded." << std::endl;
        std::cout << "Call \'saveToPath\' and provide a file path instead." << std::endl;
    } else {
        std::ofstream ofile(filePath.c_str(), std::ofstream::binary);
        
        if(ofile)
        {
            std::string str = digest();
            ofile.write(str.c_str(), str.length());
            ofile.close();
        }
        else
        {
            std::cout << "Failed to save." << std::endl;
        }
    }
    
}

void data::saveToPath(std::string fpath)
{
    filePath = fpath;
    save();
}

/*** Bytes retrieval ***/
byte data::operator[](int i)
{
    if (i < 0) {
        std::cout << "Fatal error: Attempted to access an index less than 0." << std::endl;
        exit(1);
    }
    
    if (i > count) {
        std::cout << "Fatal error: Index out of bounds." << std::endl;
        exit(1);
    }
    
    return bytes[i];
}

byte * data::bytesInRange(data::range &r)
{
    if (r.upperBound() > count){
        std::cout << "Fatal error: Range out of bounds." << std::endl;
        exit(1);
    }
    
    byte *buffer = new byte[r.rangeDistance()];
    
    for(int i = 0; i < r.upperBound(); i++)
        buffer[i] = bytes[i + r.lowerBound()];
    
    return buffer;
}

/*** Bytes manipulation ***/
void data::appendByte(byte b)
{
    if (maxCapacity == -1)
    {
        if (count == internalCapacity - 1)
            allocate();
        bytes[++count] = b;
        
    }
    else if (count == maxCapacity - 1)
    {
        std::cout << "Error: Attempted to append a byte when the buffer has no more capacity." << std::endl;
    }
    else
    {
        if (count == internalCapacity - 1)
            allocate();
        bytes[++count] = b;
    }
    
}

void data::prependByte(byte b)
{
    if (maxCapacity == -1)
    {
        if (count == internalCapacity - 1)
            allocate();
        
        byte *tmp = new byte[internalCapacity];
        tmp[0] = b;
        for(int i = 0; i <= count; i++)
            tmp[i + 1] = bytes[i];
        count++;
        delete [] bytes;
        bytes = tmp;
        
    }
    else if (count == maxCapacity - 1)
    {
        std::cout << "Error: Attempted to append a byte when the buffer has no more capacity." << std::endl;
    }
    else
    {
        if (count == internalCapacity - 1)
            allocate();
        
        byte *tmp = new byte[internalCapacity];
        tmp[0] = b;
        for(int i = 0; i <= count; i++)
            tmp[i + 1] = bytes[i];
        count++;
        delete [] bytes;
        for(int i = 0; i < count + 1; i++)
            bytes[i] = tmp[i];
    }
    
}

void data::insertBytes(byte *b, int n, int i)
{
    if (i < 0) {
        std::cout << "Fatal error: Attempted to insert at an index less than 0." << std::endl;
        exit(1);
    }
    
    if (i > count) {
        std::cout << "Fatal error: Index out of bounds." << std::endl;
        exit(1);
    }
    
    if (maxCapacity == -1)
    {
        while (count + n > internalCapacity - 1)
            allocate();
        
        byte *tmp = new byte[internalCapacity];
        
        for(int j = 0; j < i; j++)
            tmp[j] = bytes[j];
        
        for (int j = 0; j < n; j++)
            tmp[j + i] = b[j];
        
        for(int j = i; j <= count; j++)
            tmp[j + n] = bytes[j];
        
        delete [] bytes;
        bytes = tmp;
        count += n;
    }
    else if (count == maxCapacity - 1)
    {
        std::cout << "Error: Attempted to insert a buffer of bytes when there is no more capacity." << std::endl;
    }
    else if (count + n == maxCapacity - 1)
    {
        std::cout << "Error: Attempted to insert a buffer of bytes when it will exceed the capacity." << std::endl;
    }
    else
    {
        while (count + n > internalCapacity - 1)
            allocate();
        byte *tmp = new byte[internalCapacity];
        
        for(int j = 0; j < i; j++)
            tmp[j] = bytes[j];
        
        for (int j = 0; j < n; j++)
            tmp[j + i] = b[j];
        
        for(int j = i; j <= count; j++)
            tmp[j + n] = bytes[j];
        
        delete [] bytes;
        bytes = tmp;
        count += n;
    }
}

void data::overrideBytes(byte *b, int n, range &r)
{
    if (r.upperBound() > count) {
        std::cout << "Fatal error: Range out of bounds." << std::endl;
        exit(1);
    }
    
    if (maxCapacity == -1)
    {
        while (count + n > internalCapacity - 1)
            allocate();
        
        byte *tmp = new byte[internalCapacity];
        
        int i;
        for (i = 0; i < r.lowerBound(); i++)
            tmp[i] = bytes[i];
        
        for (int j = 0; j < n; j++)
            tmp[j + r.lowerBound()] = b[j];
        
        for(int j = r.upperBound() + 1; j <= count; j++)
        {
            tmp[i + n] = bytes[j];
            i++;
        }
        
        delete [] bytes;
        bytes = tmp;
        count += n - r.rangeDistance() - 1;
    }
    else if (count == maxCapacity - 1)
    {
        std::cout << "Error: Attempted to insert a buffer of bytes when there is no more capacity." << std::endl;
    }
    else if (count + n == maxCapacity - 1)
    {
        std::cout << "Error: Attempted to insert a buffer of bytes when it will exceed the capacity." << std::endl;
    }
    else
    {
        while (count + n > internalCapacity - 1)
            allocate();
        
        byte *tmp = new byte[internalCapacity];
        
        int i;
        for (i = 0; i < r.lowerBound(); i++)
            tmp[i] = bytes[i];
        
        for (int j = 0; j < n; j++)
            tmp[j + r.lowerBound()] = b[j];
        
        for(int j = r.upperBound() + 1; j <= count; j++)
        {
            tmp[i + n] = bytes[j];
            i++;
        }
        
        delete [] bytes;
        bytes = tmp;
        count += n - r.rangeDistance() - 1;
    }
}

void data::setCapacity(int n)
{
    
    if (n < -1 || n == 0)
    {
        std::cout << "Fatal error: Attempted to set an invalid capacity." << std::endl;
        exit(1);
    }
    
    if (n == -1)
    {
        maxCapacity = -1;
    }
    else
    {
        byte *tmp = new byte[n];
        (n < internalCapacity) ? memcpy(tmp, bytes, n) : memcpy(tmp, bytes, count + 1);
        delete [] bytes;
        bytes = tmp;
        maxCapacity = n;
        internalCapacity = n;
    }
    
}

/*** Size information ***/
unsigned int data::size()
{
    return count + 1;
}

double data::kSize()
{
    return static_cast<double>(size()) / 1024.0;
}

double data::mSize()
{
    return static_cast<double>(kSize()) / 1024.0;
}

double data::gSize()
{
    return static_cast<double>(mSize()) / 1024.0;
}

int data::bufferCapacity()
{
    return maxCapacity;
}

/** Private Member Functions **/

void data::allocate()
{
    
    int n = internalCapacity * 2;
    
    if (maxCapacity != -1)
        n = std::max(maxCapacity, n); // Constraints to maxCapacity
    
    byte *tmp = new byte[n];
    for (int i = 0; i < count + 1; i++)
        tmp[i] = bytes[i];
    delete [] bytes;
    bytes = tmp;
    internalCapacity = n;
}


/** Friend Functions **/

std::istream & operator>>(std::istream &in, data &d)
{
    std::string s;
    in >> s;
    return in;
}

std::ostream& operator<<(std::ostream &out, data &d)
{
    out << "<" << d.bytes << "> " << d.size() << " bytes";
    return out;
}
