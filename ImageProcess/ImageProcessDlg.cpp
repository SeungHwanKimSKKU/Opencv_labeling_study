
// ImageProcessDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ImageProcess.h"
#include "ImageProcessDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//��� â �̸�
CStatic m_cam_main;								//CAM ��� â
CStatic m_show_result;							//window for showing image processing result
CStatic m_img_process;
CStatic m_img_process2;
CStatic m_img_process3;
CStatic m_img_process4;
CStatic m_img_process5;
////////////// Array  //////////////////
CString PortArray[] = { "COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "COM10" };
DWORD BaurateArray[] = { CBR_9600, CBR_19200, CBR_115200 };
char Rx_Buff[256] = { 0, };

//Flag Parameter
////Combat Box
int Flag_Program_Start = 0;
////Cam
int Flag_Thread1_State = 0;  //0: Off, 1: 0n
int Flag_Thread1_End_Check = 0;	//0: Thread END, 1: Thread not END


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageProcessDlg ��ȭ ����



CImageProcessDlg::CImageProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageProcessDlg::IDD, pParent)
	, m_th_under_H(0)
	, m_th_under_S(0)
	, m_th_upper_H(0)
	, m_th_upper_S(0)
	, m_index_color(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAM_MAIN, m_cam_main);
	DDX_Control(pDX, IDC_SHOW_RESULT, m_show_result);
	DDX_Text(pDX, IDC_TH_UNDER_H, m_th_under_H);
	DDX_Text(pDX, IDC_TH_UNDER_S, m_th_under_S);
	DDX_Text(pDX, IDC_TH_UPPER_H, m_th_upper_H);
	DDX_Text(pDX, IDC_TH_UPPER_S, m_th_upper_S);
	DDX_CBIndex(pDX, IDC_COMBO_COLOR, m_index_color);
	DDX_Control(pDX, IDC_IMG_PROCESS, m_img_process);
	DDX_Control(pDX, IDC_IMG_PROCESS2, m_img_process2);
	DDX_Control(pDX, IDC_IMG_PROCESS3, m_img_process3);
	DDX_Control(pDX, IDC_IMG_PROCESS4, m_img_process4);
	DDX_Control(pDX, IDC_IMG_PROCESS5, m_img_process5);
}

BEGIN_MESSAGE_MAP(CImageProcessDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_THREAD1_START, &CImageProcessDlg::OnBnClickedThread1Start)
	ON_BN_CLICKED(IDOK, &CImageProcessDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_THREAD1_STOP, &CImageProcessDlg::OnBnClickedThread1Stop)
	ON_CBN_SELCHANGE(IDC_COMBO_COLOR, &CImageProcessDlg::OnCbnSelchangeComboColor)
	ON_EN_CHANGE(IDC_TH_UPPER_H, &CImageProcessDlg::OnEnChangeThUpperH)
	ON_EN_CHANGE(IDC_TH_UNDER_H, &CImageProcessDlg::OnEnChangeThUnderH)
	ON_EN_CHANGE(IDC_TH_UPPER_S, &CImageProcessDlg::OnEnChangeThUpperS)
	ON_EN_CHANGE(IDC_TH_UNDER_S, &CImageProcessDlg::OnEnChangeThUnderS)
	ON_BN_CLICKED(IDC_SET_PARAMETER, &CImageProcessDlg::OnBnClickedSetParameter)
END_MESSAGE_MAP()


// CImageProcessDlg �޽��� ó����

BOOL CImageProcessDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	//Set Parameter Value
	m_th_ball_H_L = TH_BALL_H_L;
	m_th_ball_H_H = TH_BALL_H_H;
	m_th_ball_S_L = TH_BALL_S_L;
	m_th_ball_S_H = TH_BALL_S_H;

	m_th_red_H_L = TH_RED_H_L;
	m_th_red_H_H = TH_RED_H_H;
	m_th_red_S_L = TH_RED_S_L;
	m_th_red_S_H = TH_RED_S_H;
	m_th_blue_H_L = TH_BLUE_H_L;
	m_th_blue_H_H = TH_BLUE_H_H;
	m_th_blue_S_L = TH_BLUE_S_L;
	m_th_blue_S_H = TH_BLUE_S_H;
	m_th_green_H_L = TH_GREEN_H_L;
	m_th_green_H_H = TH_GREEN_H_H;
	m_th_green_S_L = TH_GREEN_S_L;
	m_th_green_S_H = TH_GREEN_S_H;

	OnCbnSelchangeComboColor();

	//Set Dialog Value & Display
	Flag_Program_Start = 1;
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CImageProcessDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CImageProcessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CImageProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT MyThread1(LPVOID IpParam)
{
	CImageProcessDlg* pClass = (CImageProcessDlg*)IpParam;
	int iReturn = pClass->Thread1Function();
	return 0L;
}

