/*
* This program could get the text's semantic
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "msp_cmn.h"
#include "msp_errors.h"

#define RESULTTEXT  "semantic.txt" //�������ı�

int main(int argc,char* argv[])
{
  const char*  login_params   = "appid = 582199a5";   // ��¼������appid��msc���,��������Ķ�
  int          ret            = MSP_SUCCESS;  //����ֵ
  FILE*        fp_rd          = NULL;   //��ȡ���ļ�
  FILE*        fp_wr          = NULL;   //д����ļ�
  long         txtSize        = 0;  //�ļ��ߴ�
  char*        text           = NULL;   //�ļ�������
  long         read_size      = 0;    //��ȡ�Ĵ�С
  unsigned int str_len        = 0;
  const char*  rec_text       = NULL;

  /*�û���¼*/
  ret = MSPLogin(NULL, NULL, login_params); //��һ���������û������ڶ������������룬����NULL���ɣ������������ǵ�¼����  
  if (MSP_SUCCESS != ret)
  {
    printf("MSPLogin failed , Error code %d.\n",ret);
    goto main_exit; //��¼ʧ�ܣ��˳���¼
  }
  /*��Ҫת�����ļ�*/
  fp_rd=fopen(argv[1],"rb");
  if(NULL == fp_rd)
  {
    printf("\nopen [%s] failed! \n",argv[1]);
    goto main_exit;
  }

  fseek(fp_rd, 0, SEEK_END);
  txtSize = ftell(fp_rd);
  fseek(fp_rd, 0, SEEK_SET);

  text = (char *)malloc(txtSize+1);
  if (NULL == text)
  {
    printf("\nout of memory! \n");
    goto main_exit;
  }

  read_size = fread((void *)text,1, txtSize, fp_rd);
  if (read_size != txtSize)
  {
    printf("\nread [%s] error!\n", argv[1]);
    goto main_exit;
  }
  text[txtSize]='\0';
  str_len = strlen(text);
  /*��ʼ�������*/
  rec_text = MSPSearch("nlp_version=2.0",text,&str_len,&ret);
  if(MSP_SUCCESS !=ret)
  {
    printf("MSPSearch failed ,error code is:%d\n",ret);
    goto main_exit;
  }
  //��д����ļ�
  fp_wr=fopen(RESULTTEXT,"wb");
  if(NULL == fp_wr)
  {
    printf("\nopen [%s] failed! \n",RESULTTEXT);
    goto main_exit;
  }

//  read_size = fwrite(rec_text,1,str_len,fp_wr);
  fprintf(fp_wr, "%s\n",rec_text );
  if(read_size != str_len)
  {
    printf("\nwrite [%s] error!\n", RESULTTEXT);
    goto main_exit;
  }

  MSPLogout(); //�˳���¼
  return 1;

main_exit:

  if (NULL != fp_rd)
  {
    fclose(fp_rd);
    fp_rd = NULL;
  }
  if (NULL != fp_wr)
  {
    fclose(fp_wr);
    fp_wr = NULL;
  }
  if (NULL != text)
  {
    free(text);
    text = NULL;
  }
  MSPLogout(); //�˳���¼

  return 0;

}


