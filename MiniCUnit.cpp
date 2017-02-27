//---------------------------------------------------------------------------
#	pragma warn -8004
#	pragma warn -8008
#	pragma warn -8066
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "MiniCUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMiniC *MiniC;
//---------------------------------------------------------------------------
// #define _printf  MiniC->Printf
//---------------------------------------------------------------------------
__fastcall TMiniC::TMiniC(TComponent* Owner)
    : TForm(Owner)
{
char    tmp[256];
    Caption = "Mini Environment Developpement Interface v1.0.2";
    Printf("Starting %s ...\n\n", Caption.c_str());

    if (ParamCount())
        {
        for (int i = 1; i <= ParamCount(); i++)
            {
            Printf("Argument %d = \"%s\"", i, ParamStr(i));
            sprintf(tmp, "%s%s", ExtractFilePath(Application->ExeName).c_str(), ParamStr(i));
            Printf("Running '%s'...\n", tmp);
            }
        }
}
//---------------------------------------------------------------------------
void TMiniC::Printf(const char *format, ...)
{
va_list     argptr;
char        tmp[8 * 1024];
AnsiString  curline;

    if (strlen(format) > sizeof(tmp))
        {
        sprintf(tmp, "ERROR Need %d bytes to be able to print sucessfully. Just have %d available.", strlen(format), sizeof (tmp));
        MiniC->Display->Lines->Add("");
        MiniC->Display->Lines->Add(tmp);
        return;
        }

    va_start(argptr, format);
    vsprintf(tmp, format, argptr);
    va_end(argptr);

    for (int i = 0, j = 0; i < sizeof(tmp); i++)
        {
        switch (tmp[i])
            {
            case    0:              // end of buffer
                if (strlen(&tmp[j]))
                    {
                    curline = MiniC->Display->Lines->Strings[GetLine()];
                    MiniC->Display->Lines->Strings[GetLine()] =  curline +  &tmp[j];
                    }
                ZeroMemory(tmp, sizeof(tmp));
                return;

            case    '\n':           // end of line
                tmp[i] = 0;
                curline = MiniC->Display->Lines->Strings[GetLine()];
                MiniC->Display->Lines->Strings[GetLine()] =  curline +  &tmp[j];
                MiniC->Display->Lines->Add("");
                j = i + 1;
                break;

            case    '\t':           // tabs
            case    '\r':           // return
                break;              // above control characters are handled by richedit object

            default:
                if  (tmp[i] < ' ')
                    tmp[i] = '¤';
                break;
            }
        }
}
//---------------------------------------------------------------------------
// TPoint  LastPos;
char    LastKey;
int     CurLine, LastLine;
int     CurColumn, LastColumn;

