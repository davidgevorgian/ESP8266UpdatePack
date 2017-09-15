{
 [1B] - magic byte A5
 [file]
 [file]
 .....
 [file]

 -- file --
 [1B] - type (0 - ROM, 1 - SPIFFS add file, 2 - SPIFFS del file, 3 - SPIFFS format)
 if (1,2)
    [32B] - file name
 if (0,1)
    [4B] - File size
    [xxB] - File body
 -- end file --
}


unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TForm1 = class(TForm)
    OpenDialog: TOpenDialog;
    BT_Pack: TButton;
    SaveDialog: TSaveDialog;
    GroupBox1: TGroupBox;
    BT_Add_ROM: TButton;
    BT_Del_ROM: TButton;
    ED_ROM_NAME: TEdit;
    GroupBox2: TGroupBox;
    BT_Add_SPIFFS: TButton;
    BT_Add_Command: TButton;
    BT_Delete: TButton;
    ListBox: TListBox;
    PR_Command: TComboBox;
    Label2: TLabel;
    Label1: TLabel;
    PR_FName: TEdit;
    procedure BT_Click(Sender: TObject);
    procedure ListBoxClick(Sender: TObject);
    procedure PR_CommandChange(Sender: TObject);
    procedure PR_FNameExit(Sender: TObject);
    procedure BT_PackClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

  Titemrecord = record
                     fpath : string;
                     typ   : byte;
               end;
  Titems = array of Titemrecord;

var
  Form1   : TForm1;
  items   : Titems;
  romitem : string;

implementation

{$R *.dfm}
procedure DeleteX(var A: Titems; const Index: Integer);
var
  ALength    : Integer;
  i          : Integer;
begin
  ALength := Length(A);
  for i := Index + 1 to ALength - 1 do A[i - 1] := A[i];
  SetLength(A, ALength - 1);
end;

procedure TForm1.BT_Click(Sender: TObject);
Var
   i      : cardinal;
   p      : cardinal;
begin
     if (sender = BT_Add_ROM) then begin
      opendialog.Options := [];
      if(opendialog.Execute) then begin
         romitem := opendialog.FileName;
         ED_ROM_NAME.Text := romitem;
      end;
     end;
     if (sender = BT_Del_ROM) then begin
         romitem := '';
         ED_ROM_NAME.Text := romitem;
     end;

     if (sender = BT_Add_SPIFFS) then begin
      opendialog.Options := [ofAllowMultiSelect];
      if(opendialog.Execute) then begin
         p:=length(items);
         setlength(items,length(items)+opendialog.Files.Count);
         for i:=0 to opendialog.Files.Count-1 do begin
             with items[p+i] do begin
                  fpath := opendialog.Files[i];
                  typ   := 1;
                  listbox.Items.Add('/'+extractfilename(fpath));
             end;
         end;
      end;
     end;

     if (sender = BT_Add_Command) then begin
        p:=length(items);
        setlength(items,p+1);
        with items[p] do begin
             fpath := '';
             typ   := 2;
             listbox.Items.Add('/file');
        end;
     end;
     if (sender = BT_Delete) then begin
        if listbox.ItemIndex > -1 then begin
           DeleteX(items,listbox.ItemIndex);
           listbox.Items.Delete(listbox.ItemIndex);
        end;
     end;
end;

procedure TForm1.ListBoxClick(Sender: TObject);
begin
     if (listbox.ItemIndex >-1) then begin
        PR_Command.ItemIndex := items[listbox.ItemIndex].typ-1;
        PR_FName.Text := listbox.items[listbox.ItemIndex];
     end;
end;

procedure TForm1.PR_CommandChange(Sender: TObject);
begin
     if (listbox.ItemIndex >-1) then begin
        items[listbox.ItemIndex].typ := PR_Command.ItemIndex + 1;
     end;
end;

procedure TForm1.PR_FNameExit(Sender: TObject);
begin
     if (listbox.ItemIndex >-1) then begin
        listbox.items[listbox.ItemIndex] := PR_FName.text;
     end;
end;

procedure TForm1.BT_PackClick(Sender: TObject);
Var
   i      : cardinal;
   b      : byte;
   f      : file;
   fd     : file;
   st     : string[40];
   buf    : array[0..1023] of byte;
begin
     if ((length(items)>0) or (romitem <> '')) and (savedialog.execute) then begin
        Assignfile(f, savedialog.FileName);
        Rewrite(f,1);
        b:=$A5; //Magic
        blockwrite(f,b,1);

        if length(items) > 0 then begin //SPIFFS
           for b:=0 to length(items)-1 do begin
               blockwrite(f,items[b].typ,1);
               if items[b].typ in [1,2] then begin
                  st := copy(listbox.items[b],1,31);
                  st := st + #0;
                  blockwrite(f,st[1],32);
               end;
               if items[b].typ in [1] then begin
                  AssignFile(fd,items[b].fpath);
                  Reset(fd,1);
                  i := filesize(fd);
                  blockwrite(f,i,4);
                  while not eof(fd) do begin
                        blockread(fd,buf[0],sizeof(buf),i);
                        blockwrite(f,buf[0],i);
                  end;
                  Closefile(fd);
               end;
           end;
        end;

        if (romitem <> '') then begin //ROM
           b:=0;
           blockwrite(f,b,1);
           AssignFile(fd,romitem);
           Reset(fd,1);
           i := filesize(fd);
           blockwrite(f,i,4);
           while not eof(fd) do begin
                 blockread(fd,buf[0],sizeof(buf),i);
                 blockwrite(f,buf[0],i);
           end;
           Closefile(fd);
        end;

        closefile(f);
     end;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
     caption:=application.Title;
end;

end.
