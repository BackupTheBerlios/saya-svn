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

    virtual void OnExit();
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
        {
            cout << "o" << flush;
        }
        syThread::Sleep(500);
    }

    return 0;
}

void SYThreadTest::OnExit() {
    cout << "Secondary thread finished." << endl;
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

    cout << "Main thread running";
    for(int i=0;i<10;i++){
        syMilliSleep(100);
        {
            cout << "." << flush;
        }
    }
    thread->Delete();
    // What happens if we don't wait for the thread to finish?
    // syMilliSleep(1000);

    return 0;
}
