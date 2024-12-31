VERSION 5.00
Begin VB.Form Form1 
   Caption         =   "FV Client Test"
   ClientHeight    =   4110
   ClientLeft      =   1650
   ClientTop       =   2055
   ClientWidth     =   9105
   LinkTopic       =   "Form1"
   ScaleHeight     =   4110
   ScaleWidth      =   9105
   Begin VB.ComboBox language_list 
      Height          =   315
      ItemData        =   "Form1.frx":0000
      Left            =   120
      List            =   "Form1.frx":000A
      TabIndex        =   24
      Text            =   "English"
      Top             =   1920
      Width           =   2055
   End
   Begin VB.ComboBox file_type 
      Height          =   315
      ItemData        =   "Form1.frx":0022
      Left            =   3600
      List            =   "Form1.frx":002C
      TabIndex        =   21
      Text            =   "wav"
      Top             =   1920
      Width           =   1575
   End
   Begin VB.TextBox writeSpeed_text 
      Height          =   285
      Left            =   2880
      TabIndex        =   19
      Text            =   "100"
      Top             =   3360
      Width           =   975
   End
   Begin VB.Frame Voice_Prop 
      Caption         =   "Voice Properties"
      Height          =   3615
      Left            =   5520
      TabIndex        =   10
      Top             =   240
      Width           =   3375
      Begin VB.ComboBox Voices 
         Height          =   315
         ItemData        =   "Form1.frx":003A
         Left            =   240
         List            =   "Form1.frx":004A
         TabIndex        =   14
         Text            =   "Julie"
         Top             =   3000
         Width           =   1455
      End
      Begin VB.HScrollBar Pitch 
         Height          =   255
         Left            =   240
         Max             =   200
         Min             =   20
         TabIndex        =   13
         Top             =   2160
         Value           =   100
         Width           =   2895
      End
      Begin VB.HScrollBar Speed 
         Height          =   255
         Left            =   240
         Max             =   200
         Min             =   20
         TabIndex        =   12
         Top             =   1440
         Value           =   100
         Width           =   2895
      End
      Begin VB.HScrollBar Volume 
         Height          =   255
         Left            =   240
         Max             =   200
         Min             =   20
         TabIndex        =   11
         Top             =   600
         Value           =   100
         Width           =   2895
      End
      Begin VB.Label Label11 
         Caption         =   "Speaker's name"
         Height          =   255
         Left            =   240
         TabIndex        =   18
         Top             =   2760
         Width           =   1335
      End
      Begin VB.Label Label10 
         Caption         =   "Pitch"
         Height          =   255
         Left            =   240
         TabIndex        =   17
         Top             =   1920
         Width           =   975
      End
      Begin VB.Label Label2 
         Caption         =   "Speed"
         Height          =   255
         Left            =   240
         TabIndex        =   16
         Top             =   1200
         Width           =   975
      End
      Begin VB.Label Label1 
         Caption         =   "Volume"
         Height          =   255
         Left            =   240
         TabIndex        =   15
         Top             =   360
         Width           =   735
      End
   End
   Begin VB.CommandButton ButtonStart 
      Caption         =   "Start"
      Height          =   495
      Left            =   4200
      TabIndex        =   9
      Top             =   3120
      Width           =   975
   End
   Begin VB.Frame Connection 
      Caption         =   "Server connection"
      Height          =   975
      Left            =   120
      TabIndex        =   4
      Top             =   2880
      Width           =   2535
      Begin VB.TextBox host_name 
         Height          =   285
         Left            =   120
         TabIndex        =   6
         Text            =   "localhost"
         Top             =   480
         Width           =   1335
      End
      Begin VB.TextBox port_text 
         Height          =   285
         Left            =   1800
         MaxLength       =   5
         TabIndex        =   5
         Text            =   "5000"
         Top             =   480
         Width           =   615
      End
      Begin VB.Label Label6 
         Caption         =   "Host address"
         Height          =   255
         Left            =   120
         TabIndex        =   8
         Top             =   240
         Width           =   1215
      End
      Begin VB.Label Label9 
         Caption         =   "Port #"
         Height          =   255
         Left            =   1800
         TabIndex        =   7
         Top             =   240
         Width           =   615
      End
   End
   Begin VB.TextBox TextToSpeech 
      Height          =   285
      Left            =   120
      TabIndex        =   1
      Text            =   "Hello world!"
      Top             =   1080
      Width           =   5055
   End
   Begin VB.TextBox FileName 
      Height          =   285
      Left            =   120
      TabIndex        =   0
      Text            =   "Test.wav"
      Top             =   360
      Width           =   5055
   End
   Begin VB.Label Label5 
      Caption         =   "Language"
      Height          =   255
      Left            =   120
      TabIndex        =   23
      Top             =   1560
      Width           =   1215
   End
   Begin VB.Label Label8 
      Caption         =   "File format"
      Height          =   255
      Index           =   2
      Left            =   3600
      TabIndex        =   22
      Top             =   1560
      Width           =   1575
   End
   Begin VB.Label writeSpeed 
      Caption         =   "Write speed"
      Height          =   255
      Left            =   2880
      TabIndex        =   20
      Top             =   3120
      Width           =   975
   End
   Begin VB.Label Label3 
      Caption         =   "Text to speek:"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   840
      Width           =   1215
   End
   Begin VB.Label label4 
      Caption         =   "File name:"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   1095
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim old_language As String

