VERSION 5.00
Object = "{7493F856-8B5B-47C2-8336-71D5CA8433D9}#1.0#0"; "WebKitX.ocx"
Object = "{BCA00000-0F85-414C-A938-5526E9F1E56A}#4.0#0"; "cmax40.dll"
Begin VB.Form Form1 
   AutoRedraw      =   -1  'True
   Caption         =   "Form1"
   ClientHeight    =   9060
   ClientLeft      =   225
   ClientTop       =   855
   ClientWidth     =   16140
   LinkTopic       =   "Form1"
   ScaleHeight     =   604
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   1076
   StartUpPosition =   3  'Windows Default
   Begin CodeMax4Ctl.CodeMax CodeMax1 
      Height          =   3405
      Left            =   7425
      OleObjectBlob   =   "Form1.frx":0000
      TabIndex        =   1
      Top             =   90
      Width           =   6570
   End
   Begin WebKitXLib.WebKitX WebKitX1 
      Height          =   4830
      Left            =   285
      TabIndex        =   0
      Top             =   120
      Width           =   6825
      _Version        =   65536
      _ExtentX        =   12039
      _ExtentY        =   8520
      _StockProps     =   0
   End
   Begin VB.Menu mnuFile 
      Caption         =   "File"
      Begin VB.Menu mnuOpen 
         Caption         =   "Open"
      End
      Begin VB.Menu mnuGet 
         Caption         =   "Get HTML"
      End
      Begin VB.Menu mnuPut 
         Caption         =   "Put HTML"
      End
      Begin VB.Menu sp1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuJS 
         Caption         =   "Execute JavaScript..."
      End
      Begin VB.Menu sp3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuSelectInner 
         Caption         =   "Select Inner"
      End
      Begin VB.Menu mnuSelectOuter 
         Caption         =   "Select Outer"
      End
      Begin VB.Menu mnuMoveCaret 
         Caption         =   "Move Caret"
      End
      Begin VB.Menu sp2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuEditable 
         Caption         =   "Editable"
      End
   End
   Begin VB.Menu mnuEdit 
      Caption         =   "Edit"
      Begin VB.Menu mnuUndo 
         Caption         =   "Undo"
         Shortcut        =   ^Z
      End
      Begin VB.Menu mnuRedo 
         Caption         =   "Redo"
         Shortcut        =   ^R
      End
      Begin VB.Menu sp4 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCut 
         Caption         =   "Cut"
         Shortcut        =   +{DEL}
      End
      Begin VB.Menu mnuCopy 
         Caption         =   "Copy"
         Shortcut        =   ^{INSERT}
      End
      Begin VB.Menu mnuPaste 
         Caption         =   "Paste"
         Shortcut        =   +{INSERT}
      End
      Begin VB.Menu sp17 
         Caption         =   "-"
      End
      Begin VB.Menu mnuDelete 
         Caption         =   "Delete"
         Shortcut        =   {DEL}
      End
      Begin VB.Menu mnuForwardDelete 
         Caption         =   "Forward Delete"
      End
      Begin VB.Menu sp11 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCreateLink 
         Caption         =   "Link..."
         Shortcut        =   ^H
      End
      Begin VB.Menu mnuUnlink 
         Caption         =   "Unlink"
      End
      Begin VB.Menu sp15 
         Caption         =   "-"
      End
      Begin VB.Menu mnuSelectAll 
         Caption         =   "Select All"
         Shortcut        =   ^A
      End
   End
   Begin VB.Menu mnuFormat 
      Caption         =   "Format"
      Begin VB.Menu mnuAlign 
         Caption         =   "Align"
         Begin VB.Menu mnuLeft 
            Caption         =   "Left"
         End
         Begin VB.Menu mnuCenter 
            Caption         =   "Center"
         End
         Begin VB.Menu mnuRight 
            Caption         =   "Right"
         End
         Begin VB.Menu mnuFull 
            Caption         =   "Justify"
         End
      End
      Begin VB.Menu sp20 
         Caption         =   "-"
      End
      Begin VB.Menu mnuBold 
         Caption         =   "Bold"
         Shortcut        =   ^B
      End
      Begin VB.Menu mnuItalic 
         Caption         =   "Italic"
         Shortcut        =   ^I
      End
      Begin VB.Menu mnuUnderline 
         Caption         =   "Underline"
         Shortcut        =   ^U
      End
      Begin VB.Menu mnuStrikeThrough 
         Caption         =   "Strike Through"
      End
      Begin VB.Menu sp18 
         Caption         =   "-"
      End
      Begin VB.Menu mnuSubscript 
         Caption         =   "Subscript"
      End
      Begin VB.Menu mnuSuperscript 
         Caption         =   "Superscript"
      End
      Begin VB.Menu sp6 
         Caption         =   "-"
      End
      Begin VB.Menu mnuIndent 
         Caption         =   "Indent"
      End
      Begin VB.Menu mnuOutdent 
         Caption         =   "Outdent"
      End
      Begin VB.Menu sp21 
         Caption         =   "-"
      End
      Begin VB.Menu mnuForeColor 
         Caption         =   "Fore Color..."
      End
      Begin VB.Menu mnuBackColor 
         Caption         =   "Back Color..."
      End
      Begin VB.Menu sp7 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFont 
         Caption         =   "Font..."
      End
      Begin VB.Menu sp12 
         Caption         =   "-"
      End
      Begin VB.Menu mnuStyleWithCSS 
         Caption         =   "Style with CSS"
      End
      Begin VB.Menu mnuInsertBrOnReturn 
         Caption         =   "Insert <BR> on Enter"
      End
      Begin VB.Menu sp22 
         Caption         =   "-"
      End
      Begin VB.Menu mnuRemoveFormat 
         Caption         =   "Remove Format"
         Shortcut        =   ^W
      End
   End
   Begin VB.Menu mnuInsert 
      Caption         =   "Insert"
      Begin VB.Menu mnuHead 
         Caption         =   "Heading"
         Begin VB.Menu mnuHeading 
            Caption         =   "H1"
            Index           =   1
         End
         Begin VB.Menu mnuHeading 
            Caption         =   "H2"
            Index           =   2
         End
         Begin VB.Menu mnuHeading 
            Caption         =   "H3"
            Index           =   3
         End
         Begin VB.Menu mnuHeading 
            Caption         =   "H4"
            Index           =   4
         End
         Begin VB.Menu mnuHeading 
            Caption         =   "H5"
            Index           =   5
         End
         Begin VB.Menu mnuHeading 
            Caption         =   "H6"
            Index           =   6
         End
      End
      Begin VB.Menu mnuInsertText 
         Caption         =   "Text..."
      End
      Begin VB.Menu mnuInsertHTML 
         Caption         =   "HTML..."
      End
      Begin VB.Menu mnuInsertImage 
         Caption         =   "Image..."
      End
      Begin VB.Menu sp8 
         Caption         =   "-"
      End
      Begin VB.Menu mnuInsertOrderedList 
         Caption         =   "Ordered List"
         Shortcut        =   ^L
      End
      Begin VB.Menu mnuInsertUnorderedList 
         Caption         =   "Unordered List"
         Shortcut        =   ^T
      End
      Begin VB.Menu sp9 
         Caption         =   "-"
      End
      Begin VB.Menu mnuInsertHorizontalRule 
         Caption         =   "Horizontal Rule"
      End
      Begin VB.Menu mnuInsertParagraph 
         Caption         =   "Paragraph"
      End
      Begin VB.Menu mnuFormatBlock 
         Caption         =   "Block Quote"
         Shortcut        =   ^Q
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Const S_HTML = "<html><body><button id='btn1'>Hello CEF Ελληνικά</button><br><div id='div1'>This is <span>some</span> text</div><br><div id='div2'>This is <span>some</span> text</div><br><table border=1><tr><td>col1</td><td>col2</td></tr></table><script>alert('test');</script></body></html>"

