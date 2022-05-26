#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
 
#include "simcom_test_main.h"

 
int main(int argc,char* argv[]) 
{
    printf("SDK_VER : %s\n",get_simcom_sdk_version());
    printf("DEMO_VER: %s\n",get_simcom_sdk_version());     

 
    
     
    simcom_test_main();
    

 
    return 0;
}
 