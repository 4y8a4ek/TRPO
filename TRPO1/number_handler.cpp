#include <string>
#include <windows.h>
#include <iostream>
class handler
{
private:
    char en[256];

public:
    const void set_en(const char *input)
    {
        strncpy(en, input, sizeof(en));
        en[sizeof(en) - 1] = '\0';
    }

    const char* get_en() const {
        return en;
    }

    const void BackSpace()
    {
        int len = strlen(en);
        if (len > 0)
        {
            en[len - 1] = '\0';
        }
    }

    const void Clear(){
        en[0] = '\0';
    }

    int Acc(){
        return 5;
    }
};
