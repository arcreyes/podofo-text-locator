#include "ObjectExtractor.h"

#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

#include <stack>

//using namespace std;


#define MAX_THREADS		3
#define BUF_SIZE		255

DWORD WINAPI MyThreadFunction(LPVOID lpParam);
void ErrorHandler(LPTSTR lpszFunction);

// Sample custom data structure for threads to use.
// This is passed by void pointer so it can be any data type
// that can be passed using a single void pointer (LPVOID).
typedef struct MyData {
	ObjectExtractor *pObjExtractor;
	PdfMemDocument *pDocument;
	const char *pszFileName;
	int iStartPage;
	int iEndPage;
} MYDATA, *PMYDATA;

ObjectExtractor::ObjectExtractor(const char *pszFilename, DocObjects *pvDocObjects)
{
	m_hMutex = CreateMutex(
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex


	if (pvDocObjects == NULL || pszFilename == NULL)
	{
		throw std::invalid_argument("ObjectExtractor: NULL parameter passed!");
	}

	m_strFilename = std::string(pszFilename);

	int start_s = clock();
	m_document.Load(pszFilename);
	int end_s = clock();
	printf("PoDoFo load took %g seconds...\n", (end_s - start_s) / (double)CLOCKS_PER_SEC);


	ClearDocObjects(pvDocObjects);
	m_pvDocObjects = pvDocObjects;

	m_iCurPageIndex = 0;
	m_iMaxPage = m_document.GetPageCount();
	m_pvDocObjects->resize(m_iMaxPage);
}


ObjectExtractor::~ObjectExtractor()
{
	//int start_s = clock();
	//delete m_pDocument;
	//int end_s = clock();
	//printf("PoDoFo cleanup took %g seconds...\n", (end_s - start_s) / (double)CLOCKS_PER_SEC);


	CloseHandle(m_hMutex);
}

int ObjectExtractor::SingleProcess(void)
{
	return ExtractDocumentObject(&m_document, 0, m_iMaxPage);
}

int ObjectExtractor::DivideAndConquer(void)
{
	int start_s = clock();

	int iPageDiv = m_iMaxPage / MAX_THREADS;

	PMYDATA pDataArray[MAX_THREADS];
	DWORD   dwThreadIdArray[MAX_THREADS];
	HANDLE  hThreadArray[MAX_THREADS];

	for (int i = 0; i < MAX_THREADS; i++)
	{
		// Allocate memory for thread data.

		pDataArray[i] = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
			sizeof(MYDATA));

		if (pDataArray[i] == NULL)
		{
			// If the array allocation fails, the system is out of memory
			// so there is no point in trying to print an error message.
			// Just terminate execution.
			ExitProcess(2);
		}

		// Generate unique data for each thread to work with.
		pDataArray[i]->pObjExtractor = this;
		pDataArray[i]->pDocument = i == 0 ? &m_document : NULL;
		pDataArray[i]->pszFileName = m_strFilename.c_str();
		pDataArray[i]->iStartPage = i * iPageDiv;
		pDataArray[i]->iEndPage = i + 1 < MAX_THREADS ? (i + 1)*iPageDiv : m_iMaxPage;

		// Create the thread to begin execution on its own.

		hThreadArray[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			MyThreadFunction,       // thread function name
			pDataArray[i],          // argument to thread function 
			0,                      // use default creation flags 
			&dwThreadIdArray[i]);   // returns the thread identifier 


		// Check the return value for success.
		// If CreateThread fails, terminate execution. 
		// This will automatically clean up threads and memory. 

		if (hThreadArray[i] == NULL)
		{
			ErrorHandler(TEXT("CreateThread"));
			ExitProcess(3);
		}
	} // End of main thread creation loop.

	// Wait until all threads have terminated.

	WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);

	// Close all thread handles and free memory allocations.

	for (int i = 0; i < MAX_THREADS; i++)
	{
		CloseHandle(hThreadArray[i]);
		if (pDataArray[i] != NULL)
		{
			HeapFree(GetProcessHeap(), 0, pDataArray[i]);
			pDataArray[i] = NULL;    // Ensure address is not reused.
		}
	}

	int end_s = clock();
	printf("extraction took %g seconds...\n", (end_s - start_s) / (double)CLOCKS_PER_SEC);

	return 0;

}

