#pragma once
#include "wx/wx.h"

class Main : public wxFrame
{
public:
	Main();
	~Main();

public:
	//Labels
	wxStaticText* m_stxt_Vendor = nullptr;
	wxStaticText* m_stxt_Brand = nullptr;
	wxStaticText* m_stxt_SteppingID = nullptr;
	wxStaticText* m_stxt_Model = nullptr;
	wxStaticText* m_stxt_FamilyID = nullptr;
	wxStaticText* m_stxt_ExtModel = nullptr;
	wxStaticText* m_stxt_ExtFamily = nullptr;
	wxStaticText* m_stxt_Instrucitons = nullptr;
	wxStaticText* m_stxt_PhysicalProc = nullptr;
	wxStaticText* m_stxt_LogicalProc = nullptr;
	wxStaticText* m_stxt_LogicalPerCore = nullptr;
	wxStaticText* m_stxt_RamCapacity = nullptr;
	wxStaticText* m_stxt_InUse = nullptr;
	wxStaticText* m_stxt_FreeMem = nullptr;

	//Text Boxes
	wxTextCtrl* m_txt_Vendor = nullptr;
	wxTextCtrl* m_txt_Brand = nullptr;
	wxTextCtrl* m_txt_SteppingID = nullptr;
	wxTextCtrl* m_txt_Model = nullptr;
	wxTextCtrl* m_txt_FamilyID = nullptr;
	wxTextCtrl* m_txt_ExtModel = nullptr;
	wxTextCtrl* m_txt_ExtFamily = nullptr;
	wxTextCtrl* m_txt_Instrucitons = nullptr;
	wxTextCtrl* m_txt_PhysicalProc = nullptr;
	wxTextCtrl* m_txt_LogicalProc = nullptr;
	wxTextCtrl* m_txt_LogicalPerCore = nullptr;
	wxTextCtrl* m_txt_RamCapacity = nullptr;
	wxTextCtrl* m_txt_InUse = nullptr;
	wxTextCtrl* m_txt_FreeMem = nullptr;

	//Button
	wxButton* m_btn_run_bech = nullptr;

	//Sizer
	wxBoxSizer* m_box = nullptr;

	void OnButtonClicked(wxCommandEvent& e);

public:
	void updateMem();
	void onClose(wxCloseEvent& evt);

private:
	void initializeSTXT();
	void initializeTXT();
	void initializeBTN();
	void initializeSizer();
};