VERSION 5.00
Begin VB.Form frmTileEditor 
   Caption         =   "Tile Editor"
   ClientHeight    =   6840
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   10560
   LinkTopic       =   "Form1"
   ScaleHeight     =   456
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   704
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picPreview 
      AutoRedraw      =   -1  'True
      BorderStyle     =   0  'None
      Height          =   480
      Left            =   3840
      ScaleHeight     =   32
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   32
      TabIndex        =   8
      Top             =   6240
      Width           =   480
   End
   Begin VB.CommandButton cmdAdd 
      Caption         =   "Add"
      Height          =   255
      Left            =   120
      TabIndex        =   7
      Top             =   6240
      Width           =   855
   End
   Begin VB.CommandButton cmdDelete 
      Caption         =   "Delete"
      Height          =   255
      Left            =   1080
      TabIndex        =   6
      Top             =   6240
      Width           =   855
   End
   Begin VB.CommandButton cmdClose 
      Cancel          =   -1  'True
      Caption         =   "Close"
      Height          =   375
      Left            =   9120
      TabIndex        =   5
      Top             =   6360
      Width           =   1335
   End
   Begin VB.TextBox txtEdit 
      Height          =   5775
      Left            =   5280
      MultiLine       =   -1  'True
      TabIndex        =   4
      Top             =   360
      Width           =   5175
   End
   Begin VB.ListBox lstTiles 
      Columns         =   2
      Height          =   5820
      IntegralHeight  =   0   'False
      Left            =   120
      TabIndex        =   1
      Top             =   360
      Width           =   4215
   End
   Begin VB.CommandButton cmdRefresh 
      Caption         =   "Refresh"
      Height          =   255
      Left            =   3480
      TabIndex        =   0
      Top             =   120
      Width           =   855
   End
   Begin VB.Label lblEdit 
      AutoSize        =   -1  'True
      Caption         =   "Edit tile:"
      Height          =   195
      Left            =   7560
      TabIndex        =   3
      Top             =   120
      Width           =   555
   End
   Begin VB.Label lblAvailable 
      AutoSize        =   -1  'True
      Caption         =   "Available tiles:"
      Height          =   195
      Left            =   1440
      TabIndex        =   2
      Top             =   120
      Width           =   1005
   End
End
Attribute VB_Name = "frmTileEditor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit


Private NumTileFiles As Integer
Private TileFiles() As String


Private Const MinScaleWidth = 404
Private Const MinScaleHeight = 265

Private Sub cmdAdd_Click()
    Dim Free As Integer
    Dim Append As String
    Dim I As Integer
    Dim TileName As String
    
    'get tile name
    TileName = InputBox("Enter a name for the tile: ", "New Tile")
    If TileName <> "" Then
        
        'add a new tile
        Append = ""
        Do While FileExists(App.Path & "\" & TilesFolder & "\" & TileName & Append & ".tile.txt")
            Append = Val(Append) + 1
        Loop
        Free = FreeFile
        FileCopy App.Path & "\default_tile.tile.txt", App.Path & "\" & TilesFolder & "\" & TileName & Append & ".tile.txt"
        
        RefreshTileList
        
        For I = 0 To lstTiles.ListCount - 1
            If lstTiles.List(I) = "New_Tile" & Append Then
                lstTiles.ListIndex = I
                Exit For
            End If
        Next I
    End If
End Sub

Private Sub cmdClose_Click()
    Me.Hide
End Sub

Private Sub cmdDelete_Click()
    Dim PrevIndex As Integer
    
    'delete the tile
    If lstTiles.ListIndex > -1 Then
        PrevIndex = lstTiles.ListIndex
        Kill App.Path & "\" & TilesFolder & "\" & lstTiles.List(lstTiles.ListIndex) & ".tile.txt"
        RefreshTileList
        If PrevIndex >= lstTiles.ListCount Then PrevIndex = lstTiles.ListCount - 1
        lstTiles.ListIndex = PrevIndex
    End If
End Sub

Private Sub cmdRefresh_Click()
    RefreshTileList
End Sub

Public Sub RefreshTileList()
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
    If lstTiles.ListIndex >= 0 Then LastTileName = lstTiles.List(lstTiles.ListIndex)
    
    lstTiles.Visible = False
    lstTiles.Clear
    For I = 0 To NumTileFiles - 1
        NiceName = StripExtension(StripExtension(TileFiles(I)))
        lstTiles.AddItem NiceName
        If NiceName = LastTileName Then LastTileIndex = I
    Next I
    lstTiles.Visible = True
    
    'select previously selected index
    If LastTileName <> "" Then
        lstTiles.ListIndex = LastTileIndex
        lstTiles_Click
    End If
    

End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    If UnloadMode = vbFormControlMenu Then
        Cancel = -1
        cmdClose_Click
    End If
End Sub

Private Sub Form_Resize()
    If Me.ScaleWidth > MinScaleWidth And Me.ScaleHeight > MinScaleHeight Then
        cmdClose.Move Me.ScaleWidth - Margin - cmdClose.Width, Me.ScaleHeight - Margin - cmdClose.Height
        lstTiles.Move Margin, lblAvailable.Top + lblAvailable.Height + Margin, Me.ScaleWidth / 2 - Margin * 2, cmdClose.Top - Margin - (lblAvailable.Top + lblAvailable.Height + Margin)
        lblAvailable.Left = lstTiles.Left + lstTiles.Width / 2 - lblAvailable.Width / 2
        cmdRefresh.Left = lstTiles.Left + lstTiles.Width - cmdRefresh.Width
        
        txtEdit.Move lstTiles.Left + lstTiles.Width + Margin * 2, lblEdit.Top + lblEdit.Height + Margin, Me.ScaleWidth - Margin - (lstTiles.Left + lstTiles.Width + Margin * 2), cmdClose.Top - Margin - (lblEdit.Top + lblEdit.Height + Margin)
        lblEdit.Left = txtEdit.Left + txtEdit.Width / 2 - lblEdit.Width / 2
        
        cmdAdd.Move lstTiles.Left, lstTiles.Top + lstTiles.Height + Margin
        cmdDelete.Move cmdAdd.Left + cmdAdd.Width + Margin, cmdAdd.Top
        picPreview.Move lstTiles.Left + lstTiles.Width - picPreview.Width, cmdAdd.Top, TileWidth, TileHeight
    End If
End Sub

Private Sub lstTiles_Click()
    Dim Temp As New Tile
    
    On Error GoTo TileErr
    
    Temp.FromFile App.Path & "\" & TilesFolder & "\" & TileFiles(lstTiles.ListIndex)
    
    txtEdit.Text = Temp.ToString
    
    'tile preview
    ComputeTileBitmap Temp
    picPreview.Cls
    picPreview.PaintPicture frmMain.picMaskAnd.Image, 0, 0, , , , , , , vbSrcAnd
    picPreview.PaintPicture frmMain.picMaskOr.Image, 0, 0, , , , , , , vbSrcPaint
    
    
    Exit Sub
TileErr:
    ProcessError "Error loading tile or bitmap: " & Err.Description
    Exit Sub
End Sub



Private Sub lstTiles_KeyDown(KeyCode As Integer, Shift As Integer)
    If KeyCode = vbKeyDelete Then cmdDelete_Click
End Sub

Private Sub txtEdit_Change()
    Dim Temp As New Tile
    If lstTiles.ListIndex > -1 Then
        Temp.FromString txtEdit.Text
        Temp.ToFile App.Path & "\" & TilesFolder & "\" & TileFiles(lstTiles.ListIndex)
    End If
End Sub

