Attribute VB_Name = "Hexadecimal"
Option Explicit

Public Type RGBTriplet
    Red As Long
    Green As Long
    Blue As Long
End Type


Public Function DecToRGB(ByVal Dec As Long) As RGBTriplet
    Const STEPGREEN = 256
    Const STEPBLUE = 65536
    
    
    DecToRGB.Blue = Int(Dec / STEPBLUE)
    Dec = Dec Mod STEPBLUE
    DecToRGB.Green = Int(Dec / STEPGREEN)
    DecToRGB.Red = Dec Mod STEPGREEN
End Function

Public Function BitsFromByte(ByVal Num As Long, Optional LenStr As Integer) As String
    Dim I As Integer
    Dim Bit As Integer
    Dim Max As Integer
    
    BitsFromByte = ""
    
    Do Until 2 ^ Max > Num
        Max = Max + 1
    Loop
    Max = Max - 1
    
    For I = Max To 0 Step -1
        Bit = Int(Num / (2 ^ I))
        Num = Num Mod (2 ^ I)
        
        BitsFromByte = BitsFromByte & CStr(Bit)
    Next I
    
    If LenStr > 0 Then
        Do Until Len(BitsFromByte) >= LenStr
            BitsFromByte = "0" & BitsFromByte
        Loop
        
        BitsFromByte = Right(BitsFromByte, LenStr)
    End If
End Function

Public Function AdjustBrightness(Color As Long, Percent As Double) As Long
    'percent = 1 returns white, percent = -1 returns black, percent = 0 returns no change


    Dim MinOffset As Long, MaxOffset As Long
    Dim Amt As Long
    Dim Col As RGBTriplet
    
    Col = DecToRGB(Color)
    
    If -Col.Red < MinOffset Then MinOffset = -Col.Red
    If -Col.Green < MinOffset Then MinOffset = -Col.Green
    If -Col.Blue < MinOffset Then MinOffset = -Col.Blue
    
    If Col.Red < MaxOffset Then MaxOffset = 255 - Col.Red
    If Col.Green < MaxOffset Then MaxOffset = 255 - Col.Green
    If Col.Blue < MaxOffset Then MaxOffset = 255 - Col.Blue

    Amt = Math.Round((MaxOffset - MinOffset) * Percent, 0)
    
    AdjustBrightness = RGB(Max(Min(Col.Red + Amt, 255), 0), Max(Min(Col.Green + Amt, 255), 0), Max(Min(Col.Blue + Amt, 255), 0))
End Function

Public Function ColorDifference(Color1 As Long, Color2 As Long) As Long
    Dim Color1RGB As RGBTriplet
    Dim Color2RGB As RGBTriplet
    
    Color1RGB = DecToRGB(Color1)
    Color2RGB = DecToRGB(Color2)
    
    ColorDifference = Math.Abs(Color1RGB.Red - Color2RGB.Red) + Math.Abs(Color1RGB.Green - Color2RGB.Green) + Math.Abs(Color1RGB.Blue - Color2RGB.Blue)
End Function