Public Enum FVClientFileFormat
    'Invalid file format
    FVClientFileFormat_Invalid
    'Raw format (without header)
    FVClientFileFormat_RAW
    'Windows's wav format
    FVClientFileFormat_WAV
End Enum

Public Enum FVClientLanguage
    FVClient_English
    FVClient_Hungarian
End Enum

Public Enum FVClientErrorCode

    'The request was successfully done.
    FVClient_Error_OK
        
    'General failure.
    FVClient_Error_Fail
    
    'Out of memory.
    FVClient_Error_OutOfMemory
    
    'Invalid handle.
    FVClient_Error_InvalidHandle
    
    'Error occured during server connection.
    FVClient_Error_CannotConnect
    
    'Client is not connected to the server. (see \Ref{FVNetClientVBFile_Connect}).
    FVClient_Error_NotConnected
    
    'The speak request failed.
    FVClient_Error_CannotSpeak
    
    'All channels of the server are busy.
    FVClient_Error_ServerBusy
    
    'The client is speaking and can't recieve more request.
    FVClient_Error_ClientBusy
    
    'Invalid argument(s).
    '    One or more arguments passed to the method are invalid.
    FVClient_Error_InvalidArgument
    
    'Invalid file format.
    'The given fileformat is invalid or not supported.
    FVNetClientVBFile_Error_InvalidFileFormat
    
    'Not implemented.
    '    The given method is not yet implemented.
    FVClient_Error_NotImplemented
    
End Enum

Private Declare Function FVClientCreate _
    Lib "FVNetClient_VBFile" Alias "_FVNetClientVBFile_Create@0" () As Long

Private Declare Function FVClientConnect _
    Lib "FVNetClient_VBFile" Alias "_FVNetClientVBFile_Connect@12" ( _
        ByVal handle As Long, _
        ByVal host As String, _
        ByVal port As Long _
    ) As Long

Private Declare Function FVClientSpeakToFile _
    Lib "FVNetClient_VBFile" Alias "_FVNetClientVBFile_SpeakToFile@40" ( _
        ByVal handle As Long, _
        ByVal text As String, _
        ByVal language As FVClientLanguage, _
        ByVal FileName As String, _
        ByVal Format As FVClientFileFormat, _
        ByVal speaker As String, _
        ByVal relVolume As Long, _
        ByVal relSpeed As Long, _
        ByVal relPitch As Long, _
        ByVal writeSpeed As Long _
    ) As Long

Private Declare Function FVClientSpeakToFileRequest _
    Lib "FVNetClient_VBFile" Alias "_FVNetClientVBFile_SpeakToFileRequest@40" ( _
        ByVal handle As Long, _
        ByVal text As String, _
        ByVal language As FVClientLanguage, _
        ByVal FileName As String, _
        ByVal Format As FVClientFileFormat, _
        ByVal speaker As String, _
        ByVal relVolume As Long, _
        ByVal relSpeed As Long, _
        ByVal relPitch As Long, _
        ByVal writeSpeed As Long _
    ) As Long
    
