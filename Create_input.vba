Sub Create_Input()
    Dim ApiName As String
    ApiName = ActiveSheet.Name
    
    Dim TcID As Integer, TcID_Col As Integer, TcID_Row As Integer
    
    'Initialize the first cell
    TcID_Col = 3
    TcID_Row = 6
    TcID = 1
    
    'Get range of parameter num
    Dim paraNum As Range
    Set paraNum = Range("B:B").Find("num (RAI_UINT)")
    
    Dim paraNum_FirstRow As Integer, paraNum_LastRow As Integer
    paraNum_FirstRow = paraNum.Row + 1
    paraNum_LastRow = paraNum.Row + 1

    Do Until Cells(paraNum_LastRow, 2).Interior.ColorIndex <> -4142    '-4142 is index of default color (white)
        paraNum_LastRow = paraNum_LastRow + 1
    Loop
    
    'Create data files and link them to PCL.
    Do Until TcID = 0
        Dim Formula As String

        TcID = Cells(TcID_Row, TcID_Col)
        If TcID = 0 Then Exit Do

        'Get the number of elements
        Dim count As Integer
        Dim num As Integer
        For count = paraNum_FirstRow To (paraNum_LastRow - 1)
            If Cells(count, TcID_Col) = "O" Then
                num = Cells(count, 2)
                Exit For
            End If
        Next count

        'Create new sheet
        Dim Link As String
        Dim NewSheetName As String
        
        NewSheetName = "TC" & Format(CStr(TcID), "00#")
        If CheckIfSheetExists(NewSheetName) = False Then
            Worksheets("Template").Copy After:=Worksheets(Worksheets.count)
            ActiveSheet.Name = NewSheetName
            
            Dim index As Range
            Set index = Range("A1:A30").Find("Index")
            
            For count = 1 To num
                Cells(index.Row, count + 1) = count
            Next count
            
            Dim tmp As Range
            Set tmp = Range("A:A").Find("Number of elements")
            Range(Cells(1, 1), Cells(30, num + 1)).Locked = False
            Cells(tmp.Row, (tmp.Column + 1)) = num
            Range(Cells(tmp.Row, tmp.Column), Cells(tmp.Row, (tmp.Column + 1))).Interior.ColorIndex = 44
            
            'Cells(tmp.row, (tmp.Column + 1)).Locked = True
            'ActiveSheet.Shape.Locked = False
            'ActiveSheet.Protect Password:="123"
            
        End If
        
        Worksheets(ApiName).Activate
        
        'Add hyperlink
        ActiveSheet.Hyperlinks.Add Anchor:=Cells(TcID_Row, TcID_Col), Address:="", SubAddress:=NewSheetName & "!A1"
        
        'Next test case
        TcID_Col = TcID_Col + 1
    Loop
    
    Worksheets(ApiName).Range("6:6").Font.Underline = False
    
End Sub

Function CheckIfSheetExists(SheetName As String) As Boolean
      CheckIfSheetExists = False
      For Each WS In Worksheets
        If SheetName = WS.Name Then
          CheckIfSheetExists = True
          Exit Function
        End If
      Next WS
End Function