int ObjectExtractor::ExtractDocumentObject(PdfMemDocument *pDocument, const char *pszFileName, int iStartPage, int iEndPage)
{
	if (pDocument == NULL)
	{
		PdfMemDocument document(pszFileName);
		return ExtractDocumentObject(&document, iStartPage, iEndPage);
	}
	else
	{
		return ExtractDocumentObject(pDocument, iStartPage, iEndPage);
	}
	
}

int ObjectExtractor::ExtractDocumentObject(PdfMemDocument *pDocument, int iStartPage, int iEndPage)
{
	int iDocSize = pDocument->GetPageCount();

	if (iStartPage < 0 || iStartPage > iDocSize)
	{
		return -1;
	}

	if (iEndPage < 0 || iEndPage > iDocSize)
	{
		return -1;
	}

	if (iEndPage < iStartPage)
	{
		return -1;
	}

	int i = GetCurPage();
	while (i != -1)
	{
		PdfPage *pPage = pDocument->GetPage(i);
		if (pPage == NULL)
		{
			printf("ERROR! page not found\n");
			return -1;
		}

		PageObjects *pPageObjects = new (std::nothrow) PageObjects;
		if (pPageObjects == NULL)
		{
			printf("FATAL! not enough memory\n");
			return -1;
		}


		ExtractPageObjects(pDocument, pPage, i, pPageObjects);

		AddPageObject(i, pPageObjects);

		i = GetCurPage();
	}

	return 0;
}

