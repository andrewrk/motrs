VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmMain 
   Caption         =   "MOTRS Map Editor"
   ClientHeight    =   8010
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   10125
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   ScaleHeight     =   534
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   675
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picTilePreview 
      AutoRedraw      =   -1  'True
      BorderStyle     =   0  'None
      Height          =   495
      Left            =   960
      ScaleHeight     =   33
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   33
      TabIndex        =   25
      TabStop         =   0   'False
      Top             =   4440
      Width           =   495
   End
   Begin VB.PictureBox picMaskInput 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1560
      ScaleHeight     =   17
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   17
      TabIndex        =   24
      Top             =   4920
      Visible         =   0   'False
      Width           =   255
   End
   Begin VB.PictureBox picMaskOr 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      BackColor       =   &H00000000&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1920
      ScaleHeight     =   17
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   17
      TabIndex        =   23
      Top             =   4560
      Visible         =   0   'False
      Width           =   255
   End
   Begin VB.PictureBox picMaskAnd 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      BackColor       =   &H00FFFFFF&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1560
      ScaleHeight     =   17
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   17
      TabIndex        =   22
      Top             =   4560
      Visible         =   0   'False
      Width           =   255
   End
   Begin MSComDlg.CommonDialog cdl 
      Left            =   1920
      Top             =   5160
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.PictureBox picWork 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      ScaleHeight     =   17
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   17
      TabIndex        =   21
      Top             =   4560
      Visible         =   0   'False
      Width           =   255
   End
   Begin VB.CommandButton cmdChooseTiles 
      Caption         =   "Choose"
      Height          =   255
      Left            =   1680
      TabIndex        =   1
      TabStop         =   0   'False
      Top             =   480
      Width           =   735
   End
   Begin VB.CommandButton cmdDeleteLayer 
      Caption         =   "Delete"
      Height          =   255
      Left            =   0
      TabIndex        =   4
      TabStop         =   0   'False
      Top             =   5760
      Width           =   615
   End
   Begin VB.CommandButton cmdAddLayer 
      Caption         =   "Add"
      Height          =   255
      Left            =   1800
      TabIndex        =   6
      TabStop         =   0   'False
      Top             =   5760
      Width           =   615
   End
   Begin VB.CommandButton cmdMapIncRight 
      Caption         =   "+"
      Height          =   255
      Left            =   9600
      TabIndex        =   17
      TabStop         =   0   'False
      Top             =   7320
      Width           =   255
   End
   Begin VB.CommandButton cmdMapDecLeft 
      Caption         =   "-"
      Height          =   255
      Left            =   2760
      TabIndex        =   20
      TabStop         =   0   'False
      Top             =   7320
      Width           =   255
   End
   Begin VB.CommandButton cmdMapIncLeft 
      Caption         =   "+"
      Height          =   255
      Left            =   2520
      TabIndex        =   19
      TabStop         =   0   'False
      Top             =   7320
      Width           =   255
   End
   Begin VB.CommandButton cmdMapDecRight 
      Caption         =   "-"
      Height          =   255
      Left            =   9360
      TabIndex        =   18
      TabStop         =   0   'False
      Top             =   7320
      Width           =   255
   End
   Begin VB.CommandButton cmdMapIncBottom 
      Caption         =   "+"
      Height          =   255
      Left            =   9840
      TabIndex        =   15
      TabStop         =   0   'False
      Top             =   7080
      Width           =   255
   End
   Begin VB.CommandButton cmdMapDecBottom 
      Caption         =   "-"
      Height          =   255
      Left            =   9840
      TabIndex        =   16
      TabStop         =   0   'False
      Top             =   6840
      Width           =   255
   End
   Begin VB.CommandButton cmdMapDecTop 
      Caption         =   "-"
      Height          =   255
      Left            =   9840
      TabIndex        =   14
      TabStop         =   0   'False
      Top             =   720
      Width           =   255
   End
   Begin VB.CommandButton cmdMapIncTop 
      Caption         =   "+"
      Height          =   255
      Left            =   9840
      TabIndex        =   13
      TabStop         =   0   'False
      Top             =   480
      Width           =   255
   End
   Begin VB.Timer tmrClearErr 
      Enabled         =   0   'False
      Interval        =   4000
      Left            =   9120
      Top             =   7560
   End
   Begin VB.ListBox lstTiles 
      Height          =   3180
      IntegralHeight  =   0   'False
      Left            =   0
      TabIndex        =   2
      Top             =   720
      Width           =   2415
   End
   Begin VB.ListBox lstLayers 
      Height          =   1860
      IntegralHeight  =   0   'False
      ItemData        =   "frmMain.frx":0000
      Left            =   0
      List            =   "frmMain.frx":0002
      Style           =   1  'Checkbox
      TabIndex        =   7
      Top             =   6000
      Width           =   2415
   End
   Begin VB.HScrollBar hsbMap 
      Height          =   255
      Left            =   5160
      TabIndex        =   11
      TabStop         =   0   'False
      Top             =   7320
      Width           =   1575
   End
   Begin VB.VScrollBar vsbMap 
      Height          =   1815
      Left            =   9840
      TabIndex        =   10
      TabStop         =   0   'False
      Top             =   1320
      Width           =   255
   End
   Begin VB.PictureBox picMap 
      BackColor       =   &H00FFFFFF&
      Height          =   6855
      Left            =   2520
      ScaleHeight     =   453
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   485
      TabIndex        =   8
      Top             =   480
      Width           =   7335
   End
   Begin MSComctlLib.ImageList imlToolbar 
      Left            =   240
      Top             =   5040
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   23
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0004
            Key             =   "quit"
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":03FA
            Key             =   "play"
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0761
            Key             =   "arrow"
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0ADD
            Key             =   "new"
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":0EB1
            Key             =   ""
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":1277
            Key             =   ""
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":162D
            Key             =   "save"
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":1A17
            Key             =   "saveas"
         EndProperty
         BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":1E00
            Key             =   "copy"
         EndProperty
         BeginProperty ListImage10 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":21C9
            Key             =   "cut"
         EndProperty
         BeginProperty ListImage11 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":259E
            Key             =   "delete"
         EndProperty
         BeginProperty ListImage12 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":2974
            Key             =   "paste"
         EndProperty
         BeginProperty ListImage13 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":2D65
            Key             =   "redo"
         EndProperty
         BeginProperty ListImage14 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":3142
            Key             =   ""
         EndProperty
         BeginProperty ListImage15 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":350F
            Key             =   "undo"
         EndProperty
         BeginProperty ListImage16 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":38EC
            Key             =   "eraser"
         EndProperty
         BeginProperty ListImage17 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":3C6E
            Key             =   ""
         EndProperty
         BeginProperty ListImage18 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":4052
            Key             =   "pan"
         EndProperty
         BeginProperty ListImage19 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":43DC
            Key             =   "pencil"
         EndProperty
         BeginProperty ListImage20 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":4755
            Key             =   "options"
         EndProperty
         BeginProperty ListImage21 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":4B12
            Key             =   "zoomin"
         EndProperty
         BeginProperty ListImage22 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":4EFA
            Key             =   "zoom100"
         EndProperty
         BeginProperty ListImage23 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMain.frx":52E1
            Key             =   "zoomout"
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.Toolbar tbr 
      Align           =   1  'Align Top
      Height          =   390
      Left            =   0
      TabIndex        =   9
      Top             =   0
      Width           =   10125
      _ExtentX        =   17859
      _ExtentY        =   688
      ButtonWidth     =   609
      ButtonHeight    =   582
      AllowCustomize  =   0   'False
      ImageList       =   "imlToolbar"
      _Version        =   393216
      BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
         NumButtons      =   10
         BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "arrow"
            Object.ToolTipText     =   "Select the arrow tool"
            ImageKey        =   "arrow"
            Style           =   1
            Value           =   1
         EndProperty
         BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "pencil"
            Object.ToolTipText     =   "select the pencil tool"
            ImageKey        =   "pencil"
            Style           =   1
         EndProperty
         BeginProperty Button3 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "eraser"
            Object.ToolTipText     =   "Select the eraser tool"
            ImageKey        =   "eraser"
            Style           =   1
         EndProperty
         BeginProperty Button4 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "pan"
            Object.ToolTipText     =   "Select the pan tool"
            ImageKey        =   "pan"
            Style           =   1
         EndProperty
         BeginProperty Button5 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button6 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "zoomin"
            Object.ToolTipText     =   "Zoom In"
            ImageKey        =   "zoomin"
         EndProperty
         BeginProperty Button7 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "zoomout"
            Object.ToolTipText     =   "Zoom Out"
            ImageKey        =   "zoomout"
         EndProperty
         BeginProperty Button8 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "zoom100"
            Object.ToolTipText     =   "Zoom normal"
            ImageKey        =   "zoom100"
         EndProperty
         BeginProperty Button9 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button10 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Key             =   "playtest"
            Object.ToolTipText     =   "Playtest your map"
            ImageKey        =   "play"
         EndProperty
      EndProperty
   End
   Begin VB.Image imgInvalidBitmap 
      Height          =   480
      Left            =   120
      Picture         =   "frmMain.frx":56C8
      Top             =   4560
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgInvisibleTile 
      Height          =   480
      Left            =   120
      Picture         =   "frmMain.frx":630A
      Top             =   4560
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgPlayerIconMask 
      Height          =   720
      Left            =   120
      Picture         =   "frmMain.frx":6F4C
      Stretch         =   -1  'True
      Top             =   4560
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgTileMask 
      Height          =   375
      Index           =   0
      Left            =   120
      Top             =   4800
      Visible         =   0   'False
      Width           =   375
   End
   Begin VB.Image imgTile 
      Height          =   375
      Index           =   0
      Left            =   120
      Top             =   4800
      Visible         =   0   'False
      Width           =   375
   End
   Begin VB.Label lblErr 
      ForeColor       =   &H000000FF&
      Height          =   195
      Left            =   2520
      TabIndex        =   12
      Top             =   7680
      Visible         =   0   'False
      Width           =   7320
   End
   Begin VB.Label lblLayers 
      Alignment       =   2  'Center
      Caption         =   "Layers:"
      Height          =   195
      Left            =   0
      TabIndex        =   5
      Top             =   5760
      Width           =   2415
   End
   Begin VB.Label lblPreview 
      Alignment       =   2  'Center
      Caption         =   "Preview:"
      Height          =   195
      Left            =   0
      TabIndex        =   3
      Top             =   4200
      Width           =   2415
   End
   Begin VB.Label lblTiles 
      Alignment       =   2  'Center
      Caption         =   "Tiles"
      Height          =   195
      Left            =   0
      TabIndex        =   0
      Top             =   480
      Width           =   2415
   End
   Begin VB.Image imgPlayerIcon 
      Height          =   720
      Left            =   120
      Picture         =   "frmMain.frx":7696
      Stretch         =   -1  'True
      Top             =   4560
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Menu mnu 
      Caption         =   "&File"
      Index           =   0
      Begin VB.Menu mnuFile 
         Caption         =   "&New"
         Index           =   0
         Shortcut        =   ^N
      End
      Begin VB.Menu mnuFile 
         Caption         =   "&Open..."
         Index           =   1
         Shortcut        =   ^O
      End
      Begin VB.Menu mnuFile 
         Caption         =   "&Save"
         Index           =   2
         Shortcut        =   ^S
      End
      Begin VB.Menu mnuFile 
         Caption         =   "Save &As..."
         Index           =   3
      End
      Begin VB.Menu mnuFile 
         Caption         =   "-"
         Index           =   4
      End
      Begin VB.Menu mnuFile 
         Caption         =   "&Map Properties..."
         Index           =   5
      End
      Begin VB.Menu mnuFile 
         Caption         =   "&Playtest Map"
         Index           =   6
         Shortcut        =   {F5}
      End
      Begin VB.Menu mnuFile 
         Caption         =   "-"
         Index           =   7
      End
      Begin VB.Menu mnuFile 
         Caption         =   "E&xit"
         Index           =   8
      End
   End
   Begin VB.Menu mnu 
      Caption         =   "&Edit"
      Index           =   1
      Begin VB.Menu mnuEdit 
         Caption         =   "&Undo"
         Index           =   0
         Shortcut        =   ^Z
      End
      Begin VB.Menu mnuEdit 
         Caption         =   "&Redo"
         Index           =   1
         Shortcut        =   ^Y
      End
      Begin VB.Menu mnuEdit 
         Caption         =   "-"
         Index           =   2
      End
      Begin VB.Menu mnuEdit 
         Caption         =   "&Cut"
         Index           =   3
         Shortcut        =   ^X
      End
      Begin VB.Menu mnuEdit 
         Caption         =   "C&opy"
         Index           =   4
         Shortcut        =   ^C
      End
      Begin VB.Menu mnuEdit 
         Caption         =   "&Paste"
         Index           =   5
         Shortcut        =   ^V
      End
      Begin VB.Menu mnuEdit 
         Caption         =   "&Delete"
         Index           =   6
         Shortcut        =   {DEL}
      End
      Begin VB.Menu mnuEdit 
         Caption         =   "-"
         Index           =   7
      End
      Begin VB.Menu mnuEdit 
         Caption         =   "&Select All"
         Index           =   8
         Shortcut        =   ^A
      End
   End
   Begin VB.Menu mnu 
      Caption         =   "&Tools"
      Index           =   2
      Begin VB.Menu mnuTools 
         Caption         =   "&Arrow"
         Index           =   0
      End
      Begin VB.Menu mnuTools 
         Caption         =   "&Pencil"
         Index           =   1
      End
      Begin VB.Menu mnuTools 
         Caption         =   "&Eraser"
         Index           =   2
      End
      Begin VB.Menu mnuTools 
         Caption         =   "&Pan"
         Index           =   3
      End
      Begin VB.Menu mnuTools 
         Caption         =   "-"
         Index           =   4
      End
      Begin VB.Menu mnuTools 
         Caption         =   "&Tile Editor..."
         Index           =   5
      End
      Begin VB.Menu mnuTools 
         Caption         =   "&Bitmap Splicer..."
         Index           =   6
      End
      Begin VB.Menu mnuTools 
         Caption         =   "Remove &Unused Tiles"
         Index           =   7
      End
   End
   Begin VB.Menu mnu 
      Caption         =   "&View"
      Index           =   3
      Begin VB.Menu mnuView 
         Caption         =   "Zoom &In"
         Index           =   0
      End
      Begin VB.Menu mnuView 
         Caption         =   "Zoom &Out"
         Index           =   1
      End
      Begin VB.Menu mnuView 
         Caption         =   "Zoom &100%"
         Index           =   2
      End
      Begin VB.Menu mnuView 
         Caption         =   "-"
         Index           =   3
      End
      Begin VB.Menu mnuView 
         Caption         =   "&Grid"
         Checked         =   -1  'True
         Index           =   4
      End
   End
   Begin VB.Menu mnuPopLayerOptions 
      Caption         =   "LayerOptions"
      Visible         =   0   'False
      Begin VB.Menu mnuLayerOptions 
         Caption         =   "&Set as player layer"
         Index           =   0
      End
      Begin VB.Menu mnuLayerOptions 
         Caption         =   "-"
         Index           =   1
      End
      Begin VB.Menu mnuLayerOptions 
         Caption         =   "Move &Up"
         Index           =   2
      End
      Begin VB.Menu mnuLayerOptions 
         Caption         =   "Move &Down"
         Index           =   3
      End
   End
   Begin VB.Menu mnuPopSquContext 
      Caption         =   "SquareContextMenu"
      Visible         =   0   'False
      Begin VB.Menu mnuSquContext 
         Caption         =   "&Set playtest start here"
         Index           =   0
      End
   End
   Begin VB.Menu mnuPopPlayerContext 
      Caption         =   "PlayerContextMenu"
      Visible         =   0   'False
      Begin VB.Menu mnuPlayerContext 
         Caption         =   "Switch test &Character..."
         Index           =   0
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Enum MenuFile
    FileNew = 0
    FileOpen = 1
    FileSave = 2
    FileSaveAs = 3
    
    FileMapProperties = 5
    FilePlaytestMap = 6
    
    FileExit = 8