void CImageProcessDlg::CreateThread1(UINT _method)
{
	if (pThread1 != NULL)
	{
		printf("Thread is running\n");
		return;
	}

	pThread1 = AfxBeginThread(MyThread1, this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);

	if (pThread1 == NULL)
		printf("Fail to create camera Thread\n");

	pThread1->m_bAutoDelete = FALSE;
	pThread1->ResumeThread();
}

bool CImageProcessDlg::DestoryThread1(void)
{
	if (pThread1 != NULL)
	{
		DWORD dwResult = ::WaitForSingleObject(pThread1->m_hThread, INFINITE);

		if (dwResult == WAIT_TIMEOUT)
			printf("Time Out\n");
		else if (dwResult == WAIT_OBJECT_0)
			printf("Thread END\n");
		pThread1 = NULL;
	}
	return true;
}

int CImageProcessDlg::Thread1Function(void)
{
	//�ʱ�ȭ
	//Variable
	int Flag_ImageProcess_Init = 0;
	//Picture Control
	CRect rect_cam_main, rect_show_result, rect_img_process, rect_img_process2, rect_img_process3, rect_img_process4, rect_img_process5;
	CvvImage cvv_cam_main, cvv_show_result, cvv_img_process, cvv_img_process2, cvv_img_process3, cvv_img_process4, cvv_img_process5;
	CDC *pDC_cam_main, *pDC_show_result, *pDC_img_process, *pDC_img_process2, *pDC_img_process3, *pDC_img_process4, *pDC_img_process5;
	CvSize img_size;
	
	pDC_cam_main = m_cam_main.GetDC();
	pDC_show_result = m_show_result.GetDC();
	pDC_img_process = m_img_process.GetDC();
	pDC_img_process2 = m_img_process2.GetDC();
	pDC_img_process3 = m_img_process3.GetDC();
	pDC_img_process4 = m_img_process4.GetDC();
	pDC_img_process5 = m_img_process5.GetDC();
	m_cam_main.GetClientRect(&rect_cam_main);
	m_show_result.GetClientRect(&rect_show_result);
	m_img_process.GetClientRect(&rect_img_process);
	m_img_process2.GetClientRect(&rect_img_process2);
	m_img_process3.GetClientRect(&rect_img_process3);
	m_img_process4.GetClientRect(&rect_img_process4);
	m_img_process5.GetClientRect(&rect_img_process5);
	//////Cam
	CvCapture* capture = cvCaptureFromCAM(0); //���� �νĵ� ��ķ�� ã��,
	cvGrabFrame(capture);
	img_Cam = cvRetrieveFrame(capture);
	//////Image
	img_Img = cvLoadImage("image.jpg");
	img_Img2 = cvLoadImage("image.jpg");
	img_Img3 = cvLoadImage("image.jpg");
	img_Img4 = cvLoadImage("image.jpg");
	img_Img5 = cvLoadImage("image.jpg");
	//����
	cvv_img_process2.CopyOf(img_Img2, 1);
	cvv_img_process2.DrawToHDC(pDC_img_process2->m_hDC, &rect_img_process2);
	cvv_img_process3.CopyOf(img_Img3, 1);
	cvv_img_process3.DrawToHDC(pDC_img_process3->m_hDC, &rect_img_process3);
	cvv_img_process4.CopyOf(img_Img4, 1);
	cvv_img_process4.DrawToHDC(pDC_img_process4->m_hDC, &rect_img_process4);
	cvv_img_process5.CopyOf(img_Img5, 1);
	cvv_img_process5.DrawToHDC(pDC_img_process5->m_hDC, &rect_img_process5);
	//�ݺ���
	while (Flag_Thread1_State == 1)
	{
		//////Cam
		//cvGrabFrame(capture);
		//img_Cam = cvRetrieveFrame(capture); // ���� �νĵ� ����� �޾ƿ���image�� �ִ´�.
		//Image
		img_Cam = cvLoadImage("image.jpg",1);

		if (Flag_ImageProcess_Init == 0) // one time running
		{
			//get cam image size
			img_size.height = img_Cam->height;
			img_size.width = img_Cam->width;
			//memory allocation
			Init_ImageProcess(img_size);
			//chage condition
			Flag_ImageProcess_Init = 1;
		}
		//Image Processing Part
		ImageProcess();
		
		//Show image processing result in sub frame
		cvv_show_result.CopyOf(img_Show, 1);
		cvv_show_result.DrawToHDC(pDC_show_result->m_hDC, &rect_show_result);

		//Show cam result in main frame
		cvv_cam_main.CopyOf(img_Cam, 1);
		cvv_cam_main.DrawToHDC(pDC_cam_main->m_hDC, &rect_cam_main);

		//�̹�����
		cvv_img_process.CopyOf(img_Img, 1);
		cvv_img_process.DrawToHDC(pDC_img_process->m_hDC, &rect_img_process);
	}
	//�����
	printf("Memory Release\n");
	//////Cam
	cvReleaseCapture(&capture);
	
	m_cam_main.ReleaseDC(pDC_cam_main);
	
	return 0;
}

