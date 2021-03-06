/********************************************************************************
The MIT License

Copyright (c) 2016 eaizy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
********************************************************************************/

#include "protocol.h"
#include <string.h>

int login_by_qrcode(QQClient * client)
{
    FILE * fp;
    MemoryStruct * data;
    char* cp;
    int i;
    char *url=NULL;

    data=malloc(sizeof(MemoryStruct));
    data->ptr=NULL;
    data->len=0;

    fp=fopen(QRCODE_FILE,"wb");

    if (fp==NULL){
        printf("\033[31m");
        print_time();
        fprintf(stderr, "protocol.c:30 Cannot open file for QRcode. fopen returnd NULL");
        printf("\033[0m\n");
        return -1;
    }

    print_time();
    printf("Trying to login by qrcode.\n");
    curl_get("https://ui.ptlogin2.qq.com/cgi-bin/login?daid=164&target=self&style=16&mibao_css=m_webqq&appid=501004106&enable_qlogin=0&no_verifyimg=1&s_url=http%3A%2F%2Fw.qq.com%2Fproxy.html&f_url=loginerroralert&strong_login=1&login_state=10&t=20131024001",data);
    cp=mem_match(data,"<input type=\"hidden\" name=\"aid\" value=\"");
    for(i=0;i<10;i++){
        client->appid[i]=cp[i+39];
    }
    client->appid[9]='\0';
    print_time();
    printf("Get appid: %s\n",client->appid);

    url=malloc(73*sizeof(char));
    url[0]='\0';
    strcpy(url, "https://ssl.ptlogin2.qq.com/ptqrshow?appid=");
    strncat(url,client->appid,9);
    strncat(url,"&e=0&l=L&s=8&d=72&v=4" , 21 );

    printf("Downlanding 2D code");
    curl_getfile(url,fp);

    mem_free(data);
    free(data);

    fclose(fp);
    return 0;
}
