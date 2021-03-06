
// MFC_HW1Dlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "iostream"
#include "stdlib.h"
#include "math.h"
#include "windows.h"
#include "MFC_HW1.h"
#include "MFC_HW1Dlg.h"
#include "afxdialogex.h"
#include "cv.h"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc.hpp"
# define M_PI 3.1415926
using namespace std;
using namespace cv;

int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;
int x_ = 0;
int y_ = 0;
int click_times = 0;

Point2i top_left;
Point2i bottom_left;
Point2i bottom_right;
Point2i top_right;
Point2f tl_after = Point2f(20, 20);
Point2f bl_after = Point2f(20, 450);
Point2f br_after = Point2f(450, 450);
Point2f tr_after = Point2f(450, 20);

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
static IplImage* img1_1;
static IplImage* img1_2; //img的翻轉
static IplImage* img1_3; //destination of img and img2 的疊加

// 對 App About 使用 CAboutDlg 對話方塊

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCHW1Dlg 對話方塊



CMFCHW1Dlg::CMFCHW1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFC_HW1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCHW1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCHW1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCHW1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCHW1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCHW1Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCHW1Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCHW1Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCHW1Dlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CMFCHW1Dlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CMFCHW1Dlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CMFCHW1Dlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CMFCHW1Dlg::OnBnClickedButton10)
END_MESSAGE_MAP()


// CMFCHW1Dlg 訊息處理常式

BOOL CMFCHW1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	AllocConsole();
	freopen("CONOUT$", "w", stdout);


	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

void CMFCHW1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CMFCHW1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CMFCHW1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCHW1Dlg::OnBnClickedButton1()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	IplImage* img = cvLoadImage("dog.bmp");
	cvNamedWindow("1.1 Load Image File ", CV_WINDOW_AUTOSIZE);
	cvShowImage("1.1 Load Image File ", img);
	printf("Height = %d \n", img->height);
	printf("Width = %d \n", img->width);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("1.1 Load Image File ");
	
}