void CImageProcessDlg::Init_ImageProcess(CvSize img_size)
{
	//Create Image Frame
	img_Process = cvCreateImage(img_size, IPL_DEPTH_8U, 3);
	img_HSV = cvCreateImage(img_size, IPL_DEPTH_8U, 3);
	img_Gray = cvCreateImage(img_size, IPL_DEPTH_8U, 1);
	img_Label = cvCreateImage(img_size, IPL_DEPTH_8U, 1);
}
void CImageProcessDlg::Labelling()
{
	int r, c;
	int width = img_Cam->width;
	int height = img_Cam->height;
	int index = 1;
	unsigned char min_index, max_index;
	for (r = 0; r < height; r++)
	{
		for (c = 0; c < width; c++)
		{
			min_index = 255;
			if ((unsigned char)img_Gray->imageData[r*width + c] == 255)//���(��ü)�̸�
			{
				if ((unsigned char)img_Gray->imageData[(r - 1)*width + c - 1] != 0)
					min_index = img_Gray->imageData[(r - 1)*width + c - 1];

				if ((unsigned char)img_Gray->imageData[(r - 1)*width + c] != 0)
				{
					if ((unsigned char)img_Gray->imageData[(r - 1)*width + c] < min_index)
						min_index = img_Gray->imageData[(r - 1)*width + c];
				}
				if ((unsigned char)img_Gray->imageData[(r - 1)*width + c + 1] != 0)
				{
					if ((unsigned char)img_Gray->imageData[(r - 1)*width + c + 1] < min_index)
						min_index = img_Gray->imageData[(r - 1)*width + c + 1];
				}
				if ((unsigned char)img_Gray->imageData[r*width + c - 1] != 0)
				{
					if ((unsigned char)img_Gray->imageData[r*width + c - 1] < min_index)
						min_index = img_Gray->imageData[r*width + c - 1];
				}
				if (min_index == 255)
				{
					img_Gray->imageData[r*width + c] = index;
					index += 10;
				}
				else
					img_Gray->imageData[r*width + c] = min_index;
			}

		}
	}
	for (r = height - 1; r >= 0; r--)
	{
		for (c = width - 1; c >= 0; c--)
		{
			if ((unsigned char)img_Gray->imageData[r*width + c] != 0)
			{
				max_index = img_Gray->imageData[r*width + c];
				if ((unsigned char)img_Gray->imageData[r*width + c + 1] != 0)
				{
					if ((unsigned char)img_Gray->imageData[r*width + c + 1] >= max_index)
						max_index = img_Gray->imageData[r*width + c + 1];
				}
				if ((unsigned char)img_Gray->imageData[(r + 1)*width + c + 1] != 0)
				{
					if ((unsigned char)img_Gray->imageData[(r + 1)*width + c + 1] >= max_index)
						max_index = img_Gray->imageData[(r + 1)*width + c + 1];
				}
				if ((unsigned char)img_Gray->imageData[(r + 1)*width + c] != 0)
				{
					if ((unsigned char)img_Gray->imageData[(r + 1)*width + c] >= max_index)
						max_index = img_Gray->imageData[(r + 1)*width + c];
				}
				if ((unsigned char)img_Gray->imageData[(r + 1)*width + c - 1] != 0)
				{
					if ((unsigned char)img_Gray->imageData[(r + 1)*width + c - 1] >= max_index)
						max_index = img_Gray->imageData[(r + 1)*width + c - 1];
				}
				if (max_index > index)
				{
					img_Gray->imageData[r*width + c] = max_index;
				}
				else
				{
					img_Gray->imageData[r*width + c] = index;
					index -= 10;
				}
			}
		}
	}
}
void CImageProcessDlg::ImageProcess(void)
{
	//local variable
	int r, c;
	char H, S, V;
	int width = img_Cam->width;
	int height = img_Cam->height;
	int widthStep = img_Cam->widthStep;

	//Copy Cam Image
	cvCopy(img_Cam, img_Process);//img_Cam �̹��� img_Process�� ����

	//Change Gray Image
	cvCvtColor(img_Process, img_Gray, CV_BGR2GRAY);
	cvCvtColor(img_Process, img_HSV, CV_BGR2HSV);
	//Code Here
	/*
	for (r = 0; r < height; r++)
	{
		for (c = 0; c < width; c++)
		{
			//Get Image Pixel 3 channel value
			H = img_HSV->imageData[(r*width + c) * 3 + 0];
			S = img_HSV->imageData[(r*width + c) * 3 + 1];
			V = img_HSV->imageData[(r*width + c) * 3 + 2];

			//
			if (H > m_th_under_H && H < m_th_upper_H && H > m_th_under_S && H < m_th_upper_H)
			{
				img_Gray->imageData[r*width + c] = 255;
			}
			else
			{
				img_Gray->imageData[r*width + c] = 0;
			}
		}
	}
	*/
	//Binary Image
	for (r = 0; r < height; r++)
	{
		for (c = 0; c < width; c++)
		{
			if ((unsigned char)img_Gray->imageData[r*width + c] > 150)
				img_Gray->imageData[r*width + c] = 0;
			else
				img_Gray->imageData[r*width + c] = 255;
		}
	}

	//Labelling
	Labelling();
	//Image Show
	//img_Show = img_Process;
	img_Show = img_Gray;
}






