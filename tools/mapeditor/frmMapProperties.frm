VERSION 5.00
Begin VB.Form frmMapProperties 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Map Properties"
   ClientHeight    =   2700
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   4575
   Icon            =   "frmMapProperties.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   180
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   305
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.TextBox txtLayers 
      Height          =   285
      Left            =   720
      TabIndex        =   5
      Top             =   1560
      Width           =   855
   End
   Begin VB.TextBox txtMapDimY 
      Height          =   285
      Left            =   1920
      TabIndex        =   3
      Top             =   600
      Width           =   855
   End
   Begin VB.TextBox txtMapDimX 
      Height          =   285
      Left            =   720
      TabIndex        =   1
      Top             =   600
      Width           =   855
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   1680
      TabIndex        =   6
      Top             =   2160
      Width           =   1335
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   375
      Left            =   3120
      TabIndex        =   7
      Top             =   2160
      Width           =   1335
   End
   Begin VB.Label lbl 
      AutoSize        =   -1  'True
      Caption         =   "Number of layers:"
      Height          =   195
      Index           =   2
      Left            =   240
      TabIndex        =   4
      Top             =   1200
      Width           =   1230
   End
   Begin VB.Label lbl 
      AutoSize        =   -1  'True
      Caption         =   "x"
      Height          =   195
      Index           =   1
      Left            =   1680
      TabIndex        =   2
      Top             =   720
      Width           =   75
   End
   Begin VB.Label lbl 
      AutoSize        =   -1  'True
      Caption         =   "Map Dimensions:"
      Height          =   195
      Index           =   0
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   1215
   End
End
Attribute VB_Name = "frmMapProperties"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Canceled As Boolean

Private Sub cmdCancel_Click()
    Canceled = True
    Me.Hide
End Sub

Private Sub cmdOK_Click()
    Canceled = False
    Me.Hide
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    If UnloadMode = vbFormControlMenu Then
        Cancel = -1
        cmdCancel_Click
    End If
End Sub