void CMFCHW1Dlg::OnBnClickedButton2()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	IplImage* img = cvLoadImage("color.png");
	cvNamedWindow("1.2 Color Conversion after ", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("1.2 Color Conversion before ", CV_WINDOW_AUTOSIZE);
	cvShowImage("1.2 Color Conversion before ", img);
	//printf("%d\n", img->nChannels);
	//printf("%c\n%c\n%c\n", img->channelSeq[0], img->channelSeq[1], img->channelSeq[2]);

	CvScalar s;
	for (int y = 0; y <img->height; y++) {
		for (int x = 0; x < img->width; x++) {
			s = cvGet2D(img,y, x);	//拿出img的一個pixel, s.val[0],s.val[1],s.val[2]就是BGR的值
			

			
				

			double temp;
			temp = s.val[0];
			s.val[0] = s.val[1];
			s.val[1] = s.val[2];
			s.val[2] = temp;
			
		/*
			if (y ==55) {
				printf("%f\n%f\n%f\n", s.val[0], s.val[1], s.val[2]);  //CvSvcalar的val, type是double
				cvWaitKey(0);
			}
		*/	
			cvSet2D(img, y, x, s);
				
		}	
	}

		
	cvSaveImage("color_changed.png", img);
	cvWaitKey(0);
	cvShowImage("1.2 Color Conversion after ", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("1.2 Color Conversion before ");
	cvDestroyWindow("1.2 Color Conversion after ");
}

void CMFCHW1Dlg::OnBnClickedButton3()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	IplImage* img = cvLoadImage("dog.bmp");
	IplImage* img2;
	CvSize img2size = CvSize(img->width, img->height);
	img2 = cvCreateImage(img2size, img->depth, img->nChannels);//創出一個跟img一樣大小的空Image
	cvNamedWindow("1.3 Flipping after ", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("1.3 Flipping before ", CV_WINDOW_AUTOSIZE);
	cvShowImage("1.3 Flipping before ", img);

	cvFlip(img, img2, 1);

	cvSaveImage("dog2.bmp", img2);
	cvWaitKey(0);
	cvShowImage("1.3 Flipping after ", img2);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvReleaseImage(&img2);
	cvDestroyWindow("1.3 Flipping before ");
	cvDestroyWindow("1.3 Flipping after ");


	
}

void CMFCHW1Dlg::OnBnClickedButton4()
{
	// TODO: 在此加入控制項告知處理常式程式碼


	cvNamedWindow("1.4 Blending ", CV_WINDOW_AUTOSIZE);
	
	int slidervalue = 0;
	int slidermax = 100;
	
	createTrackbar("Blend", "1.4 Blending ", &slidervalue, slidermax, trackbar);
	trackbar(slidervalue,0); //TrackBar CallBack function,OpenCV定義成void function_name(int,void*)
	cvWaitKey(0);
	cvReleaseImage(&img1_3);
	cvReleaseImage(&img1_1);
	cvReleaseImage(&img1_2);
	
	cvDestroyWindow("1.4 Blending ");
}

void CMFCHW1Dlg::trackbar(int slidervalue, void* userdata) {
	img1_1 = cvLoadImage("dog.bmp");


	CvSize img2size = CvSize(img1_1->width, img1_1->height);
	img1_2 = cvCreateImage(img2size, img1_1->depth, img1_1->nChannels);
	cvFlip(img1_1, img1_2, 1); //得到翻轉的dog.bmp 存在img1_2


	CvSize img3size = CvSize(img1_1->width, img1_1->height);
	img1_3 = cvCreateImage(img2size, img1_1->depth, img1_1->nChannels);
	double alpha = 1.0 - (double)slidervalue / 100;
	cvAddWeighted(img1_1, alpha, img1_2, 1 - alpha, 0, img1_3); //設置img&img2疊加時 各自的權重

	cvShowImage("1.4 Blending ", img1_3);
	
}

void CMFCHW1Dlg::OnBnClickedButton5()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	IplImage* img = cvLoadImage("M8.jpg");
	cvNamedWindow("2.1 Vertical edges", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("2.1 Horizontal edges", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Magnitude ", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("Angle ", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("2. Edge Detection before ", CV_WINDOW_AUTOSIZE);
	cvShowImage("2. Edge Detection before ", img);
	IplImage* img_gray = cvCreateImage(cvGetSize(img),img->depth,1);
	
//	CvSize img2size = CvSize(img->width, img->height);
//	img2 = cvCreateImage(img2size, img->depth, img->nChannels);//創出一個跟img一樣大小的空Image
	

////////// RGB to Grayscale /////////////
	try
	{
		// your code here
		cvCvtColor(img, img_gray, CV_RGB2GRAY); 
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
	cvSaveImage("M8_gray.jpg", img_gray);

	
	cvWaitKey(0);

///////// Gaussian smooth //////////////
	Mat src; Mat dst;
	src = imread("M8_gray.jpg", IMREAD_GRAYSCALE);
	dst = src.clone();
	GaussianBlur( src,dst , Size(3, 3), 0, 0);
	IplImage img_smooth = IplImage(dst);
	cvSaveImage("M8_smooth.jpg", &img_smooth);

///////// Sobel Edge Detection /////////
	int sobel_x[3][3] = {  -1,0,1 , -2,0,2 , -1,0,1  };
	int sobel_y[3][3] = {  1,2,1 , 0,0,0 , -1,-2,-1  };
	Mat src_sobel = imread("M8_smooth.jpg", IMREAD_GRAYSCALE);
	Mat dst_padding;
	Mat gradient_x = src_sobel.clone();
	Mat gradient_y = src_sobel.clone();
	Mat gradient = src_sobel.clone();
	
	int threshold = 40;
	int slidervalue = 40;
	int slidermax = 255;
	int slidervalue2 = 10;
	int slidermax2 = 360;
	copyMakeBorder(src_sobel, dst_padding, 1, 1, 1, 1, BORDER_REFLECT_101);

	//////////// gradient_x //////////
	for (int row = 1; row < dst_padding.rows - 1; row++) {
		for (int col = 1; col < dst_padding.cols - 1; col++) {

			int sum = 0;

			for (int kernel_r = -1; kernel_r <= 1; kernel_r++) {
				for (int kernel_c = -1; kernel_c <= 1; kernel_c++) {

					sum += dst_padding.at<uchar>(row + kernel_r, col + kernel_c) * sobel_x[kernel_r + 1][kernel_c + 1];

				}
			}
			
			
			gradient_x.at<uchar>(row -1, col-1) = abs(sum);
			
			if (gradient_x.at<uchar>(row - 1, col - 1) < threshold)
				gradient_x.at<uchar>(row - 1, col - 1) = 0;
			if (gradient_y.at<uchar>(row - 1, col - 1) > 255)
				gradient_y.at<uchar>(row - 1, col - 1) = 255;
			//else
			//	gradient_x.at<uchar>(row - 1, col - 1) = 255;
			
			
			
		}
		
	}
	/////////// gradient_y //////////	
	for (int row = 1; row < dst_padding.rows - 1; row++) {
		for (int col = 1; col < dst_padding.cols - 1; col++) {

			int sum = 0;

			for (int kernel_r = -1; kernel_r <= 1; kernel_r++) {
				for (int kernel_c = -1; kernel_c <= 1; kernel_c++) {

					sum += dst_padding.at<uchar>(row + kernel_r, col + kernel_c) * sobel_y[kernel_r + 1][kernel_c + 1];


				}
				
				//else
				//	gradient_y.at<uchar>(row - 1, col - 1) = 255;
				
			}

			gradient_y.at<uchar>(row - 1, col - 1) = abs(sum);

			if (gradient_y.at<uchar>(row - 1, col - 1) < threshold)
				gradient_y.at<uchar>(row - 1, col - 1) = 0;
			if (gradient_y.at<uchar>(row - 1, col - 1) > 255)
				gradient_y.at<uchar>(row - 1, col - 1) = 255;
		}
	}
/*
	///////// gradient //////////
	for (int row = 0; row < gradient.rows; row++) {
		for (int col = 0; col < gradient.cols; col++) {

			gradient.at<uchar>(row, col) = abs(gradient_x.at<uchar>(row, col)) + abs(gradient_x.at<uchar>(row, col));

			if (gradient.at<uchar>(row, col) < threshold)
				gradient.at<uchar>(row, col) = 0;

		}

	}
*/
	
	IplImage vertical = IplImage(gradient_x);
	IplImage horizontal = IplImage(gradient_y);
//	IplImage magnitude = IplImage(gradient);
	cvSaveImage("M8_horizontal.jpg", &horizontal);
	cvSaveImage("M8_vertical.jpg", &vertical);
//	cvSaveImage("M8_magnitude.jpg", &magnitude);
	cvShowImage("2.1 Vertical edges", &vertical);
	cvWaitKey(0);
	cvShowImage("2.1 Horizontal edges", &horizontal);
	cvWaitKey(0);
	
	//////////// Magnitude & Angle /////////////////
	createTrackbar("Magnitude", "Magnitude ", &slidervalue, slidermax, trackbar_2);
	trackbar_2(slidervalue, 0); //TrackBar CallBack function,OpenCV定義成void function_name(int,void*)
	createTrackbar("Angle", "Angle ", &slidervalue2, slidermax2, trackbar_3);
	trackbar_3(slidervalue2,0);

	cvWaitKey(0);
	
	//cvShowImage("Magnitude ", &magnitude);
	
	cvReleaseImage(&img);
	cvReleaseImage(&img_gray);
	cvDestroyWindow("2. Edge Detection before ");
	cvDestroyWindow("2.1 Vertical edges");
	cvDestroyWindow("2.1 Horizontal edges");
	cvDestroyWindow("Magnitude ");
	cvDestroyWindow("Angle ");
}

void CMFCHW1Dlg::trackbar_2(int slidervalue, void* userdata) {

	///////// Sobel Edge Detection /////////
	int sobel_x[3][3] = { -1,0,1 , -2,0,2 , -1,0,1 };
	int sobel_y[3][3] = { 1,2,1 , 0,0,0 , -1,-2,-1 };
	Mat src_sobel = imread("M8_smooth.jpg", IMREAD_GRAYSCALE);
	Mat dst_padding;
	Mat gradient_x = src_sobel.clone();
	Mat gradient_y = src_sobel.clone();
	Mat gradient = src_sobel.clone();

	
	copyMakeBorder(src_sobel, dst_padding, 1, 1, 1, 1, BORDER_REFLECT_101);

	//////////// gradient_x //////////
	for (int row = 1; row < dst_padding.rows - 1; row++) {
		for (int col = 1; col < dst_padding.cols - 1; col++) {

			int sum = 0;

			for (int kernel_r = -1; kernel_r <= 1; kernel_r++) {
				for (int kernel_c = -1; kernel_c <= 1; kernel_c++) {

					sum += dst_padding.at<uchar>(row + kernel_r, col + kernel_c) * sobel_x[kernel_r + 1][kernel_c + 1];

				}
			}
			gradient_x.at<uchar>(row - 1, col - 1) = abs(sum);

			if (gradient_x.at<uchar>(row - 1, col - 1) < slidervalue)
				gradient_x.at<uchar>(row - 1, col - 1) = 0;
			
			//else
			//	gradient_x.at<uchar>(row - 1, col - 1) = 255;

		}

	}
	/////////// gradient_y //////////	
	for (int row = 1; row < dst_padding.rows - 1; row++) {
		for (int col = 1; col < dst_padding.cols - 1; col++) {

			int sum = 0;

			for (int kernel_r = -1; kernel_r <= 1; kernel_r++) {
				for (int kernel_c = -1; kernel_c <= 1; kernel_c++) {

					sum += dst_padding.at<uchar>(row + kernel_r, col + kernel_c) * sobel_y[kernel_r + 1][kernel_c + 1];


				}
				
				
			//	else
			//		gradient_y.at<uchar>(row-1, col-1) = 255;
			}

			gradient_y.at<uchar>(row - 1, col - 1) = abs(sum);

			if (gradient_y.at<uchar>(row - 1, col - 1) < slidervalue)
				gradient_y.at<uchar>(row - 1, col - 1) = 0;
		}
	}

	///////// gradient //////////
	for (int row = 0; row < gradient.rows; row++) {
		for (int col = 0; col < gradient.cols; col++) {

			gradient.at<uchar>(row, col) = abs(gradient_x.at<uchar>(row, col) + gradient_y.at<uchar>(row, col));

			if (gradient.at<uchar>(row, col) < slidervalue)
				gradient.at<uchar>(row, col) = 0;
			
			//else
			//	gradient.at<uchar>(row, col) = 255;

		}

	}


	
	IplImage magnitude = IplImage(gradient);
	
	cvSaveImage("M8_magnitude.jpg", &magnitude);
	cvShowImage("Magnitude ", &magnitude);
}

void CMFCHW1Dlg::trackbar_3(int slidervalue, void* userdata) {

	///////// Sobel Edge Detection /////////
	int sobel_x[3][3] = { -1,0,1 , -2,0,2 , -1,0,1 };
	int sobel_y[3][3] = { 1,2,1 , 0,0,0 , -1,-2,-1 };
	Mat src_sobel = imread("M8_smooth.jpg", IMREAD_GRAYSCALE);
	Mat dst_padding;
	Mat gradient_x; gradient_x.create(src_sobel.rows, src_sobel.cols, CV_8U);
	Mat gradient_x_; gradient_x_.create(src_sobel.rows, src_sobel.cols, CV_8U);
	Mat gradient_y; gradient_y.create(src_sobel.rows, src_sobel.cols, CV_8U);
	Mat gradient_y_; gradient_y_.create(src_sobel.rows, src_sobel.cols, CV_8U);
	Mat gradient; gradient.create(src_sobel.rows, src_sobel.cols, CV_8U);
	


	copyMakeBorder(src_sobel, dst_padding, 1, 1, 1, 1, BORDER_REFLECT_101);

	//////////// gradient_x //////////
	for (int row = 1; row < dst_padding.rows - 1; row++) {
		for (int col = 1; col < dst_padding.cols - 1; col++) {

			int sum = 0;

			for (int kernel_r = -1; kernel_r <= 1; kernel_r++) {
				for (int kernel_c = -1; kernel_c <= 1; kernel_c++) {

					sum += dst_padding.at<uchar>(row + kernel_r, col + kernel_c) * sobel_x[kernel_r + 1][kernel_c + 1];

				}
			}
			gradient_x.at<uchar>(row - 1, col - 1) = sum;
			

			gradient_x_.at<uchar>(row - 1, col - 1) = abs(sum);

			if (gradient_x_.at<uchar>(row - 1, col - 1) < 40) {
				gradient_x_.at<uchar>(row - 1, col - 1) = 0;
			}
			
		/*
			if (gradient_x.at<uchar>(row - 1, col - 1) < 40)
				gradient_x.at<uchar>(row - 1, col - 1) = 0;
			else
				gradient_x.at<uchar>(row - 1, col - 1) = 255;
		*/
		}
	}
	/////////// gradient_y //////////	
	for (int row = 1; row < dst_padding.rows - 1; row++) {
		for (int col = 1; col < dst_padding.cols - 1; col++) {

			int sum = 0;

			for (int kernel_r = -1; kernel_r <= 1; kernel_r++) {
				for (int kernel_c = -1; kernel_c <= 1; kernel_c++) {

					sum += dst_padding.at<uchar>(row + kernel_r, col + kernel_c) * sobel_y[kernel_r + 1][kernel_c + 1];


				}
				
			/*
				if (gradient_y.at<uchar>(row - 1, col - 1) < 40)
					gradient_y.at<uchar>(row - 1, col - 1) = 0;
				else
					gradient_y.at<uchar>(row - 1, col - 1) = 255;
			*/
			}

			gradient_y.at<uchar>(row - 1, col - 1) = sum;

			gradient_y_.at<uchar>(row - 1, col - 1) = abs(sum);



			if (gradient_y_.at<uchar>(row - 1, col - 1) < 40) {
				gradient_y_.at<uchar>(row - 1, col - 1) = 0;
			}
			
		}
	}
/*
	///////// gradient //////////
	for (int row = 0; row < gradient.rows; row++) {
		for (int col = 0; col < gradient.cols; col++) {

			gradient.at<uchar>(row, col) = abs(gradient_x.at<uchar>(row, col)) + abs(gradient_x.at<uchar>(row, col));

			if (gradient.at<uchar>(row, col) < 40)
				gradient.at<uchar>(row, col) = 0;
			else
				gradient.at<uchar>(row, col) = 255;

		}

	}
*/
	

	double rad;
	int deg;

	for (int row = 0; row < gradient.rows; row++) {
		for (int col = 0; col < gradient.cols; col++) {

			gradient.at<uchar>(row, col) = abs(gradient_x_.at<uchar>(row, col) + gradient_y_.at<uchar>(row, col));

			if (gradient.at<uchar>(row, col) < 40)
				gradient.at<uchar>(row, col) = 0;
			
			



			rad = atan2(gradient_y.at<char>(row, col), gradient_x.at<char>(row, col)) + M_PI;
			
			
			//rad = atan2(gradient_y.at<uchar>(row, col), gradient_x.at<uchar>(row, col));


			deg = (int) (rad / M_PI * 180.0 + 0.5);
			
		
				
			if (!(deg <= slidervalue + 10 && deg >= slidervalue - 10))
				gradient.at<uchar>(row, col) = 0;
			
			
			/*
			if (deg <= slidervalue + 10 && deg >= slidervalue - 10)
				gradient.at<uchar>(row, col) = 255;
			else
				gradient.at<uchar>(row, col) = 0;
			*/
		}
		
		
	}



	IplImage angle = IplImage(gradient);

	cvSaveImage("M8_Angle.jpg", &angle);
	cvShowImage("Angle ", &angle);
}


void CMFCHW1Dlg::OnBnClickedButton6()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	////////////// Gaussion_down_level_1 //////////////
	Mat G_0 = imread("pyramids_Gray.jpg",IMREAD_GRAYSCALE);
	Mat G_1; 
	G_1.create(G_0.rows/2, G_0.cols/2, CV_8U);
	IplImage img_G_0 = IplImage(G_0);
	IplImage img_G_1 = IplImage(G_1);


	cvPyrDown(&img_G_0, &img_G_1, CV_GAUSSIAN_5x5);
	cvSaveImage("pyramids_Gray_Gaussion_1.jpg", &img_G_1);
	cvWaitKey(0);



	///////////// Laplacian_level_0 ///////////////////

	Mat G_1_up; 
	G_1_up.create(G_0.rows,G_0.cols, CV_8U);
	IplImage img_G_1_up = IplImage(G_1_up);

	cvPyrUp(&img_G_1,&img_G_1_up,CV_GAUSSIAN_5x5);
	cvSaveImage("pyramids_Gray_Gaussian_1_up.jpg", &img_G_1_up);
	

	Mat L_0;
	G_0.convertTo(G_0, CV_8U);
	G_1_up.convertTo(G_1_up, CV_8U);
	L_0.convertTo(L_0, CV_8U);
	L_0 = G_0 - G_1_up;
	IplImage img_L_0 = IplImage(L_0);
	cvSaveImage("pyramids_Gray_Laplacian_0.jpg", &img_L_0);

	
	//////////// Gaussian_down_level_2 ///////////////

	Mat G_2; G_2.create(G_1.rows/2,G_1.cols/2,CV_8U);

	IplImage img_G_2 = IplImage(G_2);

	cvPyrDown(&img_G_1, &img_G_2, CV_GAUSSIAN_5x5);
	cvSaveImage("pyramids_Gray_Gaussian_2.jpg", &img_G_2);

	
	//////////// Laplacian_level_1 ////////////////
	
	Mat G_2_up;
	G_2_up.create(G_1.rows, G_1.cols, CV_8U);
	IplImage img_G_2_up = IplImage(G_2_up);

	cvPyrUp(&img_G_2, &img_G_2_up, CV_GAUSSIAN_5x5);
	cvSaveImage("pyramids_Gray_Gaussian_2_up.jpg", &img_G_2_up);


	Mat L_1;
	G_1.convertTo(G_1, CV_8U);
	G_2_up.convertTo(G_2_up, CV_8U);
	L_1.convertTo(L_1, CV_8U);
	L_1 = G_1 - G_2_up;
	IplImage img_L_1 = IplImage(L_1);
	cvSaveImage("pyramids_Gray_Laplacian_1.jpg", &img_L_1);


	//////////// Inverse_level_1 //////////////
	Mat I_1;
	I_1.convertTo(I_1, CV_8U);
	I_1 = L_1 + G_2_up;
	IplImage img_I_1 = IplImage(I_1);
	cvSaveImage("pyramids_Gray_Inverse_1.jpg", &img_I_1);

	/////////// Inverse_level_0 ///////////////

	Mat I_0;
	I_0.convertTo(I_0, CV_8U);
	I_0 = L_0 + G_1_up;
	IplImage img_I_0 = IplImage(I_0);
	cvSaveImage("pyramids_Gray_Inverse_0.jpg", &img_I_0);
	

	



	////////// Show Result ////////////////
	cvNamedWindow("3.1 Gaussion Level1", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("3.2 Laplacian Level0", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("3.3 Inverse Level1", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("3.4 Inverse Level0", CV_WINDOW_AUTOSIZE);
	
	cvShowImage("3.1 Gaussion Level1", &img_G_1);
	cvShowImage("3.2 Laplacian Level0", &img_L_0);
	cvShowImage("3.3 Inverse Level1", &img_I_1);
	cvShowImage("3.4 Inverse Level0", &img_I_0);
	cvWaitKey(0);
	cvDestroyWindow("3.1 Gaussion Level1");
	cvDestroyWindow("3.2 Laplacian Level0");
	cvDestroyWindow("3.3 Inverse Level1");
	cvDestroyWindow("3.4 Inverse Level0");
	G_0.release();
	G_1.release();
	G_1_up.release();
	L_0.release();
	G_2.release();
	G_2_up.release();
	L_1.release();
	L_0.release();
	I_1.release();
	I_0.release();


}


void CMFCHW1Dlg::OnBnClickedButton7()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	IplImage* img = cvLoadImage("QR.png",CV_LOAD_IMAGE_ANYDEPTH);
	IplImage* img_global = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, img->nChannels);
	
	cvNamedWindow("4.1 Global Threshold", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("4.1 Original Image", CV_WINDOW_AUTOSIZE);
	

	cvThreshold(img, img_global,80, 255, CV_THRESH_BINARY);

	

	cvShowImage("4.1 Original Image", img);
	cvWaitKey(0);
	cvShowImage("4.1 Global Threshold", img_global);
	cvWaitKey(0);
	
	cvReleaseImage(&img);
	cvReleaseImage(&img_global);
	cvDestroyWindow("4.1 Global Threshold");
	cvDestroyWindow("4.1 Original Image");
	
}


void CMFCHW1Dlg::OnBnClickedButton8()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	IplImage* img = cvLoadImage("QR.png", CV_LOAD_IMAGE_ANYDEPTH);
	IplImage* img_local = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, img->nChannels);
	cvNamedWindow("2.2 Original Image", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("2.2 Local Threshold", CV_WINDOW_AUTOSIZE);

	cvAdaptiveThreshold(img, img_local, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 19, -1);

	cvShowImage("2.2 Original Image", img);
	cvWaitKey(0);
	cvShowImage("2.2 Local Threshold", img_local);
	cvWaitKey(0);

	cvReleaseImage(&img);
	cvReleaseImage(&img_local);
	cvDestroyWindow("2.2 Original Image");
	cvDestroyWindow("2.2 Local Threshold");

}


void CMFCHW1Dlg::OnBnClickedButton9()
{
	// TODO: 在此加入控制項告知處理常式程式碼



	CEdit *Edit1 = (CEdit *)GetDlgItem(IDC_EDIT1);
	CEdit *Edit2 = (CEdit *)GetDlgItem(IDC_EDIT2);
	CEdit *Edit3 = (CEdit *)GetDlgItem(IDC_EDIT3);
	CEdit *Edit4 = (CEdit *)GetDlgItem(IDC_EDIT4);
	CString str1;
	CString str2;
	CString str3;
	CString str4;
	int angle ;
	double scale ;
	int Tx ;
	int Ty ;
	
	Edit1->GetWindowText(str1);
	angle = _ttoi(str1);

	Edit2->GetWindowText(str2);
	scale = _ttof(str2);

	Edit3->GetWindowText(str3);
	Tx = _ttoi(str3);

	Edit4->GetWindowText(str4);
	Ty = _ttoi(str4);

	IplImage* img = cvLoadImage("OriginalTransform.png");
	cvNamedWindow("5.1 Original Transform", CV_WINDOW_AUTOSIZE);
	cvShowImage("5.1 Original Transform", img);
	cvWaitKey(0);


	Mat src = imread("OriginalTransform.png");
	Mat dst = Mat::zeros(src.rows, src.cols, src.type());

	Mat trans_mat = (Mat_<double>(2, 3) << 1, 0, Tx, 0, 1, Ty);
	warpAffine(src, src, trans_mat, src.size());
	Point center = Point(130 + Tx, 125 + Ty);
	Mat rot_mat = getRotationMatrix2D(center, angle, scale);
	warpAffine(src, dst, rot_mat, dst.size());

	cvNamedWindow("5.1 Rotation + Scale + Translation Image", CV_WINDOW_AUTOSIZE);
	IplImage img_dst = IplImage(dst);
	cvSaveImage("AfterTransform.png", &img_dst);
	cvShowImage("5.1 Rotation + Scale + Translation Image", &img_dst);
	cvWaitKey(0);


	cvReleaseImage(&img);
	cvDestroyWindow("5.1 Original Transform");
	cvDestroyWindow("5.1 Rotation + Scale + Translation Image");


}

void CMFCHW1Dlg::OnBnClickedButton10()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	click_times = 0;
	IplImage* origin = cvLoadImage("OriginalPerspective.png");
	cvNamedWindow("5.2 Original Perspective", CV_WINDOW_AUTOSIZE);
	cvShowImage("5.2 Original Perspective", origin);

	cvSetMouseCallback("5.2 Original Perspective",onMouse);

	
	cvWaitKey(0);


	cvReleaseImage(&origin);
	cvDestroyWindow("5.2 Original Perspective");
	
}

void CMFCHW1Dlg::onMouse(int event, int x, int y, int flags, void* param)
{

	if (event == CV_EVENT_LBUTTONDOWN) {
		x_ = x;
		y_ = y;
		click_times++;
		if (click_times == 1) {
			top_left = Point2i(x_, y_);
		}
		else if (click_times == 2) {
			bottom_left = Point2i(x_, y_);
		}
		else if (click_times == 3) {
			bottom_right = Point2d(x_, y_);
		}
		else if (click_times == 4) {
			top_right = Point2d(x_, y_);
			showResult();
		}
		else if (click_times > 4) {

		}



		printf("now x: %d y: %d\n", x, y);
	}
}

void CMFCHW1Dlg::showResult() {

	Point2f before[] = { top_left,bottom_left,bottom_right,top_right };
	Point2f after[] = { tl_after,bl_after,br_after,tr_after };
	Mat perspective_mat = getPerspectiveTransform(before, after);


	Mat src = imread("OriginalPerspective.png");
	Mat dst = Mat::zeros(src.rows, src.cols, src.type());
	warpPerspective(src, dst, perspective_mat, src.size(), INTER_LINEAR);


	Mat dst2 = dst(Range(20, 450), Range(20, 450));



	IplImage img_dst = IplImage(dst2);
	cvSaveImage("PerspectiveResult.png", &img_dst);

	cvNamedWindow("5.2 Perspective Result Image", CV_WINDOW_AUTOSIZE);
	cvShowImage("5.2 Perspective Result Image", &img_dst);

	cvWaitKey(0);
	cvDestroyWindow("5.2 Perspective Result Image");
}
