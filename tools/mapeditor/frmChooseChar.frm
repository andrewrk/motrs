VERSION 5.00
Begin VB.Form frmChooseChar 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Choose Playtest Character"
   ClientHeight    =   3930
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   6765
   Icon            =   "frmChooseChar.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   262
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   451
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picPreview 
      BorderStyle     =   0  'None
      Height          =   975
      Left            =   4560
      ScaleHeight     =   65
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   97
      TabIndex        =   4
      TabStop         =   0   'False
      Top             =   1200
      Width           =   1455
   End
   Begin VB.CommandButton cmdRefresh 
      Caption         =   "Refresh"
      Height          =   255
      Left            =   2760
      TabIndex        =   1
      Top             =   120
      Width           =   855
   End
   Begin VB.ListBox lstChars 
      Height          =   3060
      IntegralHeight  =   0   'False
      Left            =   120
      TabIndex        =   2
      Top             =   360
      Width           =   3495
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   3840
      TabIndex        =   5
      Top             =   3480
      Width           =   1335
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   375
      Left            =   5280
      TabIndex        =   6
      Top             =   3480
      Width           =   1335
   End
   Begin VB.Label lblPreview 
      AutoSize        =   -1  'True
      Caption         =   "Preview:"
      Height          =   195
      Left            =   5040
      TabIndex        =   3
      Top             =   480
      Width           =   615
   End
   Begin VB.Label lbl 
      AutoSize        =   -1  'True
      Caption         =   "Available characters:"
      Height          =   195
      Index           =   0
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   1485
   End
End
Attribute VB_Name = "frmChooseChar"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private NumCharFiles As Integer
Private CharFiles() As String

Public Cancelled As Boolean

Private Sub cmdCancel_Click()
    Cancelled = True
    Me.Hide
End Sub

Private Sub cmdOK_Click()
    Cancelled = False
    
    'put the character in frmmain's images
    frmMain.imgPlayerIcon.Picture = frmMain.picMaskOr.Image
    frmMain.imgPlayerIconMask.Picture = frmMain.picMaskAnd.Image
    frmMain.imgPlayerIcon.Move 0, 0, frmMain.picMaskInput.Width, frmMain.picMaskInput.Height
    frmMain.imgPlayerIconMask.Move 0, 0, frmMain.picMaskInput.Width, frmMain.picMaskInput.Height
    
    Me.Hide
End Sub

Public Sub RefreshCharsBox()
    Dim File As String
    Dim LastCharName As String, LastCharIndex As Integer
    Dim NiceName As String
    Dim I As Integer
    
    'scan directory
    NumCharFiles = 0
    File = Dir(App.Path & "\" & CharactersFolder & "\*.char.txt", vbNormal)
    Do While Len(File)
        ReDim Preserve CharFiles(NumCharFiles)
        CharFiles(NumCharFiles) = File
        
        NumCharFiles = NumCharFiles + 1
        File = Dir()
    Loop
    
    'sort alphabetically
    QSort CharFiles, 0, NumCharFiles - 1
    
    'add to list box
    If lstChars.ListIndex >= 0 Then LastCharName = lstChars.List(lstChars.ListIndex)
    
    lstChars.Visible = False
    lstChars.Clear
    For I = 0 To NumCharFiles - 1
        NiceName = StripExtension(StripExtension(CharFiles(I)))
        lstChars.AddItem NiceName
        If NiceName = LastCharName Then LastCharIndex = I
    Next I
    lstChars.Visible = True
    
    'select previously selected index
    If LastCharName <> "" Then
        lstChars.ListIndex = LastCharIndex
        lstChars_Click
    End If
    

End Sub

Private Sub cmdRefresh_Click()
    RefreshCharsBox
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    If UnloadMode = vbFormControlMenu Then
        Cancel = -1
        cmdCancel_Click
    End If
End Sub

Private Sub lstChars_Click()
    frmMain.LoadCharacter lstChars.List(lstChars.ListIndex) & ".char"
    
    picPreview.Move lblPreview.Left + lblPreview.Width / 2 - frmMain.imgPlayerIcon.Width / 2, lblPreview.Top + lblPreview.Height + Margin, frmMain.imgPlayerIcon.Width, frmMain.imgPlayerIcon.Height
    picPreview_Paint
End Sub


Private Sub picPreview_Paint()
    picPreview.Cls
    picPreview.PaintPicture frmMain.imgPlayerIconMask.Picture, 0, 0, , , , , , , vbSrcAnd
    picPreview.PaintPicture frmMain.imgPlayerIcon.Picture, 0, 0, , , , , , , vbSrcPaint
End Sub
