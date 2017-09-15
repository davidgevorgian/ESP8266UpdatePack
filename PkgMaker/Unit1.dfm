object Form1: TForm1
  Left = 1527
  Top = 146
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Form1'
  ClientHeight = 345
  ClientWidth = 353
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object BT_Pack: TButton
    Left = 8
    Top = 304
    Width = 89
    Height = 33
    Caption = 'Pack'
    TabOrder = 0
    OnClick = BT_PackClick
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 337
    Height = 49
    Caption = 'ROM'
    TabOrder = 1
    object BT_Add_ROM: TButton
      Left = 8
      Top = 16
      Width = 49
      Height = 25
      Caption = 'Add'
      TabOrder = 0
      OnClick = BT_Click
    end
    object BT_Del_ROM: TButton
      Left = 64
      Top = 16
      Width = 49
      Height = 25
      Caption = 'Del'
      TabOrder = 1
      OnClick = BT_Click
    end
    object ED_ROM_NAME: TEdit
      Left = 120
      Top = 16
      Width = 209
      Height = 21
      TabOrder = 2
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 64
    Width = 337
    Height = 233
    Caption = 'SPIFFS'
    TabOrder = 2
    object Label2: TLabel
      Left = 17
      Top = 180
      Width = 56
      Height = 13
      AutoSize = False
      Caption = 'Command'
      Layout = tlCenter
    end
    object Label1: TLabel
      Left = 17
      Top = 204
      Width = 56
      Height = 13
      AutoSize = False
      Caption = 'Name'
      Layout = tlCenter
    end
    object BT_Add_SPIFFS: TButton
      Left = 8
      Top = 32
      Width = 89
      Height = 25
      Caption = 'Add SPIFFS'
      TabOrder = 0
      OnClick = BT_Click
    end
    object BT_Add_Command: TButton
      Left = 8
      Top = 64
      Width = 89
      Height = 25
      Caption = 'Add Command'
      TabOrder = 1
      OnClick = BT_Click
    end
    object BT_Delete: TButton
      Left = 8
      Top = 96
      Width = 89
      Height = 25
      Caption = 'Delete'
      TabOrder = 2
      OnClick = BT_Click
    end
    object ListBox: TListBox
      Left = 104
      Top = 16
      Width = 225
      Height = 145
      ExtendedSelect = False
      ItemHeight = 13
      TabOrder = 3
      OnClick = ListBoxClick
    end
    object PR_Command: TComboBox
      Left = 72
      Top = 176
      Width = 257
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      ItemIndex = 0
      TabOrder = 4
      Text = 'SPIFFS Add'
      OnChange = PR_CommandChange
      Items.Strings = (
        'SPIFFS Add'
        'SPIFFS Delete'
        'SPIFFS Format')
    end
    object PR_FName: TEdit
      Left = 72
      Top = 200
      Width = 257
      Height = 21
      TabOrder = 5
      OnExit = PR_FNameExit
    end
  end
  object OpenDialog: TOpenDialog
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofEnableSizing]
    Left = 296
    Top = 184
  end
  object SaveDialog: TSaveDialog
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 264
    Top = 184
  end
end
