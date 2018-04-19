// MyMiniBlockChainView.cpp : CMyMiniBlockChainView ���ʵ��
//

#include "stdafx.h"
#ifndef SHARED_HANDLERS
#include "MyMiniBlockChain.h"
#endif

#include "MyMiniBlockChainDoc.h"
#include "MyMiniBlockChainView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MainFrm.h"
#pragma comment(lib,"Rpcrt4.lib")  
// CMyMiniBlockChainView

IMPLEMENT_DYNCREATE(CMyMiniBlockChainView, CScrollView)

BEGIN_MESSAGE_MAP(CMyMiniBlockChainView, CScrollView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMyMiniBlockChainView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDM_CREATEWORLD_BLOCK, &CMyMiniBlockChainView::OnCreateWorldBlock)
	ON_COMMAND(IDM_NODE1_MINE_BLOCK, &CMyMiniBlockChainView::OnNode1MineBlock)
	ON_COMMAND(IDM_NODE2_MINE_BLOCK, &CMyMiniBlockChainView::OnNode2MineBlock)
	ON_COMMAND(IDM_NODE1_TRANSACTION, &CMyMiniBlockChainView::OnNode1Transaction)
	ON_COMMAND(IDM_NODE2_TRANSACTION, &CMyMiniBlockChainView::OnNode2Transaction)
	ON_COMMAND(IDM_NODE2_GET_CHAIN, &CMyMiniBlockChainView::OnNode2GetChain)
	ON_MESSAGE(WM_CUTNODE, OnEditCut)
	ON_COMMAND(IDM_REGISTER_NODE2, &CMyMiniBlockChainView::OnRegisterNode2forNode1)
	ON_COMMAND(IDM_REGISTER_NODE1, &CMyMiniBlockChainView::OnRegisterNode1ForNode2)
	ON_COMMAND(IDM_VALID_CHAIN, &CMyMiniBlockChainView::OnValidChain2)
	ON_COMMAND(IDM_CONSENSUS_FORNODE1, &CMyMiniBlockChainView::OnConsensusForNode1)
	ON_COMMAND(IDM_CONSENSUS_FORNODE2, &CMyMiniBlockChainView::OnConsensusForNode2)
END_MESSAGE_MAP()

// CMyMiniBlockChainView ����/����

CMyMiniBlockChainView::CMyMiniBlockChainView()
{
	// TODO: �ڴ˴���ӹ������
	m_pMiniBlockChain = new CMiniBlockChain;
	m_pMiniBlockChain2 = new CMiniBlockChain;
	m_iVerticalSpace = 10;
	m_iHorizontalSpace = 10;

	UUID globalUUID;
	RPC_WSTR tempRPCString;
	CString  Node1Uuid;
	CString  Node2Uuid;
	UuidCreate(&globalUUID);
	UuidToString(&globalUUID, &tempRPCString);
	Node1Uuid.Format(_T("%s"), tempRPCString);
	RpcStringFree(&tempRPCString);
	Node1Uuid.Replace(_T("-"), _T(""));
	UuidCreate(&globalUUID);
	UuidToString(&globalUUID, &tempRPCString);
	Node2Uuid.Format(_T("%s"), tempRPCString);
	RpcStringFree(&tempRPCString);
	Node2Uuid.Replace(_T("-"), _T(""));

	int length = Node1Uuid.GetLength();
	m_strNode1Uuid = (char*)malloc(sizeof(char)*(length + 1));
	TcharToChar(Node1Uuid.GetBuffer(0), m_strNode1Uuid);
	length = Node2Uuid.GetLength();
	m_strNode2Uuid = (char*)malloc(sizeof(char)*(length + 1));
	TcharToChar(Node2Uuid.GetBuffer(0), m_strNode2Uuid);
}

CMyMiniBlockChainView::~CMyMiniBlockChainView()
{
	if (m_pMiniBlockChain != NULL)
	{
		free(m_pMiniBlockChain);
		m_pMiniBlockChain = NULL;
	}
	
	if (m_pMiniBlockChain2 != NULL)
	{
		free(m_pMiniBlockChain2);
		m_pMiniBlockChain2 = NULL;
	}
		
	if (m_strNode1Uuid != NULL)
	{
		free(m_strNode1Uuid);
		m_strNode1Uuid = NULL;
	}
		
	if (m_strNode1Uuid != NULL)
	{
		free(m_strNode2Uuid);
		m_strNode2Uuid = NULL;
	}
}

BOOL CMyMiniBlockChainView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	theApp.pMyMiniBlockChainView = this;
	return CScrollView::PreCreateWindow(cs);
}

void CMyMiniBlockChainView::CharToTchar(const char * _char, TCHAR * tchar)
{
	int iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
	return;
}

void CMyMiniBlockChainView::TcharToChar(const TCHAR * tchar, char * _char)
{
	int iLength = WideCharToMultiByte(CP_ACP, 0, tchar, -1, NULL, 0, NULL, FALSE);
	WideCharToMultiByte(CP_ACP, NULL, tchar, -1, _char, iLength, NULL, FALSE);
	return;
}