Private Sub Form_Resize()
    On Error Resume Next
    WebKitX1.Move 0, 0, ScaleWidth / 2 - 4, ScaleHeight
    CodeMax1.Move ScaleWidth / 2, 0, ScaleWidth / 2, ScaleHeight
    Err.Clear
End Sub

Private Sub mnuBackColor_Click()
    WebKitX1.ExecCommand EDIT_CMD_BACKCOLOR, "yellow"
End Sub

Private Sub mnuBold_Click()
    WebKitX1.ExecCommand EDIT_CMD_BOLD
End Sub

Private Sub mnuCenter_Click()
    WebKitX1.ExecCommand EDIT_CMD_JUSTIFYCENTER
End Sub

Private Sub mnuCopy_Click()
    WebKitX1.ExecCommand EDIT_CMD_COPY
End Sub

Private Sub mnuCreateLink_Click()
    WebKitX1.ExecCommand EDIT_CMD_CREATELINK, "http://www.mobilefx.com"
End Sub

Private Sub mnuCut_Click()
    WebKitX1.ExecCommand EDIT_CMD_CUT
End Sub

Private Sub mnuDelete_Click()
    WebKitX1.ExecCommand EDIT_CMD_DELETE
End Sub

Private Sub mnuEditable_Click()
    mnuEditable.Checked = Not mnuEditable.Checked
    If mnuEditable.Checked Then
        WebKitX1.Edit
    Else
        WebKitX1.Preview
    End If