End Enum

Private Enum MenuEdit
    EditUndo = 0
    EditRedo = 1
    
    EditCut = 3
    EditCopy = 4
    EditPaste = 5
    EditDelete = 6
    
    EditSelectAll = 8
End Enum

Private Enum MenuTools
    ToolsArrow = 0
    ToolsPencil = 1
    ToolsEraser = 2
    ToolsPan = 3
    
    ToolsTileEditor = 5
    ToolsBitmapSplicer = 6
    ToolsRemoveUnusedTiles = 7
End Enum

Private Enum MenuView
    ViewZoomIn = 0
    ViewZoomOut = 1
    ViewZoom100 = 2
    
    ViewGrid = 4
End Enum

Private Enum MenuLayerOptions
    LayerOptionsSetPlayerLayer = 0
    
    LayerOptionsMoveUp = 2
    LayerOptionsMoveDown = 3
End Enum

Private Enum MenuSquContext
    SquContextSetPlaytestStart = 0
End Enum

Private Enum MenuPlayerContext
    PlayerContextSwitchCharacter = 0
End Enum

Private Enum ClickModeConstants
    cm_Arrow = 0
    cm_Pencil
    cm_Eraser
    cm_Pan
End Enum



'map data

Private NumLayers As Integer

Private MapWidth As Integer
Private MapHeight As Integer
Private Map() As Integer

Private NumTiles As Integer
Private Tiles() As String
Private TileObj() As Tile

Private PlayerLayer As Integer

'editor data
Private ClickMode As ClickModeConstants
Private MouseDownX As Single, MouseDownY As Single
Private MouseDownOffsetX As Integer, MouseDownOffsetY As Integer
Private MouseDownMoveSel As Boolean, MouseDownMovePlayerSt As Boolean
Private DownSelLeft As Integer, DownSelTop As Integer
Private DownPStX As Integer, DownPStY As Integer
Private OldClickMode As Integer
Private LastXIndex As Integer, LastYIndex As Integer

Private PlayerStartX As Integer, PlayerStartY As Integer
Private PlaytestChar As String

Private Zoom As Single
Private OffsetX As Integer, OffsetY As Integer
Private Grid As Boolean

Private FileTitle As String, FilePath As String
Private Const FormCaption As String = "MOTRS Map Editor"
Private Altered As Boolean

Private CodeGenerated As Boolean

Private IsSel As Boolean
Private SelLeft As Integer, SelTop As Integer
Private SelWidth As Integer, SelHeight As Integer
Private Selection() As Integer
Private DraggingSel As Boolean

'display data

Private Const MinFormHeight As Integer = 300
Private Const MinFormWidth As Integer = 260

Private Const SelDrawWidth = 4

Private Const MinZoom = 0.1


Private Sub cmdAddLayer_Click()
    ResizeMap MapWidth, MapHeight, NumLayers + 1
    
    lstLayers.ListIndex = lstLayers.ListCount - 1
End Sub

Public Sub UpdateTilesList()
    Dim I As Integer
    Dim X As Integer, Y As Integer, J As Integer
    Dim AlphaColor As Long, PtColor As Long
    Dim HadToRemove As Boolean
    