int ObjectExtractor::ExtractPageObjects(PdfMemDocument *pDocument, PdfPage *pPage, int iPageIndex, PageObjects *pPageObjects)
{
	const char*      pszToken = NULL;
	PdfVariant       var;
	EPdfContentsType eType;

	PdfContentsTokenizer tokenizer(pPage);

	double dCurPosX = 0.0;
	double dCurPosY = 0.0;

	bool   bTextBlock = false;

	double fontSize = 0.0;
	PdfFont* pCurFont = NULL;
	PdfName fontName;

	Matrix Tm;
	Matrix cm;

	double dTL = 0.0;

	std::stack<PdfVariant> stack;
	std::stack<Matrix> stackCm;

	double Tw = 0.0;
	double Tc = 0.0;

	while (tokenizer.ReadNext(eType, pszToken, var))
	{
		if (eType == ePdfContentsType_Keyword)
		{
			// support 'l' and 'm' tokens
			if (strcmp(pszToken, "l") == 0 ||
				strcmp(pszToken, "m") == 0)
			{
				dCurPosX = stack.top().GetReal();
				stack.pop();
				dCurPosY = stack.top().GetReal();
				stack.pop(); 
			}
			else if (strcmp(pszToken, "BT") == 0)
			{
				bTextBlock = true;
			}
			else if (strcmp(pszToken, "ET") == 0)
			{
				if (!bTextBlock)
					fprintf(stderr, "WARNING: Found ET without BT!\n");

				bTextBlock = false;
			}
			else if (strcmp(pszToken, "q") == 0)
			{
				stackCm.push(cm);
			}
			else if (strcmp(pszToken, "Q") == 0)
			{
				cm = stackCm.top();
				stackCm.pop();
			}
			else if (strcmp(pszToken, "cm") == 0)
			{
				double a, b, c, d, e, f;
				f = stack.top().GetReal();
				stack.pop();
				e = stack.top().GetReal();
				stack.pop();
				d = stack.top().GetReal();
				stack.pop();
				c = stack.top().GetReal();
				stack.pop();
				b = stack.top().GetReal();
				stack.pop();
				a = stack.top().GetReal();
				stack.pop();

				cm.SetMatrix(a, b, c, d, e, f);
			}

			else if (strcmp(pszToken, "Do") == 0)
			{
				PdfName imageName = stack.top().GetName();
				stack.pop();

				PdfObject *pObj = pPage->GetFromResources(PdfName("XObject"), imageName);
				if (!pObj)
				{
					PODOFO_RAISE_ERROR_INFO(ePdfError_InvalidHandle, "Cannot create font!");
				}

				PdfObject *pObjSubType = pObj->GetDictionary().GetKey(PdfName::KeySubtype);
				if (pObjSubType && pObjSubType->IsName() &&
					(pObjSubType->GetName().GetName() == "Image"))
				{
					ImageObj *pObj = new (std::nothrow) ImageObj(imageName, cm);
					if (pObj == NULL)
					{
						printf("FATAL! no more memory\n");
						return -1;
					}

					pPageObjects->push_back(pObj);
				}

			}

			if (bTextBlock)
			{
				if (strcmp(pszToken, "Tf") == 0)
				{
					fontSize = stack.top().GetReal();
					stack.pop();

					fontName = stack.top().GetName();
					PdfObject* pFont = pPage->GetFromResources(PdfName("Font"), fontName);
					if (!pFont)
					{
						PODOFO_RAISE_ERROR_INFO(ePdfError_InvalidHandle, "Cannot create font!");
					}

					pCurFont = pDocument->GetFont(pFont);
					if (!pCurFont)
					{
						fprintf(stderr, "WARNING: Unable to create font for object %i %i R\n",
							pFont->Reference().ObjectNumber(),
							pFont->Reference().GenerationNumber());
					}
				}
				else if (strcmp(pszToken, "'") == 0)
				{
					Tm.Translate(0, dTL);

					TextObj *pObj = new (std::nothrow) TextObj(stack.top().GetString(), pCurFont, fontSize, fontName, Tm, Tw, Tc);
					if (pObj == NULL)
					{
						printf("FATAL! no more memory\n");
						return -1;
					}

					stack.pop();

					pPageObjects->push_back(pObj);
				}
				else if (strcmp(pszToken, "Tj") == 0)
				{
					TextObj *pObj = new (std::nothrow) TextObj(stack.top().GetString(), pCurFont, fontSize, fontName, Tm, Tw, Tc);
					if (pObj == NULL)
					{
						printf("FATAL! no more memory\n");
						return -1;
					}
					stack.pop();

					pPageObjects->push_back(pObj);
				}
				else if (strcmp(pszToken, "\"") == 0)
				{
					TextObj *pObj = new (std::nothrow) TextObj(stack.top().GetString(), pCurFont, fontSize, fontName, Tm, Tw, Tc);
					if (pObj == NULL)
					{
						printf("FATAL! no more memory\n");
						return -1;
					}
					stack.pop();

					Tc = stack.top().GetReal();
					stack.pop(); // remove char spacing from stack
					Tw = stack.top().GetReal();
					stack.pop(); // remove word spacing from stack

					pObj->SetCharacterSpacing(Tc);
					pObj->SetWordSpacing(Tw);

					Tm.Translate(0, dTL);

					pPageObjects->push_back(pObj);
				}
				else if (strcmp(pszToken, "TJ") == 0)
				{
					PdfArray pdfArray = stack.top().GetArray();
					stack.pop();

					double dX = Tm.GetE();
					Matrix Tm2 = Tm;

					for (int i = 0; i < pdfArray.size(); i++)
					{
						if (pdfArray[i].IsNumber())
						{
							dX = (dX - ((pdfArray[i].GetNumber() / 1000.0) * Tm.GetA()));
						}
						else if (pdfArray[i].IsString() || pdfArray[i].IsHexString())
						{
							Tm2.SetMatrix(Tm.GetA(),
								Tm.GetB(),
								Tm.GetC(),
								Tm.GetD(),
								dX,
								Tm.GetF());

							TextObj *pObj = new (std::nothrow) TextObj(pdfArray[i].GetString(), pCurFont, fontSize, fontName, Tm2, Tw, Tc);
							if (pObj == NULL)
							{
								printf("FATAL! no more memory\n");
								return -1;
							}
							pPageObjects->push_back(pObj);

							dX += pCurFont->GetFontMetrics()->StringWidth(pdfArray[i].GetString());
						}
					}

				}
				else if (strcmp(pszToken, "Tm") == 0)
				{
					double a, b, c, d, e, f;
					f = stack.top().GetReal();
					stack.pop();
					e = stack.top().GetReal();
					stack.pop();
					d = stack.top().GetReal();
					stack.pop();
					c = stack.top().GetReal();
					stack.pop();
					b = stack.top().GetReal();
					stack.pop();
					a = stack.top().GetReal();
					stack.pop();

					Tm.SetMatrix(a, b, c, d, e, f);

				}
				else if (strcmp(pszToken, "Td") == 0)
				{
					double dTy = stack.top().GetReal();
					stack.pop();
					double dTx = stack.top().GetReal();
					stack.pop();

					Tm.Translate(dTx, dTy);
				}
				else if (strcmp(pszToken, "TD") == 0)
				{
					double dTy = stack.top().GetReal();
					stack.pop();
					double dTx = stack.top().GetReal();
					stack.pop();

					dTL = dTy;

					Tm.Translate(dTx, dTy);
				}
				else if (strcmp(pszToken, "T*") == 0)
				{
					Tm.Translate(0, dTL);
				}
				else if (strcmp(pszToken, "Tw") == 0)
				{
					Tw = stack.top().GetReal();
					stack.pop(); // remove word spacing from stack
				}
				else if (strcmp(pszToken, "Tc") == 0)
				{
					Tc = stack.top().GetReal();
					stack.pop(); // remove char spacing from stack
				}
			}
		}
		else if (eType == ePdfContentsType_Variant)
		{
			stack.push(var);
		}
		else
		{
			// Impossible; type must be keyword or variant
			PODOFO_RAISE_ERROR(ePdfError_InternalLogic);
		}
	}

	return 0;
}

