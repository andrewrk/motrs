VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmBitmapSplicer 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Bitmap Splicer"
   ClientHeight    =   4065
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   8610
   Icon            =   "frmBitmapSplicer.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   271
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   574
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picPreview 
      AutoRedraw      =   -1  'True
      BorderStyle     =   0  'None
      Height          =   480
      Left            =   3720
      ScaleHeight     =   32
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   32
      TabIndex        =   9
      TabStop         =   0   'False
      Top             =   3000
      Width           =   480
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   375
      Left            =   7200
      TabIndex        =   13
      Top             =   3600
      Width           =   1335
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Height          =   375
      Left            =   5760
      TabIndex        =   12
      Top             =   3600
      Width           =   1335
   End
   Begin VB.TextBox txtGenericName 
      Height          =   285
      Left            =   1320
      TabIndex        =   4
      Top             =   3000
      Width           =   1455
   End
   Begin VB.TextBox txtTileEdit 
      Height          =   2295
      Left            =   5160
      MultiLine       =   -1  'True
      TabIndex        =   11
      Top             =   600
      Width           =   3255
   End
   Begin VB.ListBox lstTiles 
      Height          =   2340
      IntegralHeight  =   0   'False
      Left            =   3000
      TabIndex        =   7
      Top             =   600
      Width           =   1935
   End
   Begin VB.CommandButton cmdBrowse 
      Caption         =   "Browse..."
      Height          =   375
      Left            =   1080
      TabIndex        =   2
      Top             =   1440
      Width           =   975
   End
   Begin VB.PictureBox picBitmap 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   2295
      Left            =   120
      ScaleHeight     =   153
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   177
      TabIndex        =   1
      Top             =   600
      Width           =   2655
   End
   Begin MSComDlg.CommonDialog cdl 
      Left            =   120
      Top             =   120
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Shape shpAlpha 
      FillColor       =   &H00FF00FF&
      FillStyle       =   0  'Solid
      Height          =   255
      Left            =   1320
      Top             =   3360
      Width           =   255
   End
   Begin VB.Label lblAlpha 
      AutoSize        =   -1  'True
      Caption         =   "Alpha color:"
      Height          =   195
      Left            =   120
      TabIndex        =   5
      Top             =   3360
      Width           =   840
   End
   Begin VB.Label lblPreview 
      AutoSize        =   -1  'True
      Caption         =   "Preview:"
      Height          =   195
      Left            =   3000
      TabIndex        =   8
      Top             =   3000
      Width           =   615
   End
   Begin VB.Label lblGenericName 
      AutoSize        =   -1  'True
      Caption         =   "Generic name:"
      Height          =   195
      Left            =   120
      TabIndex        =   3
      Top             =   3000
      Width           =   1035
   End
   Begin VB.Label lblTileEdit 
      AutoSize        =   -1  'True
      Caption         =   "Tile properties:"
      Height          =   195
      Left            =   6360
      TabIndex        =   10
      Top             =   240
      Width           =   1035
   End
   Begin VB.Label lblResults 
      AutoSize        =   -1  'True
      Caption         =   "Resulting tiles:"
      Height          =   195
      Left            =   3480
      TabIndex        =   6
      Top             =   240
      Width           =   1020
   End
   Begin VB.Label lbl 
      AutoSize        =   -1  'True
      Caption         =   "Bitmap"
      Height          =   195
      Index           =   0
      Left            =   1200
      TabIndex        =   0
      Top             =   240
      Width           =   480
   End
End
Attribute VB_Name = "frmBitmapSplicer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Type TileEntry
    TileItem As Tile
    TileName As String
    Index As Integer 'which lstTiles index it is
End Type

Private Type Coord
    X As Integer
    Y As Integer
End Type

Private Const InitPicWidth As Integer = 177
Private Const InitPicHeight As Integer = 153

Private NewTiles() As TileEntry
Private NewTilesWidth As Integer, NewTilesHeight As Integer

Private TileIndexes() As Coord

Private CodeGenerated As Boolean

