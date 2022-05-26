 
#include "simcom_common.h"
#include <ctype.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
 
 
 
void test1053();

 

void print_option_menu(const char *options_list[], int array_size)
{
    int i, j;
    for (i = 0; i < (array_size / 2); i++)
    {
        printf("%s", options_list[2 * i]);
        for (j = 30 - strlen(options_list[2 * i]); j > 0; j--)
        {
            printf(" ");
        }
        printf("%s\n", options_list[2 * i + 1]);
    }

    if (array_size % 2 != 0)
    {
        printf("%s\n", options_list[array_size - 1]);
    }
}

 


void simcom_test_main()
{
    int opt = 0;
    int i, j;
    char scan_string[100] = {0};
    const char *options_list[] = {
#ifdef FEATURE_DEMO_WIFI
        "1. WIFI",
#endif
        "2. VOICE CALL",
        "3. DATA CALL",
        "4. SMS",
        "5. WDS(APN Manager)",
        "6. NAS",
        "7. AT",
        "8. OTA",
        "9. TTS",
        "10. GPIO",
        "11. GPS",
        "12. Bluetooth",
        "13. TCP/UDP",
        "14. Timer",
        "15. ADC",
        "16. I2C",
        "17. UIM(SimCard)",
        "18. DMS(IMEI)",
        "19. UART",
        "20. SPI",
        "21. Version",
        "22. Ethernet",
        "23. FTP",
        "24. SSL",
        "25. HTTP(S)",
        "26. FTP(S)",
#ifdef FEATURE_DEMO_MQTTS
        "27. MQTT(S)",
#endif
        "28. JSON",
        "29. LBS",
        "30. SYSTEM",
        "31. PhoneBook",
        "32. DMZ",
        "33. test1053",
        "99. EXIT",
    };

    int array_size = sizeof(options_list) / sizeof(options_list[0]);
    while (1)
    {
        printf("\nPlease select an option to test from the items listed below.\n");
        print_option_menu(options_list, sizeof(options_list) / sizeof(options_list[0]));
        printf("Option > ");

        memset(scan_string, 0, sizeof(scan_string));
        if (fgets(scan_string, sizeof(scan_string), stdin) == NULL)
            continue;

        /* Convert the option to an integer, and switch on the option entered. */
        opt = atoi(scan_string);
        switch (opt)
        {
      
       
            case 33:
            test1053();
            break;
        case 99:
            exit(0);
        default:
            break;
        }
    }
}

   
