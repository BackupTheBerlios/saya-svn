/****************************************************************
 * Name:      Test2.cpp
 * Purpose:   Unit test for syThread class
 * Author:    Rigoberto Calleja (rcallejac {at} gmail {dot} com)
 * Date :     2008-09-24
 * Copyright: (c) Rigoberto Calleja
 * License:   WxWindows License
 ***************************************************************/
#include <iostream>
#include "../saya/core/sythread.h"

using namespace std;

class Test1 : public syThread
{
public:
    Test1();

    // thread execution starts here
    virtual int Entry();

    virtual void OnExit();
};

Test1::Test1()
             :syThread()
{}

int Test1::Entry()
{
    for ( ; ;  )
    {
        // check if we were asked to exit
        if ( TestDestroy() )
            break;
        {
            cout << "o" << flush;
        }
        syThread::Sleep(150);
    }
    return 0;
}

void Test1::OnExit() {
    cout << "Secondary thread finished." << endl;
}

int main()
{
    Test1 *thread = new Test1();

    if ( thread->Create() != syTHREAD_NO_ERROR )
    {
        cout << "Can't create secondary thread!\n";
    }

    // This test doesn't start the thread until the user presses enter
    cout << "Press enter to start the thread";
    cin.get();

    if ( thread->Run() != syTHREAD_NO_ERROR )
    {
        cout << "Can't start secondary thread!\n";
    }

    cout << "Main thread running";
    for(int i=0;i<100;i++){
        syMilliSleep(100);
        if(i == 20) {
            thread->Pause();
        }
        if(i == 60) {
            thread->Resume();
        }
        {
            cout << "." << flush;
        }
    }
    thread->Delete();
    // What happens if we don't wait for the thread to finish?
    // syMilliSleep(1000);

    return 0;
}
