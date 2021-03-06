/* THIS FILE IS PART OF *SIREN* SOURCE CODES.
 * WHEN YOU WANT THE LICENSE OF THIS FILE, YOU CAN GET IT
 * IN README OR LICENSE DOCMENTATION OF THIS PROJECT.
 * ---------------------------------------------------------
 * AUTHOR: dyama <dyama@member.fsf.org>
 */

#pragma once
#using <mscorlib.dll>
#include "OCCViewer.h"

namespace sirenenv
{

public ref class Viewer
{

private:
	OCCViewer* myOCCViewer;
public:

	bool InitViewer(System::IntPtr wnd)
	{
		if (myOCCViewer != NULL)
			return myOCCViewer->InitViewer(wnd.ToPointer());
		else
			return false;
	}

	bool ImportBrep(System::String^ filename)
	{
		if (myOCCViewer == NULL)
			return false;
		int length = filename->Length;
		wchar_t* fname = new wchar_t[length+1];
		for(int i = 0; i<length; i++)
			fname[i] = (wchar_t)filename->ToCharArray()[i];

		fname[length] = '\0';

		bool res = myOCCViewer->ImportBRep(fname);
		delete [] fname;
		return res;
	}

	void RedrawView(void)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->RedrawView();
	}

	void UpdateView(void)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->UpdateView();
	}

	void SetDegenerateModeOn(void)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->SetDegenerateModeOn();
	}

	void SetDegenerateModeOff(void)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->SetDegenerateModeOff();
	}

	void WindowFitAll(int Xmin, int Ymin, int Xmax, int Ymax)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->WindowFitAll(Xmin, Ymin, Xmax, Ymax);
	}

	void Place(int x, int y, float zoomFactor)
	{	
		if (myOCCViewer != NULL)
			myOCCViewer->Place(x, y, zoomFactor);
	}

	void Zoom(int x1, int y1, int x2, int y2)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->Zoom(x1, y1, x2, y2);
	}

	void Pan(int x, int y)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->Pan(x, y);
	}

	void Rotation(int x, int y)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->Rotation(x, y);
	}

	void StartRotation(int x, int y)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->StartRotation(x, y);
	}

	void Select(int x1, int y1, int x2, int y2)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->Select(x1, y1, x2, y2);
	}

	void Select(void)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->Select();
	}

	void MoveTo(int x, int y)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->MoveTo(x, y);
	}

	void ShiftSelect(int x1, int y1, int x2, int y2)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->ShiftSelect(x1, y1, x2, y2);
	}

	void ShiftSelect(void)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->ShiftSelect();
	}

	int GetBGColR(void)
	{
		int r, b, g;
		if (myOCCViewer != NULL)
			myOCCViewer->BackgroundColor(r,g,b);
		return r;
	}

	int GetBGColG(void)
	{
		int r, b, g;
		if (myOCCViewer != NULL)
			myOCCViewer->BackgroundColor(r,g,b);
		return g;
	}

	int GetBGColB(void)
	{
		int r, b, g;
		if (myOCCViewer != NULL)
			myOCCViewer->BackgroundColor(r,g,b);
		return b;
	}

	void UpdateCurrentViewer(void)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->UpdateCurrentViewer();
	}

	void setProjection(TypeOfOrientation dir)
	{
		if (myOCCViewer != NULL) {
			V3d_TypeOfOrientation o = static_cast<V3d_TypeOfOrientation>(dir);
			myOCCViewer->setProjection(o);
		}
	}

	float Scale(void)
	{
		if (myOCCViewer != NULL)
			return myOCCViewer->Scale();
		else
			return -1;
	}

	void SetScale(double f)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->setScale(f);
	}

	void ZoomAllView(void)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->ZoomAllView();
	}

	void Reset(void)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->ResetView();
	}

	void SetDisplayMode(int aMode)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->SetDisplayMode(aMode);
	}

	void SetColor(int r, int g, int b)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->SetColor(r, g, b);
	}

	int GetObjColR(void)
	{
		int r, g, b;
		if (myOCCViewer != NULL)
			myOCCViewer->ObjectColor(r, g, b);
		return r;
	}

	int GetObjColG(void)
	{
		int r, g, b;
		if (myOCCViewer != NULL)
			myOCCViewer->ObjectColor(r, g, b);
		return g;
	}

	int GetObjColB(void)
	{
		int r, g, b;
		if (myOCCViewer != NULL)
			myOCCViewer->ObjectColor(r, g, b);
		return b;
	}

	void SetBackgroundColor(int r, int g, int b)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->SetBackgroundColor(r, g, b);
	}

	void EraseObjects(void)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->EraseObjects();
	}

	float GetOCCVersion(void)
	{
		if (myOCCViewer == NULL)
			return 0.0;
		else
			return myOCCViewer->GetVersion();
	}

	void SetMaterial(int theMaterial)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->SetMaterial(theMaterial);
	}

	void SetTransparency(int TheTrans)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->SetTransparency(TheTrans);
	}

	void SetAntialiasing(bool isOn)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->SetAntialiasing(isOn);
	}

	bool TranslateModel(System::String^ filename, int format, bool IsImport)
	{
		if (myOCCViewer == NULL)
			return false;

		int length = filename->Length;

		wchar_t* fname = new wchar_t[length+1];
		for(int i = 0; i<length; i++)
		    fname[i] = (wchar_t)filename->ToCharArray()[i];

		fname[length] = L'\0';
		bool res;
		if (IsImport)
		{
			switch(format)
			{
				case 0:
					res=myOCCViewer->ImportBRep(fname);
					break;
				case 1:
					res=myOCCViewer->ImportCsfdb(fname);
					break;
				case 2:
					res=myOCCViewer->ImportStep(fname);
					break;
				case 3:
					res=myOCCViewer->ImportIges(fname);
					break;
				default:
					res=false;
			}
		} else 
		{
			switch(format)
			{
				case 0:
					res=myOCCViewer->ExportBRep(fname);
					break;
				case 2:
					res=myOCCViewer->ExpotStep(fname);
					break;
				case 3:
					res=myOCCViewer->ExportIges(fname);
					break;
				case 4:
					res=myOCCViewer->ExportVrml(fname);
					break;
				case 5:
					res=myOCCViewer->ExportStl(fname);
					break;
				case 6:
					res=myOCCViewer->Dump(fname);
					break;
				default:
					res=false;
			}
		}
		delete [] fname;
		return res;
	}

	bool IsObjectSelected(void)
	{
		if (myOCCViewer != NULL)
			return myOCCViewer->IsObjectSelected();
		else
			return false;
	}

	int DisplayMode(void)
	{
		if (myOCCViewer != NULL)
			return myOCCViewer->DisplayMode();
		else return -1;
	}

	void CreateNewView(System::IntPtr wnd)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->CreateNewView(wnd.ToPointer());
	}

	bool SetAISContext(Viewer^ v)
	{
		if (myOCCViewer != NULL)
			return this->myOCCViewer->SetAISContext(v->GetOCCViewer());
		else
			return false;
	}

	OCCViewer* GetOCCViewer(void)
	{
		return myOCCViewer;
	}

	void InitOCCViewer(void)
	{
		myOCCViewer = new OCCViewer();
	}

	int CharToInt(System::String^ symbol)
	{
		if (myOCCViewer == NULL)
			return -1;
    char s = (char)symbol->ToCharArray()[0];
		return myOCCViewer->CharToInt(s);
	}

	void setHighlightColor(NameOfColor color)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->SetHighlightColor(static_cast<Quantity_NameOfColor>(color));
		return;
	}

	void setSelectionColor(NameOfColor color)
	{
		if (myOCCViewer != NULL)
			myOCCViewer->SetSelectionColor(static_cast<Quantity_NameOfColor>(color));
		return;
	}

	// mruby

	int mruby_exec(System::String^ str,[System::Runtime::InteropServices::Out] System::String^% errmsg)
	{

		if (myOCCViewer == NULL || myOCCViewer->myMirb == NULL)
			return -1;

		char cmd[1024] = {0};

		for (int i=0; i<str->Length; i++) {
			cmd[i] = (char)str->ToCharArray()[i];
		}

		std::string _errmsg;
		int res = myOCCViewer->mruby_exec(cmd, _errmsg);

		errmsg = gcnew System::String(_errmsg.c_str());

		return res;
	}

	System::String^ mruby_p()
	{
		System::String^ result = System::String::Empty;

		if (!myOCCViewer)
			return result;

		std::string res;
		myOCCViewer->myMirb->p(1, res);	

		return gcnew System::String(res.c_str());
	}

	bool mruby_isCodeBlockOpen()
	{
		if (!myOCCViewer)
			return false;

		return myOCCViewer->myMirb->isCodeBlockOpen();
	}
};

}
