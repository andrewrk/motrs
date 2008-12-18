Attribute VB_Name = "Globals"
Option Explicit


Public Const TileWidth As Integer = 32
Public Const TileHeight As Integer = 32

Public Const CharactersFolder As String = "..\..\characters"
Public Const TilesFolder As String = "..\..\tiles"
Public Const BitmapsFolder As String = "..\..\resource\bitmaps"
Public Const MapsFolder As String = "..\..\resource\maps"
Public Const AnimationsFolder As String = "..\..\animations"


Public Const Margin As Integer = 6

Public Sub ProcessError(Msg As String)
    frmMain.lblErr.Caption = Msg
    frmMain.lblErr.Visible = True
    frmMain.tmrClearErr.Enabled = True
End Sub

Public Function StripExtension(FileTitle As String) As String
    Dim Pos As Integer
    
    Pos = InStrRev(FileTitle, ".")
    
    If Pos > 0 Then
        StripExtension = Mid$(FileTitle, 1, Pos - 1)
    Else
        StripExtension = FileTitle
    End If
End Function

Public Function ColorFromString(Str As String) As Long
    Dim Pos As Integer
    Dim Pos2 As Integer
    Dim Red As Integer, Green As Integer, Blue As Integer
    
    Pos = InStr(1, Str, ",")
    Pos2 = InStr(Pos + 1, Str, ",")
    
    Red = Val(Mid$(Str, 1, Pos - 1))
    Green = Val(Mid$(Str, Pos + 1, Pos2 - Pos))
    Blue = Val(Mid$(Str, Pos2 + 1, Len(Str) - Pos2))
    
    ColorFromString = RGB(Red, Green, Blue)
    
End Function

Public Function Min(Number1 As Variant, Number2 As Variant) As Variant
    If Number1 < Number2 Then
        Min = Number1
    Else
        Min = Number2
    End If
End Function

Public Function Max(Number1 As Variant, Number2 As Variant) As Variant
    If Number1 > Number2 Then
        Max = Number1
    Else
        Max = Number2
    End If
End Function

Public Function FileExists(File As String) As Boolean
    Dim Free As Integer
    
    Free = FreeFile
    On Error GoTo NoExist
    Open File For Input As #Free
    Close #Free
    FileExists = True
    Exit Function
NoExist:
    FileExists = False
End Function

Public Sub CreateMask(InPic As PictureBox, OrPic As PictureBox, AndPic As PictureBox, Alpha As Long)
    Dim PtColor As Long
    Dim X As Integer, Y As Integer
    
    AndPic.Move 0, 0, InPic.Width, InPic.Height
    OrPic.Move 0, 0, InPic.Width, InPic.Height
    OrPic.Cls
    AndPic.Cls
    For X = 0 To InPic.ScaleWidth - 1
        For Y = 0 To InPic.ScaleHeight - 1
            PtColor = InPic.Point(X, Y)
            If PtColor = Alpha Then
                OrPic.PSet (X, Y), vbBlack
                AndPic.PSet (X, Y), vbWhite
            Else
                OrPic.PSet (X, Y), PtColor
                AndPic.PSet (X, Y), vbBlack
            End If
        Next Y
    Next X
End Sub

Public Sub ChangeBackcolor(InPic As PictureBox, OutPic As PictureBox, Alpha As Long, NewBackcolor As Long)
    Dim PtColor As Long
    Dim X As Integer, Y As Integer
    
    OutPic.Move 0, 0, InPic.Width, InPic.Height
    OutPic.PaintPicture InPic.Image, 0, 0
    For X = 0 To InPic.ScaleWidth - 1
        For Y = 0 To InPic.ScaleHeight - 1
            PtColor = InPic.Point(X, Y)
            If PtColor = Alpha Then OutPic.PSet (X, Y), NewBackcolor
        Next Y
    Next X
End Sub

Public Sub ComputeTileFileBitmap(TileFile As String)
    Dim TempTile As New Tile
    
    TempTile.FromFile TileFile
    ComputeTileBitmap TempTile
End Sub

Public Sub ComputeTileBitmap(TileObj As Tile)
    Dim TileAlpha As String
    Dim TileBitmap As String
    Dim TileAnimation As String
    Dim Str As String, File As String
    Dim Ani As Tile
    
    On Error GoTo TileErr
    TileBitmap = TileObj.GetProperty("bitmap")
    TileAnimation = TileObj.GetProperty("animation")
    
    frmMain.picMaskAnd.Move 0, 0, TileWidth, TileHeight
    frmMain.picMaskOr.Move 0, 0, TileWidth, TileHeight
    frmMain.picMaskInput.Cls
    frmMain.picMaskAnd.Cls
    frmMain.picMaskOr.Cls
    If TileObj.GetProperty("visible") = "false" Then
        frmMain.picMaskAnd.PaintPicture frmMain.imgInvisibleTile.Picture, 0, 0
        Exit Sub
    ElseIf TileAnimation <> "none" And TileAnimation <> "" Then
        Str = StripExtension(TileAnimation)
        File = Dir(App.Path & "\" & AnimationsFolder & "\" & Str & "\01*.bmp")
        If File <> "" Then
            Set Ani = New Tile
            Ani.FromFile App.Path & "\" & AnimationsFolder & "\" & Str & "\properties.txt"
            
            frmMain.picMaskInput.Picture = LoadPicture(App.Path & "\" & AnimationsFolder & "\" & Str & "\" & File)
            TileAlpha = Ani.GetProperty("alphacolor")
            If TileAlpha <> "none" And TileAlpha <> "" Then
                CreateMask frmMain.picMaskInput, frmMain.picMaskOr, frmMain.picMaskAnd, GetColorFromStr(TileAlpha)
            Else
                frmMain.picMaskAnd.PaintPicture frmMain.picMaskInput.Picture, 0, 0
            End If
            
            Exit Sub
        End If
    ElseIf TileBitmap <> "none" And TileBitmap <> "" Then
        frmMain.picMaskInput.Picture = LoadPicture(App.Path & "\" & BitmapsFolder & "\" & TileBitmap)
        TileAlpha = TileObj.GetProperty("alpha")
        If TileAlpha <> "none" And TileAlpha <> "" Then
            CreateMask frmMain.picMaskInput, frmMain.picMaskOr, frmMain.picMaskAnd, GetColorFromStr(TileAlpha)
        Else
            frmMain.picMaskAnd.PaintPicture frmMain.picMaskInput.Picture, 0, 0
        End If
        Exit Sub
    End If
    
    frmMain.picMaskOr.Cls
    frmMain.picMaskAnd.PaintPicture frmMain.imgInvalidBitmap.Picture, 0, 0
    
    Exit Sub
TileErr:
    ProcessError "Error computing picture for tile: " & Err.Description
End Sub

Public Function GetColorFromStr(Str As String) As Long
    Dim AlphaStr As String
    Dim RGBArray(0 To 2) As Integer
    Dim Pos As Integer, Pos2 As Integer, Index As Integer
    
    AlphaStr = "," & Str & ","
    Pos = 1
    Pos2 = InStr(Pos + 1, AlphaStr, ",")
    Index = 0
    Do While Pos > 0 And Index < 3
        RGBArray(Index) = Trim$(Mid$(AlphaStr, Pos + 1, Pos2 - Pos - 1))
        
        Pos = Pos2
        Pos2 = InStr(Pos + 1, AlphaStr, ",")
        Index = Index + 1
    Loop
    GetColorFromStr = RGB(RGBArray(0), RGBArray(1), RGBArray(2))
End Function