void CMyMiniBlockChainView::OnInitialUpdate()
{
	CMyMiniBlockChainDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->SetTitle(_T("�������ڵ�1 "));
	CScrollView::OnInitialUpdate();
	CSize sizeTotal(30000, 40000);
	CSize sizePage(sizeTotal.cx / 2, sizeTotal.cy / 2);
	CSize sizeLine(sizeTotal.cx / 100, sizeTotal.cy / 100);
	SetScrollSizes(MM_HIMETRIC, sizeTotal, sizePage, sizeLine);
}

void CMyMiniBlockChainView::OnDraw(CDC* pDC)
{
	BeginWaitCursor();
	CMyMiniBlockChainDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	_ASSERTE(_CrtCheckMemory());
	if (!pDoc)
		return;
	char*  bs = NULL;
	Block* bTemp = m_pMiniBlockChain->getBlockChain();
	m_iVerticalSpace = -500;
	m_iHorizontalSpace = 500;
	while (bTemp != NULL)
	{
		m_pMiniBlockChain->getBlockStr(bTemp, &bs);
		bTemp = bTemp->next;
		CRect textRect;
		char* ptemp = bs;
		char* qtemp = NULL;
		qtemp = strstr(ptemp, "\n");
		while (qtemp != NULL)
		{
			int length = qtemp - ptemp;
			ptemp[length] = '\0';
			TCHAR * tcharTemp = (TCHAR*)malloc(sizeof(TCHAR)*(length + 1));
			CharToTchar(ptemp, tcharTemp);
			pDC->DrawText(tcharTemp, strlen(ptemp), &textRect, DT_CALCRECT);
			CPoint drawPoint(m_iHorizontalSpace, m_iVerticalSpace);
			pDC->TextOut(m_iHorizontalSpace, m_iVerticalSpace, tcharTemp);
			m_iVerticalSpace += textRect.Height();
			ptemp = qtemp + 1;
			qtemp = strstr(ptemp, "\n");
			free(tcharTemp);
		}
		TCHAR * tcharTemp = (TCHAR*)malloc(sizeof(TCHAR)*(strlen(ptemp) + 1));
		CharToTchar(ptemp, tcharTemp);
		pDC->DrawText(tcharTemp, strlen(ptemp), &textRect, DT_CALCRECT);
		pDC->TextOut(m_iHorizontalSpace, m_iVerticalSpace, tcharTemp);
		m_iVerticalSpace += textRect.Height();
		free(tcharTemp);
		free(bs);
	}
	EndWaitCursor();
}

void CMyMiniBlockChainView::DrawChain2Info()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_wndOutput.Reset();
	char  *bs;
	Block* pTemp = m_pMiniBlockChain2->getBlockChain();
	while (pTemp != NULL)
	{
		m_pMiniBlockChain2->getBlockStrForHash(pTemp, &bs);
		pTemp = pTemp->next;
		pMainFrame->m_wndOutput.AddInfo(bs);
		free(bs);
	}
}

// CMyMiniBlockChainView ��ӡ
void CMyMiniBlockChainView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMyMiniBlockChainView::OnPreparePrinting(CPrintInfo* pInfo)
{
	return DoPreparePrinting(pInfo);
}

void CMyMiniBlockChainView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CMyMiniBlockChainView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CMyMiniBlockChainView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMyMiniBlockChainView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

#ifdef _DEBUG
void CMyMiniBlockChainView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMyMiniBlockChainView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMyMiniBlockChainDoc* CMyMiniBlockChainView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyMiniBlockChainDoc)));
	return (CMyMiniBlockChainDoc*)m_pDocument;
}
#endif //_DEBUG

LRESULT CMyMiniBlockChainView::OnEditCut(WPARAM wParam, LPARAM lParam)
{
	long index = lParam;
	if (index < 0 && index> m_pMiniBlockChain2->getBlockLength())
		return index;
	m_pMiniBlockChain2->deleteNode(index);
	return 0;
}

void CMyMiniBlockChainView::OnCreateWorldBlock()
{
	char timeStamp[18];
	m_pMiniBlockChain->getTimeStamp(timeStamp);
	m_pMiniBlockChain->initBlockChain(timeStamp);
	m_pMiniBlockChain2->initBlockChain(timeStamp);
	//�����������ڵ�1��ͼ
	Invalidate();
	UpdateWindow();
	//�����������ڵ�2��ͼ
	DrawChain2Info();
}

void CMyMiniBlockChainView::OnNode1MineBlock()
{
	m_pMiniBlockChain->newTransaction(1, m_strNode1Uuid, "0");
	m_pMiniBlockChain->newBlock();
	//�����������ڵ�1��ͼ
	Invalidate();
	UpdateWindow();
}


void CMyMiniBlockChainView::OnNode2MineBlock()
{
	m_pMiniBlockChain2->newTransaction(1, m_strNode2Uuid, "0");
	m_pMiniBlockChain2->newBlock();
	//�����������ڵ�2��ͼ
	DrawChain2Info();
}

