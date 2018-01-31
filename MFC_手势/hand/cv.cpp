
#include "stdafx.h"
#include "cv.h"

#include <cv.h>
#include <highgui.h>

//#include <windows.h>
//#include <WinAble.h>  

#include <vector>

using namespace std;

int pmsf_value = 5;//��ֵƮ�ݷָ�ƽ��ϵ��
int MopEx_value = 2;//������
int Hmatch_value = 25;//ģ��ƥ��ϵ��

//����
int V_low = 30;
int V_high = 250;
//���Ͷ�
int S_low = 40;
int S_high = 170;
//ɫ��
int H_low_max = 40;
int H_high_min = 100;
int if_high_light = 1; //�Ƿ�߹ⲹ��

IplImage *src = 0;
IplImage *srcResize = 0;

IplImage *img_YCrCb = 0;
CvSize newSize;
CvSize sz;

IplImage *tmp1; 
IplImage *tmp2; 
IplImage *tmp3; 
IplImage *src2; 
IplImage *src1; 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IplImage *YCrCb;
IplImage *YCrCb_mask;
IplImage *Y_channel,*Cr_channel, *Cb_channel;
IplImage *Y_cmp,*Cr_cmp, *Cb_cmp;

CvScalar Y_lower;
CvScalar Y_upper;

CvScalar Cr_lower;
CvScalar Cr_upper;

CvScalar Cb_lower;
CvScalar Cb_upper;

CvScalar YCrCb_lower;
CvScalar YCrCb_upper;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

