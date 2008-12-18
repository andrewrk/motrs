Attribute VB_Name = "QSortString"
Option Explicit

Private Function QSortPartition(StrArray() As String, LeftI As Integer, RightI As Integer, PivotIndex As Integer) As Integer
    Dim PivotValue As String
    Dim StoreIndex As Integer
    Dim I As Integer
    
    PivotValue = StrArray(PivotIndex)
    QSortSwap StrArray, PivotIndex, RightI
    StoreIndex = LeftI
    For I = LeftI To RightI - 1
        If StrArray(I) <= PivotValue Then
            QSortSwap StrArray, I, StoreIndex
            StoreIndex = StoreIndex + 1
        End If
    Next I
    QSortSwap StrArray, StoreIndex, RightI
    QSortPartition = StoreIndex
End Function

Private Sub QSortSwap(StrArray() As String, Index1 As Integer, Index2 As Integer)
    Dim Temp As String
    
    Temp = StrArray(Index1)
    StrArray(Index1) = StrArray(Index2)
    StrArray(Index2) = Temp
End Sub

Public Sub QSort(StrArray() As String, LeftI As Integer, RightI As Integer)
    Dim PivotIndex As Integer, PivotNewIndex As Integer
    
    If RightI > LeftI Then
        PivotIndex = LeftI
        PivotNewIndex = QSortPartition(StrArray, LeftI, RightI, PivotIndex)
        QSort StrArray, LeftI, PivotNewIndex - 1
        QSort StrArray, PivotNewIndex + 1, RightI
    End If

End Sub

