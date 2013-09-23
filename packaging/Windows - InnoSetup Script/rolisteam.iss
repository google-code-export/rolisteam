; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{0AD9B759-512C-4B8E-A0ED-69DE0D361570}
AppName=Rolisteam
AppVerName=Rolisteam 1.6.0
AppPublisher=rolisteamTeam
AppPublisherURL=http://www.rolisteam.org
AppSupportURL=http://wiki.rolisteam.org
AppUpdatesURL=http://www.rolisteam.org
DefaultDirName={pf}\Rolisteam
DefaultGroupName=Rolisteam
LicenseFile=D:\applications\rolisteam\COPYING.txt
OutputDir=C:\Users\Renaud\Documents
OutputBaseFilename=Rolisteam-1.6.0-setup
Compression=lzma
SolidCompression=yes
SetupIconFile=D:\applications\rolisteam\resources\logo\256.ico
;UninstallIconFile=D:\applications\rolisteam\resources\logo\256.ico

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "D:\applications\build-rolisteam-Desktop-Release\release\bin\rolisteam.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.5\bin\QtXml4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.5\bin\phonon4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.5\bin\QtCore4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.5\bin\QtGui4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.5\bin\QtNetwork4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.5\bin\QtOpenGL4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.5\bin\QtSvg4.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Qt\4.8.5\plugins\*"; DestDir: "{app}"; Excludes: "*\sqldrivers\*,*\graphicssystems\*,*\designer\*,*\accessible\*"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "C:\Windows\SysWOW64\msvcr100.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Windows\SysWOW64\msvcp100.dll"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\Rolisteam"; Filename: "{app}\rolisteam.exe"
Name: "{group}\{cm:UninstallProgram,Rolisteam}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\Rolisteam"; Filename: "{app}\rolisteam.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Rolisteam"; Filename: "{app}\rolisteam.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\rolisteam.exe"; Description: "{cm:LaunchProgram,Rolisteam}"; Flags: nowait postinstall skipifsilent

