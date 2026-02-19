//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <VCLTee.Chart.hpp>
#include <VCLTee.Series.hpp>
#include <VclTee.TeeGDIPlus.hpp>
#include <VCLTee.TeEngine.hpp>
#include <VCLTee.TeeProcs.hpp>
#include <Vcl.Dialogs.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label;
	TStringGrid *StringGrid;
	TMainMenu *MainMenu;
	TMemo *Memo;
	TComboBox *ComboBoxGroup;
	TComboBox *ComboBoxExam;
	TChart *Chart;
	TBarSeries *Series1;
	TSaveDialog *SaveDialogTxt;
	TSaveDialog *SaveDialogChart;
	TButton *ButtonSaveTxt;
	TButton *ButtonSaveChart;
	TFileOpenDialog *FileOpenDialog;
	TFileSaveDialog *FileSaveDialog;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TLabel *Label2;
	TLabel *Label3;

	void __fastcall MenuItemClick(TObject *Sender);
	void __fastcall ComboBoxGroupChange(TObject *Sender);
	void __fastcall ComboBoxExamChange(TObject *Sender);
	void __fastcall ButtonSaveTxtClick(TObject *Sender);
	void __fastcall ButtonSaveChartClick(TObject *Sender);
	void __fastcall StringGridKeyPress(TObject *Sender, System::WideChar &Key);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);

    void UpdateHeaders();
	void AddGroup();
	void AddExam();
	void SaveToBinaryFile();
	void LoadFromBinaryFile();
	void ShowStatistics();
	void ClearTable();
	std::vector<int> ParseGrades(const UnicodeString &grades);
	void UpdateComboBoxes();
	void DrawHistogram(int groupIndex, int examIndex);
	void DelGroup();
	void DelExam();
    void SaveAsToBinaryFile();
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
