#include "log_funcs.h"
#include "list_struct.h"

int main()
{
    OpenLogFile("LOGE1111.html", "w");
    List list = {};
    CtorList(&list);

    Node* addresses[100];  // Массив для хранения адресов
    int addressIndex = 0;
    addresses[addressIndex++] = list.end;

    Node* lastPos = nullptr;
    PushElement(&list, list.end, 78, &lastPos);
    addresses[addressIndex++] = lastPos;  // Сохраняем адрес

    PushElement(&list, lastPos, 123, &lastPos);
    addresses[addressIndex++] = lastPos;

    PushElement(&list, lastPos, 333, &lastPos);
    addresses[addressIndex++] = lastPos;

    PushElement(&list, lastPos, 444, &lastPos);
    addresses[addressIndex++] = lastPos;

    PushElement(&list, lastPos, 445, &lastPos);
    addresses[addressIndex++] = lastPos;

    PushElement(&list, addresses[3], 11111, &lastPos);
    addresses[addressIndex++] = lastPos;

    PopElement(&list, addresses[3]);

    GenerateImage (&list);

    return 0;
}
