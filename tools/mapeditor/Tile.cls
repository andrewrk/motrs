VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "Tile"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private NumProperties As Integer


Private PropertyNames() As String
Private PropertyValues() As String


Private Sub ProcessLine(Line As String)
    Dim Pos As Integer
    
    If Line = "=" Or Line = "" Then Exit Sub
    
    'get properties
    Pos = InStr(1, Line, "=")
    If Pos > 0 Then
        ReDim Preserve PropertyNames(NumProperties)
        ReDim Preserve PropertyValues(NumProperties)
        
        PropertyNames(NumProperties) = Mid$(Line, 1, Pos - 1)
        PropertyValues(NumProperties) = Mid$(Line, Pos + 1, Len(Line) - Pos)
        
        If Right$(PropertyValues(NumProperties), 2) = vbCrLf Then PropertyValues(NumProperties) = Left$(PropertyValues(NumProperties), Len(PropertyValues(NumProperties)) - 2)
        
        NumProperties = NumProperties + 1
    End If
End Sub

Public Sub FromFile(File As String)
    Dim Free As Integer
    Dim Line As String
    Dim Pos As Integer, PosStart As Integer
    
    Class_Initialize
    
    Free = FreeFile()
    
    'load the tile file and show the preview
    Open File For Input As #Free
        Do While Not EOF(Free)
            Line Input #Free, Line
            
            'convert unix to windoze
            Pos = InStr(1, Line, vbLf)
            If Pos > 0 Then
                PosStart = 1
                Do While Pos > 0 And Mid$(Line, Pos - 1, 1) <> vbCr
                    ProcessLine Mid$(Line, PosStart, Pos - PosStart)
                    
                    PosStart = Pos + 1
                    Pos = InStr(PosStart, Line, vbLf)
                    If Pos = 0 Then Exit Do
                Loop
            Else
                ProcessLine Line
            End If
        Loop
    Close #Free

End Sub

Public Sub ToFile(File As String)
    Dim Free As Integer
    Dim I As Integer
    
    Free = FreeFile()
    
    'load the tile file and show the preview
    Open File For Output As #Free
        For I = 0 To NumProperties - 1
            Print #Free, PropertyNames(I) & "=" & PropertyValues(I)
        Next I
    Close #Free

End Sub

Public Function ToString() As String
    Dim I As Integer
    
    ToString = ""
    For I = 0 To NumProperties - 1
        ToString = ToString & PropertyNames(I) & "=" & PropertyValues(I) & vbCrLf
    Next I

End Function

Public Sub FromString(Str As String)
    Dim Pos As Integer, PosStart As Integer
    
    Class_Initialize
    
    Str = Str & vbCrLf
    
    Pos = InStr(1, Str, vbCrLf)
    If Pos > 0 Then
        PosStart = 1
        Do While Pos > 0
            ProcessLine Mid$(Str, PosStart, Pos - PosStart)
            
            PosStart = Pos + 2
            Pos = InStr(PosStart, Str, vbCrLf)
            If Pos = 0 Then Exit Do
        Loop
    Else
        ProcessLine Str
    End If

End Sub

Private Sub Class_Initialize()
    NumProperties = 0
End Sub

Public Function GetProperty(PropertyName As String)
    Dim I As Integer
    
    For I = 0 To NumProperties - 1
        If PropertyNames(I) = PropertyName Then
            GetProperty = PropertyValues(I)
            Exit Function
        End If
    Next I
    
    GetProperty = ""
End Function

Public Sub SetProperty(PropertyName As String, PropertyValue As String)
    Dim I As Integer
    
    For I = 0 To NumProperties - 1
        If PropertyNames(I) = PropertyName Then
            PropertyValues(I) = PropertyValue
            Exit Sub
        End If
    Next I
    
    'doesn't exist, add it
    ReDim Preserve PropertyNames(NumProperties)
    ReDim Preserve PropertyValues(NumProperties)
    
    PropertyNames(NumProperties) = PropertyName
    PropertyValues(NumProperties) = PropertyValue
    
    NumProperties = NumProperties + 1
End Sub



Public Property Get NumProps() As Integer
    NumProps = NumProperties
End Property