void init_hand_YCrCb()
{
	//
	img_YCrCb = cvCreateImage( sz, 8, 3);
	YCrCb_mask = cvCreateImage(sz,IPL_DEPTH_8U,1);;

	//���յ�ͼƬ
	YCrCb = cvCreateImage(sz,IPL_DEPTH_8U,3);

	
	//��ͨ��
	Y_channel = cvCreateImage(sz,IPL_DEPTH_8U,1);
	Cr_channel = cvCreateImage(sz,IPL_DEPTH_8U,1);
	Cb_channel = cvCreateImage(sz,IPL_DEPTH_8U,1); 	
	
	//����Χ��ȡ��
	Y_cmp = cvCreateImage(sz,IPL_DEPTH_8U,1);
	Cr_cmp = cvCreateImage(sz,IPL_DEPTH_8U,1);
	Cb_cmp = cvCreateImage(sz,IPL_DEPTH_8U,1);

	//Y,Cr,Cb����ɫ��Χ
	Y_lower = CV_RGB(0,0,130);
	Y_upper = CV_RGB(0,0,130);

	Cr_lower = CV_RGB(0,0,125);
	Cr_upper = CV_RGB(0,0,125);

	Cb_lower = CV_RGB(0,0,132);
	Cb_upper = CV_RGB(0,0,147);

	YCrCb_lower = cvScalar(0,0,132,0);
	YCrCb_upper = cvScalar(130,125,147,0);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hand_YCrCb()
{
	//ת����YCrBr
	cvCvtColor(src2,img_YCrCb, CV_RGB2YCrCb);
	

	//�ָY,Cr,Cb
	cvSplit(img_YCrCb,Y_channel,Cr_channel,Cb_channel,0);

	//��Y_channel��λ�� Y_lower �� Y_upper ֮���Ԫ�ظ��Ƶ� Y_tmp��
	cvInRangeS( Y_channel, Y_lower, Y_upper, Y_cmp); 
	cvInRangeS(Cr_channel,Cr_lower,Cr_upper ,Cr_cmp);
	cvInRangeS(Cb_channel,Cb_lower,Cb_upper ,Cb_cmp);

	//�ϲ�Y,Cr,Cbͨ����YCrCb��
	cvMerge(Y_cmp,Cr_cmp,Cb_cmp,0,YCrCb);

	//��ʾ���
	cvShowImage("YCrCb_mask",YCrCb);


	//cvInRangeS (img_YCrCb, YCrCb_lower, YCrCb_upper, YCrCb_mask);
	//cvShowImage( "YCrCb_mask", YCrCb_mask);
	

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	IplImage* hsv_image;
	IplImage* hsv_mask;
	CvScalar  hsv_min;
	CvScalar  hsv_max;

	IplImage *H_img,*S_img, *V_img;
	IplImage *H_mask, *H_mask1,*S_mask, *S_mask1, *V_mask, *V_mask1, *V_mask2;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void init_hand_HSV()
{

	hsv_image = cvCreateImage( sz, 8, 3);
	hsv_mask = cvCreateImage( sz, 8, 1);
	hsv_min = cvScalar(0, 20, 20, 0);
	hsv_max = cvScalar(20, 250, 255, 0);
	//hsv_mask->origin = 1;

	//����2: �����������ͨ��
	H_img = cvCreateImage(sz,IPL_DEPTH_8U,1);
	S_img = cvCreateImage(sz,IPL_DEPTH_8U,1);
	V_img = cvCreateImage(sz,IPL_DEPTH_8U,1);

	H_mask = cvCreateImage(sz,IPL_DEPTH_8U,1);
	H_mask1 = cvCreateImage(sz,IPL_DEPTH_8U,1);
	S_mask = cvCreateImage(sz,IPL_DEPTH_8U,1);
	S_mask1 = cvCreateImage(sz,IPL_DEPTH_8U,1);
	V_mask = cvCreateImage(sz,IPL_DEPTH_8U,1);
	V_mask2 = cvCreateImage(sz,IPL_DEPTH_8U,1);
	V_mask1 = cvCreateImage(sz,IPL_DEPTH_8U,1);

	

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
void color_blance()
{
	CvScalar avg = cvAvg(H_img, 0);
	printf("%f, %f, %f, %f\n", avg.val[0],avg.val[1],avg.val[2],avg.val[3]);
	double d = 128 - avg.val[0];
	avg.val[0] = d;
	cvAddS(H_img, avg, H_img, 0);


}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hand_HSV()
{

		cvCvtColor(src2, hsv_image, CV_BGR2HSV);
		//cvInRangeS (hsv_image, hsv_min, hsv_max, hsv_mask);
		//cvShowImage( "hsv_msk", hsv_mask);
		
		
		

		//����2: �����������ͨ��
		cvSplit(hsv_image,H_img,S_img,V_img,0);

		//color_blance();
		//cvMerge(H_img,S_img,V_img,0,hsv_image);
		//cvShowImage( "ɫ��ƽ���", hsv_image);
		//cvShowImage( "Hͨ��", H_img);

		

		

		//ֱ��ͼ���⻯(Ч������)
		//cvEqualizeHist(H_img, H_img);
		
		//cvShowImage( "Hͨ��_���⻯", H_img);

		//����Ӧ
		//cvAdaptiveThreshold(H_img, H_mask, 30, 0, 0, 3, 5);
		
		//cvShowImage( "H", H_img);
		//cvShowImage( "S", S_img);
		//cvShowImage( "V", V_img);

		//ɫ��
		cvInRangeS(H_img,cvScalar(0,0,0,0),cvScalar(H_low_max,0,0,0),H_mask);//��ɫ��
		cvInRangeS(H_img,cvScalar(256 - H_high_min,0,0,0),cvScalar(256,0,0,0),H_mask1);//��ɫ��

		//���Ͷ�
		cvInRangeS(S_img,cvScalar(S_low,0,0,0),cvScalar(S_high,0,0,0),S_mask); //�м���
		//cvInRangeS(S_img,cvScalar(20,0,0,0),cvScalar(100,0,0,0),S_mask1); //�ͱ��Ͷ�



		//����
		cvInRangeS(V_img,cvScalar(V_high,0,0,0),cvScalar(256,0,0,0),V_mask);//������
		cvInRangeS(V_img,cvScalar(V_low,0,0,0),cvScalar(V_high,0,0,0),V_mask1); //�м���
		//cvInRangeS(V_img,cvScalar(150,0,0,0),cvScalar(250,0,0,0),V_mask2); //������
		
		
		//���, �����ϵĻ��
		cvOr(H_mask1, H_mask, H_mask, 0);

		//�������Ͷȹ�������
		cvAnd(H_mask,S_mask,H_mask,0);

		//cvShowImage( "���Ͷȹ���", H_mask);

		//��ȥ������������
		cvAnd(H_mask,V_mask1,H_mask,0);

		//cvShowImage( "���ȹ���", H_mask);

		//cvShowImage( "hsv_msk", H_mask);

		

		//������������
		if(if_high_light){cvOr(H_mask,V_mask,H_mask,0);}

		//cvShowImage( "�����߹�", H_mask);

		//cvShowImage( "�ع���� V", V_mask);
		//cvShowImage( "�عⲹ��", S_mask);
		
		
		//�Ƿ񲹳��ع����
		hsv_mask = H_mask;

		//cvShowImage( "hsv_msk", H_mask);
		
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////


//��ֵ��
IplImage* thd_src;
IplImage* thd_dst1;
IplImage* thd_dst2;
int thd_max = 255;
int thd_val = 100;

void inti_threshold()
{
	thd_src = cvCreateImage(sz,IPL_DEPTH_8U,1);
	thd_dst1 = cvCreateImage(sz,IPL_DEPTH_8U,1);
	thd_dst2 = cvCreateImage(sz,IPL_DEPTH_8U,1);
}

void threshold()
{
	cvCvtColor(src1, thd_src, CV_RGB2GRAY);
	cvAdaptiveThreshold(thd_src, thd_dst1, thd_max, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 3, 5);
	cvThreshold(thd_src, thd_dst2, thd_val, thd_max, CV_THRESH_BINARY);
	

	cvShowImage( "��ֵǰ", thd_src);

	cvShowImage( "��ֵ��1", thd_dst1);
	cvCreateTrackbar( "thd_max", "��ֵ��1", &thd_max, 256, 0);
	cvShowImage( "��ֵ��2", thd_dst2);
	cvCreateTrackbar( "thd_val", "��ֵ��2", &thd_val, 256, 0);

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void resizeSrc()
{
	double scale = 0.5;	

	
	//���ͼ���С
	sz = cvGetSize(src);
	newSize.height = (int)(sz.height * scale);
	newSize.width = (int)(sz.width * scale);

	src = cvCreateImage(newSize,IPL_DEPTH_8U,3);

	cvResize(src, src, CV_INTER_LINEAR);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

IplImage* smooth1;
IplImage* smooth2;
IplImage* smooth3;
IplImage* smooth4;
IplImage* smooth5;

void reduce_noise()
{
	//cvSmooth(hsv_mask, smooth1, CV_GAUSSIAN, 3, 0, 0, 0);
	//cvSmooth(hsv_mask, smooth1, CV_MEDIAN, 3, 0, 0, 0);
	
	//cvSmooth(smooth1, smooth1, CV_BILATERAL, 3, 0, 0, 0);

	//cvDilate(hsv_mask, smooth1, 0 ,2);
	//cvErode(smooth1 ,smooth2,   0, 2);
	
	cvMorphologyEx(hsv_mask, smooth1, 0, CV_SHAPE_RECT, CV_MOP_CLOSE, MopEx_value);
	//cvMorphologyEx(smooth1, smooth2, 0, CV_SHAPE_RECT, CV_MOP_OPEN, 1);

	cvShowImage( "���Ÿ�ʴ", smooth1);

	//cvSmooth(smooth2, smooth3, CV_MEDIAN, 9, 0, 0, 0);

	//cvShowImage( "ƽ��", smooth3);
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//����ƥ��

IplImage*    g_image    = NULL;
IplImage*    g_gray    = NULL;
int        g_thresh  = 100;
CvMemStorage*  g_storage  = NULL;
CvMemStorage*  g_storage1  = NULL;
CvMemStorage*  g_storage2  = NULL;

CvSeq* seqMidObj = 0;//��ѡ�����������
int handNum = 0;


///////////////////////////////////////////////////////////////////////////

void hand_contours(IplImage* dst) {

  if( g_storage==NULL ) {
    g_storage = cvCreateMemStorage(0);
	g_storage1 = cvCreateMemStorage(0);
	g_storage2 = cvCreateMemStorage(0);
  } else {
    cvClearMemStorage( g_storage );
	cvClearMemStorage( g_storage1 );
	cvClearMemStorage( g_storage2 );
  }

	
  int i = 0, j=0;
  CvSeq* contours = 0;
  CvSeq* contoursHead = 0;
  CvSeq* p = NULL;
  CvSeq* q = NULL;


  seqMidObj = 0;
  handNum = 0;

  //cvThreshold( g_gray, g_gray, g_thresh, 255, CV_THRESH_BINARY );

  cvFindContours( dst, g_storage, &contours, sizeof(CvContour), CV_RETR_EXTERNAL);//ֻ����������
  contoursHead = contours;//contours��ͷ
  //cvZero( g_gray );

	

  /*CvSeq* seqApprox;
  
  CvSeq* head;
  int j=0;*/
  /*for(p=contours;p != NULL; p = p->h_next){
	
	j++;
	  
	  seqApprox = cvApproxPoly(p, sizeof(CvContour),g_storage1, CV_POLY_APPROX_DP, 2, 0);
	  seqApprox = seqApprox->h_next;
	  if(p==contours) head  = seqApprox;
  
  }
  printf("total = %d\n ",j );
  seqApprox = head;*/


  cvZero( tmp3 );
  //cvZero( tmp2 );
  
  if( contours )cvDrawContours( tmp3, contours, cvScalarAll(255),cvScalar(255,0,0,0),1);//��������
  //if( contours )cvDrawContours( tmp2, contours, cvScalar(255,0,0,0),cvScalar(255,100,0,0),1);//��������

  //cvShowImage( "��������", tmp3);

  //CvSeq* seqMidObj = 0;// = cvCreateSeq(CV_SEQ_ELTYPE_CODE, sizeof(CvSeq), sizeof(int),g_storage2);
  
 
	//ȥ���봰���ڽӵ�����
	contours = contoursHead;i=0;
	CvRect bound;
	int dat = 2;

	 //ȥ��С�������
	int contArea = 0;
	int imgArea = newSize.height * newSize.width;
 
	for( ; contours != 0; contours = contours->h_next ){
		
		i++;
		
		//��������С, ���ų�
		contArea = fabs(cvContourArea( contours, CV_WHOLE_SEQ ));
		
		if ( (double)contArea/imgArea < 0.015 ){continue;}
		
		//����߽��봰������, ���ų�
		bound = cvBoundingRect(contours, 0);

		if( bound.x < dat 
		 || bound.y < dat
		 || bound.x + bound.width  + dat > newSize.width
		 || bound.y + bound.height + dat > newSize.height)
		{ 	
			//printf(" %d, %d, %d, %d\n",bound.x, bound.y, bound.width, bound.height );
			//cvRectangle(tmp3, cvPoint(bound.x, bound.y),cvPoint(bound.x + bound.width, bound.y + bound.height),cvScalar(255,255,255,255),1,8,0);
			continue; 
		}

		//������������
		q = p;
		//p = cvCloneSeq(contours, g_storage2);
		p = contours;

		if(q == NULL){
			seqMidObj = p;
			//p->h_next = NULL;
			//p->h_prev = NULL;
			//printf("��1��!");
		}else{
			q->h_next = p;
			p->h_prev = q;
			//printf("1��!");
		}
		//j++;
		handNum++;

	}

	//printf("�ҵ�����: %d ��   ��ѡ: %d ��\n", i,j);

	

	if(seqMidObj){
		seqMidObj->h_prev = NULL;
		p->h_next = NULL;
	}
	if(handNum>0) printf("�ҵ���: %d  ",handNum);
	//CvSeq* seqMidObj_head = seqMidObj;
	



	cvZero( tmp3 );
  if( seqMidObj )cvDrawContours( tmp3, seqMidObj, cvScalarAll(255),cvScalar(255,0,0,0),1);//��������

	//cvShowImage( "������ѡ", tmp3);


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

IplImage*    tmp_img    = 0;
CvMemStorage*  storage_tmp  = 0;

CvSeq* handT = 0;
CvSeq* handT1 = 0;
CvSeq* handT2 = 0;


int handTNum = 10;//10��ģ��

char *tmp_names[] = {"1.bmp","2.bmp","3.bmp","4.bmp","5.bmp","6.bmp","7.bmp","8.bmp","9.bmp","10.bmp"};
char *num_c[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};



/////////////////////////////////////////////////////

//����ģ�������
void init_hand_template()
{

    storage_tmp = cvCreateMemStorage(0);

	int i = 0;
	for(i=0; i<handTNum; i++){

		tmp_img = cvLoadImage(tmp_names[i], CV_LOAD_IMAGE_GRAYSCALE);
		if(!tmp_img){
			printf("δ�ҵ��ļ�: %s\n", tmp_names[i]);
			continue;
		}
		//cvShowImage("����ģ��", tmp_img);
		handT1 = handT2;
		cvFindContours( tmp_img, storage_tmp, &handT2, sizeof(CvContour), CV_RETR_EXTERNAL);

		if(handT2){
			printf("����ģ��: %s �ɹ�!\n",tmp_names[i]);
			if(handT1 == NULL){
				printf("�����һ��ģ��!\n");
				handT = handT2;
			}else{
				handT2->h_prev = handT1;
				handT1->h_next = handT2;
			}
			
		}
		
	}


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool if_match_num = false; 
int match_num = -1;

//ģ��ƥ����
void hand_template_match(CvSeq* handT, CvSeq* hand){
	int i=0;
	int kind = -1;
	double hu = 1;
	
	double hutmp;
	CvSeq* handp = handT;
	int method = CV_CONTOURS_MATCH_I1;
	
	match_num = 0;

	if(handT==NULL){return;printf("handT==NULL!\n");}
	if(hand==NULL){return;printf("hand==NULL!\n");}

	for(i=0; i<handTNum; i++){
		hutmp = cvMatchShapes(handp, hand, method, 0);
		handp = handp->h_next;
		
		//�ҵ�hu����С��ģ��
		if(hu > hutmp){
			hu = hutmp;
			kind = i+1;
		}
		
		//printf("%f ", hu);
	}
	
	//��ʾƥ����
	if(hu<((double)Hmatch_value)/100){
		printf("ƥ��ģ��: %d (%f)", kind, hu);
		match_num = kind;
		if_match_num = true;
	}else{
		if_match_num = false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

//������˹�任
int sigma = 2;
int smoothType = CV_GAUSSIAN;

    IplImage* laplace = 0;
    IplImage* colorlaplace = 0;
    IplImage* planes[3] = { 0, 0, 0 };

	int ls_low = 247;


void init_laplace()
{
	int i;
    for( i = 0; i < 3; i++ )
        planes[i] = cvCreateImage(newSize, 8, 1 );
    laplace = cvCreateImage( newSize, IPL_DEPTH_16S, 1 );
    colorlaplace = cvCreateImage( newSize, 8, 3 );

}


void toLaplace(IplImage* dst)
{
	
        int i, c, ksize;

        ksize = (sigma*5)|1;
        //cvSmooth( dst, colorlaplace, smoothType, ksize, ksize, sigma, sigma );
		
        cvSplit( dst, planes[0], planes[1], planes[2], 0 );
        for( i = 0; i < 3; i++ )
        {
            cvLaplace( planes[i], laplace, 5 );
            cvConvertScaleAbs( laplace, planes[i], (sigma+1)*0.25, 0 );
        }
        cvMerge( planes[0], planes[1], planes[2], 0, colorlaplace );
        colorlaplace->origin = dst->origin;

		//ǰ��ƽ��
		//cvMorphologyEx(planes[2], planes[2], 0, CV_SHAPE_RECT, CV_MOP_CLOSE, 1);

        cvShowImage("������˹�任", colorlaplace );

            //smoothType = smoothType == CV_GAUSSIAN ? CV_BLUR : smoothType == CV_BLUR ? CV_MEDIAN : CV_GAUSSIAN;
		
		


		cvCvtColor(colorlaplace, colorlaplace, CV_BGR2HSV);


		cvSplit(colorlaplace, planes[0], planes[1], planes[2], 0);
		
		cvInRangeS (planes[2], cvScalar(ls_low,0,0,0), cvScalar(256,0,0,0), planes[2]);

		//����ƽ��
		//cvMorphologyEx(planes[2], planes[2], 0, CV_SHAPE_RECT, CV_MOP_CLOSE, 1);
		//cvSmooth(planes[2], planes[2], CV_MEDIAN, 3, 0, 0, 0);
		//cvErode(planes[2] ,planes[2], 0, 1);

		cvShowImage("������˹_��Ե��", planes[2]);



}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define UP		-1
#define DOWN	1
#define LEFT	1
#define RIGHT	-1


CvPoint hand_center  = cvPoint(0,0); //����Ӿ��������ĵ�
CvPoint hand_center_last = cvPoint(0,0); //
CvPoint hand_direct_now = cvPoint(0,0);  //���μ�⵽���ƶ�����
CvPoint hand_direct_last = cvPoint(0,0); //�ϴμ�⵽���ƶ�����
CvPoint hand_direct = cvPoint(0,0);      //�ϳɵķ���

//���ֵ��е���ƶ�����
void hand_direction(CvSeq* hand){
	
	//���û����, ��������
	if(!hand){
		 hand_center  = cvPoint(0,0); 
		 hand_center_last = cvPoint(0,0);
		 hand_direct_now = cvPoint(0,0);
		 hand_direct_last = cvPoint(0,0);
		 hand_direct = cvPoint(0,0); 
		return;
	}
	
	hand_center_last = hand_center;

	//������ĵ�
	CvRect bound = cvBoundingRect(hand, 0);
	hand_center.x = bound.x + bound.width/2;
	hand_center.y = bound.y + bound.height/2;

	if(hand_center_last.x!=0){
		hand_direct_now.x = hand_center.x - hand_center_last.x;
		hand_direct_now.y = hand_center.y - hand_center_last.y;
		
		if(hand_direct_now.x!=0){
			
			hand_direct.x = (hand_direct_now.x + hand_direct_last.x)/2;
			if(hand_direct.x!=0){
				printf("  X �ƶ�: %d ",hand_direct.x);
			}
			
		}
		if(hand_direct_now.y!=0){

			hand_direct.y = (hand_direct_now.y + hand_direct_last.y)/2;
			if(hand_direct.y!=0){
				printf("  Y �ƶ�: %d ",hand_direct.y);
			}
		}
	
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//����ʶ����
void hand_draw(IplImage* dst,CvSeq* hands )
{

	if(!hands) return;

	CvRect bound;

    //͹��
	int i, hullcount;
    CvPoint pt0;
    CvSeq* hull;

	CvSeq* handp = hands;

	//͹��ȱ��
	CvConvexityDefect* defect;
	CvSeq* hullDefect;
	//CvSeq* hullDefectSelect;
	int hullDefectNum=0;
	//cvPoint** points = (cvPoint*)malloc(sizeof(cvPoint)*3);
	//cvPoint points[3];


	//��������
	cvDrawContours( dst, handp, cvScalar(255,150,100,0),cvScalar(255,0,0,0),1, 1, 8,cvPoint(0,0));
	
	CvFont font;


	//���Ƽ�⵽������
	if(if_match_num){
		
		//����Ӱ
		cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX,1.0f,1.0f,0,5,8);
		cvPutText(dst, num_c[match_num-1], cvPoint(5, 30), &font, CV_RGB(255,255,255));		
		//����ɫ
		cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX,1.0f,1.0f,0,2,8);
		cvPutText(dst, num_c[match_num-1], cvPoint(5, 30), &font, CV_RGB(255,0,0));


	}
	
	//�����ƶ�����
	if(1){
		
		//cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX,1.0f,1.0f,0,2,8);
		//cvPutText(dst, "X: ", cvPoint(5, 30), &font, CV_RGB(255,0,0));

		

	}


	//����ȡ������������
	for( ; handp != 0; handp = handp->h_next ){
		
		bound = cvBoundingRect(handp, 0);


		//�󲢻������ĵ�

		//����ɫ
		int det = 2;
		cvRectangle(dst, 
			cvPoint(hand_center.x-det, hand_center.y-det),
			cvPoint(hand_center.x+det, hand_center.y+det), 
			CV_RGB(0,0,0),3,8,0);

		//������
		det = 3;
		cvRectangle(dst, 
			cvPoint(hand_center.x-det, hand_center.y-det),
			cvPoint(hand_center.x+det, hand_center.y+det), 
			CV_RGB(255,255,255),1,8,0);



		//�������緽��
		cvRectangle(dst, 
			cvPoint(bound.x, bound.y),
			cvPoint(bound.x + bound.width, bound.y + bound.height), 
			cvScalar(0,0,255,0),2,8,0);

		//Ѱ��͹��
        hull = cvConvexHull2( handp, 0, CV_CLOCKWISE, 0 );
        hullcount = hull->total;
		//printf("͹������: %d  ",hullcount);

		 pt0 = **CV_GET_SEQ_ELEM( CvPoint*, hull, hullcount - 1 );

		//��͹��
		for( i = 0; i < hullcount; i++ ){

			//�õ�͹���ĵ�
            CvPoint pt = **CV_GET_SEQ_ELEM( CvPoint*, hull, i );
            cvLine( dst, pt0, pt, CV_RGB( 0, 255, 0 ), 1, CV_AA, 0 );
            pt0 = pt;
        }

		//���ȱ��
		/*if(!cvCheckContourConvexity(hands)){

			hullDefect = cvConvexityDefects(hands, hull, 0);
			hullDefectNum = hullDefect->total;
			printf("ȱ�ݸ���: %d  ",hullDefectNum);
			for( i = 0; i < hullDefectNum; i++ ){

				defect = (CvConvexityDefect*)cvGetSeqElem(hullDefect, i);

				cvLine( dst, cvPoint(defect->end->x,defect->end->y),    cvPoint(defect->depth_point->x,defect->depth_point->y), CV_RGB(150,150,150),1,8,0);
				cvLine( dst, cvPoint(defect->start->x,defect->start->y),cvPoint(defect->depth_point->x,defect->depth_point->y), CV_RGB(150,150,150),1,8,0);
		

			}

		}*/

	}

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void resizeAllWindow()
{
	cvNamedWindow("src",0);

	cvNamedWindow( "���Ÿ�ʴ",0);

	cvNamedWindow( "����ʶ��",0);




	cvResizeWindow("src",newSize.width,newSize.height);

	
	cvNamedWindow( "��������",0);
	cvResizeWindow("��������",newSize.width*1.5,60*11);


	cvCreateTrackbar( "��ֵƯ���˲�", "��������", &pmsf_value, 20, 0 );
	cvCreateTrackbar( "�����㽵��", "��������", &MopEx_value, 5, 0 );

	cvCreateTrackbar( "ɫ������", "��������", &H_low_max, 150, 0 );
	cvCreateTrackbar( "ɫ��������", "��������", &H_high_min, 150, 0 );

	cvCreateTrackbar( "��������", "��������", &V_low, 100, 0 );
	cvCreateTrackbar( "��������", "��������", &V_high, 256, 0 );
	
	cvCreateTrackbar( "���Ͷ�����", "��������", &S_low, 100, 0 );
	cvCreateTrackbar( "���Ͷ�����", "��������", &S_high, 255, 0 );

	cvCreateTrackbar( "�߹ⲹ��", "��������", &if_high_light, 1, 0 );

	cvCreateTrackbar( "matchϵ��", "��������", &Hmatch_value, 50, 0 );


}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//��δ���ת�Ƶ� handDlg.cpp
//
////�������
//void control_mouse()
//{
//	CPoint point;
//	GetCursorPos(&point);
//
//	int scale = 10;
//	
//	point.x += hand_direct.x * scale;
//	point.y += hand_direct.y * scale;
//		
//	SetCursorPos( point.x, point.y);
//
//
//}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main1()

{

	int c = 0;

	double scale = 0.5;	

	//������ͷ
    CvCapture* capture = cvCaptureFromCAM(0);
	src = cvQueryFrame(capture);
	
	//������ͷʧ��
	if(!src){return NO_CAMERA_BUG;}
	
	//���ͼ���С
	sz = cvGetSize(src);
	newSize.height = (int)(sz.height * scale);
	newSize.width = (int)(sz.width * scale);

	sz = newSize;

	
	
	
	
	//���µ���ͼ���С
	//resizeSrc();

	//�������д���
	resizeAllWindow();
	
	tmp1 = cvCreateImage(sz,IPL_DEPTH_8U,1);

	tmp2 = cvCreateImage(sz,IPL_DEPTH_8U,3);

	tmp3 = cvCreateImage(sz,IPL_DEPTH_8U,3);

	src2 = cvCreateImage(sz,IPL_DEPTH_8U,3);

	smooth1 = cvCreateImage(sz,IPL_DEPTH_8U,1);
	smooth2 = cvCreateImage(sz,IPL_DEPTH_8U,1);
	smooth3 = cvCreateImage(sz,IPL_DEPTH_8U,1);
	smooth4 = cvCreateImage(sz,IPL_DEPTH_8U,1);
	smooth5 = cvCreateImage(sz,IPL_DEPTH_8U,1);

	init_hand_YCrCb();
	init_hand_HSV();
	init_laplace();
	inti_threshold();

	//����ƥ���ģ��
	init_hand_template();
	
	src1 = cvCreateImage(sz,IPL_DEPTH_8U,3);

	/////////////////////��ʼѭ��///////////////////////////////
	while( c != 27)

	{

		//��ǰͼ��
		src = cvQueryFrame(capture);
		if(!src) return NO_CAMERA_BUG;

		//��СҪ�����ͼ��(��С������)
		cvResize(src, src1, CV_INTER_LINEAR);

		cvShowImage("src",src1);	

		

		cvPyrMeanShiftFiltering(src1, src2, pmsf_value, 40, 2);
		
		//cvSmooth(src1, tmp4, CV_MEDIAN, 9, 0, 0, 0);
		
		//cvShowImage( "��ֵƯ�Ʒָ�", tmp4);

		//������˹�任
		//toLaplace(src2);


		

		//hand_YCrCb();

		//hsvɫ����ȡ
		hand_HSV();
		
		//��ֵ��
		//threshold();

		//����
		reduce_noise();

		//Ѱ��������
		hand_contours(smooth1);
		//hand_contours(planes[2]);
		//hand_contours(tmp2);
		
		//����(δʹ��)
		//handProcess(seqMidObj);

		//Ѱ��ƥ��
		hand_template_match(handT, seqMidObj);

		//���ֵ��ƶ�����
		hand_direction(seqMidObj);

		//���ƶ�����Ϣ
		cvZero( tmp2 );
		hand_draw(tmp2, seqMidObj); //�����ڼ�ⴰ��
		hand_draw(src1, seqMidObj); //������ԭ����
	
	
		//cvShowImage( "���Ÿ�ʴ", smooth1);
		cvShowImage( "����ʶ��", tmp2 );
		cvShowImage("src",src1);

		
		//�������(��ȭͷ״, ���������ƶ�)
		if(match_num == 10) control_mouse(hand_direct.x, hand_direct.y);

		printf("\n");
		c = cvWaitKey( 10);

	}
	////////////////////////////////////////////////////////////

	cvReleaseCapture( &capture);
	cvDestroyAllWindows();

	return 0;


}