void ObjectExtractor::PrintData()
{
	//int iCount = (int)m_viPages.size();

	//for (int i = 0; i < iCount; i++)
	//{
	//	printf("page(%d)\n", m_viPages[i]);
	//}
}

void ObjectExtractor::ClearDocObjects(DocObjects *pvDocObjects)
{
	int start_s = clock();
	printf("cleanup...\n");
	while (!pvDocObjects->empty())
	{
		//printf("remaining objects (%d)\n", m_vDocObjects.size());

		PageObjects *pPageObjects = pvDocObjects->back();


		while (!pPageObjects->empty())
		{
			ObjectBase *pObj = pPageObjects->back();
			delete pObj;
			pObj = NULL;

			pPageObjects->pop_back();
		}
		delete pPageObjects;
		pPageObjects = NULL;

		pvDocObjects->pop_back();

	}
	int end_s = clock();
	printf("cleanup took %g seconds...\n", (end_s - start_s) / (double)CLOCKS_PER_SEC);
}

int ObjectExtractor::AddPageObject(int iPageIndex, PageObjects *pPageObjects)
{
	DWORD dwWaitResult = WaitForSingleObject(
		m_hMutex,    // handle to mutex
		INFINITE);  // no time-out interval

	switch (dwWaitResult)
	{
		// The thread got ownership of the mutex
	case WAIT_OBJECT_0:
		__try {
			// TODO: Write to the database
			//printf("Thread %d writing to database...\n",
			//	GetCurrentThreadId());

			//printf("[%d] Add Page %d\n", GetCurrentThreadId(), iPageIndex);
			m_pvDocObjects->at(iPageIndex) = pPageObjects;
		}

		__finally {
			// Release ownership of the mutex object
			if (!ReleaseMutex(m_hMutex))
			{
				// Handle error.
			}
		}
		break;

		// The thread got ownership of an abandoned mutex
		// The database is in an indeterminate state
	case WAIT_ABANDONED:
		return -1;
	}

	return 0;
}


int ObjectExtractor::GetCurPage()
{
	DWORD dwWaitResult;
	int iCurPage = 0;

	dwWaitResult = WaitForSingleObject(
		m_hMutex,    // handle to mutex
		INFINITE);  // no time-out interval

	switch (dwWaitResult)
	{
		// The thread got ownership of the mutex
	case WAIT_OBJECT_0:
		__try {
			// TODO: Write to the database
			//printf("Thread %d writing to database...\n",
			//	GetCurrentThreadId());

			iCurPage = m_iCurPageIndex;
			m_iCurPageIndex++;

			if (iCurPage >= m_iMaxPage)
			{
				iCurPage = -1;
			}

		}

		__finally {
			// Release ownership of the mutex object
			if (!ReleaseMutex(m_hMutex))
			{
				// Handle error.
			}
		}
		break;

		// The thread got ownership of an abandoned mutex
		// The database is in an indeterminate state
	case WAIT_ABANDONED:
		return -1;
	}

	return iCurPage;
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	HANDLE hStdout;
	PMYDATA pDataArray;

	TCHAR msgBuf[BUF_SIZE];
	size_t cchStringSize;
	DWORD dwChars;

	// Make sure there is a console to receive output results. 

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;

	// Cast the parameter to the correct data type.
	// The pointer is known to be valid because 
	// it was checked for NULL before the thread was created.

	pDataArray = (PMYDATA)lpParam;


	// Print the parameter values using thread-safe functions.
	StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Analyze = %d, %d\n"),
		pDataArray->iStartPage, pDataArray->iEndPage);
	StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
	WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);


	pDataArray->pObjExtractor->ExtractDocumentObject(pDataArray->pDocument, pDataArray->pszFileName, pDataArray->iStartPage, pDataArray->iEndPage);


	return 0;
}

void ErrorHandler(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code.

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message.

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	// Free error-handling buffer allocations.

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}