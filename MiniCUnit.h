//---------------------------------------------------------------------------

#ifndef MiniCUnitH
#define MiniCUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TMiniC : public TForm
{
__published:	// Composants gérés par l'EDI
    TRichEdit *Display;
    TMainMenu *MainMenu;
    TPopupMenu *PopupMenu;
    TMenuItem *File;
    TMenuItem *Edit;
    TMenuItem *Open;
    TMenuItem *Save;
    TMenuItem *SaveAs;
    TMenuItem *N1;
    TMenuItem *Interactive;
    TMenuItem *Break;
    TMenuItem *N2;
    TMenuItem *Quit;
    TMenuItem *Cut;
    TMenuItem *Copy;
    TMenuItem *Paste;
    TMenuItem *Clear;
    TMenuItem *SelectAll;
    TMenuItem *N4;
    TMenuItem *N5;
    TMenuItem *Find;
    TMenuItem *Replace;
    TOpenDialog *OpenDialog;
    TSaveDialog *SaveDialog;
    TFindDialog *FindDialog;
    TReplaceDialog *ReplaceDialog;
    TMenuItem *Cut1;
    TMenuItem *Copy1;
    TMenuItem *Paste1;
    TMenuItem *N6;
    TMenuItem *Clear1;
    TMenuItem *SelectAll1;
    TMenuItem *N7;
    TMenuItem *Find1;
    TMenuItem *Replace1;
    void __fastcall OpenClick(TObject *Sender);
    void __fastcall SaveClick(TObject *Sender);
    void __fastcall SaveAsClick(TObject *Sender);
    void __fastcall InteractiveClick(TObject *Sender);
    void __fastcall BreakClick(TObject *Sender);
    void __fastcall QuitClick(TObject *Sender);
    void __fastcall CutClick(TObject *Sender);
    void __fastcall CopyClick(TObject *Sender);
    void __fastcall PasteClick(TObject *Sender);
    void __fastcall ClearClick(TObject *Sender);
    void __fastcall SelectAllClick(TObject *Sender);
    void __fastcall FindClick(TObject *Sender);
    void __fastcall ReplaceClick(TObject *Sender);
    void __fastcall FindDialogFind(TObject *Sender);
    void __fastcall ReplaceDialogReplace(TObject *Sender);
    void __fastcall DisplayKeyPress(TObject *Sender, char &Key);
private:	// Déclarations de l'utilisateur
public:		// Déclarations de l'utilisateur
    __fastcall TMiniC(TComponent* Owner);

//---------------------------------------------------------------------------
    bool            	    Msg(AnsiString Title, char *format, ...);
    void                    Printf(const char *format, ...);
    AnsiString __fastcall   GetString(void);
    void __fastcall 	    Cursor(int Line, int Column);
    int __fastcall 		    GetLine(void);
    int __fastcall 		    GetColumn(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TMiniC *MiniC;
//---------------------------------------------------------------------------
#endif
