VERSION 5.00
Object = "{7493F856-8B5B-47C2-8336-71D5CA8433D9}#1.0#0"; "WebKitX.ocx"
Begin VB.Form Form1 
   AutoRedraw      =   -1  'True
   Caption         =   "Form1"
   ClientHeight    =   5385
   ClientLeft      =   225
   ClientTop       =   855
   ClientWidth     =   7575
   LinkTopic       =   "Form1"
   ScaleHeight     =   359
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   505
   StartUpPosition =   3  'Windows Default
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
      Begin VB.Menu mnuEditable 
         Caption         =   "Editable"
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Const S_HTML = "<html><body><button id='btn1'>Hello CEF</button><br><div id='div1'>This is <span>some</span> text</div><br><div id='div2'>This is <span>some</span> text</div><br></body></html>"

Private Sub Form_Resize()
    On Error Resume Next
    WebKitX1.Move 0, 0, ScaleWidth, ScaleHeight
    Err.Clear
End Sub

Private Sub mnuEditable_Click()
    mnuEditable.Checked = Not mnuEditable.Checked
    If mnuEditable.Checked Then
        WebKitX1.Edit
    Else
        WebKitX1.Preview
    End If
End Sub

Private Sub mnuGet_Click()
    Debug.Print WebKitX1.HTML
End Sub

Private Sub mnuOpen_Click()
    WebKitX1.OpenURL "http://www.mobilefx.com"
End Sub

Private Sub mnuPut_Click()
    WebKitX1.HTML = S_HTML
End Sub

Private Sub WebKitX1_OnCreate()
    WebKitX1.HTML = S_HTML
End Sub

Private Sub WebKitX1_OnFocus(ByVal NodePath As String)
    Debug.Print NodePath
End Sub

Private Sub WebKitX1_OnReady()
    WebKitX1.addEventListener "btn1", "click", AddressOf Module1.OnClick, True
    WebKitX1.addEventListenerEx "btn1", "click", Me, "OnClick", True
End Sub

Public Function OnClick() As Long
    MsgBox "Clicked - handled by class"
End Function

