/****************************************************************
 * Name:      SYThreadTest.cpp
 * Purpose:   Unit test for syThread class
 * Author:    Rigoberto Calleja (rcallejac {at} gmail {dot} com)
 * Date :     2008-09-24
 * Copyright: (c) Rigoberto Calleja
 * License:   WxWindows License
 ***************************************************************/
#include <iostream>
#include "../iomgr/sythread.h"

using namespace std;

class SYThreadTest : public syThread
{
public:
    SYThreadTest();

    // thread execution starts here
    virtual int Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual int Exit();
};

SYThreadTest::SYThreadTest()
             :syThread()
{}

int SYThreadTest::Entry()
{
    for ( ; ;  )
    {
        // check if we were asked to exit
        if ( TestDestroy() )
            break;

        cout << "Secondary thread running.\n";

        syThread::Sleep(1000);
    }

    cout << "Secondary thread finished.\n";

    return 0;
}

int SYThreadTest::Exit()
{
    return 0;
}

int main()
{
    SYThreadTest *thread = new SYThreadTest();

    if ( thread->Create() != syTHREAD_NO_ERROR )
    {
        cout << "Can't create secondary thread!\n";
    }

    if ( thread->Run() != syTHREAD_NO_ERROR )
    {
        cout << "Can't start secondary thread!\n";
    }

    for(int i=0;i<100;i++){
        cout << "Main thread running\n";
    }

    return 0;
}
