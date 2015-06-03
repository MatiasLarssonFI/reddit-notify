#include <iostream>

#include "notification.hxx"

using namespace std;

int main()
{
    Notification notification("Test notification", "This is a test notification");
    notification.setTimeout(5000);
    notification.show();

    return 0;
}

