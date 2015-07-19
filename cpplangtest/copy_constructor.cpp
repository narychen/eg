#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream>
using namespace std;

stringstream sin;

class MyString
{
private:
    char* buf;

public:
    MyString(const char* in){
        cout << "construct " << endl;
        if (in != NULL) {
            buf = new char[strlen(in) + 1];
            strcpy(buf, in);
            cout << "buf point to: 0x" << hex << (int*)buf << endl;
        } else {
            buf = NULL;
        }
    }

    MyString(const MyString& copy){
        cout << "copy construct " << endl;
        if (copy.buf != NULL) {
            buf = new char[strlen(copy.buf) + 1];
            strcpy(buf, copy.buf);
            cout << "buf point to 0x" << hex << (int*)buf << endl;
        } else {
            buf = NULL;
        }
    }
    
    MyString(MyString&& moveSource) {
        cout << "move construct " << endl;
        if (moveSource.buf != NULL) {
            buf = moveSource.buf;
            moveSource.buf = NULL;
        }
    }

    ~MyString() {
        cout << "Invoke destructor " << endl;
        if (buf != NULL) {
            delete [] buf;
        }
    }

    int GetLength() {
        return strlen(buf);
    }

    const char* GetString() {
        return buf;
    }
    
    operator const char* () {
        return buf;
    }
    

};

void UseMyString(MyString in) {
    cout << "len = " << dec << in.GetLength() << endl;
    cout << "buf = " << in.GetString() << endl;
}

MyString Copy() {
    cout << "copy" << endl;
    MyString copy("xxxxddd");
    cout << "decopy" << endl;
    return copy;
}

int main()
{
    MyString sayhell("hello fuck u");
    // UseMyString(sayhell);
    MyString sayhell2 = Copy();
    cout << sayhell2 << "xx" << endl;
    return 0;
}














