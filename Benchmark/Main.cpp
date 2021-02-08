#include "Main.h"
#include "GUI.h"
#include "CPU_info.h"
#include "MEM_info.h"
#include "CPU_test.h"
#include "MEM_test.h"
#include <string.h>
#include <array>
#include <vector>

wxIMPLEMENT_APP(GUI);

void Main::initializeSTXT() 
{
	m_stxt_Vendor = new wxStaticText(this, wxID_ANY, "Vendor: ");
	m_stxt_Brand = new wxStaticText(this, wxID_ANY, "Brand: ");
	m_stxt_SteppingID = new wxStaticText(this, wxID_ANY, "Stepping: ");
	m_stxt_Model = new wxStaticText(this, wxID_ANY, "Model: ");
	m_stxt_FamilyID = new wxStaticText(this, wxID_ANY, "Family: ");
	m_stxt_ExtModel = new wxStaticText(this, wxID_ANY, "Ext. Model: ");
	m_stxt_ExtFamily = new wxStaticText(this, wxID_ANY, "Ext. Family: ");
	m_stxt_Instrucitons = new wxStaticText(this, wxID_ANY, "Instruictions: ");
	m_stxt_PhysicalProc = new wxStaticText(this, wxID_ANY, "Cores: ");
	m_stxt_LogicalProc = new wxStaticText(this, wxID_ANY, "Threads: ");
	m_stxt_LogicalPerCore = new wxStaticText(this, wxID_ANY, "Threads/Core: ");
	m_stxt_RamCapacity = new wxStaticText(this, wxID_ANY, "Ram Capacity: ");
	m_stxt_InUse = new wxStaticText(this, wxID_ANY, "In Use (%): ");
	m_stxt_FreeMem = new wxStaticText(this, wxID_ANY, "Free Memory: ");
}

void Main::initializeTXT()
{
	std::string vendor{ CPUID_getVendor() };
	std::string brand{ CPUID_getBrand() };
	std::array<unsigned, 5> proc_version{ CPUID_getProcessorVersion() };
	std::vector<std::string> proc_instr{ CPUID_getInstructions() };
	std::array<int, 3> proc_topology{ CPUID_getTopology(vendor) };

	char buffer[50];
	std::string instructions = "";
	
	m_txt_Vendor = new wxTextCtrl(this, wxID_ANY, vendor);
	m_txt_Vendor->SetEditable(false);

	m_txt_Brand = new wxTextCtrl(this, wxID_ANY, brand);
	m_txt_Brand->SetEditable(false);

	sprintf(buffer, "%X", proc_version[0]);
	m_txt_SteppingID = new wxTextCtrl(this, wxID_ANY, buffer);
	m_txt_SteppingID->SetEditable(false);

	sprintf(buffer, "%X", proc_version[1]);
	m_txt_Model = new wxTextCtrl(this, wxID_ANY, buffer);
	m_txt_Model->SetEditable(false);

	sprintf(buffer, "%X", proc_version[2]);
	m_txt_FamilyID = new wxTextCtrl(this, wxID_ANY, buffer);
	m_txt_FamilyID->SetEditable(false);

	sprintf(buffer, "%X", proc_version[3]);
	m_txt_ExtModel = new wxTextCtrl(this, wxID_ANY, buffer);
	m_txt_ExtModel->SetEditable(false);

	sprintf(buffer, "%X", proc_version[4]);
	m_txt_ExtFamily = new wxTextCtrl(this, wxID_ANY, buffer);
	m_txt_ExtFamily->SetEditable(false);

	for (int i = 0; i < proc_instr.size(); i++)
	{
		instructions.append(proc_instr.at(i));
		instructions.append(" ");
	}
	m_txt_Instrucitons = new wxTextCtrl(this, wxID_ANY, instructions, wxPoint(-1, -1), wxSize(-1, -1), wxTE_MULTILINE);
	m_txt_Instrucitons->SetEditable(false);

	m_txt_PhysicalProc = new wxTextCtrl(this, wxID_ANY, std::to_string(proc_topology[0]));
	m_txt_PhysicalProc->SetEditable(false);

	m_txt_LogicalProc = new wxTextCtrl(this, wxID_ANY, std::to_string(proc_topology[1]));
	m_txt_LogicalProc->SetEditable(false);

	m_txt_LogicalPerCore = new wxTextCtrl(this, wxID_ANY, std::to_string(proc_topology[2]));
	m_txt_LogicalPerCore->SetEditable(false);
	
	sprintf(buffer, "%g GB", MEM_getCapacityGB());
	m_txt_RamCapacity = new wxTextCtrl(this, wxID_ANY, buffer);
	m_txt_RamCapacity->SetEditable(false);

	sprintf(buffer, "%d", MEM_getPrecentageMemInUse());
	m_txt_InUse = new wxTextCtrl(this, wxID_ANY, buffer);
	m_txt_InUse->SetEditable(false);
	
	sprintf(buffer, "%g GB", MEM_getFreeMemGB());
	m_txt_FreeMem = new wxTextCtrl(this, wxID_ANY, buffer);
	m_txt_FreeMem->SetEditable(false);
}

void Main::initializeBTN()
{
	m_btn_run_bech = new wxButton(this, wxID_ANY, "Run Benchmark");
	m_btn_run_bech->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Main::OnButtonClicked, this);
}

