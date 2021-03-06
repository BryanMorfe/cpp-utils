#ifndef DATA_H
#define DATA_H
#include <iostream>
#include <string>

/** Type Definitions **/
typedef unsigned short byte;

/** data Object declaration **/

class data {
    
    friend std::istream& operator>>(std::istream &in, data &d);
    // Precondition: A file must exist in the specified path.
    // Postcondition: Loads the content of a specified filePath.
    
    friend std::ostream& operator<<(std::ostream &out, data &d);
    // Postcontidition: Displays the amount of bytes in the data object.
    
public:
    
    class range {
    public:
        range(int l, int u);
        // Postcondition: lower bound must be less than upper bound.
        
        int lowerBound();
        // Postcondition: Returns the lower bound of the range.
        
        int upperBound();
        // Postcondition: Returns the upper bound of the range.
        
        int rangeDistance();
        // Postcondition: Returns the distance between the lower and upper bound.
        
    private:
        int lower;      // Lower bound.
        int upper;      // Upper bound.
        int distance;   // Distance between the lower and upper bound.
    };
    
    /*** Constructors & Destructors ***/
    data();
    // Postcondition: Creates an empty data object.
    
    data(std::string fpath);
    // Precondition: A file must exist in the specified path.
    // Postcondition: Loaded the content of the file in the specified path.
    
    data(byte *b, int n);
    // Postcondition: Loads the buffer of passed bytes.
    
    data(data &d);
    // Postcondition: Copies the content of the data object "d".
    
    data(int n);
    // Precondition: Must be > 0 or == -1.
    // Postcondition: Sets maximum capacity to n, or no limitations if n = -1.
    
    ~data();
    // Postcondition: Destroys the buffer of bytes and prevents memory leaks.
    
    /*** Representation of buffer of bytes ***/
    std::string digest();
    // Postcontidition: Returns a string of bytes.
    
    std::string hexDigest();
    // Postcontidition: Returns a string of bytes in hex format.
    
    /*** File manipulation ***/
    void save();
    // Precondition: Must have been loaded from a file.
    // Postcondition: Overrides the currently loaded file with the new bytes.
    
    void saveToPath(std::string fpath);
    // Precondition: A valid filePath must be passed.
    // Postcondition: Creates a new file in the specified path and saves it with the content of the data obj.
    
    /*** Bytes retrieval ***/
    byte operator[](int i);
    // Precondition: The passed index must be <= than the buffer of bytes in the object.
    // Postcondition: Returns the byte in the specified index.
    
    byte *bytesInRange(range &r);
    // Precondition: The range must be between 0 and buffer of bytes - 1.
    // Postcondition: Returns a buffer of bytes in the specified range.
    
    /*** Bytes manipulation ***/
    void appendByte(byte b);
    // Postcondition: Appends a given byte to the end of the buffer.
    //                And resizes if necessary (and there is no capacity limit).
    
    void prependByte(byte b);
    // Postcondition: Prepends a given byte to the beginning of the buffer.
    //                And resizes if necessary (and there is no capacity limit).
    
    void insertBytes(byte *b, int n, int i);
    // Precondition: The given index must not be larger than the size of the buffer.
    // Postcondition: Inserts a given buffer of bytes in position i, by pushing existing bytes to the right.
    //                And resizes if necessary (and there is no capacity limit).
    
    void overrideBytes(byte *b, int n, range &r);
    // Precondition: The range must be between 0 and buffer of bytes - 1.
    // Postcondition: Overrides the bytes in the range with the passed bytes.
    //                And resizes if necessary (and there is no capacity limit).
    
    void removeByteAt(int i);
    // Precondition: i must be between 0 and the amount of bytes in the buffer
    // Postcondition: Removes the byte located at bytes[i].
    
    void removeBytesIn(range r);
    // Precondition: The specified upper bound of the range must be not exceed the amount of bytes in the buffer.
    // Postcondition: Removes the buffer of bytes in the specified range.
    
    void setCapacity(int n);
    // Precondition: Capacity must be -1 (for no limit), or greater than 0.
    // Postcondition: Sets a maximum capacity or no limits. If capacity is less than the current buffer
    //                the extra bytes lost after resizing. Contents of old buffer is copied into new
    //                buffer of bytes with capacity n.
    
    /*** Size information ***/
    unsigned int size();
    // Postcondition: Returns the size in bytes of the data obj.
    
    double kSize();
    // Postcondition: Returns the size in kilobytes of the data obj.
    
    double mSize();
    // Postcondition: Returns the size in megabytes of the data obj.
    
    double gSize();
    // Postcondition: Returns the size in gigabytes of the data obj.
    
    int bufferCapacity();
    // Postcondition: Returns the capacity of the buffer, or -1 if there is no maximum capacity.
    
protected:
    void allocate();
    // Postcondition: Doubles the capacity of the buffer.
    
private:
    byte *bytes;            // A buffer (array) of bytes.
    int count;              // The amount of bytes.
    int internalCapacity;   // The current buffer capacity.
    int maxCapacity;        // The maximum allowed allocation of bytes.
    std::string filePath;   // An optional filePath if loaded from a file.
};

#endif