//Button control
void CImageProcessDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if (Flag_Thread1_State == 1)
	{
		Flag_Thread1_State = 0;
		DestoryThread1();
	}
	delete pThread1;

	CDialogEx::OnOK();
}
void CImageProcessDlg::OnBnClickedThread1Start()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	printf("Thread Start Button\n");
	if (Flag_Thread1_State == 0)
	{
		Flag_Thread1_State = 1;
		Flag_Thread1_End_Check = 1;
		CreateThread1(0);
	}
}

void CImageProcessDlg::OnBnClickedThread1Stop()
{
	//Stop thread
	Flag_Thread1_State = 0;
	//Wait Thread End
	DestoryThread1();
}


void CImageProcessDlg::OnCbnSelchangeComboColor()
{
	//Save previous data
	if (Flag_Program_Start == 1)
	{
		OnBnClickedSetParameter();
		//Change New Data
		UpdateData(TRUE);
	}
	switch (m_index_color)
	{
	case RED :
		m_th_under_H = m_th_red_H_L;
		m_th_under_S = m_th_red_S_L;
		m_th_upper_H = m_th_red_H_H;
		m_th_upper_S = m_th_red_S_H;
		break;
	case BLUE :
		m_th_under_H = m_th_blue_H_L;
		m_th_under_S = m_th_blue_S_L;
		m_th_upper_H = m_th_blue_H_H;
		m_th_upper_S = m_th_blue_S_H;
		break;
	case GREEN :
		m_th_under_H = m_th_green_H_L;
		m_th_under_S = m_th_green_S_L;
		m_th_upper_H = m_th_green_H_H;
		m_th_upper_S = m_th_green_S_H;
		break;
	}
	UpdateData(FALSE);
}


void CImageProcessDlg::OnEnChangeThUpperH() { UpdateData(TRUE); }
void CImageProcessDlg::OnEnChangeThUnderH() { UpdateData(TRUE); }
void CImageProcessDlg::OnEnChangeThUpperS() { UpdateData(TRUE); }
void CImageProcessDlg::OnEnChangeThUnderS() { UpdateData(TRUE); }


void CImageProcessDlg::OnBnClickedSetParameter()
{
	switch (m_index_color)
	{
	case RED:
		m_th_red_H_L = m_th_under_H;
		m_th_red_S_L = m_th_under_S;
		m_th_red_H_H = m_th_upper_H;
		m_th_red_S_H = m_th_upper_S;
		break;
	case BLUE:
		m_th_blue_H_L = m_th_under_H;
		m_th_blue_S_L = m_th_under_S;
		m_th_blue_H_H = m_th_upper_H;
		m_th_blue_S_H = m_th_upper_S;
		break;
	case GREEN:
		m_th_green_H_L = m_th_under_H;
		m_th_green_S_L = m_th_under_S;
		m_th_green_H_H = m_th_upper_H;
		m_th_green_S_H = m_th_upper_S;
		break;
	}
}