StartOver:
    'load image controls for each tile
    lblErr.ForeColor = vbBlue
    lblErr.Caption = "Loading tile images..."
    lblErr.Visible = True
    Do While imgTile.Count > NumTiles And imgTile.Count > 1
        Unload imgTile(imgTile.UBound)
        Unload imgTileMask(imgTileMask.UBound)
    Loop
    Do While imgTile.Count < NumTiles
        Load imgTile(imgTile.Count)
        Load imgTileMask(imgTileMask.Count)
    Loop
    
    picMaskInput.Move 0, 0, TileWidth, TileHeight
    picMaskInput.Cls
    
    If NumTiles > 0 Then ReDim TileObj(NumTiles - 1)
    lstTiles.Clear
    For I = 0 To NumTiles - 1
        If NumTiles > 1 Then lblErr.Caption = "Creating tile masks... " & (I / (NumTiles - 1) * 100) & "%"
        'lblErr.Refresh
        
        'update list box
        lstTiles.AddItem Tiles(I)
        
        If Not FileExists(App.Path & "\" & TilesFolder & "\" & Tiles(I) & ".tile.txt") Then GoTo RemoveTile
        'cache tile image
        On Error GoTo TileLoadErr
        Set TileObj(I) = New Tile
        TileObj(I).FromFile App.Path & "\" & TilesFolder & "\" & Tiles(I) & ".tile.txt"
        
'        On Error GoTo PicLoadErr
'        picMaskInput.Picture = LoadPicture(App.Path & "\" & BitmapsFolder & "\" & TileObj(I).GetProperty("bitmap"))
        
        On Error GoTo 0
        
        ComputeTileBitmap TileObj(I)
        imgTileMask(I).Picture = picMaskAnd.Image
        imgTile(I).Picture = picMaskOr.Image
        'create tile masks
'        If TileObj(I).GetProperty("alpha") = "none" Then
'            picMaskOr.Cls
'            imgTileMask(I).Picture = picMaskOr.Image 'pure white
'            imgTile(I).Picture = picMaskInput.Picture
'        Else
'            AlphaColor = Val(ColorFromString(TileObj(I).GetProperty("alpha")))
'            CreateMask picMaskInput, picMaskOr, picMaskAnd, AlphaColor
'
'            imgTile(I).Picture = picMaskOr.Image
'            imgTileMask(I).Picture = picMaskAnd.Image
'        End If
        
        
    Next I
    
    lblErr.Visible = False
    lblErr.ForeColor = vbRed
    
    If HadToRemove Then ProcessError "One or more bitmaps were missing."
    
    Exit Sub
PicLoadErr:
    ProcessError "Error loading bitmap specified in tile - " & Err.Description
    GoTo RemoveTile
TileLoadErr:
    ProcessError "Error loading tile from disk - " & Err.Description
    GoTo RemoveTile
RemoveTile:
    HadToRemove = True
    For X = 0 To MapWidth - 1
        For Y = 0 To MapHeight - 1
            For J = 0 To NumLayers - 1
                If Map(X, Y, J) = I Then
                    Map(X, Y, J) = -1
                ElseIf Map(X, Y, J) > I Then
                    Map(X, Y, J) = Map(X, Y, J) - 1
                End If
            Next J
        Next Y
    Next X
    For I = I To NumTiles - 2
        Tiles(I) = Tiles(I + 1)
    Next I
    NumTiles = NumTiles - 1
    ReDim Preserve Tiles(NumTiles - 1)
    ReDim Preserve TileObj(NumTiles - 1)
    On Error GoTo 0
    GoTo StartOver
End Sub

Private Sub ProcessStatus(Msg As String)
    ProcessError Msg
End Sub

Private Sub UpdateMapWithNewTiles(OldTiles() As String, OldNumTiles As Integer)
    Dim I As Integer, J As Integer
    Dim Trans() As Integer
    
    'generate mapping array
    ReDim Trans(OldNumTiles - 1)
    For I = 0 To OldNumTiles - 1
        Trans(I) = -1
        For J = 0 To NumTiles - 1
            If OldTiles(I) = Tiles(J) Then
                Trans(I) = J
                Exit For
            End If
        Next J
    Next I
    
    UpdateMapWithNewTilesTrans Trans
End Sub

Private Sub UpdateMapWithNewTilesTrans(Trans() As Integer)
    Dim X As Integer, Y As Integer, I As Integer
    
    'perform the swaps
    For X = 0 To MapWidth - 1
        For Y = 0 To MapHeight - 1
            For I = 0 To NumLayers - 1
                If Map(X, Y, I) <> -1 Then Map(X, Y, I) = Trans(Map(X, Y, I))
            Next I
        Next Y
    Next X
    
End Sub

Private Sub cmdChooseTiles_Click()
    Dim I As Integer
    
    Dim TileStrings() As String, OldNumTiles As Integer
    
    frmChooseTiles.RefreshTilesBox
    
    'populate the map list box with this forms tiles
    frmChooseTiles.lstMap.Clear
    For I = 0 To NumTiles - 1
        frmChooseTiles.lstMap.AddItem Tiles(I)
    Next I

    frmChooseTiles.Show vbModal
    
    If Not frmChooseTiles.Cancelled Then
        OldNumTiles = NumTiles
        If OldNumTiles > 0 Then ReDim TileStrings(OldNumTiles - 1)
        TileStrings = Tiles
        
        NumTiles = frmChooseTiles.lstMap.ListCount
        ReDim Tiles(NumTiles - 1)
        
        For I = 0 To NumTiles - 1
            Tiles(I) = frmChooseTiles.lstMap.List(I)
        Next I
        
        If OldNumTiles > 0 Then UpdateMapWithNewTiles TileStrings, OldNumTiles
        
        UpdateTilesList
        
        SetAltered True
    End If
End Sub

Public Sub AddTile(TileName As String)
    If TileInMap(TileName) Then Exit Sub
    
    ReDim Preserve Tiles(NumTiles)
    Tiles(NumTiles) = TileName
    
    NumTiles = NumTiles + 1
    
End Sub

Private Function TileInMap(TileName As String) As Boolean
    Dim I As Integer
    
    For I = 0 To NumTiles - 1
        If Tiles(I) = TileName Then
            TileInMap = True
            Exit Function
        End If
    Next I
    
    TileInMap = False

End Function

Private Sub cmdDeleteLayer_Click()
    Dim I As Integer, X As Integer, Y As Integer
    
    If lstLayers.ListIndex = -1 Then
        ProcessError "Choose a layer first, then press delete."
    ElseIf NumLayers < 2 Then
        ProcessError "You have to have at least one layer."
    Else
        
        For Y = 0 To MapHeight - 1
            For X = 0 To MapWidth - 1
                For I = lstLayers.ListIndex To NumLayers - 2
                    Map(X, Y, I) = Map(X, Y, I + 1)
                Next I
            Next X
        Next Y
        
        ResizeMap MapWidth, MapHeight, NumLayers - 1
        SetAltered True
    End If
End Sub

Private Sub cmdMapDecBottom_Click()
    If MapHeight > 1 Then
        ResizeMap MapWidth, MapHeight - 1, NumLayers, 0, 0
        SetAltered True
    Else
        ProcessError "The map has to be at least 1 square high."
    End If
End Sub

Private Sub cmdMapDecLeft_Click()
    If MapWidth > 1 Then
        PlayerStartX = PlayerStartX - 1
        If PlayerStartX < 0 Then PlayerStartX = 0
        ResizeMap MapWidth - 1, MapHeight, NumLayers, -1, 0
        SetAltered True
    Else
        ProcessError "The map has to be at least 1 square wide."
    End If
End Sub

Private Sub cmdMapDecRight_Click()
    If MapWidth > 1 Then
        ResizeMap MapWidth - 1, MapHeight, NumLayers, 0, 0
        SetAltered True
    Else
        ProcessError "The map has to be at least 1 square wide."
    End If
    
End Sub

Private Sub cmdMapDecTop_Click()
    If MapHeight > 1 Then
        PlayerStartY = PlayerStartY - 1
        If PlayerStartY < 0 Then PlayerStartY = 0
        ResizeMap MapWidth, MapHeight - 1, NumLayers, 0, -1
        SetAltered True
    Else
        ProcessError "The map has to be at least 1 square high."
    End If
    
End Sub

Private Sub cmdMapIncBottom_Click()
    ResizeMap MapWidth, MapHeight + 1, NumLayers, 0, 0
    SetAltered True
End Sub

Private Sub cmdMapIncLeft_Click()
    PlayerStartX = PlayerStartX + 1
    ResizeMap MapWidth + 1, MapHeight, NumLayers, 1, 0
    SetAltered True
End Sub

Private Sub cmdMapIncRight_Click()
    ResizeMap MapWidth + 1, MapHeight, NumLayers, 0, 0
    SetAltered True
End Sub

Private Sub cmdMapIncTop_Click()
    PlayerStartY = PlayerStartY + 1
    ResizeMap MapWidth, MapHeight + 1, NumLayers, 0, 1
    SetAltered True
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    Select Case KeyCode
        Case vbKeyAdd
            mnuView_Click ViewZoomIn
        Case vbKeySubtract
            mnuView_Click ViewZoomOut
        Case vbKey0, vbKeyNumpad0
            mnuView_Click ViewZoom100
        Case vbKeyP
            mnuTools_Click ToolsPencil
        Case vbKeyA
            mnuTools_Click ToolsArrow
        Case vbKeyM
            mnuTools_Click ToolsPan
        Case vbKeyE
            mnuTools_Click ToolsEraser
        Case vbKeySpace
            OldClickMode = ClickMode
            SetClickMode cm_Pan
        Case vbKeyLeft
            'scroll a page to the left
            OffsetX = OffsetX - (picMap.ScaleWidth - 6 * TileWidth)
            If OffsetX < 0 Then OffsetX = 0
            CalcScrollBarOffsets
            picMap_Paint
        Case vbKeyRight
            OffsetX = OffsetX + (picMap.ScaleWidth - 6 * TileWidth)
            If OffsetX > MapWidth * TileWidth - picMap.ScaleWidth Then OffsetX = MapWidth * TileWidth - picMap.ScaleWidth
            CalcScrollBarOffsets
            picMap_Paint
        Case vbKeyUp
            OffsetY = OffsetY - (picMap.ScaleHeight - 6 * TileHeight)
            If OffsetY < 0 Then OffsetY = 0
            CalcScrollBarOffsets
            picMap_Paint
        Case vbKeyDown
            OffsetY = OffsetY + (picMap.ScaleHeight - 6 * TileHeight)
            If OffsetY > MapHeight * TileHeight - picMap.ScaleHeight Then OffsetY = MapHeight * TileHeight - picMap.ScaleHeight
            CalcScrollBarOffsets
            picMap_Paint
    End Select
End Sub



Private Sub Form_KeyUp(KeyCode As Integer, Shift As Integer)
    Select Case KeyCode
        Case vbKeySpace
            SetClickMode OldClickMode
    End Select
End Sub

Private Sub Form_Load()
    'menu shortcuts
    mnuFile(FileExit).Caption = mnuFile(FileExit).Caption & vbTab & "Alt+F4"
    mnuTools(ToolsArrow).Caption = mnuTools(ToolsArrow).Caption & vbTab & "A"
    mnuTools(ToolsPencil).Caption = mnuTools(ToolsPencil).Caption & vbTab & "P"
    mnuTools(ToolsEraser).Caption = mnuTools(ToolsEraser).Caption & vbTab & "E"
    mnuTools(ToolsPan).Caption = mnuTools(ToolsPan).Caption & vbTab & "Space"
    mnuView(ViewZoomIn).Caption = mnuView(ViewZoomIn).Caption & vbTab & "+"
    mnuView(ViewZoomOut).Caption = mnuView(ViewZoomOut).Caption & vbTab & "-"
    mnuView(ViewZoom100).Caption = mnuView(ViewZoom100).Caption & vbTab & "0"
    
    'scrollbars
    vsbMap.LargeChange = (vsbMap.Max - vsbMap.Min) * (1 / 4)
    vsbMap.SmallChange = (vsbMap.Max - vsbMap.Min) * (1 / 8)
    hsbMap.LargeChange = (hsbMap.Max - hsbMap.Min) * (1 / 4)
    hsbMap.SmallChange = (hsbMap.Max - hsbMap.Min) * (1 / 8)
    
    Grid = True
    
    
    'start new document
    DoFileNew
End Sub





Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    If UnloadMode = vbFormControlMenu Then
        If Not DoFileExit() Then Cancel = -1         'don't close
    End If
End Sub

Private Sub Form_Resize()
    If Me.ScaleHeight > MinFormHeight And Me.ScaleWidth > MinFormWidth Then
        'place map
        picMap.Move picMap.Left, picMap.Top, Me.ScaleWidth - vsbMap.Width - picMap.Left, Me.ScaleHeight - hsbMap.Height - picMap.Top - lblErr.Height
        
        'place map expanding/contracting buttons
        cmdMapIncTop.Move picMap.Left + picMap.Width, picMap.Top
        cmdMapDecTop.Move cmdMapIncTop.Left, cmdMapIncTop.Top + cmdMapIncTop.Height
        cmdMapIncBottom.Move picMap.Left + picMap.Width, picMap.Top + picMap.Height - cmdMapIncBottom.Height
        cmdMapDecBottom.Move cmdMapIncBottom.Left, cmdMapIncBottom.Top - cmdMapIncBottom.Height
        cmdMapIncLeft.Move picMap.Left, picMap.Top + picMap.Height
        cmdMapDecLeft.Move cmdMapIncLeft.Left + cmdMapIncLeft.Width, cmdMapIncLeft.Top
        cmdMapIncRight.Move picMap.Left + picMap.Width - cmdMapIncRight.Width, picMap.Top + picMap.Height
        cmdMapDecRight.Move cmdMapIncRight.Left - cmdMapDecRight.Width, cmdMapIncRight.Top
        
        'scrollbars
        hsbMap.Move cmdMapDecLeft.Left + cmdMapDecLeft.Width, picMap.Top + picMap.Height, cmdMapDecRight.Left - (cmdMapDecLeft.Left + cmdMapDecLeft.Width), hsbMap.Height
        vsbMap.Move picMap.Left + picMap.Width, cmdMapDecTop.Top + cmdMapDecTop.Height, vsbMap.Width, cmdMapDecBottom.Top - (cmdMapDecTop.Top + cmdMapDecTop.Height)
        
        'place layers box and label
        lstLayers.Move Margin, Me.ScaleHeight - Margin - lstLayers.Height, picMap.Left - Margin * 2
        lblLayers.Move lstLayers.Left, lstLayers.Top - Margin - lblLayers.Height, lstLayers.Width
        cmdAddLayer.Move cmdAddLayer.Left, lblLayers.Top
        cmdDeleteLayer.Move cmdDeleteLayer.Left, lblLayers.Top
        
        'preview box
        picTilePreview.Move picMap.Left / 2 - TileWidth / 2, lblLayers.Top - Margin - TileHeight, TileWidth, TileHeight
        lblPreview.Move lstLayers.Left, picTilePreview.Top - Margin - lblPreview.Height, lstLayers.Width
        
        'tiles box
        lstTiles.Move lstLayers.Left, lstTiles.Top, lstLayers.Width, lblPreview.Top - Margin - lstTiles.Top
        
        'error display
        lblErr.Move picMap.Left, hsbMap.Top + hsbMap.Height, Me.ScaleWidth - picMap.Left
        
        
        
        'update display
        picWork.Move 0, 0, picMap.Width, picMap.Height
        SetScrollBarOffsets
        picMap_Paint
    End If
End Sub



Private Sub SetScrollBarOffsets()
    OffsetX = (hsbMap.Value - hsbMap.Min) / (hsbMap.Max - hsbMap.Min) * (MapWidth * TileWidth * Zoom - picMap.ScaleWidth)
    OffsetY = (vsbMap.Value - vsbMap.Min) / (vsbMap.Max - vsbMap.Min) * (MapHeight * TileHeight * Zoom - picMap.ScaleHeight)
End Sub

Private Sub CalcScrollBarOffsets()
    Dim Temp As Boolean
    
    Dim NewVal As Long
    
    Temp = CodeGenerated
    CodeGenerated = True
    
    NewVal = OffsetX / (MapWidth * TileWidth - picMap.ScaleWidth) * (hsbMap.Max - hsbMap.Min) + hsbMap.Min
    If NewVal < hsbMap.Min Then NewVal = hsbMap.Min
    If NewVal > hsbMap.Max Then NewVal = hsbMap.Max
    hsbMap.Value = NewVal
    
    NewVal = OffsetY / (MapHeight * TileHeight - picMap.ScaleHeight) * (vsbMap.Max - vsbMap.Min) + vsbMap.Min
    If NewVal < vsbMap.Min Then NewVal = vsbMap.Min
    If NewVal > vsbMap.Max Then NewVal = vsbMap.Max
    vsbMap.Value = NewVal
    
    SetScrollBarOffsets
    CodeGenerated = Temp
End Sub

Private Sub hsbMap_Change()
    If Not CodeGenerated Then
        SetScrollBarOffsets
        picMap_Paint
    End If
End Sub

Private Sub hsbMap_Scroll()
    hsbMap_Change
End Sub



Private Sub lstLayers_ItemCheck(Item As Integer)
    If Not CodeGenerated Then picMap_Paint
End Sub

Private Sub lstLayers_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Button = vbKeyRButton Then
        mnuLayerOptions(LayerOptionsSetPlayerLayer).Enabled = Not (lstLayers.ListIndex = PlayerLayer)
        mnuLayerOptions(LayerOptionsMoveUp).Enabled = (lstLayers.ListIndex > 0)
        mnuLayerOptions(LayerOptionsMoveDown).Enabled = (lstLayers.ListIndex < NumLayers - 1)
        
        PopupMenu mnuPopLayerOptions
    End If
End Sub



Private Sub mnuEdit_Click(Index As Integer)
    Select Case Index
        Case EditUndo, EditRedo
            ProcessError "Sorry, no undo/redo yet! ;)"
        Case EditCut
            mnuEdit_Click EditCopy
            mnuEdit_Click EditDelete
        Case EditCopy
            DoCopy
        Case EditPaste
            DoPaste
            SetAltered True
        Case EditDelete
            IsSel = False
            picMap_Paint
        Case EditSelectAll
            SelectNone
            SelLeft = 0
            SelTop = 0
            SelWidth = MapWidth
            SelHeight = MapHeight
            IsSel = True
            CopySelToMemory
            picMap_Paint
    End Select
End Sub

Private Sub mnuLayerOptions_Click(Index As Integer)
    Dim LIndex As Integer
    Select Case Index
        Case LayerOptionsSetPlayerLayer
            PlayerLayer = lstLayers.ListIndex
            BuildLayersList
            SetAltered True
        Case LayerOptionsMoveUp
            LIndex = lstLayers.ListIndex
            SwapLayers lstLayers.ListIndex, lstLayers.ListIndex - 1
            lstLayers.ListIndex = LIndex - 1
        Case LayerOptionsMoveDown
            LIndex = lstLayers.ListIndex
            SwapLayers lstLayers.ListIndex, lstLayers.ListIndex + 1
            lstLayers.ListIndex = LIndex + 1
    End Select
End Sub

Private Sub SwapLayers(Layer1 As Integer, Layer2 As Integer)
    Dim X As Integer, Y As Integer
    Dim Temp As Integer
    Dim TempBool As Boolean
    
    'swap player layer
    If PlayerLayer = Layer1 Then
        PlayerLayer = Layer2
    ElseIf PlayerLayer = Layer2 Then
        PlayerLayer = Layer1
    End If
    
    'swap layer data
    For Y = 0 To MapHeight - 1
        For X = 0 To MapWidth - 1
            Temp = Map(X, Y, Layer1)
            Map(X, Y, Layer1) = Map(X, Y, Layer2)
            Map(X, Y, Layer2) = Temp
        Next X
    Next Y
    
    'swap whether it's checked or not
    CodeGenerated = True
    TempBool = lstLayers.Selected(Layer1)
    lstLayers.Selected(Layer1) = lstLayers.Selected(Layer2)
    lstLayers.Selected(Layer2) = TempBool
    CodeGenerated = False
    
    
    BuildLayersList
    SetAltered True
End Sub

Private Sub mnuPlayerContext_Click(Index As Integer)
    Select Case Index
        Case PlayerContextSwitchCharacter
                frmChooseChar.RefreshCharsBox
                frmChooseChar.Show vbModal
                
                If Not frmChooseChar.Cancelled Then
                    PlaytestChar = frmChooseChar.lstChars.List(frmChooseChar.lstChars.ListIndex) & ".char"
                    
                    'TODO: build masks and use this character in the map
                End If

    End Select
End Sub



Private Sub mnuSquContext_Click(Index As Integer)
    Dim OldPStX As Integer, OldPStY As Integer

    Select Case Index
        Case SquContextSetPlaytestStart
            OldPStX = PlayerStartX
            OldPStY = PlayerStartY
            
            PlayerStartX = LastXIndex
            PlayerStartY = LastYIndex
            
            UpdatePlayerSquare OldPStX, OldPStY
            UpdateSquare PlayerStartX, PlayerStartY, picMap
    End Select
End Sub

Private Sub picMap_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim XIndex As Integer, YIndex As Integer
    
    MouseDownX = AbsX(X)
    MouseDownY = AbsY(Y)
    MouseDownOffsetX = OffsetX
    MouseDownOffsetY = OffsetY
    LastXIndex = -1
    LastYIndex = -1
    
    If Button = vbKeyMButton Then
        'eyedropper
        'get square mouse is over
        If Not IsSel Then
            XIndex = Int(AbsX(X) / (TileWidth * Zoom))
            YIndex = Int(AbsY(Y) / (TileHeight * Zoom))
            If XIndex < 0 Then XIndex = 0
            If XIndex >= MapWidth Then XIndex = MapWidth
            If YIndex < 0 Then YIndex = 0
            If YIndex >= MapHeight Then YIndex = MapHeight
            
            If Map(XIndex, YIndex, lstLayers.ListIndex) <> -1 Then lstTiles.ListIndex = Map(XIndex, YIndex, lstLayers.ListIndex)
        End If
    Else
        If ClickMode = cm_Arrow Then
                If Button = vbKeyLButton Then
                If IsMouseOverSelEdge(X, Y) Then
                
                
                    MouseDownMoveSel = True
                    DownSelLeft = SelLeft
                    DownSelTop = SelTop
                
                ElseIf X > DispX(PlayerStartX * TileWidth * Zoom) + TileWidth * Zoom / 2 - imgPlayerIcon.Width * Zoom / 2 And _
                       X < DispX(PlayerStartX * TileWidth * Zoom) + TileWidth * Zoom / 2 + imgPlayerIcon.Width * Zoom / 2 And _
                       Y > DispY(PlayerStartY * TileHeight * Zoom) + TileHeight * Zoom - imgPlayerIcon.Height * Zoom And _
                       Y < DispY(PlayerStartY * TileHeight * Zoom) + TileHeight * Zoom Then
                    
                    'drag playerstart
                    MouseDownMovePlayerSt = True
                    DownPStX = PlayerStartX
                    DownPStY = PlayerStartY
                    
                Else
                
                    If lstLayers.Selected(lstLayers.ListIndex) Then
                        'get square mouse is over
                        DraggingSel = True
                        
                        XIndex = Math.Round(AbsX(X) / (TileWidth * Zoom), 0)
                        YIndex = Math.Round(AbsY(Y) / (TileHeight * Zoom), 0)
                        If XIndex < 0 Then XIndex = 0
                        If XIndex >= MapWidth Then XIndex = MapWidth
                        If YIndex < 0 Then YIndex = 0
                        If YIndex >= MapHeight Then YIndex = MapHeight
                        
                        If IsSel Then
                            DrawSelLine
                            SelectNone
                        End If
                        IsSel = True
                        SelLeft = XIndex
                        SelTop = YIndex
                        SelWidth = 0
                        SelHeight = 0
                        
                        
                        SetInvertDraw
                        
                        DrawSelLine
                        'Me.Line (picObjects.Width + RelX(SelLeft * (DefSquWidth * Zoom)), picTools.Height + RelY(SelTop * (DefSquHeight * Zoom)))-(picObjects.Width + RelX((SelLeft + SelWidth) * (DefSquWidth * Zoom)), picTools.Height + RelY((SelTop + SelHeight) * (DefSquHeight * Zoom))), , B
                    End If
                End If
            ElseIf Button = vbKeyRButton Then
                'get square mouse is over
                If Not IsSel Then
                    XIndex = Int(AbsX(X) / (TileWidth * Zoom))
                    YIndex = Int(AbsY(Y) / (TileHeight * Zoom))
                    If XIndex < 0 Then XIndex = 0
                    If XIndex >= MapWidth Then XIndex = MapWidth
                    If YIndex < 0 Then YIndex = 0
                    If YIndex >= MapHeight Then YIndex = MapHeight
                    
                    LastXIndex = XIndex
                    LastYIndex = YIndex
                    
                    If XIndex = PlayerStartX And YIndex = PlayerStartY Then
                        Me.PopupMenu mnuPopPlayerContext
                    Else
                        Me.PopupMenu mnuPopSquContext
                    End If
                End If
            End If
        End If
    End If
    
    picMap_MouseMove Button, Shift, X, Y
End Sub

Private Sub picMap_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    Dim SelX As Integer, SelY As Integer
    Dim OldSelLeft As Integer, OldSelTop As Integer
    Dim OldPStX As Integer, OldPStY As Integer
    Dim XIndex As Integer, YIndex As Integer
    Dim XI As Integer, YI As Integer
    Dim InBoundaries As Boolean
    
    If Button = 0 And Me.MousePointer = vbSizeAll And Not IsSel Then
        Me.MousePointer = vbDefault
    ElseIf Button = 0 And IsSel Then
    
        If Me.MousePointer = vbDefault Then
            'if mouse is over edges of selection, mousepointer is sizeall
            If IsMouseOverSelEdge(X, Y) Then Me.MousePointer = vbSizeAll
        ElseIf Me.MousePointer = vbSizeAll Then
            'if mouse is not over edges, mousepointer is normal
            If Not IsMouseOverSelEdge(X, Y) Then Me.MousePointer = vbDefault
        End If
    End If

    XIndex = Int(AbsX(X) / (TileWidth * Zoom))
    YIndex = Int(AbsY(Y) / (TileHeight * Zoom))
    
    If XIndex >= 0 And XIndex < MapWidth And YIndex >= 0 And YIndex < MapHeight Then
        Me.lblErr.Visible = True
        Me.lblErr.ForeColor = vbBlack
        Me.lblErr.Caption = "(" & XIndex & ", " & YIndex & ")"
    Else
        Me.lblErr.Visible = False
    End If

    If Button = vbKeyLButton Then
        
        SelX = Int(AbsX(X) / (TileWidth * Zoom))
        SelY = Int(AbsY(Y) / (TileHeight * Zoom))
        
        InBoundaries = SelX >= 0 And SelX < MapWidth And SelY >= 0 And SelY < MapHeight
                        
        Select Case ClickMode
            Case cm_Arrow
                If MouseDownMoveSel Then
            
                    XIndex = Math.Round((AbsX(X) - MouseDownX) / (TileWidth * Zoom), 0)
                    YIndex = Math.Round((AbsY(Y) - MouseDownY) / (TileWidth * Zoom), 0)
                    If DownSelLeft + SelWidth + XIndex >= MapWidth Then XIndex = XIndex - ((DownSelLeft + SelWidth + XIndex) - MapWidth)
                    If DownSelTop + SelHeight + YIndex >= MapHeight Then YIndex = YIndex - ((DownSelTop + SelHeight + YIndex) - MapHeight)
                    If DownSelLeft + XIndex < 0 Then XIndex = XIndex - (DownSelLeft + XIndex)
                    If DownSelTop + YIndex < 0 Then YIndex = YIndex - (DownSelTop + YIndex)
                    
                    If SelLeft <> DownSelLeft + XIndex Or SelTop <> DownSelTop + YIndex Then
                        
                        DrawSelLine
                        
                        OldSelLeft = SelLeft
                        OldSelTop = SelTop
                        
                        
                        SelLeft = DownSelLeft + XIndex
                        SelTop = DownSelTop + YIndex
                        
                        
                        
                        For YI = Min(OldSelTop, SelTop) To Max(OldSelTop + SelHeight - 1, SelTop + SelHeight - 1)
                            For XI = Min(OldSelLeft, SelLeft) To Max(OldSelLeft + SelWidth - 1, SelLeft + SelWidth - 1)
                                UpdateSquare XI, YI, picMap, True
                            Next XI
                        Next YI
                        
                        DrawSelLine
                        
                        SetAltered True
                    End If
                ElseIf MouseDownMovePlayerSt Then
                    XIndex = Math.Round((AbsX(X) - MouseDownX) / (TileWidth * Zoom), 0)
                    YIndex = Math.Round((AbsY(Y) - MouseDownY) / (TileHeight * Zoom), 0)
                    
                    OldPStX = PlayerStartX
                    OldPStY = PlayerStartY
                    
                    XI = DownPStX + XIndex
                    YI = DownPStY + YIndex
                    
                    If XI < 0 Then XI = 0
                    If YI < 0 Then YI = 0
                    If XI >= MapWidth Then XI = MapWidth - 1
                    If YI >= MapHeight Then YI = MapHeight - 1
                    
                    If Not IsObstacle(XI, YI) Then
                        PlayerStartX = XI
                        PlayerStartY = YI
                    
                        UpdatePlayerSquare OldPStX, OldPStY
                        UpdateSquare PlayerStartX, PlayerStartY, picMap
                    End If
                ElseIf (Button And vbKeyLButton) Then
                    'get square mouse is over
                    If lstLayers.Selected(lstLayers.ListIndex) Then
                        XIndex = Math.Round(AbsX(X) / (TileWidth * Zoom), 0)
                        YIndex = Math.Round(AbsY(Y) / (TileHeight * Zoom), 0)
                        If XIndex < 0 Then XIndex = 0
                        If XIndex >= MapWidth Then XIndex = MapWidth
                        If YIndex < 0 Then YIndex = 0
                        If YIndex >= MapHeight Then YIndex = MapHeight
                        
                        If XIndex <> LastXIndex Or YIndex <> LastYIndex Then
                            
                            DrawSelLine
                
                            SelWidth = XIndex - SelLeft
                            SelHeight = YIndex - SelTop
                            
                            DrawSelLine
                        End If
                    End If
                End If
            Case cm_Pencil
                'fill the square at the current x, y, layer, with the current tile
                If lstLayers.Selected(lstLayers.ListIndex) Then
                    
                    If InBoundaries And lstTiles.ListIndex > -1 And lstLayers.ListIndex > -1 Then
                        
                        Map(SelX, SelY, lstLayers.ListIndex) = lstTiles.ListIndex
                        SetAltered True
                        
                        
                        UpdateSquare SelX, SelY, picMap
                        
                        If SelX = PlayerStartX And SelY = PlayerStartY - 1 Then UpdateSquare PlayerStartX, PlayerStartY, picMap
                    End If
                End If
            Case cm_Eraser
                If lstLayers.Selected(lstLayers.ListIndex) Then
                    If InBoundaries And lstLayers.ListIndex > -1 Then
                        
                        Map(SelX, SelY, lstLayers.ListIndex) = -1
                        SetAltered True
                        
                        
                        UpdateSquare SelX, SelY, picMap
                        If SelX = PlayerStartX And SelY = PlayerStartY - 1 Then UpdateSquare PlayerStartX, PlayerStartY, picMap
                    End If
                End If
            Case cm_Pan
                OffsetX = MouseDownOffsetX - (AbsX(X) - MouseDownX)
                OffsetY = MouseDownOffsetY - (AbsY(Y) - MouseDownY)
                CalcScrollBarOffsets
                picMap_Paint
        End Select
        
    End If
    
    LastXIndex = XIndex
    LastYIndex = XIndex
End Sub



Private Sub SetClickMode(NewClickMode As Integer)
    ClickMode = NewClickMode
    Select Case ClickMode
        Case cm_Arrow
            mnuTools_Click ToolsArrow
        Case cm_Pencil
            mnuTools_Click ToolsPencil
        Case cm_Eraser
            mnuTools_Click ToolsEraser
        Case cm_Pan
            mnuTools_Click ToolsPan
    End Select
End Sub

Private Sub picMap_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    
    If ClickMode = cm_Arrow Then
        'get rid of that corner rubbish
        If SelWidth = 0 Or SelHeight = 0 Then
            DrawSelLine
            IsSel = False
        End If
        
        If IsSel And Not MouseDownMoveSel And (Button And vbKeyLButton) Then
            'make sure width and height are positive
            If SelWidth < 0 Then
                SelLeft = SelLeft + SelWidth
                SelWidth = -SelWidth
            End If
            If SelHeight < 0 Then
                SelTop = SelTop + SelHeight
                SelHeight = -SelHeight
            End If
            
            
            CopySelToMemory
        End If
    End If
    MouseDownMoveSel = False
    MouseDownMovePlayerSt = False
    DraggingSel = False
End Sub

Private Sub vsbMap_Change()
    If Not CodeGenerated Then
        SetScrollBarOffsets
        picMap_Paint
    End If
End Sub

Private Sub vsbMap_Scroll()
    vsbMap_Change
End Sub

Private Sub lblErr_Click()
    tmrClearErr_Timer
End Sub

Private Sub lstTiles_Click()
    picTilePreview.Cls
    picTilePreview.PaintPicture imgTileMask(lstTiles.ListIndex).Picture, 0, 0, , , , , , , vbSrcAnd
    picTilePreview.PaintPicture imgTile(lstTiles.ListIndex).Picture, 0, 0, , , , , , , vbSrcPaint
End Sub



Private Sub SetAltered(Value As Boolean)
    Altered = Value
    Me.Caption = IIf(FilePath = "", "Untitled", FileTitle) & IIf(Altered, "*", "") & " - " & FormCaption
End Sub

Private Sub BuildLayersList()
    Dim I As Integer
    
    Dim LastSel As Integer
    
    Dim NumPrevSet As Integer
    Dim PrevSet() As Boolean
    
    LastSel = lstLayers.ListIndex
    
    NumPrevSet = lstLayers.ListCount
    If NumPrevSet > 0 Then ReDim PrevSet(NumPrevSet - 1)
    For I = 0 To lstLayers.ListCount - 1
        PrevSet(I) = lstLayers.Selected(I)
    Next I
    
    CodeGenerated = True
    lstLayers.Clear
    For I = 0 To NumLayers - 1
        lstLayers.AddItem "Layer " & (I + 1) & IIf(I = PlayerLayer, " (player layer)", "")
        If I < NumPrevSet Then
            lstLayers.Selected(I) = PrevSet(I)
        Else
            lstLayers.Selected(I) = True
        End If
    Next I
    
    CodeGenerated = False
    
    If LastSel > -1 Then
        Do While LastSel >= lstLayers.ListCount
            LastSel = LastSel - 1
        Loop
        lstLayers.ListIndex = LastSel
    Else
        lstLayers.ListIndex = 0
    End If
End Sub

Private Sub ClearMap(TheMap() As Integer, MW As Integer, MH As Integer, NL As Integer)
    Dim I As Integer, X As Integer, Y As Integer
    For Y = 0 To MH - 1
        For X = 0 To MW - 1
            For I = 0 To NL - 1
                TheMap(X, Y, I) = -1
            Next I
        Next X
    Next Y
End Sub

Private Sub LoadMap(File As String)
    Dim Free As Integer
    
    Dim Int4 As Long
    Dim Char As Byte
    Dim X As Integer, Y As Integer, I As Integer
    
    Dim TileName As String
    
    
    Free = FreeFile
    Open File For Binary Access Read As #Free
        Get #Free, , Int4
        If Int4 <> 1 Then
            MsgBox "This map file is version " & Int4 & ", and this editor can only read versions up to 1." & vbCrLf & vbCrLf & "Get the newest version of the map editor to read this map file.", vbOKOnly + vbExclamation
            Close #Free
            mnuFile_Click FileNew
            Exit Sub
        End If
        
        Get #Free, , Int4
        NumTiles = Int4
        
        If NumTiles > 0 Then ReDim Tiles(NumTiles - 1)
        
        For I = 0 To NumTiles - 1
            Get #Free, , Int4
            TileName = ""
            For Y = 1 To Int4
                Get #Free, , Char
                TileName = TileName & Chr$(Char)
            Next Y
            If Right$(TileName, 5) = ".tile" Then
                Tiles(I) = Left$(TileName, Len(TileName) - 5)
            Else
                Tiles(I) = TileName
            End If
        Next I
        
        Get #Free, , Int4
        MapWidth = Int4
        
        Get #Free, , Int4
        MapHeight = Int4
        
        Get #Free, , Int4
        NumLayers = Int4
        
        Get #Free, , Int4
        PlayerLayer = NumLayers - 1 - Int4
        
        ReDim Map(MapWidth - 1, MapHeight - 1, NumLayers - 1)
        
        For Y = 0 To MapHeight - 1
            For X = 0 To MapWidth - 1
                For I = NumLayers - 1 To 0 Step -1
                    Get #Free, , Int4
                    Map(X, Y, I) = Int4
                Next I
            Next X
        Next Y
    Close #Free
    
    PlayerStartX = 0
    PlayerStartY = 0
    
    BuildLayersList
    UpdateTilesList
    
    mnuTools_Click ToolsArrow
    Zoom = 1
    hsbMap.Value = 0
    vsbMap.Value = 0
    
    picWork.Cls
    picMap_Paint
    
End Sub

Private Sub SaveMap(File As String)
    Dim Free As Integer
    
    Dim Int4 As Long
    Dim Char As Byte
    Dim X As Integer, Y As Integer, I As Integer
    Dim TileName As String
    
    Free = FreeFile
    Open File For Binary Access Write As #Free
        Int4 = 1
        Put #Free, , Int4
        
        Int4 = NumTiles
        Put #Free, , Int4
        
        For X = 0 To NumTiles - 1
            TileName = Tiles(X) & ".tile"
            
            Int4 = Len(TileName)
            Put #Free, , Int4
            
            For Y = 1 To Len(TileName)
                Char = Asc(Mid$(TileName, Y, 1))
                Put #Free, , Char
            Next Y
        Next X
        
        Int4 = MapWidth
        Put #Free, , Int4
        
        Int4 = MapHeight
        Put #Free, , Int4
        
        Int4 = NumLayers
        Put #Free, , Int4
        
        Int4 = NumLayers - 1 - PlayerLayer
        Put #Free, , Int4
        
        For Y = 0 To MapHeight - 1
            For X = 0 To MapWidth - 1
                For I = NumLayers - 1 To 0 Step -1
                    Int4 = Map(X, Y, I)
                    Put #Free, , Int4
                Next I
            Next X
        Next Y
        
        
        
    Close #Free
End Sub

Private Function DoFileOpen() As Boolean
    Dim Result As VbMsgBoxResult
    
    'returns false if the user cancels
    If Altered Then
        Result = MsgBox("Would you like to save the map before opening a new one?", vbYesNoCancel + vbQuestion)
        If Result = vbCancel Then
            Exit Function
        ElseIf Result = vbYes Then
            If Not DoFileSave Then Exit Function
        End If
    End If
    
    cdl.DialogTitle = "Open motrs map"
    cdl.Filter = "Map files (*.map)|*.map|All files (*)|*"
    cdl.FileName = ""
    cdl.Flags = cdlOFNHideReadOnly
    cdl.InitDir = App.Path & "\" & MapsFolder
    cdl.ShowOpen
    
    If cdl.FileName = "" Then Exit Function
    
    FilePath = cdl.FileName
    FileTitle = cdl.FileTitle
    LoadMap FilePath
    SetAltered False
    
    DoFileOpen = True
End Function

Private Function DoFileSave() As Boolean
    'returns false if the user cancels
    If FilePath = "" Then
        'get a file path
        DoFileSave = DoFileSaveAs()
    Else
        SaveMap FilePath
        SetAltered False
        DoFileSave = True
    End If
End Function

Private Function DoFileSaveAs() As Boolean
    'return false if the user cancels
    cdl.DialogTitle = "Save map as"
    cdl.Filter = "Map files (*.map)|*.map|All files (*)|*"
    cdl.FileName = ""
    cdl.InitDir = App.Path & "\" & MapsFolder
    cdl.Flags = cdlOFNHideReadOnly
    cdl.ShowSave
    
    If cdl.FileName = "" Then Exit Function
    
    FilePath = cdl.FileName
    FileTitle = cdl.FileTitle
    DoFileSaveAs = DoFileSave()
End Function

Private Function DoFileExit() As Boolean
    Dim Result As VbMsgBoxResult
    
    'confirm save
    If Altered Then
        Result = MsgBox("Would you like to save your map before closing?", vbYesNoCancel + vbQuestion, "Confirm Exit")
        If Result = vbCancel Then
            Exit Function
        ElseIf Result = vbYes Then
            If Not DoFileSave() Then Exit Function
        End If
        
    End If
    
    DoFileExit = True
        
    Unload frmMapProperties
    Unload frmChooseTiles
    Unload frmChooseChar
    Unload frmBitmapSplicer
    Unload frmTileEditor
    Unload Me
    
    
End Function

Private Function DoFileNew() As Boolean
    Dim Result As VbMsgBoxResult
    
    If Altered Then
        Result = MsgBox("Would you like to save your map before making a new document?", vbYesNoCancel + vbQuestion)
        If Result = vbCancel Then
            Exit Function
        ElseIf Result = vbYes Then
            If Not DoFileSave() Then Exit Function
        End If
    End If
    
    NumLayers = 1
    PlayerLayer = 0
    BuildLayersList
    
    MapWidth = 20
    MapHeight = 20
    ReDim Map(MapWidth - 1, MapHeight - 1, NumLayers - 1)
    ClearMap Map, MapWidth, MapHeight, NumLayers
    
    NumTiles = 0
    ReDim Tiles(0)
    UpdateTilesList
    
    Zoom = 1
    hsbMap.Value = 0
    vsbMap.Value = 0
    
    IsSel = False
    
    PlayerStartX = 0
    PlayerStartY = 0
    PlaytestChar = "hero.char"
    LoadCharacter PlaytestChar
    
    FileTitle = ""
    FilePath = ""
    SetAltered False
    
    picMap_Paint
End Function

Private Sub mnuFile_Click(Index As Integer)
    Dim Free As Integer
    Dim Line As String
    
    Select Case Index
        Case FileNew
            DoFileNew
        Case FileOpen
            DoFileOpen
        Case FileSave
            DoFileSave
        Case FileSaveAs
            DoFileSaveAs
        Case FileMapProperties
            frmMapProperties.txtMapDimX.Text = MapWidth
            frmMapProperties.txtMapDimY.Text = MapHeight
            frmMapProperties.txtLayers.Text = NumLayers
            frmMapProperties.Show vbModal
            
            If Not frmMapProperties.Canceled Then
                ResizeMap Val(frmMapProperties.txtMapDimX.Text), Val(frmMapProperties.txtMapDimY.Text), Val(frmMapProperties.txtLayers.Text)
                SetAltered True
            End If
        Case FilePlaytestMap
            On Error GoTo PlaytestErr
            SaveMap App.Path & "\..\..\resource\maps\test.map"
            Shell App.Path & "\playtest.bat --windowed --testmap test.map --startx " & PlayerStartX & " --starty " & PlayerStartY & " --char " & PlaytestChar, vbNormalFocus
            On Error GoTo 0
        Case FileExit
            DoFileExit
            
            'exit
    End Select
    
    Exit Sub
PlaytestErr:
    ProcessError "Error playtesting map: " & Err.Description
End Sub

Private Sub DoCopy()
    Dim Free As Integer
    Dim X As Integer, Y As Integer
    Dim Something As Boolean
    
    If IsSel Then
        Free = FreeFile
        Open App.Path & "\clipboard.bin" For Binary Access Write As #Free
            Put #Free, , SelWidth
            Put #Free, , SelHeight
            For Y = 0 To SelHeight - 1
                For X = 0 To SelWidth - 1
                    If Selection(X, Y) = -1 Then
                        Put #Free, , CInt(0)
                    Else
                        Something = True
                        Put #Free, , CInt(Len(Tiles(Selection(X, Y))))
                        Put #Free, , Tiles(Selection(X, Y))
                    End If
                Next X
            Next Y
        Close #Free
        
        If Not Something Then ProcessError "Warning: You didn't actually copy anything."
    Else
        ProcessError "Nothing to copy"
    End If
End Sub



Private Sub DoPaste()
    Dim Free As Integer
    Dim X As Integer, Y As Integer
    Dim Str As String, StrLen As Integer
    Dim I As Integer
    Dim Found As Boolean
    Dim AddedNewTiles As Boolean
    
    If FileExists(App.Path & "\clipboard.bin") Then
        Free = FreeFile
        Open App.Path & "\clipboard.bin" For Binary Access Read As #Free
            Get #Free, , SelWidth
            Get #Free, , SelHeight
            ReDim Selection(SelWidth - 1, SelHeight - 1)
            For Y = 0 To SelHeight - 1
                For X = 0 To SelWidth - 1
                    Get #Free, , StrLen
                    
                    If StrLen > 0 Then
                        Str = Space(StrLen)
                        Get #Free, , Str
                        
                        Found = False
                        For I = 0 To NumTiles - 1
                            If Tiles(I) = Str Then
                                Selection(X, Y) = I
                                Found = True
                                Exit For
                            End If
                        Next I
                        If Not Found Then
                            'add tile to tileset
                            ReDim Preserve Tiles(NumTiles)
                            Tiles(NumTiles) = Str
                            Selection(X, Y) = NumTiles
                            NumTiles = NumTiles + 1
                            AddedNewTiles = True
                        End If
                    Else
                        Selection(X, Y) = -1
                    End If
                Next X
            Next Y
        Close #Free
        
        SelLeft = Int(AbsX(TileWidth * Zoom) / (TileWidth * Zoom))
        SelTop = Int(AbsY(TileHeight * Zoom) / (TileHeight * Zoom))
        IsSel = True
        
        If AddedNewTiles Then UpdateTilesList
        
        picMap_Paint
    Else
        ProcessError "Nothing on clipboard."
    End If
    
End Sub

Private Sub ResizeMap(NewWidth As Integer, NewHeight As Integer, NewNumLayers As Integer, Optional XShift As Integer = 0, Optional YShift As Integer = 0)
    Dim NewMap() As Integer
    Dim X As Integer, Y As Integer, I As Integer
    
    
    ReDim NewMap(NewWidth - 1, NewHeight - 1, NewNumLayers - 1)
    ClearMap NewMap, NewWidth, NewHeight, NewNumLayers
    For I = 0 To NumLayers - 1
        For Y = 0 To MapHeight - 1
            For X = 0 To MapWidth - 1
                If I < NewNumLayers And Y + YShift < NewHeight And Y + YShift >= 0 And X + XShift < NewWidth And X + XShift >= 0 Then _
                    NewMap(X + XShift, Y + YShift, I) = Map(X, Y, I)
            Next X
        Next Y
    Next I
    
    MapWidth = NewWidth
    MapHeight = NewHeight
    NumLayers = NewNumLayers
    
    If PlayerLayer >= NumLayers Then PlayerLayer = NumLayers - 1
    
    ReDim Map(MapWidth - 1, MapHeight - 1, NumLayers - 1)
    
    
    'copy the array
    For I = 0 To NumLayers - 1
        For Y = 0 To MapHeight - 1
            For X = 0 To MapWidth - 1
                    Map(X, Y, I) = NewMap(X, Y, I)
            Next X
        Next Y
    Next I
    
    picWork.Cls
    SetScrollBarOffsets
    
    BuildLayersList
    picMap_Paint
End Sub

Private Sub mnuTools_Click(Index As Integer)
    Dim Used() As Boolean
    Dim X As Integer, Y As Integer, I As Integer
    
    Dim Trans() As Integer
    
    
    Select Case Index
        Case ToolsPencil
            UnpressToolButtons
            ClickMode = cm_Pencil
            tbr.Buttons("pencil").Value = tbrPressed
            
            If IsSel Then
                DrawSelLine
                SelectNone
            End If
        Case ToolsArrow
            UnpressToolButtons
            ClickMode = cm_Arrow
            tbr.Buttons("arrow").Value = tbrPressed
        Case ToolsEraser
            UnpressToolButtons
            ClickMode = cm_Eraser
            tbr.Buttons("eraser").Value = tbrPressed
            
            If IsSel Then
                DrawSelLine
                SelectNone
            End If
        Case ToolsPan
            UnpressToolButtons
            ClickMode = cm_Pan
            tbr.Buttons("pan").Value = tbrPressed
            
            If IsSel Then
                DrawSelLine
                SelectNone
            End If
        Case ToolsTileEditor
            frmTileEditor.RefreshTileList
            frmTileEditor.Show
        Case ToolsBitmapSplicer
            frmBitmapSplicer.ResetForm
            frmBitmapSplicer.Show vbModal
        Case ToolsRemoveUnusedTiles
            ReDim Used(NumTiles - 1)
            For Y = 0 To MapHeight - 1
                For X = 0 To MapWidth - 1
                    For I = 0 To NumLayers - 1
                        If Map(X, Y, I) <> -1 Then Used(Map(X, Y, I)) = True
                    Next I
                Next X
            Next Y
            
            ReDim Trans(NumTiles - 1)
            For I = 0 To NumTiles - 1
                Trans(I) = I
            Next I
            
            'delete unused
            I = 0
            Do While I < NumTiles
                If Used(I) Then
                    I = I + 1
                Else
                    'delete this tile
                    For X = I To NumTiles - 2
                        Tiles(X) = Tiles(X + 1)
                        Used(X) = Used(X + 1)
                    Next X
                    For X = 0 To UBound(Trans)
                        If Trans(X) >= I Then Trans(X) = Trans(X) - 1
                    Next X
                    NumTiles = NumTiles - 1
                    ReDim Preserve Tiles(NumTiles - 1)
                End If
            Loop
            
            UpdateMapWithNewTilesTrans Trans
            
            
            UpdateTilesList
    End Select
End Sub

Private Sub UnpressToolButtons()
    tbr.Buttons("arrow").Value = tbrUnpressed
    tbr.Buttons("pencil").Value = tbrUnpressed
    tbr.Buttons("eraser").Value = tbrUnpressed
    tbr.Buttons("pan").Value = tbrUnpressed
End Sub

Private Sub mnuView_Click(Index As Integer)
    Select Case Index
        Case ViewZoomIn
            ChangeZoom Zoom + 0.2
        Case ViewZoomOut
            ChangeZoom Zoom - 0.2
        Case ViewZoom100
            ChangeZoom 1
        Case ViewGrid
            Grid = Not Grid
            mnuView(ViewGrid).Checked = Grid
            picMap_Paint
    End Select
End Sub

Private Sub ChangeZoom(NewZoom As Single)
    Zoom = NewZoom
    If Zoom < MinZoom Then Zoom = MinZoom
    SetScrollBarOffsets
    picWork.Cls
    picMap_Paint
End Sub



Private Sub picMap_Paint()
    Dim X As Integer, Y As Integer
    Dim StartX As Long, EndX As Long
    Dim StartY As Long, EndY As Long
    
    Dim Coord As Integer
    
    'clear
    'picMap.Cls
    
    'update only visible squares
    StartX = Int(AbsX(0) / (TileWidth * Zoom))
    EndX = Int(AbsX(picMap.ScaleWidth) / (TileWidth * Zoom))
    StartY = Int(AbsY(0) / (TileHeight * Zoom))
    EndY = Int(AbsY(picMap.ScaleHeight) / (TileHeight * Zoom))
    
    If StartX < 0 Then StartX = 0
    If EndX >= MapWidth Then EndX = MapWidth - 1
    If StartY < 0 Then StartY = 0
    If EndY >= MapHeight Then EndY = MapHeight - 1
    
    For Y = StartY To EndY
        For X = StartX To EndX
            UpdateSquare X, Y, picWork
        Next X
    Next Y
    
    picMap.PaintPicture picWork.Image, 0, 0
    
End Sub


Private Function IsMouseOverSelEdge(X As Single, Y As Single)
    If Not IsSel Then
        IsMouseOverSelEdge = False
        Exit Function
    End If
    
    IsMouseOverSelEdge = False
    'top
    If Y >= DispY(SelTop * (TileHeight * Zoom)) - SelDrawWidth / 2 And _
       Y <= DispY(SelTop * (TileHeight * Zoom)) + SelDrawWidth / 2 And _
       X >= DispX(SelLeft * (TileWidth * Zoom)) - SelDrawWidth / 2 And _
       X <= DispX((SelLeft + SelWidth) * (TileWidth * Zoom)) + SelDrawWidth / 2 Then
            IsMouseOverSelEdge = True
    End If
    'bottom
    If Y >= DispY((SelTop + SelHeight) * (TileHeight * Zoom)) - SelDrawWidth / 2 And _
       Y <= DispY((SelTop + SelHeight) * (TileHeight * Zoom)) + SelDrawWidth / 2 And _
       X >= DispX(SelLeft * (TileWidth * Zoom)) - SelDrawWidth / 2 And _
       X <= DispX((SelLeft + SelWidth) * (TileWidth * Zoom)) + SelDrawWidth / 2 Then
            IsMouseOverSelEdge = True
    End If
    'left
    If Y >= DispY(SelTop * (TileHeight * Zoom)) - SelDrawWidth / 2 And _
       Y <= DispY((SelTop + SelHeight) * (TileHeight * Zoom)) + SelDrawWidth / 2 And _
       X >= DispY(SelLeft * (TileWidth * Zoom)) - SelDrawWidth / 2 And _
       X <= DispX(SelLeft * (TileWidth * Zoom)) + SelDrawWidth / 2 Then
            IsMouseOverSelEdge = True
    End If
    'right
    If Y >= DispY(SelTop * (TileHeight * Zoom)) - SelDrawWidth / 2 And _
       Y <= DispY((SelTop + SelHeight) * (TileHeight * Zoom)) + SelDrawWidth / 2 And _
       X >= DispX((SelLeft + SelWidth) * (TileWidth * Zoom)) - SelDrawWidth / 2 And _
       X <= DispX((SelLeft + SelWidth) * (TileWidth * Zoom)) + SelDrawWidth / 2 Then
            IsMouseOverSelEdge = True
    End If
End Function

Private Sub DrawSelLine()
    SetInvertDraw
    
    '4 lines
    picMap.Line (DispX(SelLeft * (TileWidth * Zoom)), DispY(SelTop * (TileHeight * Zoom)))-(DispX((SelLeft + SelWidth) * (TileWidth * Zoom)), DispY(SelTop * (TileHeight * Zoom))), , B
    picMap.Line (DispX((SelLeft + SelWidth) * (TileWidth * Zoom)), DispY(SelTop * (TileHeight * Zoom)))-(DispX((SelLeft + SelWidth) * (TileWidth * Zoom)), DispY((SelTop + SelHeight) * (TileHeight * Zoom))), , B
    picMap.Line (DispX((SelLeft + SelWidth) * (TileWidth * Zoom)), DispY((SelTop + SelHeight) * (TileHeight * Zoom)))-(DispX((SelLeft) * (TileWidth * Zoom)), DispY((SelTop + SelHeight) * (TileHeight * Zoom))), , B
    picMap.Line (DispX(SelLeft * (TileWidth * Zoom)), DispY((SelTop + SelHeight) * (TileHeight * Zoom)))-(DispX((SelLeft) * (TileWidth * Zoom)), DispY((SelTop) * (TileHeight * Zoom))), , B

End Sub

Private Sub SetInvertDraw()
    picMap.DrawWidth = SelDrawWidth
    picMap.DrawMode = vbInvert
    picMap.DrawStyle = vbSolid
    
    picWork.DrawWidth = SelDrawWidth
    picWork.DrawMode = vbInvert
    picWork.DrawStyle = vbSolid
End Sub

Private Sub SetNormalDraw()
    picMap.DrawWidth = 1
    picMap.DrawMode = vbCopyPen
    picMap.DrawStyle = vbSolid
    
    picWork.DrawWidth = 1
    picWork.DrawMode = vbCopyPen
    picWork.DrawStyle = vbSolid
End Sub

Private Sub UpdatePlayerSquare(X As Integer, Y As Integer)
    UpdateSquare X, Y, picMap
    If Y > 0 Then UpdateSquare X, Y - 1, picMap
End Sub

Private Sub UpdateSquare(X As Integer, Y As Integer, DC As PictureBox, Optional SkipSelLine As Boolean = False, Optional StartLayer As Integer = -1)
    Dim DLeft As Single, DTop As Single, DWidth As Single, DHeight As Single
    Dim I As Integer
    
    
    SetNormalDraw
    
    'calculate
    DLeft = DispX(X * TileWidth * Zoom)
    DTop = DispY(Y * TileHeight * Zoom)
    DWidth = Zoom * TileWidth
    DHeight = Zoom * TileHeight
    
    'clear
    If StartLayer = -1 Then
        DC.Line (DLeft, DTop)-(DLeft + DWidth, DTop + DHeight), vbWhite, BF
        StartLayer = NumLayers - 1
    End If
    
    
    
   
    'tile layers
    For I = StartLayer To 0 Step -1
        'paint tile at this layer
        If lstLayers.Selected(I) And Map(X, Y, I) >= 0 Then
            DC.PaintPicture imgTileMask(Map(X, Y, I)).Picture, DLeft, DTop, DWidth, DHeight, , , , , vbSrcAnd
            DC.PaintPicture imgTile(Map(X, Y, I)).Picture, DLeft, DTop, DWidth, DHeight, , , , , vbSrcPaint
        End If
        
        'paint player
        If PlayerLayer = I And X = PlayerStartX And Y = PlayerStartY Then
            DC.PaintPicture imgPlayerIconMask.Picture, DLeft + TileWidth * Zoom / 2 - imgPlayerIconMask.Width * Zoom / 2, DTop + TileHeight * Zoom - imgPlayerIconMask.Height * Zoom, imgPlayerIconMask.Width * Zoom, imgPlayerIconMask.Height * Zoom, , , , , vbSrcAnd
            DC.PaintPicture imgPlayerIcon.Picture, DLeft + TileWidth * Zoom / 2 - imgPlayerIcon.Width * Zoom / 2, DTop + TileHeight * Zoom - imgPlayerIcon.Height * Zoom, imgPlayerIcon.Width * Zoom, imgPlayerIcon.Height * Zoom, , , , , vbSrcPaint
            If Y > 0 And NumLayers > 1 Then UpdateSquare X, Y - 1, DC, True, I - 1
        End If
        
        'selection layer
        If IsSel And Not DraggingSel And I = lstLayers.ListIndex And X >= SelLeft And X < SelLeft + SelWidth And Y >= SelTop And Y < SelTop + SelHeight Then
            If Selection(X - SelLeft, Y - SelTop) >= 0 Then
                DC.PaintPicture imgTileMask(Selection(X - SelLeft, Y - SelTop)).Picture, DLeft, DTop, DWidth, DHeight, , , , , vbSrcAnd
                DC.PaintPicture imgTile(Selection(X - SelLeft, Y - SelTop)).Picture, DLeft, DTop, DWidth, DHeight, , , , , vbSrcPaint
            End If
        End If
    Next I
    
    'grid
    If Grid Then DC.Line (DLeft, DTop)-(DLeft + DWidth, DTop + DHeight), vbBlack, B
    
    'show selection rectangle
    If IsSel And Not SkipSelLine Then
        SetInvertDraw
        If (X = SelLeft And SelTop + SelHeight > Y And Y >= SelTop) Or (SelTop <= Y And SelTop + SelHeight > Y And X = SelLeft + SelWidth) Then
            'left side
            DC.Line (DLeft, DTop)-(DLeft, DTop + TileHeight * Zoom), , B
        End If
        If (Y = SelTop And SelLeft + SelWidth > X And X >= SelLeft) Or (SelLeft <= X And SelLeft + SelWidth > X And Y = SelTop + SelHeight) Then
            'top side
            DC.Line (DLeft, DTop)-(DLeft + TileWidth * Zoom, DTop), , B
        End If
        
    End If
End Sub

Private Sub SelectNone()
    Dim X As Integer, Y As Integer
    
    If Not IsSel Then Exit Sub
    
    
    
    'copy selection back to map
    For Y = SelTop To SelTop + SelHeight - 1
        For X = SelLeft To SelLeft + SelWidth - 1
           Map(X, Y, lstLayers.ListIndex) = Selection(X - SelLeft, Y - SelTop)
        Next X
    Next Y
    
    IsSel = False
End Sub

Private Sub CopySelToMemory()
    Dim X As Integer, Y As Integer
    
    'copy squares to selection and clear squares from map
    ReDim Selection(SelWidth - 1, SelHeight - 1)
    For Y = SelTop To SelTop + SelHeight - 1
        For X = SelLeft To SelLeft + SelWidth - 1
            Selection(X - SelLeft, Y - SelTop) = Map(X, Y, lstLayers.ListIndex)
            Map(X, Y, lstLayers.ListIndex) = -1
        Next X
    Next Y
End Sub


Private Sub tbr_ButtonClick(ByVal Button As MSComctlLib.Button)
    
    Select Case Button.Key
        Case "arrow"
            mnuTools_Click ToolsArrow
        Case "pencil"
            mnuTools_Click ToolsPencil
        Case "eraser"
            mnuTools_Click ToolsEraser
        Case "pan"
            mnuTools_Click ToolsPan
        Case "zoomin"
            mnuView_Click ViewZoomIn
        Case "zoomout"
            mnuView_Click ViewZoomOut
        Case "zoom100"
            mnuView_Click ViewZoom100
        Case "playtest"
            mnuFile_Click FilePlaytestMap
    End Select
End Sub

Private Sub tmrClearErr_Timer()
    lblErr.Visible = False
    tmrClearErr.Enabled = False
End Sub

Private Function DispX(AbsX As Integer) As Single
    If MapWidth * TileWidth * Zoom > picMap.ScaleWidth Then
        DispX = AbsX - OffsetX
    Else
        DispX = AbsX
    End If
End Function

Private Function DispY(AbsY As Integer) As Single
    If MapHeight * TileHeight * Zoom > picMap.ScaleHeight Then
        DispY = AbsY - OffsetY
    Else
        DispY = AbsY
    End If
End Function

Private Function AbsX(DispX As Single) As Integer
    If MapWidth * TileWidth * Zoom > picMap.ScaleWidth Then
        AbsX = DispX + OffsetX
    Else
        AbsX = DispX
    End If
End Function

Private Function AbsY(DispY As Single) As Integer
    If MapHeight * TileHeight * Zoom > picMap.ScaleHeight Then
        AbsY = DispY + OffsetY
    Else
        AbsY = DispY
    End If
End Function

Public Sub LoadCharacter(CharName As String)
    'load the standing south animation into the preview box
    Dim PreviewTile As New Tile
    Dim PropTile As New Tile
    Dim Standing As String
    Dim Pos As Integer, Pos2 As Integer
    Dim Index As Integer
    Dim Str As String
    Dim File As String

    
    On Error GoTo TileLoadErr
    PreviewTile.FromFile App.Path & "\" & CharactersFolder & "\" & CharName & ".txt"
    
    Standing = "," & PreviewTile.GetProperty("standing") & ","
    'break standing into 9 strings
    Pos = 1
    Pos2 = InStr(Pos + 1, Standing, ",")
    Index = 0
    Do While Pos > 0
        'grab the ani path
        Str = Trim$(Mid$(Standing, Pos + 1, Pos2 - Pos - 1))
        
        If Index = 5 Then
            'get rid of the .ani extension
            Str = StripExtension(Str)
            
            'grab the first frame
            File = Dir(App.Path & "\" & AnimationsFolder & "\" & Str & "\*.bmp", vbNormal)
            
            On Error GoTo PicLoadErr
            frmMain.picMaskInput.Picture = LoadPicture(App.Path & "\" & AnimationsFolder & "\" & Str & "\" & File)
            On Error GoTo 0
            
            'get the alpha color
            On Error GoTo TileLoadErr
            PropTile.FromFile App.Path & "\" & AnimationsFolder & "\" & Str & "\properties.txt"
            On Error GoTo 0
            
            
            'generate masks for if they hit OK
            CreateMask picMaskInput, picMaskOr, picMaskAnd, GetColorFromStr(PropTile.GetProperty("alphacolor"))
            imgPlayerIcon.Picture = picMaskOr.Image
            imgPlayerIconMask.Picture = picMaskAnd.Image
            imgPlayerIcon.Move 0, 0, picMaskInput.Width, picMaskInput.Height
            imgPlayerIconMask.Move 0, 0, picMaskInput.Width, picMaskInput.Height
            Exit Do
        End If
        
        Pos = Pos2
        Pos2 = InStr(Pos + 1, Standing, ",")
        Index = Index + 1
    Loop
    

    
    Exit Sub
PicLoadErr:
    ProcessError "Error loading bitmap specified in animation - " & Err.Description
    Exit Sub
TileLoadErr:
    ProcessError "Error loading character from disk - " & Err.Description
    Exit Sub
End Sub

Private Function IsObstacle(X As Integer, Y As Integer) As Boolean
    Dim I As Integer
    
    For I = 0 To NumLayers - 1
        If Map(X, Y, I) <> -1 Then
            If TileObj(Map(X, Y, I)).GetProperty("obstacle") = "true" Then
                IsObstacle = True
                Exit Function
            End If
        End If
    Next I
    
    IsObstacle = False
End Function
