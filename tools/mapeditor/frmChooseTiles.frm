VERSION 5.00
Begin VB.Form frmChooseTiles 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Choose Tiles"
   ClientHeight    =   5955
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   8010
   Icon            =   "frmChooseTiles.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   397
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   534
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.PictureBox picTilePreview 
      AutoRedraw      =   -1  'True
      BorderStyle     =   0  'None
      Height          =   480
      Left            =   4440
      ScaleHeight     =   32
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   32
      TabIndex        =   11
      Top             =   3120
      Width           =   480
   End
   Begin VB.CommandButton cmdRefresh 
      Caption         =   "Refresh"
      Height          =   255
      Left            =   3480
      TabIndex        =   1
      Top             =   120
      Width           =   855
   End
   Begin VB.CommandButton cmdAllToAvailable 
      Caption         =   "<<"
      Height          =   375
      Left            =   4440
      TabIndex        =   6
      Top             =   2640
      Width           =   615
   End
   Begin VB.CommandButton cmdAllToMap 
      Caption         =   ">>"
      Height          =   375
      Left            =   4440
      TabIndex        =   5
      Top             =   2160
      Width           =   615
   End
   Begin VB.CommandButton cmdToAvailable 
      Caption         =   "<"
      Height          =   375
      Left            =   4440
      TabIndex        =   4
      Top             =   1320
      Width           =   615
   End
   Begin VB.CommandButton cmdToMap 
      Caption         =   ">"
      Height          =   375
      Left            =   4440
      TabIndex        =   3
      Top             =   840
      Width           =   615
   End
   Begin VB.ListBox lstMap 
      Height          =   4860
      IntegralHeight  =   0   'False
      Left            =   5160
      MultiSelect     =   2  'Extended
      TabIndex        =   8
      Top             =   360
      Width           =   2775
   End
   Begin VB.ListBox lstAvailable 
      Columns         =   2
      Height          =   4860
      IntegralHeight  =   0   'False
      Left            =   120
      MultiSelect     =   2  'Extended
      TabIndex        =   2
      Top             =   360
      Width           =   4215
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   5160
      TabIndex        =   9
      Top             =   5520
      Width           =   1335
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   375
      Left            =   6600
      TabIndex        =   10
      Top             =   5520
      Width           =   1335
   End
   Begin VB.Label lbl 
      AutoSize        =   -1  'True
      Caption         =   "Tiles in this map:"
      Height          =   195
      Index           =   1
      Left            =   5880
      TabIndex        =   7
      Top             =   120
      Width           =   1170
   End
   Begin VB.Label lbl 
      AutoSize        =   -1  'True
      Caption         =   "Available tiles:"
      Height          =   195
      Index           =   0
      Left            =   1440
      TabIndex        =   0
      Top             =   120
      Width           =   1005
   End
End
Attribute VB_Name = "frmChooseTiles"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private NumTileFiles As Integer
Private TileFiles() As String

Public Cancelled As Boolean

