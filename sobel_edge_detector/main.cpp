//
//  main.cpp
//  sobel_edge_detector
//
//  Created by Jace on 2018. 11. 29..
//  Copyright © 2018년 Jace. All rights reserved.
//

#include <math.h>

#include <stdio.h>
using namespace std;


int main()
{
  
  int height= 256;
  int width=256;
  int i,j,vmax,vmin;
  int m_HistoArr[256];
  unsigned char InImg[256][256];
  unsigned char OrgImg[256][256];
  
  
  FILE *infile= fopen("AVE256.raw","rb");
  if(infile==NULL){printf("error!");return 0;}
  fread(InImg, sizeof(char),256*256,infile);
  fclose(infile);
  
  
  
  int MaskSobelX[3][3]={{-1,0,1},
    {-2,0,2},
    {-1,0,1}};
  int MaskSobelY[3][3]={{1,2,1},
    {0,0,0},
    {-1,-2,-1}};
  int heightm1=height-1;
  int widthm1=width-1;
  int mr,mc;
  int newValue;
  
  int *pImgSobelX,*pImgSobelY;
  int min,max,where;
  float constVal1,constVal2;
  
  //정수값을 갖는 이미지 동적 메모리 할당
  pImgSobelX=new int[height*width];
  pImgSobelY=new int[height*width];
  
  //결과 이미지 0으로 초기화
  for(i=0;i<height;i++)
    for(j=0;j<width;j++)
    {
      OrgImg[i][j]=0;
      where=i*width+j;
      pImgSobelX[where]=0;
      pImgSobelY[where]=0;
    }
  
  //X 방향 에지 강도 계산
  for(i=1; i<heightm1; i++)
  {
    for(j=1; j<widthm1; j++)
    {
      newValue=0; //0으로 초기화
      for(mr=0;mr<3;mr++)
        for(mc=0;mc<3;mc++)
          newValue += (MaskSobelX[mr][mc]*InImg[i+mr-1][j+mc-1]);
      pImgSobelX[i*width+j]=newValue;
    }
  }
  
  
  //Y 방향 에지 강도 계산
  for(i=1; i<heightm1; i++)
  {
    for(j=1; j<widthm1; j++)
    {
      newValue=0; //0으로 초기화
      for(mr=0;mr<3;mr++)
        for(mc=0;mc<3;mc++)
          newValue += (MaskSobelY[mr][mc]*InImg[i+mr-1][j+mc-1]);
      pImgSobelY[i*width+j]=newValue;
    }
  }
  
  
  //에지 강도 계산 절대값(X)+절대값(Y)후 pImgSobelX[]에 저장
  for(i=1;i<heightm1;i++)
    for(j=1;j<widthm1;j++)
    {
      where=i*width+j;
      constVal1=pImgSobelX[where];
      constVal2=pImgSobelY[where];
      if(constVal1<0)
        constVal1=-constVal1;
      if(constVal2<0)
        constVal2=-constVal2;
      pImgSobelX[where]=constVal1+constVal2;
    }
  
  
  //디스플레이를 위해 0에서 255사이로 값의 범위를 매핑
  //이를 위해 먼저 최대,최소값을 찾은후 이를 이용하여 매핑한다.
  min=(int)10e10;
  max=(int)-10e10;
  for(i=1; i<heightm1; i++)
  {
    for(j=1; j<widthm1; j++)
    {
      newValue=pImgSobelX[i*width+j];
      if(newValue<min)
        min=newValue;
      if(newValue>max)
        max=newValue;
    }
  }
  
  //변환시 상수값을 미리 계산
  constVal1=(float)(255.0/(max-min));
  constVal2=(float)(-255.0*min/(max-min));
  for(i=1; i<heightm1; i++)
  {
    for(j=1; j<widthm1; j++)
    {
      
      //[min,max]사이의 값을 [0,255]값으로 변환
      newValue=pImgSobelX[i*width+j];
      newValue=constVal1*newValue+constVal2;
      OrgImg[i][j]=(BYTE)newValue;
    }
  }
  
  //동적 할당 메모리 해제
  delete [] pImgSobelX;
  delete [] pImgSobelY;
  
  
  
  FILE *outfile = fopen("result.raw","wb");
  fwrite(OrgImg,sizeof(char),256*256,outfile);
  fclose(outfile);
  
  
  return 0;
}