End Sub

Private Sub mnuEnableInlineTableEditing_Click()
    WebKitX1.ExecCommand EDIT_CMD_ENABLEINLINETABLEEDITING, "true"
End Sub

Private Sub mnuEnableObjectResizing_Click()
    WebKitX1.ExecCommand EDIT_CMD_ENABLEOBJECTRESIZING
End Sub

Private Sub mnuFont_Click()
    WebKitX1.ExecCommand EDIT_CMD_FONTNAME, "Courier New"
    WebKitX1.ExecCommand EDIT_CMD_FONTSIZE, "12"
End Sub

Private Sub mnuForeColor_Click()
    WebKitX1.ExecCommand EDIT_CMD_FORECOLOR, "red"
End Sub

Private Sub mnuFormatBlock_Click()
    WebKitX1.ExecCommand EDIT_CMD_FORMATBLOCK, "PRE"
End Sub

Private Sub mnuForwardDelete_Click()
    WebKitX1.ExecCommand EDIT_CMD_FORWARDDELETE, "1"
End Sub

Private Sub mnuFull_Click()
    WebKitX1.ExecCommand EDIT_CMD_JUSTIFYFULL
End Sub

Private Sub mnuGet_Click()
    Debug.Print WebKitX1.HTML
End Sub

Private Sub mnuHeading_Click(Index As Integer)
    WebKitX1.ExecCommand EDIT_CMD_HEADING, "<H" & CStr(Index) & ">"
End Sub

Private Sub mnuHiliteColor_Click()
    WebKitX1.ExecCommand EDIT_CMD_HILITECOLOR, "blue"
End Sub

Private Sub mnuIndent_Click()
    WebKitX1.ExecCommand EDIT_CMD_INDENT
End Sub

Private Sub mnuInsertBrOnReturn_Click()
    mnuInsertBrOnReturn.Checked = Not mnuInsertBrOnReturn.Checked
    WebKitX1.ExecCommand EDIT_CMD_INSERTBRONRETURN, LCase$(mnuInsertBrOnReturn.Checked)
End Sub

Private Sub mnuInsertHorizontalRule_Click()
    WebKitX1.ExecCommand EDIT_CMD_INSERTHORIZONTALRULE
End Sub

Private Sub mnuInsertHTML_Click()
    WebKitX1.ExecCommand EDIT_CMD_INSERTHTML, "<div style='border:1px solid red'>Hello</div>"
End Sub

Private Sub mnuInsertImage_Click()
    WebKitX1.ExecCommand EDIT_CMD_INSERTIMAGE, "http://mobilefx.com/coconut2d/img/CocoNUT_32x32.png"
End Sub

Private Sub mnuInsertOrderedList_Click()
    WebKitX1.ExecCommand EDIT_CMD_INSERTORDEREDLIST
End Sub

Private Sub mnuInsertParagraph_Click()
    WebKitX1.ExecCommand EDIT_CMD_INSERTPARAGRAPH, "This is a paragraph"
End Sub

