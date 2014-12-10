VERSION 5.00
Object = "{7493F856-8B5B-47C2-8336-71D5CA8433D9}#1.0#0"; "WebKitX.ocx"
Begin VB.Form Form1 
   AutoRedraw      =   -1  'True
   Caption         =   "Form1"
   ClientHeight    =   8760
   ClientLeft      =   225
   ClientTop       =   855
   ClientWidth     =   10305
   LinkTopic       =   "Form1"
   ScaleHeight     =   584
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   687
   StartUpPosition =   3  'Windows Default
   Begin WebKitXLib.WebKitX WebKitX1 
      Height          =   3870
      Left            =   555
      TabIndex        =   0
      Top             =   390
      Width           =   5010
      _Version        =   65536
      _ExtentX        =   8837
      _ExtentY        =   6826
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

Private Sub Form_Load()
    On Error Resume Next
    
    Err.Clear
End Sub

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
    WebKitX1.HTML = "<html><body>Hello CEF</body></html>"
End Sub
