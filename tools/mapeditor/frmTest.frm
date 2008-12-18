VERSION 5.00
Begin VB.Form frmTest 
   Caption         =   "Test"
   ClientHeight    =   6690
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   9405
   LinkTopic       =   "Form1"
   ScaleHeight     =   446
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   627
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtColor 
      Height          =   285
      Left            =   120
      TabIndex        =   9
      Text            =   "16711935"
      Top             =   480
      Width           =   1335
   End
   Begin VB.PictureBox picMask 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   2415
      Left            =   6480
      ScaleHeight     =   161
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   185
      TabIndex        =   8
      Top             =   4080
      Width           =   2775
   End
   Begin VB.PictureBox picEdit 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   2415
      Left            =   3240
      ScaleHeight     =   161
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   193
      TabIndex        =   7
      Top             =   4080
      Width           =   2895
   End
   Begin VB.PictureBox picInput 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   2415
      Left            =   120
      ScaleHeight     =   161
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   185
      TabIndex        =   6
      Top             =   4080
      Width           =   2775
   End
   Begin VB.TextBox txtFile 
      Height          =   285
      Left            =   120
      TabIndex        =   5
      Text            =   "C:\Documents and Settings\andy\My Documents\face.bmp"
      Top             =   120
      Width           =   9135
   End
   Begin VB.CommandButton cmdGenerateBlack 
      Caption         =   "Create Masks"
      Height          =   375
      Left            =   4080
      TabIndex        =   4
      Top             =   1200
      Width           =   1455
   End
   Begin VB.CommandButton cmdLoad 
      Caption         =   "Load"
      Height          =   495
      Left            =   840
      TabIndex        =   3
      Top             =   1200
      Width           =   1215
   End
   Begin VB.Image imgMask 
      Height          =   1695
      Left            =   6960
      Top             =   1680
      Width           =   1695
   End
   Begin VB.Image imgEdit 
      Height          =   1695
      Left            =   3960
      Top             =   1680
      Width           =   1695
   End
   Begin VB.Image imgInput 
      Height          =   1455
      Left            =   480
      Top             =   1800
      Width           =   1695
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "mask"
      Height          =   195
      Index           =   2
      Left            =   7560
      TabIndex        =   2
      Top             =   720
      Width           =   375
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "black background"
      Height          =   195
      Index           =   1
      Left            =   4080
      TabIndex        =   1
      Top             =   720
      Width           =   1290
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "Input"
      Height          =   195
      Index           =   0
      Left            =   1080
      TabIndex        =   0
      Top             =   840
      Width           =   360
   End
End
Attribute VB_Name = "frmTest"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdGenerateBlack_Click()
    Dim X As Integer, Y As Integer
    Dim AlphaColor As Long
    
    AlphaColor = Val(txtColor.Text)
    picEdit.Move picEdit.Left, picEdit.Top, picInput.Width, picInput.Height
    picMask.Move picMask.Left, picMask.Top, picInput.Width, picInput.Height
    
    picEdit.Cls
    picMask.Cls
    
    
    For X = 0 To picInput.ScaleWidth - 1
        For Y = 0 To picInput.ScaleHeight - 1
            If picInput.Point(X, Y) = AlphaColor Then
                picEdit.PSet (X, Y), vbBlack
                picMask.PSet (X, Y), vbWhite
            Else
                picEdit.PSet (X, Y), picInput.Point(X, Y)
                picMask.PSet (X, Y), vbBlack
            End If
        Next Y
    Next X
    
    Me.imgEdit.Picture = picEdit.Image
    Me.imgMask.Picture = picMask.Image
End Sub

Private Sub cmdLoad_Click()
    'Me.imgInput.Picture = LoadPicture(Me.txtFile.Text)
    
    'Me.picInput.Picture = imgInput.Picture
    
    picInput.Picture = LoadPicture(txtFile.Text)
End Sub