Private Sub mnuInsertText_Click()
    WebKitX1.ExecCommand EDIT_CMD_INSERTTEXT, "Hello CEF"
End Sub

Private Sub mnuInsertUnorderedList_Click()
    WebKitX1.ExecCommand EDIT_CMD_INSERTUNORDEREDLIST
End Sub

Private Sub mnuItalic_Click()
    WebKitX1.ExecCommand EDIT_CMD_ITALIC
End Sub

Private Sub mnuJS_Click()
    Dim result As String
    result = WebKitX1.ExecJavaScript("document.body.innerHTML")
    Debug.Print result
End Sub

Private Sub mnuLeft_Click()
    WebKitX1.ExecCommand EDIT_CMD_JUSTIFYLEFT
End Sub

Private Sub mnuOpen_Click()
    WebKitX1.OpenURL "http://www.mobilefx.com"
End Sub

Private Sub mnuOutdent_Click()
    WebKitX1.ExecCommand EDIT_CMD_OUTDENT
End Sub

Private Sub mnuPaste_Click()
    WebKitX1.ExecCommand EDIT_CMD_PASTE
End Sub

Private Sub mnuPut_Click()
    WebKitX1.HTML = S_HTML
End Sub

Private Sub mnuMoveCaret_Click()
    WebKitX1.SelectElement "#div1", True, False
End Sub

Private Sub mnuRedo_Click()
    WebKitX1.ExecCommand EDIT_CMD_REDO
End Sub

Private Sub mnuRemoveFormat_Click()
    WebKitX1.ExecCommand EDIT_CMD_REMOVEFORMAT
End Sub

Private Sub mnuRight_Click()
    WebKitX1.ExecCommand EDIT_CMD_JUSTIFYRIGHT
End Sub

Private Sub mnuSelectAll_Click()
    WebKitX1.ExecCommand EDIT_CMD_SELECTALL
End Sub

Private Sub mnuSelectInner_Click()
    WebKitX1.SelectElement "#div1", False, True
End Sub

Private Sub mnuSelectOuter_Click()
    WebKitX1.SelectElement "#div1", False, False
End Sub

Private Sub mnuStrikeThrough_Click()
    WebKitX1.ExecCommand EDIT_CMD_STRIKETHROUGH
End Sub

Private Sub mnuStyleWithCSS_Click()
    mnuStyleWithCSS.Checked = Not mnuStyleWithCSS.Checked
    WebKitX1.ExecCommand EDIT_CMD_STYLEWITHCSS, LCase$(mnuStyleWithCSS.Checked)
End Sub

Private Sub mnuSubscript_Click()
    WebKitX1.ExecCommand EDIT_CMD_SUBSCRIPT
End Sub

Private Sub mnuSuperscript_Click()
    WebKitX1.ExecCommand EDIT_CMD_SUPERSCRIPT
End Sub

Private Sub mnuUnderline_Click()
    WebKitX1.ExecCommand EDIT_CMD_UNDERLINE
End Sub

Private Sub mnuUndo_Click()
    WebKitX1.ExecCommand EDIT_CMD_UNDO
End Sub

Private Sub mnuUnlink_Click()
    WebKitX1.ExecCommand EDIT_CMD_UNLINK
End Sub

Private Sub WebKitX1_OnCreate()
    WebKitX1.HTML = S_HTML
End Sub

Private Sub WebKitX1_OnFocus(ByVal NodePath As String)
    Debug.Print NodePath
End Sub

Private Sub WebKitX1_OnModified()
    Debug.Print "Modified"
End Sub

Private Sub WebKitX1_OnReady()
    WebKitX1.addEventListener "btn1", "click", AddressOf Module1.OnClick, True
    WebKitX1.addEventListenerEx "btn1", "click", Me, "OnClick", True
    CodeMax1.Text = WebKitX1.HTML
End Sub

Public Function OnClick() As Long
    MsgBox "Clicked - handled by class"
End Function

Private Sub WebKitX1_OnSelectionChanged(ByVal DocumentHTML As String, ByVal SelectedHTML As String)
    CodeMax1.Text = WebKitX1.TidyHTML(DocumentHTML)
    Debug.Print "Selection Changed: " + SelectedHTML
End Sub