Private Sub cmdBrowse_Click()
    Dim X As Integer, Y As Integer
    
    cdl.DialogTitle = "Find bitmap to splice"
    cdl.FileName = ""
    cdl.Filter = "Bitmaps (*.bmp)|*.bmp|All files (*)|*"
    cdl.Flags = cdlOFNHideReadOnly
    cdl.ShowOpen
    
    If cdl.FileName = "" Then Exit Sub
    
    'load picture into picturebox
    On Error GoTo PicErr
    picBitmap.Picture = LoadPicture(cdl.FileName)
    On Error GoTo 0
    
    picBitmap.Visible = True
    cmdBrowse.Visible = False
    txtTileEdit.Enabled = True
    ResizeForm
    
    NewTilesWidth = picBitmap.ScaleWidth / TileWidth
    NewTilesHeight = picBitmap.ScaleHeight / TileHeight
    
    If NewTilesWidth * TileWidth <> picBitmap.ScaleHeight Or NewTilesHeight * TileHeight <> picBitmap.ScaleHeight Then ProcessError "WARNING: This bitmap will not split perfectly into tiles."
    
    If NewTilesWidth = 0 Or NewTilesHeight = 0 Then
        ProcessError "That picture is not big enough to splice."
        cmdCancel_Click
    End If
    
    'create tiles in memory
    CodeGenerated = True
    txtGenericName.Text = StripExtension(cdl.FileTitle)
    CodeGenerated = False
    
    lstTiles.Clear
    ReDim NewTiles(NewTilesWidth - 1, NewTilesHeight - 1)
    ReDim TileIndexes(NewTilesWidth * NewTilesHeight - 1)
    For X = 0 To NewTilesWidth - 1
        For Y = 0 To NewTilesHeight - 1
            Set NewTiles(X, Y).TileItem = New Tile
            NewTiles(X, Y).TileItem.SetProperty "bitmap", txtGenericName.Text & " (" & X & ", " & Y & ").bmp"
            NewTiles(X, Y).TileItem.SetProperty "alpha", ColorToRGBStr(shpAlpha.FillColor)
            NewTiles(X, Y).TileItem.SetProperty "obstacle", "false"
            
            NewTiles(X, Y).Index = lstTiles.ListCount
            NewTiles(X, Y).TileName = txtGenericName.Text & " (" & X & ", " & Y & ")"
            lstTiles.AddItem NewTiles(X, Y).TileName
            
            TileIndexes(NewTiles(X, Y).Index).X = X
            TileIndexes(NewTiles(X, Y).Index).Y = Y
            
        Next Y
    Next X
    
    Exit Sub
PicErr:
    ProcessError "Unable to load picture to splice bitmap: " & Err.Description
End Sub

Private Function ColorToRGBStr(Color As Long) As String
    Dim Ret As RGBTriplet
    
    Ret = DecToRGB(Color)
    ColorToRGBStr = CStr(Ret.Red) & ", " & CStr(Ret.Green) & ", " & CStr(Ret.Blue)
End Function


Public Sub ResetForm()
    picBitmap.Picture = LoadPicture()
    cmdBrowse.Visible = True
    picBitmap.Visible = False
    shpAlpha.FillColor = vbMagenta
    txtTileEdit.Enabled = False
    
    
    picBitmap.Move picBitmap.Left, picBitmap.Top, InitPicWidth, InitPicHeight
    ResizeForm
End Sub


Private Sub ResizeForm()
    lstTiles.Move picBitmap.Left + Max(picBitmap.Width, InitPicWidth) + Margin, picBitmap.Top
    txtTileEdit.Move lstTiles.Left + lstTiles.Width + Margin, picBitmap.Top
    
    lblResults.Left = lstTiles.Left + lstTiles.Width / 2 - lblResults.Width / 2
    lblTileEdit.Left = txtTileEdit.Left + txtTileEdit.Width / 2 - lblTileEdit.Width / 2
    
    cmdBrowse.Move picBitmap.Left + picBitmap.Width / 2 - cmdBrowse.Width / 2, picBitmap.Top + picBitmap.Height / 2 - cmdBrowse.Height / 2
    lblGenericName.Move picBitmap.Left, picBitmap.Top + Max(picBitmap.Height, InitPicHeight) + Margin
    txtGenericName.Move lblGenericName.Left + lblGenericName.Width + Margin, lblGenericName.Top
    
    shpAlpha.Move txtGenericName.Left, txtGenericName.Top + txtGenericName.Height + Margin
    lblAlpha.Move picBitmap.Left, shpAlpha.Top
    
    lblPreview.Move lstTiles.Left, lblGenericName.Top
    picPreview.Move lblPreview.Left + lblPreview.Width + Margin, lblPreview.Top, TileWidth, TileHeight
    
    cmdOK.Top = txtGenericName.Top + txtGenericName.Height + Margin
    cmdCancel.Top = cmdOK.Top
    
    Me.Move Me.Left, Me.Top, (txtTileEdit.Left + txtTileEdit.Width + Margin) * Screen.TwipsPerPixelX, (cmdOK.Top + cmdOK.Height + Margin) * Screen.TwipsPerPixelY
    Do While Me.ScaleWidth < txtTileEdit.Left + txtTileEdit.Width + Margin
        Me.Width = Me.Width + 10
    Loop
    Do While Me.ScaleHeight < cmdOK.Top + cmdOK.Height + Margin
        Me.Height = Me.Height + 10
    Loop
    
    
    cmdCancel.Left = Me.ScaleWidth - Margin - cmdCancel.Width
    cmdOK.Left = cmdCancel.Left - Margin - cmdOK.Width
End Sub

Private Sub cmdCancel_Click()
    Me.Hide
End Sub