Private Declare Function FVClientVBFileWait _
    Lib "FVNetClient_VBFile" Alias "_FVNetClientVBFile_Wait@4" ( _
        ByVal handle As Long _
    ) As Long
    
Private Declare Function FVClientDestroy _
    Lib "FVNetClient_VBFile" Alias "_FVNetClientVBFile_Destroy@4" ( _
        ByVal handle As Long _
    ) As Long
    
Private Declare Function FVClientAutoDestroy _
    Lib "FVNetClient_VBFile" Alias "_FVNetClientVBFile_AutoDestroy@4" ( _
        ByVal handle As Long _
    ) As Long


Public Sub Convert_error(error_code As Long)

Select Case error_code
    Case FVClient_Error_OK
            MsgBox "Client OK"
        
    Case FVClient_Error_Fail
            MsgBox "General failure."
    
    Case FVClient_Error_OutOfMemory
            MsgBox "Out of memory."
            
    Case FVClient_Error_InvalidHandle
            MsgBox "Invalid handle."
                
    Case FVClient_Error_CannotConnect
            MsgBox "Error occured during server connection."
    
    Case FVClient_Error_NotConnected
            MsgBox "Client is not connected to the server. (see \Ref{FVNetClientVBFile_Connect})."
    
    Case FVClient_Error_CannotSpeak
            MsgBox "The speak request failed."
    
    Case FVClient_Error_ServerBusy
            MsgBox "All channels of the server are busy."
    
    Case FVClient_Error_ClientBusy
            MsgBox "The client is speaking and can't recieve more request."
    
    Case FVClient_Error_InvalidArgument
            MsgBox "Invalid argument(s).One or more arguments passed to the method are invalid."
    
    Case FVClient_Error_NotImplemented
            MsgBox "The given method is not yet implemented."
    
    End Select

End Sub


Private Sub ButtonStart_Click()

    Dim localResult     As Long
    Dim fileFormat      As FVClientFileFormat
    Dim language        As FVClientLanguage
            
    Select Case file_type.text
        Case "raw"
            fileFormat = FVClientFileFormat_RAW
        Case "wav"
            fileFormat = FVClientFileFormat_WAV
    End Select
                        
    Select Case language_list.text
        Case "English"
            language = FVClient_English
        Case "Hungarian"
            language = FVClient_Hungarian
    End Select
    
    Dim port_number As Long
    Dim writeSpeed As Long
    
    port_number = Val(port_text.text)
    writeSpeed = Val(writeSpeed_text.text)
    
    If (1 > port_number) Or (port_number > 32767) Then MsgBox "Uncorrect port number"
        
    Dim clientHandle As Long
        
    clientHandle = FVClientCreate()
    If clientHandle = 0 Then MsgBox "Couldn't create client"
    
    localResult = FVClientConnect(clientHandle, Me.host_name, port_number)
    If localResult <> 0 Then Convert_error (localResult)
        
    localResult = FVClientSpeakToFileRequest(clientHandle _
                                            , Me.TextToSpeech _
                                            , language _
                                            , Me.FileName _
                                            , fileFormat _
                                            , Voices.text _
                                            , Volume.Value _
                                            , Speed.Value _
                                            , Pitch.Value _
                                            , writeSpeed _
                                            )
      FVClientAutoDestroy (clientHandle)
    If localResult <> 0 Then Convert_error (localResult)
    
    FVClientDestroy (clientHandle)
  
End Sub

Sub do_text()
Select Case language_list.text
        Case "English"
            TextToSpeech.text = "Hello World!"
             Voices.List(0) = "Julie"
             Voices.List(1) = "Tom"
             Voices.List(2) = "Ben"
             Voices.List(3) = "Leslie"
             Voices.text = "Julie"
             old_language = "English"
        Case "Hungarian"
            TextToSpeech.text = "Szia világ!"
            Voices.List(0) = "Zita"
            Voices.List(1) = "Peter"
            Voices.text = "Zita"
            If old_language = "English" Then
            Voices.RemoveItem (3)
            Voices.RemoveItem (2)
            End If
            old_language = "Hungarian"
         End Select
End Sub


Private Sub Form_Load()
old_language = "English"
End Sub

Private Sub language_list_Click()
do_text
End Sub