AnsiString __fastcall TMiniC::GetString(void)
{
    CurLine= GetLine();
    CurColumn = GetColumn();
    LastKey = 0;
    while (LastKey != '\r')
        {
        Application->ProcessMessages();
        Sleep(10);                  // sleep time in milliseconds
        }
    // collect the string
    return Display->Lines->Strings[LastLine].SubString(CurColumn + 1,
        Display->Lines->Strings[LastLine].Length() - CurColumn);
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::DisplayKeyPress(TObject *Sender, char &Key)
{
    LastLine = GetLine();
    LastColumn = GetColumn();
    LastKey = Key;
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::OpenClick(TObject *Sender)
{
    OpenDialog->Title = "Open C file";
    OpenDialog->DefaultExt = "";
    OpenDialog->Filter = "C Files (*.c)|*.c|All files (*.*)|*.*";
    OpenDialog->InitialDir = ExtractFilePath(Application->ExeName);
    if (!OpenDialog->Execute())
        return;
    Printf("Loading C File '%s'...", OpenDialog->FileName);
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::SaveClick(TObject *Sender)
{
    Printf("Saving C File '%s'...", OpenDialog->FileName);
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::SaveAsClick(TObject *Sender)
{
    Printf("Saving C File '%s' As...", OpenDialog->FileName);
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::InteractiveClick(TObject *Sender)
{
int         i = 0;
AnsiString  cmd;
AnsiString  fname;

    Printf("Interactive Mode\n");
    while(true)
        {
        Printf("%03d\t->", i++);
        cmd = GetString().UpperCase();  // get a command

        if (cmd == "QUIT")
            {
            Printf("Exiting Interactive Mode...\n");
            break;
            }
        else if (cmd == "SYSTEM")
            {
            Close();
            break;
            }
        else if (cmd == "HELP")
            {
            Printf("QUIT                Exit Interactive Mode\n");
            Printf("SYSTEM              Exit EDI\n");
            Printf("CLS                 Clear display\n");
            Printf("LOAD filename       Load source file\n");
            Printf("CLEAN filename      Clean source file\n");
            Printf("SAVE filename       Save source file\n");
            Printf("EDIT n              Edit source file\n");
            Printf("LIST                List source file\n");
            }
        else if (cmd.SubString(0,3) == "CLS")
            Display->Lines->Clear();

        else if (cmd.SubString(0, 5) == "LOAD ")
            {
            fname = cmd.SubString(6, cmd.Length() - 5).LowerCase();
            if (fname.IsEmpty())
                Printf("Nothing to load.\n");
            else
                {
                Printf("Loading '%s'...\n", fname.c_str());
                Printf("Path = '%s'\n", ExtractFilePath(fname));
                Printf("Name = '%s'\n", ExtractFileName(fname));
                Printf("Ext  = '%s'\n", ExtractFileExt(fname));

                }
            }
        else if (cmd == "LIST")
            Printf("Listing...\n");
        else
            Printf("???\n");
        }
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::BreakClick(TObject *Sender)
{
    Printf("Break\n");
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::QuitClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
// Pop up a dialog message with a formatted string
bool TMiniC::Msg(AnsiString Title, char *format, ...)
{
va_list argptr;
char	tmp[128];

    va_start(argptr, format);
    vsprintf(tmp, format, argptr);
    va_end(argptr);
	if (MessageBox(NULL, tmp, Title.c_str(), MB_OKCANCEL|MB_ICONEXCLAMATION) == IDOK)
    	return true;
    return false;
}
//---------------------------------------------------------------------------
// Manipulate cursor into the text control
// Line and column are zero based
void __fastcall TMiniC::Cursor(int Line, int Column)
{
	Display->SelStart = Display->Perform(EM_LINEINDEX, Line, 0) + Column;
	Display->Perform(EM_SCROLLCARET, 0, 0);
}
//---------------------------------------------------------------------------
int __fastcall TMiniC::GetLine(void)
{
	return Display->Perform(EM_EXLINEFROMCHAR, 0, Display->SelStart);
}
//---------------------------------------------------------------------------
int __fastcall TMiniC::GetColumn(void)
{
int line = Display->Perform(EM_EXLINEFROMCHAR, 0, Display->SelStart);
	return Display->SelStart - Display->Perform(EM_LINEINDEX, line, 0);
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::CutClick(TObject *Sender)
{
    Display->CutToClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::CopyClick(TObject *Sender)
{
    Display->CopyToClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::PasteClick(TObject *Sender)
{
    Display->PasteFromClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::ClearClick(TObject *Sender)
{
    Display->ClearSelection();
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::SelectAllClick(TObject *Sender)
{
    Display->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::FindClick(TObject *Sender)
{
    FindDialog->FindText = Display->SelText;
    FindDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::FindDialogFind(TObject *Sender)
{
// Dialog that generated the event could be either Find or Replace dialog so just cast Sender to a TFindDialog.

TFindDialog* Dialog = dynamic_cast <TFindDialog*>(Sender);
TSearchTypes Options;               // Set up the options.

    if (Dialog->Options.Contains(frWholeWord))
        Options << stWholeWord;
    if (Dialog->Options.Contains(frMatchCase))
        Options << stMatchCase;

// Deselect any selected text and advance the cursor position. Prevents us from selecting the same text over and over again.
    if (Display->SelLength)
        {
        Display->SelStart += Display->SelLength;
        Display->SelLength = 0;
        }

// Find the text.
int Pos = Display->FindText(Dialog->FindText, Display->SelStart, Display->GetTextLen(), Options);

// If text is found, highlight it, scroll window to found text, and set focus to RichEdit control.
// If no text is found, let user know.

    if (Pos != -1)
        {
        Display->SelStart = Pos;
        Display->SelLength = Dialog->FindText.Length();
        Display->Perform(EM_SCROLLCARET, 0, 0);
        Display->SetFocus();
        }
    else
        Msg("Find Text", "Search '%s' text not found.", Dialog->FindText);
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::ReplaceClick(TObject *Sender)
{
    ReplaceDialog->FindText = Display->SelText;
    ReplaceDialog->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TMiniC::ReplaceDialogReplace(TObject *Sender)
{
// If user clicked Replace All, then do find/replace operation until no more matches are found.
    if (ReplaceDialog->Options.Contains(frReplaceAll))
        {
static int Count;
TSearchTypes Options;
        if (ReplaceDialog->Options.Contains(frWholeWord))
            Options << stWholeWord;
        if (ReplaceDialog->Options.Contains(frMatchCase))
            Options << stMatchCase;
// Find the first match.
int Pos = Display->FindText(ReplaceDialog->FindText, Display->SelStart, Display->GetTextLen(), Options);
// Loop while we are still finding text.
        while (Pos != -1)
            {
// Same code as in the OnFind event handler.
            Display->SelStart = Pos;
            Display->SelLength = ReplaceDialog->FindText.Length();
// Replace the found text with the new text.
            Display->SetSelTextBuf( ReplaceDialog->ReplaceText.c_str());
            Display->Perform(EM_SCROLLCARET, 0, 0);
// Increment the counter so we can tell the user how many replacements we made.
            Count++;
// Do another search.
            Pos = Display->FindText(ReplaceDialog->FindText, Display->SelStart, Display->GetTextLen(), Options);
            }

// Dismiss the Replace dialog.
        ReplaceDialog->CloseDialog();
// Tell the user what we did.
        Msg("Replace Text", "Finished searching '%s' text. %d replacements made.", ReplaceDialog->FindText, Count);
// Reset the counter.
        Count = 0;
        }
// If not doing a Replace All, then just replace the found text with the new text.
    else
        {
// No text selected? Then return.
        if (Display->SelLength == 0)
            return;
        Display->SetSelTextBuf( ReplaceDialog->ReplaceText.c_str());
        Display->SelLength = 0;
        }
// Set focus to the RichEdit.
    Display->SetFocus();
}
//---------------------------------------------------------------------------

