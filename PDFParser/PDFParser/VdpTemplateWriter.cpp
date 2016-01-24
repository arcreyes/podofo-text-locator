#include "VdpTemplateWriter.h"


VdpTemplateWriter::VdpTemplateWriter(PdfMemDocument *pSourceDocument)
	:
	m_pSourceDocument(pSourceDocument)
{
	m_pTargetDocument = new PdfMemDocument();
	m_pTargetDocument->SetPdfVersion(m_pSourceDocument->GetPdfVersion());

	PdfInfo *sInfo(m_pSourceDocument->GetInfo());
	PdfInfo *tInfo(m_pTargetDocument->GetInfo());

	if (sInfo->GetAuthor() != PdfString::StringNull)
		tInfo->SetAuthor(sInfo->GetAuthor());
	if (sInfo->GetCreator() != PdfString::StringNull)
		tInfo->SetCreator(sInfo->GetCreator());
	if (sInfo->GetSubject() != PdfString::StringNull)
		tInfo->SetSubject(sInfo->GetSubject());
	if (sInfo->GetTitle() != PdfString::StringNull)
		tInfo->SetTitle(sInfo->GetTitle());
	if (sInfo->GetKeywords() != PdfString::StringNull)
		tInfo->SetKeywords(sInfo->GetKeywords());

	if (sInfo->GetTrapped() != PdfName::KeyNull)
		tInfo->SetTrapped(sInfo->GetTrapped());
}


VdpTemplateWriter::~VdpTemplateWriter()
{
	if (m_pTargetDocument)
		delete m_pTargetDocument;
}

int VdpTemplateWriter::Write(const char *pszTemplateFilePath, VdpDocument *pVdpDocument, int templatePageCount)
{
	int iRet = 0;
	int pageCount = m_pSourceDocument->GetPageCount();

	if (templatePageCount > pageCount)
	{
		return -1;
	}

	for (int i = 0; i < templatePageCount; i++)
	{
		PdfPage *pSrcPage = m_pSourceDocument->GetPage(i);
		if (pSrcPage == NULL)
		{
			return -2;
		}

		PdfPage *pDestPage = m_pTargetDocument->CreatePage(pSrcPage->GetPageSize());
		if (pDestPage == NULL)
		{
			return -2;
		}

		iRet = this->CopyPage(pSrcPage, pDestPage);
		if (iRet < 0)
		{
			break;
		}

		//iRet = this->RemoveVariableData(pDestPage, pVdpDocument->GetPage(i));
		//if (iRet < 0)
		//{
		//	break;
		//}
	}

	m_pTargetDocument->Write(pszTemplateFilePath);

	return iRet;
}

int VdpTemplateWriter::CopyPage(PdfPage *pSrcPage, PdfPage *pDestPage)
{
	int iRet = 0;

	if (pSrcPage->GetObject()->IsDictionary())
	{
		TKeyMap resmap = pSrcPage->GetObject()->GetDictionary().GetKeys();
		for (TCIKeyMap itres = resmap.begin(); itres != resmap.end(); ++itres)
		{
			if (itres->first == PdfName("Parent"))
				continue;

			PdfObject *o = itres->second;
			pDestPage->GetObject()->GetDictionary().AddKey(itres->first, this->MigrateResource(o));
		}

		if (pSrcPage->GetObject()->HasStream())
		{
			*(pDestPage->GetObject()->GetStream()) = *(pSrcPage->GetObject()->GetStream());
		}
	}
	else
	{
		iRet = -3;
		//This is not possible!
	}

	return iRet;
}


PdfObject* VdpTemplateWriter::MigrateResource(PdfObject * obj)
{
	PdfObject *ret(0);

	if (obj->IsDictionary())
	{
		ret = m_pTargetDocument->GetObjects().CreateObject(*obj);

		TKeyMap resmap = obj->GetDictionary().GetKeys();
		for (TCIKeyMap itres = resmap.begin(); itres != resmap.end(); ++itres)
		{
			PdfObject *o = itres->second;
			ret->GetDictionary().AddKey(itres->first, this->MigrateResource(o));
		}

		if (obj->HasStream())
		{
			*(ret->GetStream()) = *(obj->GetStream());
		}
	}
	else if (obj->IsArray())
	{
		PdfArray carray(obj->GetArray());
		PdfArray narray;
		for (unsigned int ci = 0; ci < carray.GetSize(); ++ci)
		{
			PdfObject *co(this->MigrateResource(&carray[ci]));
			narray.push_back(*co);
		}
		ret = m_pTargetDocument->GetObjects().CreateObject(narray);
	}
	else if (obj->IsReference())
	{
		if (m_map.find(obj->GetReference().ToString()) != m_map.end())
		{
			return new PdfObject(m_map[obj->GetReference().ToString()]->Reference());
		}

		PdfObject * o(this->MigrateResource(m_pSourceDocument->GetObjects().GetObject(obj->GetReference())));

		ret = new PdfObject(o->Reference());
	}
	else
	{
		ret = new PdfObject(*obj);
	}


	m_map.insert(std::pair<std::string, PdfObject*>(obj->Reference().ToString(), ret));


	return ret;

}

int VdpTemplateWriter::RemoveVariableData(PdfPage *pPdfPage, VdpPage *pVdpPage)
{
	int iRet = 0;

	return iRet;
}


