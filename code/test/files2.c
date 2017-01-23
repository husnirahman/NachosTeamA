#include "syscall.h"

int main(){
	int i;
	int fp = fopen("small");
	char s[45];
	fread(fp,s,45);
	Puts("Reading\n");
	for (i = 0; i<45 ; i++)
		PutChar(s[i]);
    
	char* r = "ABCD";
	Puts("Writing\n");
	fseek(fp,0);
	fwrite(fp, r, 4);
	
	char res[38];
	Puts("Reading after writing\n");
	fseek(fp,0);
	if(fread(fp, res, 38) != -1) {
		for (i = 0; i<38 ; i++)
			PutChar(res[i]);    
    }
    fclose(fp);
		
	return 0;
}