//ģ���������һ���û�ID��Ϊsender��ڵ�1�û���������
void CMyMiniBlockChainView::OnNode1Transaction()
{
	UUID globalUUID;
	RPC_WSTR tempRPCString;
	CString  NodeUuid;

	UuidCreate(&globalUUID);
	UuidToString(&globalUUID, &tempRPCString);
	NodeUuid.Format(_T("%s"), tempRPCString);
	RpcStringFree(&tempRPCString);
	NodeUuid.Replace(_T("-"), _T(""));

	int length = NodeUuid.GetLength();
	char* temp = (char*)malloc(sizeof(char)*(length + 1));
	TcharToChar(NodeUuid.GetBuffer(0), temp);
	m_pMiniBlockChain->newTransaction(1, m_strNode1Uuid, temp);
	char tempBuffer[50];
	sprintf(tempBuffer, "������Ϣ������ӵ�����{ %d }", m_pMiniBlockChain->getBlockLength() + 1);
	TCHAR * tcharTemp = (TCHAR*)malloc(sizeof(TCHAR)*(strlen(tempBuffer) + 1));
	CharToTchar(tempBuffer, tcharTemp);
	AfxMessageBox(tcharTemp);
	free(tcharTemp);
}

//ģ���������һ���û�ID��Ϊsender��ڵ�2�û���������
void CMyMiniBlockChainView::OnNode2Transaction()
{
	UUID globalUUID;
	RPC_WSTR tempRPCString;
	CString  NodeUuid;

	UuidCreate(&globalUUID);
	UuidToString(&globalUUID, &tempRPCString);
	NodeUuid.Format(_T("%s"), tempRPCString);
	RpcStringFree(&tempRPCString);
	NodeUuid.Replace(_T("-"), _T(""));

	int length = NodeUuid.GetLength();
	char* temp = (char*)malloc(sizeof(char)*(length + 1));
	TcharToChar(NodeUuid.GetBuffer(0), temp);
	m_pMiniBlockChain2->newTransaction(1, m_strNode2Uuid, temp);
	char tempBuffer[50];
	sprintf(tempBuffer, "������Ϣ������ӵ�����{ %d }", m_pMiniBlockChain2->getBlockLength() + 1);
	TCHAR * tcharTemp = (TCHAR*)malloc(sizeof(TCHAR)*(strlen(tempBuffer) + 1));
	CharToTchar(tempBuffer, tcharTemp);
	AfxMessageBox(tcharTemp);
	free(tcharTemp);
}

void CMyMiniBlockChainView::OnNode2GetChain()
{
	//��ȡ�ڵ�2���������ݣ��������������ڵ�2��ͼ
	DrawChain2Info();
}

void CMyMiniBlockChainView::OnRegisterNode2forNode1()
{
	m_pMiniBlockChain->registerNode(m_strNode2Uuid);
	AfxMessageBox(_T("ע���������ڵ�2Ϊ�������ڵ�1��ͬ���ڵ�ɹ���"));
}

void CMyMiniBlockChainView::OnRegisterNode1ForNode2()
{
	m_pMiniBlockChain2->registerNode(m_strNode1Uuid);
	AfxMessageBox(_T("ע���������ڵ�1Ϊ�������ڵ�2��ͬ���ڵ�ɹ���"));
}


void CMyMiniBlockChainView::OnValidChain2()
{
	int ret=m_pMiniBlockChain2->valid_chain();
	if(ret == -1)
		AfxMessageBox(_T("���������ݱ��ƻ���"));
	else if (ret == 1)
		AfxMessageBox(_T("����������������"));
}


void CMyMiniBlockChainView::OnConsensusForNode1()
{
	//��ʶ�㷨�����ͻ,ʹ���������������Ϊ��ʶ
	if(m_pMiniBlockChain->getNodeRegister()==NULL)
		AfxMessageBox(_T("��ǰû��ע��ͬ���ڵ㣡"));
	else if ((m_pMiniBlockChain->getBlockLength() < m_pMiniBlockChain2->getBlockLength())
		&& (m_pMiniBlockChain2->valid_chain() == 1))
		m_pMiniBlockChain->copyChain(m_pMiniBlockChain2->getBlockChain(), m_pMiniBlockChain2->getBlockLength());
		
	Invalidate();
	UpdateWindow();
	return;
}


void CMyMiniBlockChainView::OnConsensusForNode2()
{
	//��ʶ�㷨�����ͻ,ʹ���������������Ϊ��ʶ
	if (m_pMiniBlockChain2->getNodeRegister() == NULL)
		AfxMessageBox(_T("��ǰû��ע��ͬ���ڵ㣡"));
	else if ((m_pMiniBlockChain2->getBlockLength() < m_pMiniBlockChain->getBlockLength())
		&& (m_pMiniBlockChain->valid_chain() == 1))
		m_pMiniBlockChain2->copyChain(m_pMiniBlockChain->getBlockChain(), m_pMiniBlockChain->getBlockLength()); 
	DrawChain2Info();
	return;
}