Public Sub RefreshTilesBox()
    Dim File As String
    Dim LastTileName As String, LastTileIndex As Integer
    Dim NiceName As String
    Dim I As Integer
    
    'scan directory
    NumTileFiles = 0
    File = Dir(App.Path & "\" & TilesFolder & "\*.tile.txt", vbNormal)
    Do While Len(File)
        ReDim Preserve TileFiles(NumTileFiles)
        TileFiles(NumTileFiles) = File
        
        NumTileFiles = NumTileFiles + 1
        File = Dir()
    Loop
    
    'sort alphabetically
    QSort TileFiles, 0, NumTileFiles - 1
    
    'add to list box
    If lstAvailable.ListIndex >= 0 Then LastTileName = lstAvailable.List(lstAvailable.ListIndex)
    
    lstAvailable.Visible = False
    lstAvailable.Clear
    For I = 0 To NumTileFiles - 1
        NiceName = StripExtension(StripExtension(TileFiles(I)))
        lstAvailable.AddItem NiceName
        If NiceName = LastTileName Then LastTileIndex = I
    Next I
    lstAvailable.Visible = True
    
    'select previously selected index
    If LastTileName <> "" Then
        lstAvailable.ListIndex = LastTileIndex
        lstAvailable_Click
    End If
    
End Sub



Private Sub cmdAllToAvailable_Click()
    lstMap.Clear
End Sub

Private Sub cmdAllToMap_Click()
    Dim I As Integer
    
    lstMap.Clear
    For I = 0 To NumTileFiles - 1
        lstMap.AddItem lstAvailable.List(I)
        lstMap.ItemData(I) = I
    Next I
End Sub

Private Sub cmdCancel_Click()
    Cancelled = True
    Me.Hide
End Sub

Private Sub cmdOK_Click()
    Cancelled = False
    Me.Hide
End Sub

Private Sub cmdRefresh_Click()
    RefreshTilesBox
End Sub

Private Sub cmdToAvailable_Click()
    Dim I As Integer

    'If lstMap.ListIndex >= 0 Then lstMap.RemoveItem lstMap.ListIndex
    
    For I = 0 To lstMap.ListCount - 1
        If I >= lstMap.ListCount Then Exit For
        If lstMap.Selected(I) Then
            lstMap.RemoveItem I
            I = I - 1
        End If
    Next I
End Sub

Private Sub cmdToMap_Click()
    Dim I As Integer
'    If lstAvailable.ListIndex >= 0 Then
'        lstMap.AddItem lstAvailable.List(lstAvailable.ListIndex)
'        lstMap.ItemData(lstMap.ListCount - 1) = lstAvailable.ListIndex
'    End If
    
    For I = 0 To lstAvailable.ListCount - 1
        If lstAvailable.Selected(I) Then
            If Not TileInMap(lstAvailable.List(I)) Then
                lstMap.AddItem lstAvailable.List(I)
                lstMap.ItemData(lstMap.ListCount - 1) = I
            End If
        End If
    Next I
End Sub

Private Function TileInMap(TileName As String) As Boolean
    Dim I As Integer
    
    For I = 0 To lstMap.ListCount - 1
        If lstMap.List(I) = TileName Then
            TileInMap = True
            Exit Function
        End If
    Next I
    
    TileInMap = False
End Function

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    If UnloadMode = vbFormControlMenu Then
        Cancel = -1
        cmdCancel_Click
    End If
End Sub

Private Sub lstAvailable_Click()
    ShowPreviewTile App.Path & "\" & TilesFolder & "\" & TileFiles(lstAvailable.ListIndex)
End Sub

Private Sub ShowPreviewTile(File As String)
'    Dim PreviewTile As New Tile
    
    
    ComputeTileFileBitmap File
    
    picTilePreview.Cls
    picTilePreview.PaintPicture frmMain.picMaskAnd.Image, 0, 0, , , , , , , vbSrcAnd
    picTilePreview.PaintPicture frmMain.picMaskOr.Image, 0, 0, , , , , , , vbSrcPaint
    
    
'    imgTilePreview.Picture = LoadPicture()
'
'    On Error GoTo TileLoadErr
'    PreviewTile.FromFile File
'
'    On Error GoTo PicLoadErr
'    If PreviewTile.GetProperty("alpha") = "" Or PreviewTile.GetProperty("alpha") = "none" Then
'        imgTilePreview.Picture = LoadPicture(App.Path & "\" & BitmapsFolder & "\" & PreviewTile.GetProperty("bitmap"))
'    Else
'        frmMain.picMaskInput.Picture = LoadPicture(App.Path & "\" & BitmapsFolder & "\" & PreviewTile.GetProperty("bitmap"))
'        ChangeBackcolor frmMain.picMaskInput, frmMain.picMaskOr, GetColorFromStr(PreviewTile.GetProperty("alpha")), Me.BackColor
'        imgTilePreview.Picture = frmMain.picMaskOr.Image
'        imgTilePreview.Move imgTilePreview.Left, imgTilePreview.Top, frmMain.picMaskOr.Width, frmMain.picMaskOr.Height
'    End If
'
'    Exit Sub
'PicLoadErr:
'    ProcessError "Error loading bitmap specified in tile - " & Err.Description
'    Resume Next
'TileLoadErr:
'    ProcessError "Error loading tile from disk - " & Err.Description
'    Exit Sub

End Sub

Private Sub lstMap_Click()
    ShowPreviewTile App.Path & "\" & TilesFolder & "\" & lstMap.List(lstMap.ListIndex) & ".tile.txt"
End Sub