void Main::initializeSizer()
{
	m_box = new wxBoxSizer(wxHORIZONTAL);

	wxFlexGridSizer* fgs1 = new wxFlexGridSizer(11, 2, 0, 0);
	wxFlexGridSizer* fgs2 = new wxFlexGridSizer(7, 2, 0, 0);

	wxBoxSizer* box1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* box2 = new wxBoxSizer(wxHORIZONTAL);
	
	wxFlexGridSizer* grid = new wxFlexGridSizer(3, 1, 9, 25);

	fgs1->Add(m_stxt_Vendor, 1, wxEXPAND | wxALL);
	fgs1->Add(m_txt_Vendor, 1, wxEXPAND | wxALL);

	fgs1->Add(m_stxt_Brand, 1, wxEXPAND | wxALL);
	fgs1->Add(m_txt_Brand, 1, wxEXPAND | wxALL);

	fgs1->Add(m_stxt_SteppingID, 1, wxEXPAND | wxALL);
	fgs1->Add(m_txt_SteppingID, 1, wxEXPAND | wxALL);

	fgs1->Add(m_stxt_Model, 1, wxEXPAND | wxALL);
	fgs1->Add(m_txt_Model, 1, wxEXPAND | wxALL);

	fgs1->Add(m_stxt_FamilyID, 1, wxEXPAND | wxALL);
	fgs1->Add(m_txt_FamilyID, 1, wxEXPAND | wxALL);

	fgs1->Add(m_stxt_ExtModel, 1, wxEXPAND | wxALL);
	fgs1->Add(m_txt_ExtModel, 1, wxEXPAND | wxALL);

	fgs1->Add(m_stxt_ExtFamily, 1, wxEXPAND | wxALL);
	fgs1->Add(m_txt_ExtFamily, 1, wxEXPAND | wxALL);

	fgs1->Add(m_stxt_Instrucitons, 1, wxEXPAND | wxALL);
	fgs1->Add(m_txt_Instrucitons, 1, wxEXPAND | wxALL);

	fgs1->Add(m_stxt_PhysicalProc, 1, wxEXPAND | wxALL);
	fgs1->Add(m_txt_PhysicalProc, 1, wxEXPAND | wxALL);

	fgs1->Add(m_stxt_LogicalProc, 1, wxEXPAND | wxALL);
	fgs1->Add(m_txt_LogicalProc, 1, wxEXPAND | wxALL);

	fgs1->Add(m_stxt_LogicalPerCore, 1, wxEXPAND | wxALL);
	fgs1->Add(m_txt_LogicalPerCore, 1, wxEXPAND | wxALL);

	fgs2->Add(m_stxt_RamCapacity, 1, wxEXPAND | wxALL);
	fgs2->Add(m_txt_RamCapacity, 1, wxEXPAND | wxALL);

	fgs2->Add(m_stxt_InUse, 1, wxEXPAND | wxALL);
	fgs2->Add(m_txt_InUse, 1, wxEXPAND | wxALL);

	fgs2->Add(m_stxt_FreeMem, 1, wxEXPAND | wxALL);
	fgs2->Add(m_txt_FreeMem, 1, wxEXPAND | wxALL);

	fgs1->AddGrowableRow(7, 1);
	fgs1->AddGrowableCol(1, 1);

	fgs2->AddGrowableCol(1, 1);

	box1->Add(fgs1, 1, wxEXPAND | wxALL, 15);
	box2->Add(fgs2, 1, wxEXPAND | wxALL, 15);
	
	grid->Add(box1, 1, wxEXPAND | wxALL);
	grid->Add(box2, 1, wxEXPAND | wxALL);
	grid->Add(m_btn_run_bech, 1, wxEXPAND | wxALL);

	grid->AddGrowableCol(0, 1);

	m_box->Add(grid, 1, wxEXPAND | wxALL, 15);
}

void Main::updateMem()
{
	char buffer[50];

	sprintf(buffer, "%d", MEM_getPrecentageMemInUse());
	m_txt_InUse->SetLabel(buffer);

	sprintf(buffer, "%g GB", MEM_getFreeMemGB());
	m_txt_FreeMem->SetLabel(buffer);
}

Main::Main() : wxFrame(nullptr, wxID_ANY, "Benchmark", wxPoint(50, 50), wxSize(450, 650))
{
	initializeSTXT();
	initializeTXT();
	initializeBTN();

	initializeSizer();

	this->SetSizer(m_box);
}

Main::~Main()
{
}

void Main::OnButtonClicked(wxCommandEvent& e)
{
	char results[100];

	m_btn_run_bech->Enable(false);

	m_btn_run_bech->SetLabel("Running tests...");

	long score_cpu = CPU_run_test();
	long score_mem = MEM_run_test();

	m_btn_run_bech->SetLabel("Benchmark complete...");

	long score = score_cpu + score_mem;
	
	sprintf(results, "CPU test score: %d\nMem test score: %d\n\nBenchmark score: %d", score_cpu, score_mem, score);
	
	wxMessageBox(results);

	m_btn_run_bech->SetLabel("Run Benchmark");
	m_btn_run_bech->Enable(true);

	e.Skip();
}

void onClose(wxCloseEvent& evt)
{
	wxGetApp().activateRenderLoop(false);

	evt.Skip(); // don't stop event, we still want window to close
}