Private Sub cmdOK_Click()
    Dim X As Integer, Y As Integer
    Dim SquX As Integer, SquY As Integer
    'save tiles and include in map editor
    
    frmMain.picMaskOr.Cls
    frmMain.picMaskOr.Move 0, 0, TileWidth, TileHeight
    
    For SquX = 0 To NewTilesWidth - 1
        For SquY = 0 To NewTilesHeight - 1
            'save to file
            NewTiles(SquX, SquY).TileItem.ToFile App.Path & "\" & TilesFolder & "\" & NewTiles(SquX, SquY).TileName & ".tile.txt"
            
            'save bitmap to file
            frmMain.picMaskOr.PaintPicture picBitmap.Picture, 0, 0, , , SquX * TileWidth, SquY * TileHeight, TileWidth, TileHeight
            SavePicture frmMain.picMaskOr.Image, App.Path & "\" & BitmapsFolder & "\" & NewTiles(SquX, SquY).TileItem.GetProperty("bitmap")
            
            'add to tile list in frmmain
            frmMain.AddTile NewTiles(SquX, SquY).TileName
        Next SquY
    Next SquX
    
    'build masks
    frmMain.UpdateTilesList
    
    'go back to main form
    Me.Hide
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    If UnloadMode = vbFormControlMenu Then
        Cancel = -1
        cmdCancel_Click
    End If
End Sub

Private Sub lstTiles_Click()
    Dim X As Integer, Y As Integer
    Dim PtColor As Long
    
    If lstTiles.ListIndex = -1 Then Exit Sub
    
    
    picBitmap_Paint
    
    txtTileEdit.Text = NewTiles(TileIndexes(lstTiles.ListIndex).X, TileIndexes(lstTiles.ListIndex).Y).TileItem.ToString

    'show preview
    picPreview.Cls
    picBitmap.Cls
    For Y = 0 To TileHeight - 1
        For X = 0 To TileWidth - 1
            PtColor = picBitmap.Point(TileIndexes(lstTiles.ListIndex).X * TileWidth + X, TileIndexes(lstTiles.ListIndex).Y * TileHeight + Y)
            If PtColor <> shpAlpha.FillColor Then
                picPreview.PSet (X, Y), PtColor
            End If
        Next X
    Next Y
    
    picBitmap_Paint
End Sub

Private Sub picBitmap_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim SquX As Integer, SquY As Integer
    If Button = vbKeyRButton Then
        'set alpha color
        picBitmap.Cls
        shpAlpha.FillColor = picBitmap.Point(X, Y)
        picBitmap_Paint
        
        For SquX = 0 To NewTilesWidth - 1
            For SquY = 0 To NewTilesHeight - 1
                NewTiles(SquX, SquY).TileItem.SetProperty "alpha", ColorToRGBStr(shpAlpha.FillColor)
            Next SquY
        Next SquX
        
        
        lstTiles_Click
    ElseIf Button = vbKeyLButton Then
        'select this square
        SquX = Int(X / TileWidth)
        SquY = Int(Y / TileHeight)
        lstTiles.ListIndex = NewTiles(SquX, SquY).Index
    End If
End Sub

Private Sub picBitmap_Paint()
    Dim I As Single
    
    picBitmap.Cls
    
    For I = 0 To picBitmap.ScaleHeight Step TileHeight
        picBitmap.Line (0, I)-(picBitmap.ScaleWidth, I), vbBlack
    Next I
    For I = 0 To picBitmap.ScaleWidth Step TileWidth
        picBitmap.Line (I, 0)-(I, picBitmap.ScaleHeight), vbBlack
    Next I
    
    'selected square
    If lstTiles.ListIndex > -1 Then
        picBitmap.DrawWidth = 3
        picBitmap.DrawMode = vbInvert
        picBitmap.Line (TileIndexes(lstTiles.ListIndex).X * TileWidth, TileIndexes(lstTiles.ListIndex).Y * TileHeight)-(TileIndexes(lstTiles.ListIndex).X * TileWidth + TileWidth, TileIndexes(lstTiles.ListIndex).Y * TileHeight + TileHeight), vbBlack, B
        picBitmap.DrawWidth = 1
        picBitmap.DrawMode = vbCopyPen
    End If
End Sub

Private Sub txtGenericName_Change()
    Dim X As Integer, Y As Integer
    If Not CodeGenerated Then
        'update tiles
        For X = 0 To NewTilesWidth - 1
            For Y = 0 To NewTilesHeight - 1
                NewTiles(X, Y).TileName = txtGenericName.Text & " (" & X & ", " & Y & ")"
                NewTiles(X, Y).TileItem.SetProperty "bitmap", NewTiles(X, Y).TileName & ".bmp"
                lstTiles.List(NewTiles(X, Y).Index) = NewTiles(X, Y).TileName
            Next Y
        Next X
        
        If lstTiles.ListIndex > -1 Then lstTiles_Click
    End If
End Sub

Private Sub txtTileEdit_Change()
    NewTiles(TileIndexes(lstTiles.ListIndex).X, TileIndexes(lstTiles.ListIndex).Y).TileItem.FromString txtTileEdit.Text
End Sub
